/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ordering/impl/on_demand_os_client_grpc.hpp"

#include "backend/protobuf/proposal.hpp"
#include "backend/protobuf/transaction.hpp"
#include "cryptography/public_key.hpp"
#include "interfaces/common_objects/peer.hpp"
#include "interfaces/iroha_internal/transaction_batch.hpp"
#include "logger/logger.hpp"
#include "network/impl/grpc_channel_builder.hpp"

using namespace iroha;
using namespace iroha::ordering;
using namespace iroha::ordering::transport;

OnDemandOsClientGrpc::OnDemandOsClientGrpc(
    std::unique_ptr<proto::OnDemandOrdering::StubInterface> stub,
    std::shared_ptr<network::AsyncGrpcClient<google::protobuf::Empty>>
        async_call,
    std::shared_ptr<TransportFactoryType> proposal_factory,
    std::function<TimepointType()> time_provider,
    std::chrono::milliseconds proposal_request_timeout,
    std::shared_ptr<shared_model::interface::types::PubkeyType> self_peer_key,
    logger::LoggerPtr log)
    : log_(std::move(log)),
      stub_(std::move(stub)),
      async_call_(std::move(async_call)),
      proposal_factory_(std::move(proposal_factory)),
      time_provider_(std::move(time_provider)),
      proposal_request_timeout_(proposal_request_timeout),
      self_peer_key_(self_peer_key) {}

void OnDemandOsClientGrpc::onBatches(CollectionType batches) {
  proto::BatchesRequest request;
  for (auto &batch : batches) {
    for (auto &transaction : batch->transactions()) {
      *request.add_transactions() = std::move(
          static_cast<shared_model::proto::Transaction *>(transaction.get())
              ->getTransport());
    }
  }

  *request.mutable_peer_key() =
      shared_model::crypto::toBinaryString(*self_peer_key_);

  log_->debug("Propagating: '{}'", request.DebugString());

  async_call_->Call([&](auto context, auto cq) {
    return stub_->AsyncSendBatches(context, request, cq);
  });
}

boost::optional<std::shared_ptr<const OdOsNotification::ProposalType>>
OnDemandOsClientGrpc::onRequestProposal(consensus::Round round) {
  grpc::ClientContext context;
  context.set_deadline(time_provider_() + proposal_request_timeout_);
  proto::ProposalRequest request;

  request.mutable_round()->set_block_round(round.block_round);
  request.mutable_round()->set_reject_round(round.reject_round);

  *request.mutable_peer_key() =
      shared_model::crypto::toBinaryString(*self_peer_key_);

  proto::ProposalResponse response;
  auto status = stub_->RequestProposal(&context, request, &response);
  if (not status.ok()) {
    log_->warn("RPC failed: {}", status.error_message());
    return boost::none;
  }
  if (not response.has_proposal()) {
    return boost::none;
  }
  return proposal_factory_->build(response.proposal())
      .match(
          [&](iroha::expected::Value<
              std::unique_ptr<shared_model::interface::Proposal>> &v) {
            return boost::make_optional(
                std::shared_ptr<const OdOsNotification::ProposalType>(
                    std::move(v).value));
          },
          [this](iroha::expected::Error<TransportFactoryType::Error> &error) {
            log_->info(error.error.error);  // error
            return boost::optional<
                std::shared_ptr<const OdOsNotification::ProposalType>>();
          });
}

OnDemandOsClientGrpcFactory::OnDemandOsClientGrpcFactory(
    std::shared_ptr<network::AsyncGrpcClient<google::protobuf::Empty>>
        async_call,
    std::shared_ptr<TransportFactoryType> proposal_factory,
    std::function<OnDemandOsClientGrpc::TimepointType()> time_provider,
    OnDemandOsClientGrpc::TimeoutType proposal_request_timeout,
    std::shared_ptr<shared_model::interface::types::PubkeyType> self_key_peer,
    logger::LoggerPtr client_log)
    : async_call_(std::move(async_call)),
      proposal_factory_(std::move(proposal_factory)),
      time_provider_(time_provider),
      proposal_request_timeout_(proposal_request_timeout),
      self_key_peer_(self_key_peer),
      client_log_(std::move(client_log)) {}

std::unique_ptr<OdOsNotification> OnDemandOsClientGrpcFactory::create(
    const shared_model::interface::types::AddressType &to) {
  return std::make_unique<OnDemandOsClientGrpc>(
      network::createClient<proto::OnDemandOrdering>(to),
      async_call_,
      proposal_factory_,
      time_provider_,
      proposal_request_timeout_,
      self_key_peer_,
      client_log_);
}

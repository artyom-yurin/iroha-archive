/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_SHARED_MODEL_TYPES_HPP
#define IROHA_SHARED_MODEL_TYPES_HPP

#include <cstdint>
#include <set>
#include <string>
#include <vector>

namespace shared_model {

  namespace crypto {
    class Blob;
    class Hash;
    class PublicKey;
    class Signed;
  }  // namespace crypto

  namespace interface {

    class Signature;
    class Transaction;
    class AccountAsset;

    namespace types {
      /// Type of hash
      using HashType = crypto::Hash;
      /// Blob type
      using BlobType = crypto::Blob;
      /// Type of account id
      using AccountIdType = std::string;
      /// Type of precision
      using PrecisionType = uint8_t;
      /// Type of height (for Block, Proposal etc)
      using HeightType = uint64_t;
      /// Type of peer address
      using AddressType = std::string;
      /// Type of public key
      using PubkeyType = crypto::PublicKey;
      /// Type of public keys' collection
      using PublicKeyCollectionType = std::vector<PubkeyType>;
      /// Type of role (i.e admin, user)
      using RoleIdType = std::string;
      /// Iroha domain id type
      using DomainIdType = std::string;
      /// Type of asset id
      using AssetIdType = std::string;
      /// Permission type used in permission commands
      using PermissionNameType = std::string;
      /// Permission set
      using PermissionSetType = std::set<PermissionNameType>;
      /// Type of Quorum used in transaction and set quorum
      using QuorumType = uint16_t;
      /// Type of timestamp
      using TimestampType = uint64_t;
      /// Type of peer address
      using AddressType = std::string;
      /// Type of counter
      using CounterType = uint64_t;
      /// Type of account name
      using AccountNameType = std::string;
      /// Type of asset name
      using AssetNameType = std::string;
      /// Type of detail
      using DetailType = std::string;
      /// Type of JSON data
      using JsonType = std::string;
      /// Type of account detail key
      using AccountDetailKeyType = std::string;
      /// Type of account detail value
      using AccountDetailValueType = std::string;
      /// Type of a number of transactions in block and query response page
      using TransactionsNumberType = uint16_t;
      /// Type of the transfer message
      using DescriptionType = std::string;
      /// Type of setting key
      using SettingKeyType = std::string;
      /// Type of setting value
      using SettingValueType = std::string;

      enum class BatchType { ATOMIC = 0, ORDERED = 1 };

    }  // namespace types
  }    // namespace interface
}  // namespace shared_model

#endif  // IROHA_SHARED_MODEL_TYPES_HPP

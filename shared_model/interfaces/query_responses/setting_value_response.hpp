/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_SHARED_MODEL_SETTING_VALUE_RESPONSE_HPP
#define IROHA_SHARED_MODEL_SETTING_VALUE_RESPONSE_HPP

#include "interfaces/base/model_primitive.hpp"
#include "interfaces/common_objects/types.hpp"

namespace shared_model {
  namespace interface {
    /**
     * Provide response with setting value
     */
    class SettingValueResponse : public ModelPrimitive<SettingValueResponse> {
     public:
      /**
       * @return value of setting
       */
      virtual const types::SettingValueType &value() const = 0;

      std::string toString() const override;

      bool operator==(const ModelType &rhs) const override;
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_SETTING_VALUE_RESPONSE_HPP

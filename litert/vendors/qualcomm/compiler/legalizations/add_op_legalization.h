// Copyright 2024 Google LLC.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ODML_LITERT_LITERT_VENDORS_QUALCOMM_COMPILER_LEGALIZATIONS_ADD_OP_LEGALIZATION_H_
#define ODML_LITERT_LITERT_VENDORS_QUALCOMM_COMPILER_LEGALIZATIONS_ADD_OP_LEGALIZATION_H_

#include <alloca.h>
#include <stdio.h>

#include <cstdint>
#include <memory>

#include "litert/c/litert_common.h"
#include "litert/cc/litert_model.h"
#include "litert/vendors/qualcomm/compiler/graph_mapper.h"
#include "litert/vendors/qualcomm/compiler/legalizations/legalization.h"
#include "third_party/qairt/latest/include/QNN/QnnTypes.h"

namespace litert::qnn {

class AddOpLegalization : public Legalization {
 public:
  AddOpLegalization() = default;
  ~AddOpLegalization() = default;
  using Ptr = std::unique_ptr<AddOpLegalization>;
  static Ptr Create() { return std::make_unique<AddOpLegalization>(); }

  LiteRtStatus LegalizeOp(const litert::Op& src, Qnn_OpConfig_t& dest,
                          GraphMapper& graph_mapper);

 private:
  // Counter to ensure unique op names.
  uint32_t op_counter_ = 0;
};

}  // namespace litert::qnn

#endif  // ODML_LITERT_LITERT_VENDORS_QUALCOMM_COMPILER_LEGALIZATIONS_ADD_OP_LEGALIZATION_H_

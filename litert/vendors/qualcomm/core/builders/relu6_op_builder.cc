// Copyright (c) Qualcomm Innovation Center, Inc.
// All Rights Reserved.

#include <vector>

#include "litert/vendors/qualcomm/core/builders/op_builder.h"
#include "litert/vendors/qualcomm/core/tensor_pool.h"
#include "litert/vendors/qualcomm/core/wrappers/op_wrapper.h"
#include "litert/vendors/qualcomm/core/wrappers/tensor_wrapper.h"
#include "third_party/qairt/latest/include/QNN/QnnOpDef.h"

namespace qnn {

std::vector<OpWrapper> BuildRelu6Op(
    TensorPool& tensor_pool, const std::vector<TensorWrapperRef>& inputs,
    const std::vector<TensorWrapperRef>& outputs) {
  std::vector<OpWrapper> res;

  CreateSimpleActivationOp(res, QNN_OP_RELU6, inputs[0], outputs[0]);

  return res;
}

}  // namespace qnn

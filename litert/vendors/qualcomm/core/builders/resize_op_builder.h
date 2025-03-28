// Copyright (c) Qualcomm Innovation Center, Inc. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

#ifndef ODML_LITERT_LITERT_VENDORS_QUALCOMM_CORE_BUILDERS_RESIZE_OP_BUILDER_H_
#define ODML_LITERT_LITERT_VENDORS_QUALCOMM_CORE_BUILDERS_RESIZE_OP_BUILDER_H_

#include <vector>

#include "litert/vendors/qualcomm/core/tensor_pool.h"
#include "litert/vendors/qualcomm/core/wrappers/op_wrapper.h"
#include "litert/vendors/qualcomm/core/wrappers/tensor_wrapper.h"

namespace qnn {

std::vector<OpWrapper> BuildResizeBilinearOp(
    TensorPool& tensor_pool, const std::vector<TensorWrapperRef>& inputs,
    const std::vector<TensorWrapperRef>& outputs, const bool align_corners,
    const bool half_pixel_centers);

std::vector<OpWrapper> BuildResizeNearestOp(
    TensorPool& tensor_pool, const std::vector<TensorWrapperRef>& inputs,
    const std::vector<TensorWrapperRef>& outputs, const bool align_corners,
    const bool half_pixel_centers);

}  // namespace qnn

#endif  // ODML_LITERT_LITERT_VENDORS_QUALCOMM_CORE_BUILDERS_RESIZE_OP_BUILDER_H_

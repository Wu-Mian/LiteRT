// Copyright 2025 Google LLC.
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

#include "litert/cc/litert_tensor_buffer.h"

#include <cstddef>

#include "litert/c/litert_common.h"
#include "litert/c/litert_gl_types.h"
#include "litert/c/litert_model.h"
#include "litert/c/litert_tensor_buffer.h"
#include "litert/c/litert_tensor_buffer_types.h"
#include "litert/cc/litert_expected.h"
#include "litert/cc/litert_handle.h"
#include "litert/cc/litert_macros.h"
#include "litert/cc/litert_model.h"

namespace litert {

Expected<TensorBuffer> TensorBuffer::Duplicate() const {
  if (!IsOwned()) {
    return Unexpected(kLiteRtStatusErrorInvalidArgument,
                      "Cannot duplicate a non-owned tensor buffer");
  }
  LITERT_RETURN_IF_ERROR(LiteRtDuplicateTensorBuffer(Get()));
  return TensorBuffer(Get(), OwnHandle::kYes);
}

Expected<TensorBuffer> TensorBuffer::CreateManaged(
    LiteRtTensorBufferType buffer_type, const RankedTensorType& tensor_type,
    size_t buffer_size) {
  LiteRtTensorBuffer tensor_buffer;
  auto litert_tensor_type = static_cast<LiteRtRankedTensorType>(tensor_type);
  LITERT_RETURN_IF_ERROR(LiteRtCreateManagedTensorBuffer(
      buffer_type, &litert_tensor_type, buffer_size, &tensor_buffer));
  return TensorBuffer(tensor_buffer, OwnHandle::kYes);
}

Expected<TensorBuffer> TensorBuffer::CreateFromHostMemory(
    const RankedTensorType& tensor_type, void* host_mem_addr,
    size_t buffer_size) {
  LiteRtTensorBuffer tensor_buffer;
  auto litert_tensor_type = static_cast<LiteRtRankedTensorType>(tensor_type);

  LITERT_RETURN_IF_ERROR(LiteRtCreateTensorBufferFromHostMemory(
      &litert_tensor_type, host_mem_addr, buffer_size,
      /*deallocator=*/nullptr, &tensor_buffer));
  return TensorBuffer(tensor_buffer, OwnHandle::kYes);
}

Expected<TensorBuffer> TensorBuffer::CreateFromAhwb(
    const RankedTensorType& tensor_type, AHardwareBuffer* ahwb,
    size_t ahwb_offset) {
#if LITERT_HAS_AHWB_SUPPORT
  LiteRtTensorBuffer tensor_buffer;
  auto litert_tensor_type = static_cast<LiteRtRankedTensorType>(tensor_type);

  LITERT_RETURN_IF_ERROR(LiteRtCreateTensorBufferFromAhwb(
      &litert_tensor_type, ahwb, ahwb_offset,
      /*deallocator=*/nullptr, &tensor_buffer));
  return TensorBuffer(tensor_buffer, OwnHandle::kYes);
#else
  return litert::Unexpected(
      kLiteRtStatusErrorRuntimeFailure,
      "AHardwareBuffer is not supported on this platform");
#endif
}

Expected<TensorBuffer> TensorBuffer::CreateFromGlBuffer(
    const RankedTensorType& tensor_type, LiteRtGLenum target, LiteRtGLuint id,
    size_t size_bytes, size_t offset) {
  LiteRtTensorBuffer tensor_buffer;
  auto litert_tensor_type = static_cast<LiteRtRankedTensorType>(tensor_type);
  LITERT_RETURN_IF_ERROR(LiteRtCreateTensorBufferFromGlBuffer(
      &litert_tensor_type, target, id, size_bytes, offset,
      /*deallocator=*/nullptr, &tensor_buffer));
  return TensorBuffer(tensor_buffer, OwnHandle::kYes);
}

Expected<TensorBuffer> TensorBuffer::CreateFromGlTexture(
    const RankedTensorType& tensor_type, LiteRtGLenum target, LiteRtGLuint id,
    LiteRtGLenum format, size_t size_bytes, LiteRtGLint layer) {
  LiteRtTensorBuffer tensor_buffer;
  auto litert_tensor_type = static_cast<LiteRtRankedTensorType>(tensor_type);
  LITERT_RETURN_IF_ERROR(LiteRtCreateTensorBufferFromGlTexture(
      &litert_tensor_type, target, id, format, size_bytes, layer,
      /*deallocator=*/nullptr, &tensor_buffer));
  return TensorBuffer(tensor_buffer, OwnHandle::kYes);
}

bool TensorBuffer::IsOpenClMemory() const {
  LiteRtTensorBufferType tensor_buffer_type;
  if (auto status = LiteRtGetTensorBufferType(Get(), &tensor_buffer_type);
      status != kLiteRtStatusOk) {
    return false;
  }
  switch (tensor_buffer_type) {
    case kLiteRtTensorBufferTypeOpenClBuffer:
    case kLiteRtTensorBufferTypeOpenClBufferFp16:
    case kLiteRtTensorBufferTypeOpenClTexture:
    case kLiteRtTensorBufferTypeOpenClTextureFp16:
    case kLiteRtTensorBufferTypeOpenClImageBuffer:
    case kLiteRtTensorBufferTypeOpenClImageBufferFp16:
      return true;
    default:
      return false;
  }
}

}  // namespace litert

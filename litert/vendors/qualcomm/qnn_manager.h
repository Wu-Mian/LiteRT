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
//
// Copyright (c) Qualcomm Innovation Center, Inc. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

#ifndef ODML_LITERT_LITERT_VENDORS_QUALCOMM_QNN_MANAGER_H_
#define ODML_LITERT_LITERT_VENDORS_QUALCOMM_QNN_MANAGER_H_

#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "absl/strings/string_view.h"  // from @com_google_absl
#include "absl/types/span.h"  // from @com_google_absl
#include "litert/c/litert_common.h"
#include "litert/c/litert_logging.h"
#include "litert/cc/litert_expected.h"
#include "litert/cc/litert_macros.h"  // IWYU pragma: keep
#include "litert/cc/litert_shared_library.h"
#include "litert/vendors/qualcomm/common.h"
#include "litert/vendors/qualcomm/core/backends/htp_device_config.h"
#include "litert/vendors/qualcomm/core/schema/soc_table.h"
#include "third_party/qairt/latest/include/QNN/QnnBackend.h"
#include "third_party/qairt/latest/include/QNN/QnnCommon.h"
#include "third_party/qairt/latest/include/QNN/QnnContext.h"
#include "third_party/qairt/latest/include/QNN/QnnDevice.h"
#include "third_party/qairt/latest/include/QNN/QnnInterface.h"
#include "third_party/qairt/latest/include/QNN/QnnLog.h"
#include "third_party/qairt/latest/include/QNN/QnnTypes.h"
#include "third_party/qairt/latest/include/QNN/System/QnnSystemContext.h"
#include "third_party/qairt/latest/include/QNN/System/QnnSystemInterface.h"

//===----------------------------------------------------------------------===//
//
//                                                                   QnnManger
//
// Syntactic sugar for various Qnn Sdk routines.
//
// Provides various utilities for linking shared libraries at runtime
// against Qnn symbols as well as convience getters and storage of handles
// (pointers). Provides simple wrappers for freeing handles and returning
// LiteRtStatus rather than Qnn ones. Additionally exposes hooks for dumping
// api and shared libarary details.
//
// Does not own any memory and will always have trivial cstor/dstor. The
// user is responsible for freeing any Qnn handles explicitly. Note,
// Qnn handles will be automatically freed when the library is unloaded
// if they have been already.
//
//===----------------------------------------------------------------------===//

namespace litert::qnn {

class QnnManager;

namespace internal {

void Dump(const QnnManager& qnn, std::ostream& out);

}  // namespace internal

class QnnManager {
  friend void internal::Dump(const QnnManager& qnn, std::ostream& out);

 public:
  using Ptr = std::unique_ptr<QnnManager>;
  using SystemContextHandle =
      std::unique_ptr<std::remove_pointer<QnnSystemContext_Handle_t>::type,
                      QnnSystemContext_FreeFn_t>;
  class ContextHandle;

  ~QnnManager();

  static Expected<Ptr> Create(
      absl::Span<const QnnBackend_Config_t*> configs,
      std::optional<std::string> shared_library_dir = std::nullopt,
      std::optional<::qnn::SocInfo> soc_info = std::nullopt,
      QnnLog_Level_t log_level = QNN_LOG_LEVEL_INFO);

  static absl::Span<const QnnBackend_Config_t*> DefaultBackendConfigs();
  static absl::Span<const QnnContext_Config_t*> DefaultContextConfigs();
  static absl::Span<const QnnContext_Config_t*> WeightSharingContextConfigs();
  // Get resolved function pointers for qnn sdk calls. Nullptr if functions
  // have not been resolved yet.
  const QnnApi* Api() const;

  // Get resolved function pointers for qnn sdk calls. Nullptr if functions
  // have not been resolved yet.
  const QnnSystemApi* SystemApi() const;

  //
  // QNN SDK Objects.
  //

  // Create system context handle.
  Expected<SystemContextHandle> CreateSystemContextHandle();

  // Create a context handle for compilation.
  Expected<ContextHandle> CreateContextHandle(
      absl::Span<const QnnContext_Config_t*> configs);

  // Create a context handle for inference, from a given bytecode.
  Expected<ContextHandle> CreateContextHandle(
      absl::Span<const QnnContext_Config_t*> configs,
      absl::Span<const uint8_t> bytecode, Qnn_ProfileHandle_t profile_handle);

  //
  // Context Binary
  //

  // Generates QNN context binary from current context. Writes to given
  // buffer.
  LiteRtStatus GenerateContextBinary(Qnn_ContextHandle_t context_handle,
                                     std::vector<char>& buffer);

  LiteRtStatus ValidateOp(const Qnn_OpConfig_t& op_config);

  bool IsLegacySocModel() { return soc_info_.dsp_arch == ::qnn::DspArch::V68; }

  // Get qnn backend handle. Nullptr if backendCreate has not been successfully
  // called.
  Qnn_BackendHandle_t& BackendHandle() { return backend_handle_; }

 private:
  QnnManager() = default;

  LiteRtStatus Init(absl::Span<const QnnBackend_Config_t*> configs,
                    std::optional<std::string> shared_library_dir,
                    std::optional<::qnn::SocInfo> soc_info,
                    QnnLog_Level_t log_level);

  //
  // Manage libQnn*.so Loading
  //

  // Loads the libQnn*.so at given path.
  LiteRtStatus LoadLib(absl::string_view path);

  // Loads the libQnnSystem.so at given path.
  LiteRtStatus LoadSystemLib(absl::string_view path);

  //
  // Resolve and Access QNN SDK Functions
  //

  // Resolve all available QNN SDK functions from (already) loaded so. If
  // multiple providers are found, selects the first one with a suitable
  // version. Fails if none can be found.
  LiteRtStatus ResolveApi();

  // Resolve all available QNN SDK functions from (already) loaded so. If
  // multiple providers are found, selects the first one with a suitable
  // version. Fails if none can be found.
  LiteRtStatus ResolveSystemApi();

  // Get qnn log handle. Nullptr if logCreate has not been successfully called.
  Qnn_LogHandle_t& LogHandle() { return log_handle_; }

  // Get qnn device handle. Nullptr if deviceCreate has not been successfully
  // called.
  Qnn_DeviceHandle_t& DeviceHandle() { return device_handle_; }

  // Signal QNN SDK to free any memory related to the device. Does nothing
  // if deviceCreate has not been called.
  LiteRtStatus FreeDevice();

  // Signal QNN SDK to free any memory related to logging. Does nothing
  // if logCreate has not been called.
  LiteRtStatus FreeLogging();

  // Signal QNN SDK to free any memory related to backend. Does nothing
  // if backendCreate has not been called.
  LiteRtStatus FreeBackend();

  // Handle to the shared library that implements the API. The library is
  // released when the manager is destroyed.
  SharedLibrary lib_;

  // Handle to the system shared library that implements the API. The library is
  // released when the manager is destroyed.
  SharedLibrary lib_system_;

  const QnnInterface_t* interface_ = nullptr;
  const QnnSystemInterface_t* system_interface_ = nullptr;

  Qnn_LogHandle_t log_handle_ = nullptr;
  Qnn_BackendHandle_t backend_handle_ = nullptr;
  Qnn_DeviceHandle_t device_handle_ = nullptr;
  ::qnn::SocInfo soc_info_ = ::qnn::kSocInfos[0];
  std::unique_ptr<::qnn::HtpDeviceConfig> htp_device_config_;
  std::vector<QnnDevice_Config_t> device_configs_;
};

// Unfortunately we can't use std::unique_ptr with a deleter because
// QnnContext_FreeFn_t takes a profile handle as a second argument.
class QnnManager::ContextHandle {
 public:
  ContextHandle(Qnn_ContextHandle_t context_handle, Qnn_ProfileHandle_t profile,
                QnnContext_FreeFn_t free_fn,
                QnnProfile_FreeFn_t profile_free_fn)
      : context_handle_(context_handle),
        profile_(profile),
        free_fn_(free_fn),
        profile_free_fn_(profile_free_fn) {}

  ~ContextHandle() {
    if (profile_ && profile_free_fn_) {
      if (auto status = profile_free_fn_(profile_); status != QNN_SUCCESS) {
        LITERT_LOG(LITERT_ERROR, "%s", "Failed to free profile handle\n");
      }
      profile_ = nullptr;
    }
    if (context_handle_ && free_fn_) {
      if (auto status = free_fn_(context_handle_, profile_);
          status != QNN_SUCCESS) {
        LITERT_LOG(LITERT_ERROR, "%s", "Failed to free context handle\n");
      }
      context_handle_ = nullptr;
    }
  }

  ContextHandle(ContextHandle&& other) { *this = std::move(other); }

  ContextHandle(const ContextHandle& other) = delete;

  ContextHandle& operator=(ContextHandle&& other) {
    std::swap(context_handle_, other.context_handle_);
    std::swap(profile_, other.profile_);
    std::swap(free_fn_, other.free_fn_);
    std::swap(profile_free_fn_, other.profile_free_fn_);
    return *this;
  }

  ContextHandle& operator=(const ContextHandle& other) = delete;

  Qnn_ContextHandle_t get() const noexcept { return context_handle_; }
  explicit operator bool() const noexcept { return context_handle_ != nullptr; }

 private:
  Qnn_ContextHandle_t context_handle_ = nullptr;
  Qnn_ProfileHandle_t profile_ = nullptr;
  QnnContext_FreeFn_t free_fn_ = nullptr;
  QnnProfile_FreeFn_t profile_free_fn_ = nullptr;
};

}  // namespace litert::qnn

#endif  // ODML_LITERT_LITERT_VENDORS_QUALCOMM_QNN_MANAGER_H_

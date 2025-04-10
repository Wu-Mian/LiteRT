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

#ifndef ODML_LITERT_LITERT_CC_LITERT_COMPILATION_OPTIONS_H_
#define ODML_LITERT_LITERT_CC_LITERT_COMPILATION_OPTIONS_H_

#include "litert/c/litert_accelerator_compilation_options.h"
#include "litert/c/litert_common.h"
#include "litert/c/litert_compilation_options.h"
#include "litert/cc/litert_accelerator_compilation_options.h"
#include "litert/cc/litert_expected.h"
#include "litert/cc/litert_handle.h"
#include "litert/cc/litert_macros.h"

namespace litert {

class CompilationOptions
    : public internal::Handle<LiteRtCompilationOptions,
                              LiteRtDestroyCompilationOptions> {
 public:
  CompilationOptions() = default;

  // Parameter `owned` indicates if the created CompilationOptions object
  // should take ownership of the provided `compilation_options` handle.
  explicit CompilationOptions(LiteRtCompilationOptions compilation_options,
                              OwnHandle owned)
      : internal::Handle<LiteRtCompilationOptions,
                         LiteRtDestroyCompilationOptions>(compilation_options,
                                                          owned) {}

  static Expected<CompilationOptions> Create() {
    LiteRtCompilationOptions options;
    LITERT_RETURN_IF_ERROR(LiteRtCreateCompilationOptions(&options));
    return CompilationOptions(options, OwnHandle::kYes);
  }

  Expected<void> SetHardwareAccelerators(LiteRtHwAcceleratorSet accelerators) {
    LITERT_RETURN_IF_ERROR(
        LiteRtSetCompilationOptionsHardwareAccelerators(Get(), accelerators));
    return {};
  }

  Expected<LiteRtHwAcceleratorSet> GetHardwareAccelerators() {
    LiteRtHwAcceleratorSet accelerators;
    LITERT_RETURN_IF_ERROR(
        LiteRtGetCompilationOptionsHardwareAccelerators(Get(), &accelerators));
    return accelerators;
  }

  Expected<void> AddAcceleratorCompilationOptions(
      AcceleratorCompilationOptions&& options) {
    LITERT_RETURN_IF_ERROR(
        LiteRtAddAcceleratorCompilationOptions(Get(), options.Release()));
    return {};
  }

  Expected<AcceleratorCompilationOptions> GetAcceleratorCompilationOptions() {
    LiteRtAcceleratorCompilationOptions options;
    LITERT_RETURN_IF_ERROR(
        LiteRtGetAcceleratorCompilationOptions(Get(), &options));
    return AcceleratorCompilationOptions(options, OwnHandle::kNo);
  }
};

}  // namespace litert

#endif  // ODML_LITERT_LITERT_CC_LITERT_COMPILATION_OPTIONS_H_

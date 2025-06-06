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

#ifndef THIRD_PARTY_ODML_LITERT_LITERT_TOOLS_FLAGS_APPLY_PLUGIN_FLAGS_H_
#define THIRD_PARTY_ODML_LITERT_LITERT_TOOLS_FLAGS_APPLY_PLUGIN_FLAGS_H_

#include <string>

#include "absl/flags/declare.h"  // from @com_google_absl
#include "litert/tools/flags/flag_types.h"

ABSL_DECLARE_FLAG(std::string, cmd);

ABSL_DECLARE_FLAG(std::string, compiler_flags);

ABSL_DECLARE_FLAG(::litert::tools::IntList, subgraphs);

#endif  // THIRD_PARTY_ODML_LITERT_LITERT_TOOLS_FLAGS_APPLY_PLUGIN_FLAGS_H_

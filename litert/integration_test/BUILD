# Copyright 2025 Google LLC.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# copybara:comment_begin(oss-only)
load("//litert/integration_test:litert_device.bzl", "device_rlocation", "litert_device_test", "litert_integration_test")
# copybara:comment_end

package(
    # copybara:uncomment default_applicable_licenses = ["@org_tensorflow//tensorflow:license"],
    default_visibility = ["//litert:litert_stable_abi_users"],
)

sh_binary(
    name = "run_on_device_driver_OSS",
    srcs = ["run_on_device_driver_OSS.sh"],
)

# Internal test to validate the macros and underlying shell scripts.
litert_device_test(
    name = "tools_test",
    srcs = ["smoketests/tools_test.cc"],
    backend_id = "cpu",
    copts = ["-DGOOGLE_COMMANDLINEFLAGS_FULL_API=1"],
    data = ["//litert/integration_test/models:single_op"],
    exec_args = ["--models_dir={}".format(device_rlocation("//litert/integration_test/models:single_op"))],
    deps = [
        "@com_google_googletest//:gtest_main",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

# C++ TEST SCAFFOLD ################################################################################

cc_test(
    name = "gen_device_test",
    srcs = ["gen_device_test.cc"],
    copts = ["-DGOOGLE_COMMANDLINEFLAGS_FULL_API=1"],
    data = ["//litert/integration_test/models:single_op"],
    linkopts = select({
        "@org_tensorflow//tensorflow:android": ["-landroid"],
        "//conditions:default": [],
    }),
    tags = ["manual"],
    deps = [
        ":gen_device_test_lib",
        "@com_google_googletest//:gtest",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_absl//absl/strings:string_view",
        "@com_google_absl//absl/types:span",
        "//litert/c:litert_logging",
        "//litert/cc:litert_environment",
        "//litert/cc:litert_model",
        "//litert/test:common",
        "//litert/test:matchers",
        "//litert/tools:dump",
    ],
)

cc_library(
    name = "gen_device_test_lib",
    testonly = True,
    hdrs = ["gen_device_test_lib.h"],
    deps = [
        "//testing/base/public:gunit_for_library_testonly",
        "@com_google_absl//absl/strings:string_view",
        "//litert/c:litert_common",
        "//litert/cc:litert_compiled_model",
        "//litert/cc:litert_environment",
        "//litert/cc:litert_model",
        "//litert/cc:litert_tensor_buffer",
        "//litert/core/model",
        "//litert/test:matchers",
    ],
)

# ON DEVICE INTEGRATION TESTS ######################################################################

# NOTE: Everything here should be built with -c opt --config=android_arm64.

# CPU ----------------------------------------------------------------------------------------------

litert_integration_test(
    name = "single_op_device_tests_cpu",
    backend_id = "cpu",
    models = "//litert/integration_test/models:single_op",
)

litert_integration_test(
    name = "classic_ml_device_tests_cpu",
    backend_id = "cpu",
    models = "//litert/integration_test/models:classic_ml",
)

# GOOGLE TENSOR ------------------------------------------------------------------------------------

litert_device_test(
    name = "google_tensor_smoketest",
    srcs = ["smoketests/google_tensor_smoketest.cc"],
    backend_id = "google_tensor",
    deps = [
        "@com_google_googletest//:gtest_main",
        "//litert/cc:litert_shared_library",
    ],
)

# QUALCOMM -----------------------------------------------------------------------------------------

litert_device_test(
    name = "qnn_smoketest",
    srcs = ["smoketests/qnn_smoketest.cc"],
    backend_id = "qualcomm",
    deps = [
        "@com_google_googletest//:gtest_main",
        "//litert/cc:litert_shared_library",
        "//litert/vendors/qualcomm:common",
        "//litert/vendors/qualcomm:qnn_manager",
    ],
)

litert_integration_test(
    name = "single_op_device_tests_qualcomm_JIT",
    backend_id = "qualcomm",
    models = "//litert/integration_test/models:single_op",
    skips = [
        "greater_f32",  # TODO: lukeboyer - Investigate (segfault).
        "less_f32",  # TODO: lukeboyer - Investigate (segfault).
    ],
)

litert_integration_test(
    name = "classic_ml_device_tests_qualcomm_JIT",
    backend_id = "qualcomm",
    models = "//litert/integration_test/models:classic_ml",
)

litert_integration_test(
    name = "pre_compiled_device_tests_qualcomm",
    backend_id = "qualcomm",
    models = "//litert/integration_test/models:pre_compiled",
)

# MEDIATEK -----------------------------------------------------------------------------------------

litert_device_test(
    name = "mediatek_smoketest",
    srcs = ["smoketests/mediatek_smoketest.cc"],
    backend_id = "mediatek",
    deps = [
        "@com_google_googletest//:gtest_main",
        "//litert/cc:litert_shared_library",
        "//litert/vendors/mediatek:neuron_adapter_api",
    ],
)

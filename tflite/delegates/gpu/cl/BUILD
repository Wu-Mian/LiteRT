load("@flatbuffers//:build_defs.bzl", "flatbuffer_cc_library")
load("@org_tensorflow//tensorflow:tensorflow.bzl", "if_google", "workspace_root")
load(
    "@org_tensorflow//tensorflow/core/platform:build_config_root.bzl",
    "tf_gpu_tests_tags",
)

package(
    # copybara:uncomment default_applicable_licenses = ["@org_tensorflow//tensorflow:license"],
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

_API_NO_GL_DEPS = [
    ":cl_command_queue",
    ":cl_errors",
    ":cl_event",
    ":environment",
    ":inference_context",
    ":opencl_wrapper",
    ":tensor",
    ":tensor_type_util",
    "@com_google_absl//absl/memory",
    "@com_google_absl//absl/types:span",
    "//tflite/delegates/gpu:api",
    "//tflite/delegates/gpu:tflite_profile",
    "//tflite/delegates/gpu/cl/kernels:converter",
    "//tflite/delegates/gpu/common:data_type",
    "//tflite/delegates/gpu/common:model",
    "//tflite/delegates/gpu/common:precision",
    "//tflite/delegates/gpu/common:shape",
    "//tflite/delegates/gpu/common:status",
    "//tflite/delegates/gpu/common:tensor",
    "//tflite/delegates/gpu/common/task:tensor_desc",
]

_GPU_API_DELEGATE_NO_GL_DEPS = [
    ":opencl_wrapper",
    ":tensor_type_util",
    "@com_google_absl//absl/types:span",
    "//tflite:kernel_api",
    "//tflite/core/c:common",
    "//tflite/delegates/gpu:api",
    "//tflite/delegates/gpu/common:model",
    "//tflite/delegates/gpu/common:model_builder",
    "//tflite/delegates/gpu/common:model_transformer",
    "//tflite/delegates/gpu/common:status",
    "//tflite/delegates/gpu/common/transformations:model_transformations",
]

config_setting(
    name = "opencl_delegate_no_gl",
    values = {"copt": "-DCL_DELEGATE_NO_GL"},
)

# copybara:uncomment_begin(google-only)
# cc_library(
#     name = "api_no_gl",
#     srcs = ["api.cc"],
#     hdrs = ["api.h"],
#     defines = ["CL_DELEGATE_NO_GL"],
#     deps = _API_NO_GL_DEPS + ["@org_tensorflow//third_party/GL:EGL_headers"],
# )
# copybara:uncomment_end

cc_library(
    name = "api",
    srcs = ["api.cc"],
    hdrs = ["api.h"],
    deps = select({
        ":opencl_delegate_no_gl": [],
        "//conditions:default": [
            ":egl_sync",
            ":gl_interop",
        ],
    }) + _API_NO_GL_DEPS,
)

cc_library(
    name = "buffer",
    srcs = ["buffer.cc"],
    hdrs = ["buffer.h"],
    deps = [
        ":cl_command_queue",
        ":cl_context",
        ":gpu_object",
        ":opencl_wrapper",
        ":util",
        "//tflite/delegates/gpu/common:data_type",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common/task:buffer_desc",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
    ],
)

cc_test(
    name = "buffer_test",
    srcs = ["buffer_test.cc"],
    linkstatic = True,
    tags = tf_gpu_tests_tags() + [
        "linux",
        "local",
    ],
    deps = [
        ":buffer",
        ":cl_test",
        "//tflite/delegates/gpu/common:status",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "cl_test",
    testonly = 1,
    hdrs = ["cl_test.h"],
    deps = [
        ":environment",
        ":opencl_wrapper",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "cl_arguments",
    srcs = ["cl_arguments.cc"],
    hdrs = ["cl_arguments.h"],
    deps = [
        ":buffer",
        ":cl_context",
        ":gpu_object",
        ":qcom_thin_filter",
        ":tensor",
        "//tflite/delegates/gpu/common:gpu_info",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common:util",
        "//tflite/delegates/gpu/common/task:arguments",
        "//tflite/delegates/gpu/common/task:util",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "cl_arguments_test",
    srcs = ["cl_arguments_test.cc"],
    linkstatic = True,
    tags = tf_gpu_tests_tags() + [
        "linux",
        "local",
    ],
    deps = [
        ":buffer",
        ":cl_arguments",
        ":cl_test",
        ":gpu_object",
        "//tflite/delegates/gpu/common:gpu_info",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "cl_command_buffer",
    srcs = ["cl_command_buffer.cc"],
    hdrs = ["cl_command_buffer.h"],
    deps = [
        ":cl_command_queue",
        ":cl_event",
        ":opencl_wrapper",
        ":util",
        "//tflite/delegates/gpu/common:status",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "cl_command_queue",
    srcs = ["cl_command_queue.cc"],
    hdrs = ["cl_command_queue.h"],
    deps = [
        ":cl_context",
        ":cl_device",
        ":cl_event",
        ":cl_kernel",
        ":opencl_wrapper",
        ":util",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common:types",
        "//tflite/delegates/gpu/common/task:profiling_info",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "cl_context",
    srcs = ["cl_context.cc"],
    hdrs = ["cl_context.h"],
    deps = [
        ":cl_device",
        ":cl_image_format",
        ":opencl_wrapper",
        ":util",
        "//tflite/delegates/gpu/common:data_type",
        "//tflite/delegates/gpu/common:status",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "cl_device",
    srcs = ["cl_device.cc"],
    hdrs = ["cl_device.h"],
    deps = [
        ":opencl_wrapper",
        ":util",
        "//tflite/delegates/gpu/common:gpu_info",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common:types",
        "//tflite/experimental/acceleration/compatibility:android_info",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_test(
    name = "cl_device_test",
    srcs = ["cl_device_test.cc"],
    deps = [
        ":cl_device",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "cl_errors",
    hdrs = ["cl_errors.h"],
    deps = [
        ":util",
        "//tflite/delegates/gpu/common:status",
    ],
)

cc_library(
    name = "cl_event",
    srcs = ["cl_event.cc"],
    hdrs = ["cl_event.h"],
    deps = [
        ":opencl_wrapper",
    ],
)

cc_library(
    name = "cl_image_format",
    srcs = ["cl_image_format.cc"],
    hdrs = ["cl_image_format.h"],
    deps = [
        ":opencl_wrapper",
        "//tflite/delegates/gpu/common:data_type",
    ],
)

cc_library(
    name = "cl_kernel",
    srcs = ["cl_kernel.cc"],
    hdrs = ["cl_kernel.h"],
    deps = [
        ":cl_context",
        ":cl_device",
        ":cl_program",
        ":opencl_wrapper",
        ":util",
        "//tflite/delegates/gpu/common:kernel_info",
        "//tflite/delegates/gpu/common:status",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "cl_memory",
    srcs = ["cl_memory.cc"],
    hdrs = ["cl_memory.h"],
    deps = [
        ":opencl_wrapper",
        "//tflite/delegates/gpu/common:access_type",
        "//tflite/delegates/gpu/common:status",
    ],
)

cc_library(
    name = "cl_operation",
    srcs = ["cl_operation.cc"],
    hdrs = ["cl_operation.h"],
    deps = [
        ":cl_arguments",
        ":cl_command_queue",
        ":cl_context",
        ":cl_device",
        ":cl_kernel",
        ":program_cache",
        ":tensor",
        "//tflite/delegates/gpu/common/task:compiler_options",
        "//tflite/delegates/gpu/common/task:gpu_operation",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "cl_program",
    srcs = ["cl_program.cc"],
    hdrs = ["cl_program.h"],
    deps = [
        ":cl_context",
        ":cl_device",
        ":opencl_wrapper",
        ":util",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common/task:compiler_options",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
    ],
)

flatbuffer_cc_library(
    name = "compiled_program_cache_cc_fbs",
    srcs = ["compiled_program_cache.fbs"],
    flatc_args = [
        "--scoped-enums",
    ],
)

cc_library(
    name = "egl_sync",
    srcs = ["egl_sync.cc"],
    hdrs = ["egl_sync.h"],
    defines = [
        "EGL_EGLEXT_PROTOTYPES",
    ],
    deps = [
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/gl:gl_call",
    ],
)

cc_library(
    name = "environment",
    srcs = ["environment.cc"],
    hdrs = ["environment.h"],
    deps = [
        ":cl_command_queue",
        ":cl_context",
        ":cl_device",
        ":program_cache",
        "//tflite/delegates/gpu/common:data_type",
        "//tflite/delegates/gpu/common:gpu_info",
        "//tflite/delegates/gpu/common:precision",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common:tensor",
        "//tflite/delegates/gpu/common/task:tensor_desc",
    ],
)

cc_library(
    name = "gl_interop",
    srcs = ["gl_interop.cc"],
    hdrs = ["gl_interop.h"],
    deps = [
        ":cl_command_queue",
        ":cl_context",
        ":cl_device",
        ":cl_errors",
        ":cl_event",
        ":cl_memory",
        ":egl_sync",
        ":environment",
        ":opencl_wrapper",
        "//tflite/delegates/gpu:spi",
        "//tflite/delegates/gpu/common:access_type",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/gl:gl_call",
        "//tflite/delegates/gpu/gl:gl_sync",
        "//tflite/delegates/gpu/gl:portable",
        "@com_google_absl//absl/strings",
    ],
)

# copybara:uncomment_begin(google-only)
# cc_library(
#     name = "gpu_api_delegate_no_gl",
#     srcs = ["gpu_api_delegate.cc"],
#     hdrs = ["gpu_api_delegate.h"],
#     defines = ["CL_DELEGATE_NO_GL"],
#     linkopts = select({
#         "@org_tensorflow//tensorflow:android": [
#             "-lEGL",
#             "-lGLESv3",
#         ],
#         "//conditions:default": [],
#     }),
#     deps = _GPU_API_DELEGATE_NO_GL_DEPS + [
#         ":api_no_gl",
#         "@org_tensorflow//third_party/GL:EGL_headers",
#         "@org_tensorflow//third_party/GL:GLES3_headers",
#         "//tflite/delegates/gpu:delegate_no_gl",
#     ],
# )
# copybara:uncomment_end

cc_library(
    name = "gpu_api_delegate",
    srcs = ["gpu_api_delegate.cc"],
    hdrs = ["gpu_api_delegate.h"],
    linkopts = select({
        "@org_tensorflow//tensorflow:android": [
            "-lEGL",
            "-lGLESv3",
        ],
        "//conditions:default": [],
    }),
    deps = _GPU_API_DELEGATE_NO_GL_DEPS + [
        ":api",
        "//tflite/delegates/gpu:delegate",
    ],
)

cc_library(
    name = "gpu_object",
    hdrs = ["gpu_object.h"],
    deps = [
        ":opencl_wrapper",
        "//tflite/delegates/gpu/common:access_type",
        "//tflite/delegates/gpu/common:data_type",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common/task:gpu_object_desc",
    ],
)

cc_library(
    name = "inference_context",
    srcs = [
        "inference_context.cc",
    ],
    hdrs = [
        "inference_context.h",
    ],
    deps = [
        ":buffer",
        ":cl_command_buffer",
        ":cl_command_queue",
        ":cl_device",
        ":cl_event",
        ":cl_operation",
        ":environment",
        ":gpu_object",
        ":opencl_wrapper",
        ":recordable_queue_builder",
        ":serialization_cc_fbs",
        ":tensor",
        "//tflite/delegates/gpu/common:data_type",
        "//tflite/delegates/gpu/common:gpu_model",
        "//tflite/delegates/gpu/common:gpu_model_cc_fbs",
        "//tflite/delegates/gpu/common:memory_management",
        "//tflite/delegates/gpu/common:model",
        "//tflite/delegates/gpu/common:model_hints",
        "//tflite/delegates/gpu/common:precision",
        "//tflite/delegates/gpu/common:shape",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common:tensor",
        "//tflite/delegates/gpu/common:types",
        "//tflite/delegates/gpu/common:util",
        "//tflite/delegates/gpu/common/task:gpu_operation",
        "//tflite/delegates/gpu/common/task:serialization_base",
        "//tflite/delegates/gpu/common/task:tensor_desc",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
    ],
)

cc_library(
    name = "opencl_wrapper",
    srcs = ["opencl_wrapper.cc"],
    hdrs = ["opencl_wrapper.h"],
    linkopts = select({
        "@org_tensorflow//tensorflow:android": ["-lm"],
        "//conditions:default": [],
    }) + ["-ldl"],  # opencl_wrapper calls dlopen()
    local_defines = select({
        # copybara:uncomment_begin(google-only)
        # "//tools/cc_target_os:linux-google": ["__LINUX_GOOGLE__"],
        # copybara:uncomment_end
        "//conditions:default": [],
    }),
    deps = [
        "//tflite/delegates/gpu/common:status",
        "@com_google_absl//absl/strings",
        "@opencl_headers",
        "//tflite/delegates/gpu/cl/" + if_google("google", "default") + ":qcom_wrapper",
        "//tflite/tools:logging",
    ] + select({
        # copybara:uncomment_begin(google-only)
        # "//tools/cc_target_os:linux-google": ["@org_tensorflow//third_party/opencl_icd_loader"],
        # copybara:uncomment_end
        "//conditions:default": [],
    }),
)

cc_library(
    name = "program_cache",
    srcs = ["program_cache.cc"],
    hdrs = ["program_cache.h"],
    deps = [
        ":cl_context",
        ":cl_device",
        ":cl_kernel",
        ":cl_program",
        ":compiled_program_cache_cc_fbs",
        ":util",
        "//tflite/delegates/gpu/common:status",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/types:span",
        "@farmhash_archive//:farmhash",
        "@flatbuffers",
    ],
)

cc_library(
    name = "qcom_thin_filter",
    srcs = ["qcom_thin_filter.cc"],
    hdrs = ["qcom_thin_filter.h"],
    deps = [
        ":cl_context",
        ":gpu_object",
        ":util",
        "//tflite/delegates/gpu/common/task:qcom_thin_filter_desc",
    ],
)

cc_library(
    name = "recordable_queue",
    hdrs = ["recordable_queue.h"],
    deps = [
        ":cl_command_queue",
        ":cl_context",
        ":cl_device",
        ":cl_operation",
        ":opencl_wrapper",
        "//tflite/delegates/gpu/common:status",
    ],
)

cc_library(
    name = "recordable_queue_builder",
    hdrs = ["recordable_queue_builder.h"],
    deps = [
        ":cl_context",
        ":cl_device",
        ":cl_operation",
        ":recordable_queue",
        "//tflite/delegates/gpu/cl/" + if_google("google", "default") + ":recordable_queue",
    ],
)

flatbuffer_cc_library(
    name = "serialization_cc_fbs",
    srcs = ["serialization.fbs"],
    flatc_args = [
        "--scoped-enums",
        "-I " + workspace_root,
    ],
    includes = [
        "//tflite/delegates/gpu/common:gpu_model_cc_fbs_includes",
        "//tflite/delegates/gpu/common/task:serialization_base_cc_fbs_includes",
    ],
)

cc_library(
    name = "tensor",
    srcs = ["tensor.cc"],
    hdrs = ["tensor.h"],
    deps = [
        ":buffer",
        ":cl_command_queue",
        ":cl_context",
        ":cl_device",
        ":cl_image_format",
        ":cl_memory",
        ":gpu_object",
        ":util",
        "//tflite/delegates/gpu/common:data_type",
        "//tflite/delegates/gpu/common:shape",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common:tensor",
        "//tflite/delegates/gpu/common:types",
        "//tflite/delegates/gpu/common/task:gpu_tensor",
        "//tflite/delegates/gpu/common/task:tensor_desc",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "tensor_test",
    srcs = ["tensor_test.cc"],
    linkstatic = True,
    tags = tf_gpu_tests_tags() + [
        "linux",
        "local",
    ],
    deps = [
        ":cl_test",
        ":tensor",
        "//tflite/delegates/gpu/common:data_type",
        "//tflite/delegates/gpu/common:shape",
        "//tflite/delegates/gpu/common:status",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "tensor_type_util",
    srcs = ["tensor_type_util.cc"],
    hdrs = ["tensor_type_util.h"],
    deps = [
        "//tflite/delegates/gpu:api",
        "//tflite/delegates/gpu/common/task:tensor_desc",
    ],
)

cc_library(
    name = "util",
    srcs = ["util.cc"],
    hdrs = ["util.h"],
    deps = [
        ":opencl_wrapper",
        "//tflite/delegates/gpu/common:data_type",
        "//tflite/delegates/gpu/common:status",
        "//tflite/delegates/gpu/common:tensor",
        "//tflite/delegates/gpu/common:util",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
        "//tflite/delegates/gpu/cl/" + if_google("google", "default") + ":util",
    ],
)

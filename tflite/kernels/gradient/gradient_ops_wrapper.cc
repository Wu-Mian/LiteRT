/* Copyright 2021 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include <cstdint>

#include "pybind11/pybind11.h"  // from @pybind11
#include "pybind11/pytypes.h"  // from @pybind11
#include "tflite/kernels/gradient/gradient_ops.h"
#include "tflite/mutable_op_resolver.h"

PYBIND11_MODULE(pywrap_gradient_ops, m) {
  m.doc() = R"pbdoc(
    pywrap_gradient_ops
    -----
  )pbdoc";
  m.def(
      "GradientOpsRegisterer",
      [](uintptr_t resolver) {
        tflite::ops::custom::AddGradientOps(
            reinterpret_cast<tflite::MutableOpResolver*>(resolver));
      },
      R"pbdoc(
        Gradient op registerer function with the correct signature. Registers
        Gradient custom ops.
      )pbdoc");
}

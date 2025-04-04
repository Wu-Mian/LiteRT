# Copyright 2024 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================

# pylint: disable=g-import-not-at-top
# pytype: disable=import-error
# pytype: disable=not-callable

"""Shim layer for TFLXX while it is in experimental."""


import importlib.util
from typing import Callable

call_tflxx: Callable[[bytes, str], bytes] = lambda input, pass_name: input


def tflxx_enabled() -> bool:
  return False
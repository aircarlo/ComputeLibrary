/*
 * Copyright (c) 2018 ARM Limited.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "arm_compute/runtime/CL/functions/CLNormalizePlanarYUVLayer.h"

#include "arm_compute/core/Error.h"
#include "arm_compute/core/TensorInfo.h"
#include "arm_compute/core/Types.h"
#include "arm_compute/core/Validate.h"
#include "arm_compute/runtime/CL/CLScheduler.h"

namespace arm_compute
{
CLNormalizePlanarYUVLayer::CLNormalizePlanarYUVLayer()
    : _norm_kernel()
{
}

void CLNormalizePlanarYUVLayer::configure(const ICLTensor *input, ICLTensor *output, const ICLTensor *mean, const ICLTensor *std)
{
    _norm_kernel.configure(input, output, mean, std);
}

Status CLNormalizePlanarYUVLayer::validate(const ITensorInfo *input, const ITensorInfo *output,
                                           const ITensorInfo *mean, const ITensorInfo *std)
{
    return CLNormalizePlanarYUVLayerKernel::validate(input, output, mean, std);
}

void CLNormalizePlanarYUVLayer::run()
{
    CLScheduler::get().enqueue(_norm_kernel, true);
}
} // namespace arm_compute

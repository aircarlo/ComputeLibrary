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
#include "arm_compute/graph/nodes/DepthwiseConvolutionLayerNode.h"

#include "arm_compute/core/Utils.h"
#include "arm_compute/graph/Graph.h"
#include "arm_compute/graph/INodeVisitor.h"
#include "arm_compute/graph/Utils.h"

namespace arm_compute
{
namespace graph
{
DepthwiseConvolutionLayerNode::DepthwiseConvolutionLayerNode(PadStrideInfo info, DepthwiseConvolutionMethod method)
    : _info(std::move(info)), _method(method), _fused_activation()
{
    _input_edges.resize(3, EmptyEdgeID);
    _outputs.resize(1, NullTensorID);
}

void DepthwiseConvolutionLayerNode::set_depthwise_convolution_method(DepthwiseConvolutionMethod method)
{
    _method = method;
}

DepthwiseConvolutionMethod DepthwiseConvolutionLayerNode::depthwise_convolution_method() const
{
    return _method;
}

PadStrideInfo DepthwiseConvolutionLayerNode::convolution_info() const
{
    return _info;
}

ActivationLayerInfo DepthwiseConvolutionLayerNode::fused_activation() const
{
    return _fused_activation;
}

void DepthwiseConvolutionLayerNode::set_fused_activation(ActivationLayerInfo fused_activation)
{
    _fused_activation = fused_activation;
}

TensorDescriptor DepthwiseConvolutionLayerNode::compute_output_descriptor(const TensorDescriptor &input_descriptor,
                                                                          const TensorDescriptor &weights_descriptor,
                                                                          const PadStrideInfo    &info)
{
    unsigned int output_width  = 0;
    unsigned int output_height = 0;

    const unsigned int input_width   = get_dimension_size(input_descriptor, DataLayoutDimension::WIDTH);
    const unsigned int input_height  = get_dimension_size(input_descriptor, DataLayoutDimension::HEIGHT);
    const unsigned int kernel_width  = get_dimension_size(weights_descriptor, DataLayoutDimension::WIDTH);
    const unsigned int kernel_height = get_dimension_size(weights_descriptor, DataLayoutDimension::HEIGHT);

    std::tie(output_width, output_height) = scaled_dimensions(input_width, input_height, kernel_width, kernel_height, info);

    TensorDescriptor output_descriptor = input_descriptor;
    output_descriptor.shape.set(get_dimension_idx(output_descriptor, DataLayoutDimension::WIDTH), output_width);
    output_descriptor.shape.set(get_dimension_idx(output_descriptor, DataLayoutDimension::HEIGHT), output_height);

    return output_descriptor;
}

bool DepthwiseConvolutionLayerNode::forward_descriptors()
{
    if((input_id(0) != NullTensorID) && (input_id(1) != NullTensorID) && (output_id(0) != NullTensorID))
    {
        Tensor *dst = output(0);
        ARM_COMPUTE_ERROR_ON(dst == nullptr);
        dst->desc() = configure_output(0);
        return true;
    }
    return false;
}

TensorDescriptor DepthwiseConvolutionLayerNode::configure_output(size_t idx) const
{
    ARM_COMPUTE_UNUSED(idx);
    const Tensor *src     = input(0);
    const Tensor *weights = input(1);

    ARM_COMPUTE_ERROR_ON(src == nullptr || weights == nullptr);

    return compute_output_descriptor(src->desc(), weights->desc(), _info);
}

NodeType DepthwiseConvolutionLayerNode::type() const
{
    return DepthwiseConvolutionLayerNode::node_type;
}

void DepthwiseConvolutionLayerNode::accept(INodeVisitor &v)
{
    v.visit(*this);
}
} // namespace graph
} // namespace arm_compute
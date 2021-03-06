// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <memory>

#include "base/command_line.h"
#include "build/build_config.h"
#include "gpu/command_buffer/client/shared_memory_limits.h"
#include "gpu/command_buffer/client/webgpu_implementation.h"
#include "gpu/config/gpu_switches.h"
#include "gpu/ipc/webgpu_in_process_context.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace gpu {

namespace {

class WebGPUInProcessCommandBufferTest : public ::testing::Test {
 public:
  std::unique_ptr<WebGPUInProcessContext> CreateWebGPUInProcessContext() {
    base::CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kEnableUnsafeWebGPU);

    ContextCreationAttribs attributes;
    attributes.bind_generates_resource = false;
    attributes.enable_gles2_interface = false;
    attributes.context_type = CONTEXT_TYPE_WEBGPU;

    static scoped_refptr<CommandBufferTaskExecutor> task_executor = nullptr;
    static constexpr GpuMemoryBufferManager* memory_buffer_manager = nullptr;
    static constexpr ImageFactory* image_factory = nullptr;
    static constexpr GpuChannelManagerDelegate* channel_manager = nullptr;
    auto context = std::make_unique<WebGPUInProcessContext>();
    auto result = context->Initialize(
        task_executor, attributes, SharedMemoryLimits(), memory_buffer_manager,
        image_factory, channel_manager);
    DCHECK_EQ(result, ContextResult::kSuccess);
    return context;
  }

  void SetUp() override {
    context_ = CreateWebGPUInProcessContext();
    wg_ = context_->GetImplementation();
  }

  void TearDown() override { context_.reset(); }

 protected:
  webgpu::WebGPUInterface* wg_;  // not owned
  std::unique_ptr<WebGPUInProcessContext> context_;
};

}  // namespace

TEST_F(WebGPUInProcessCommandBufferTest, Dummy) {
  wg_->Dummy();
  // TODO
}

}  // namespace gpu

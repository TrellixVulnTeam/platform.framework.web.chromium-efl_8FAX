// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gin/public/isolate_holder.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <memory>
#include <utility>

#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/single_thread_task_runner.h"
#include "base/sys_info.h"
#include "build/build_config.h"
#include "gin/debug_impl.h"
#include "gin/function_template.h"
#include "gin/per_isolate_data.h"
#include "gin/run_microtasks_observer.h"
#include "gin/v8_initializer.h"
#include "gin/v8_isolate_memory_dump_provider.h"

namespace gin {

namespace {
v8::ArrayBuffer::Allocator* g_array_buffer_allocator = nullptr;
}  // namespace

IsolateHolder::IsolateHolder(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : IsolateHolder(std::move(task_runner), AccessMode::kSingleThread) {}

IsolateHolder::IsolateHolder(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner,
    AccessMode access_mode)
    : IsolateHolder(std::move(task_runner),
                    access_mode,
                    kAllowAtomicsWait,
                    nullptr,
                    nullptr) {}

IsolateHolder::IsolateHolder(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner,
    AccessMode access_mode,
    AllowAtomicsWaitMode atomics_wait_mode,
    const intptr_t* reference,
    v8::StartupData* startup_data)
    : access_mode_(access_mode) {
  v8::ArrayBuffer::Allocator* allocator = g_array_buffer_allocator;
  CHECK(allocator) << "You need to invoke gin::IsolateHolder::Initialize first";

  v8::Isolate::CreateParams params;
  params.entry_hook = DebugImpl::GetFunctionEntryHook();
  params.code_event_handler = DebugImpl::GetJitCodeEventHandler();
  params.constraints.ConfigureDefaults(base::SysInfo::AmountOfPhysicalMemory(),
                                       base::SysInfo::AmountOfVirtualMemory());
#if defined(OS_TIZEN_TV_PRODUCT)
  // V8 heap memory for TV should be constrained tighter than default,
  // because other modules use memory as well.
  // We constrain max memory to be 352MB (New Space 16MB*2, Old Space 320MB)
  // TODO: Dynamically set up memory size, instead of fixed number
  //       ex) constrain differently for different embedded board
  params.constraints.set_max_semi_space_size_in_kb(16384);
  params.constraints.set_max_old_space_size(320);
#endif
  params.array_buffer_allocator = allocator;
  params.allow_atomics_wait = atomics_wait_mode == kAllowAtomicsWait;
  params.external_references = reference;

  if (startup_data) {
    CHECK(reference);
    V8Initializer::GetV8ContextSnapshotData(startup_data);
    if (startup_data->data) {
      params.snapshot_blob = startup_data;
    }
  }
  isolate_ = v8::Isolate::New(params);

  SetUp(std::move(task_runner));
}

IsolateHolder::IsolateHolder(const intptr_t* reference_table,
                             v8::StartupData* existing_blob)
    : access_mode_(AccessMode::kSingleThread) {
  CHECK(existing_blob);

  v8::StartupData unused_natives;
  V8Initializer::GetV8ExternalSnapshotData(&unused_natives, existing_blob);
  if (!existing_blob->data) {
    existing_blob = nullptr;
  }

  snapshot_creator_.reset(
      new v8::SnapshotCreator(reference_table, existing_blob));
  isolate_ = snapshot_creator_->GetIsolate();

  SetUp(nullptr);
}

IsolateHolder::~IsolateHolder() {
  if (task_observer_.get())
    base::MessageLoop::current()->RemoveTaskObserver(task_observer_.get());
#if defined(OS_WIN)
  {
    void* code_range;
    size_t size;
    isolate_->GetCodeRange(&code_range, &size);
    Debug::CodeRangeDeletedCallback callback =
        DebugImpl::GetCodeRangeDeletedCallback();
    if (code_range && callback)
      callback(code_range);
  }
#endif
  isolate_memory_dump_provider_.reset();
  isolate_data_.reset();
  isolate_->Dispose();
  isolate_ = nullptr;
}

// static
void IsolateHolder::Initialize(ScriptMode mode,
                               V8ExtrasMode v8_extras_mode,
                               v8::ArrayBuffer::Allocator* allocator) {
  CHECK(allocator);
  V8Initializer::Initialize(mode, v8_extras_mode);
  g_array_buffer_allocator = allocator;
}

void IsolateHolder::AddRunMicrotasksObserver() {
  DCHECK(!task_observer_.get());
  task_observer_.reset(new RunMicrotasksObserver(isolate_));
  base::MessageLoop::current()->AddTaskObserver(task_observer_.get());
}

void IsolateHolder::RemoveRunMicrotasksObserver() {
  DCHECK(task_observer_.get());
  base::MessageLoop::current()->RemoveTaskObserver(task_observer_.get());
  task_observer_.reset();
}

void IsolateHolder::EnableIdleTasks(
    std::unique_ptr<V8IdleTaskRunner> idle_task_runner) {
  DCHECK(isolate_data_.get());
  isolate_data_->EnableIdleTasks(std::move(idle_task_runner));
}

void IsolateHolder::SetUp(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner) {
  v8::ArrayBuffer::Allocator* allocator = g_array_buffer_allocator;
  CHECK(allocator) << "You need to invoke gin::IsolateHolder::Initialize first";
  isolate_data_.reset(
      new PerIsolateData(isolate_, allocator, access_mode_, task_runner));
  isolate_memory_dump_provider_.reset(new V8IsolateMemoryDumpProvider(this));
#if defined(OS_WIN)
  {
    void* code_range;
    size_t size;
    isolate_->GetCodeRange(&code_range, &size);
    Debug::CodeRangeCreatedCallback callback =
        DebugImpl::GetCodeRangeCreatedCallback();
    if (code_range && size && callback)
      callback(code_range, size);
  }
#endif
}

}  // namespace gin

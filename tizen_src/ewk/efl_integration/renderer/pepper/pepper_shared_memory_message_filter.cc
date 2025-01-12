// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Copyright 2016 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is copied from chrome/renderer/pepper/
// TODO(a.bujalski) consider moving class from chrome/ to content/

#include "renderer/pepper/pepper_shared_memory_message_filter.h"

#include "base/memory/shared_memory.h"
#include "base/process/process_handle.h"
#include "content/public/common/content_client.h"
#include "content/public/renderer/pepper_plugin_instance.h"
#include "content/public/renderer/render_thread.h"
#include "content/public/renderer/renderer_ppapi_host.h"
#include "ppapi/host/ppapi_host.h"
#include "ppapi/proxy/ppapi_messages.h"
#include "ppapi/shared_impl/var_tracker.h"

PepperSharedMemoryMessageFilter::PepperSharedMemoryMessageFilter(
    content::RendererPpapiHost* host)
    : InstanceMessageFilter(host->GetPpapiHost()), host_(host) {}

PepperSharedMemoryMessageFilter::~PepperSharedMemoryMessageFilter() {}

bool PepperSharedMemoryMessageFilter::OnInstanceMessageReceived(
    const IPC::Message& msg) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(PepperSharedMemoryMessageFilter, msg)
  IPC_MESSAGE_HANDLER(PpapiHostMsg_SharedMemory_CreateSharedMemory,
                      OnHostMsgCreateSharedMemory)
  IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}

bool PepperSharedMemoryMessageFilter::Send(IPC::Message* msg) {
  return host_->GetPpapiHost()->Send(msg);
}

void PepperSharedMemoryMessageFilter::OnHostMsgCreateSharedMemory(
    PP_Instance instance,
    uint32_t size,
    int* host_handle_id,
    ppapi::proxy::SerializedHandle* plugin_handle) {
  plugin_handle->set_null_shmem();
  *host_handle_id = -1;
  std::unique_ptr<base::SharedMemory> shm(std::move(
      content::RenderThread::Get()->HostAllocateSharedMemoryBuffer(size)));
  if (!shm.get())
    return;

  base::SharedMemoryHandle host_shm_handle = shm->handle().Duplicate();
  *host_handle_id =
      content::PepperPluginInstance::Get(instance)
          ->GetVarTracker()
          ->TrackSharedMemoryHandle(instance, host_shm_handle, size);

  // We set auto_close to false since we need our file descriptor to
  // actually be duplicated on linux. The shared memory destructor will
  // close the original handle for us.
  plugin_handle->set_shmem(
      host_->ShareSharedMemoryHandleWithRemote(host_shm_handle), size);
}

// Copyright 2014-2018 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EWK_EFL_INTEGRATION_WRT_V8WIDGET_H_
#define EWK_EFL_INTEGRATION_WRT_V8WIDGET_H_

#include <string>

#include "url/gurl.h"
#include "v8/include/v8.h"

class DynamicPlugin;
namespace base {
class CommandLine;
}
namespace content {
class RenderThreadObserver;
}

// Have to be created on the RenderThread.
class V8Widget {
 public:
  enum class Type { HBBTV, WRT };

  virtual ~V8Widget() {}  // LCOV_EXCL_LINE

  virtual content::RenderThreadObserver* GetObserver() = 0;

  bool ParseUrl(const GURL& url, GURL& new_url, bool* is_encrypted_file);
#if defined(OS_TIZEN_TV_PRODUCT)
  bool GetFileDecryptedDataBuffer(const GURL& url, std::vector<char>* data);
#endif

  void SetId(const std::string& id) { id_ = id; }
  Type GetType() const { return type_; }
  void SetPlugin(DynamicPlugin* plugin) { plugin_ = plugin; }  // LCOV_EXCL_LINE

  virtual void StartSession(v8::Handle<v8::Context>,
                            int routing_handle,
                            const char* session_blob);
  void StopSession(v8::Handle<v8::Context>);

  static V8Widget* CreateWidget(Type type,
                                const base::CommandLine& command_line);

 protected:
  /* LCOV_EXCL_START */
  explicit V8Widget(Type type) : type_(type), plugin_(nullptr) {}
  /* LCOV_EXCL_STOP */

  std::string id_;
  Type type_;
  DynamicPlugin* plugin_;
};

#endif  // EWK_EFL_INTEGRATION_WRT_V8WIDGET_H_

// Copyright 2014 The Chromium Authors. All rights reserved.
// Copyright 2016 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EWK_EFL_INTEGRATION_BROWSER_PUSH_MESSAGING_PUSH_MESSAGING_APP_IDENTIFIER_EFL_H_
#define EWK_EFL_INTEGRATION_BROWSER_PUSH_MESSAGING_PUSH_MESSAGING_APP_IDENTIFIER_EFL_H_

#include <stdint.h>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "url/gurl.h"

namespace content {
class BrowserContext;
}

// Type used to identify a Service Worker registration from a Push API
// perspective. These can be persisted to prefs, in a 1:1 mapping between
// app_id (which includes origin) and service_worker_registration_id.
// Legacy mapped values saved by old versions of Chrome are also supported;
// these don't contain the origin in the app_id, so instead they map from
// app_id to pair<origin, service_worker_registration_id>.
class PushMessagingAppIdentifierEfl {
 public:
  // Generates a new app identifier, with partially random app_id.
  static PushMessagingAppIdentifierEfl Generate(
      const GURL& origin,
      int64_t service_worker_registration_id);

  static PushMessagingAppIdentifierEfl Generate(const std::string& app_id);

  static PushMessagingAppIdentifierEfl GenerateFromPushData(
      const std::string& push_data,
      std::string& data);

  ~PushMessagingAppIdentifierEfl();

  // Returns true if this identifier does not represent an app (i.e. this was
  // returned by a failed Find call).
  bool is_null() const { return service_worker_registration_id_ < 0; }

  // String that should be passed to push services like GCM to identify a
  // particular Service Worker (so we can route incoming messages). Example:
  // wp:https://foo.example.com:8443/#9CC55CCE-B8F9-4092-A364-3B0F73A3AB5F
  // Legacy app_ids have no origin, e.g. wp:9CC55CCE-B8F9-4092-A364-3B0F73A3AB5F
  const std::string& app_id() const {
    DCHECK(!is_null());
    return app_id_;
  }

  const GURL& origin() const {
    DCHECK(!is_null());
    return origin_;
  }

  int64_t service_worker_registration_id() const {
    DCHECK(!is_null());
    return service_worker_registration_id_;
  }

  std::string push_data(const std::string& data) const;

 private:
  // Constructs an invalid app identifier.
  PushMessagingAppIdentifierEfl();
  // Constructs a valid app identifier.
  PushMessagingAppIdentifierEfl(const std::string& app_id,
                                const GURL& origin,
                                int64_t service_worker_registration_id);

  // Validates that all the fields contain valid values.
  void DCheckValid() const;

  std::string app_id_;
  GURL origin_;
  int64_t service_worker_registration_id_;
};

#endif  // EWK_EFL_INTEGRATION_BROWSER_PUSH_MESSAGING_PUSH_MESSAGING_APP_IDENTIFIER_EFL_H_

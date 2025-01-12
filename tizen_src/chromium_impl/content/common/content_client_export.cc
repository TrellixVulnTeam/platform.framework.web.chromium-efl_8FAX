// Copyright 2015 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content_client_export.h"

#include "content/public/common/content_client.h"

namespace content {

ContentClient* GetContentClientExport() {
  return GetContentClient();
}

}  // namespace content


// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_WEBKIT_COMMON_MIME_UTIL_MIME_UTIL_H__
#define THIRD_PARTY_WEBKIT_COMMON_MIME_UTIL_MIME_UTIL_H__

#include <string>
#include "third_party/WebKit/common/common_export.h"

namespace blink {

// Check to see if a particular MIME type is in the list of
// supported/recognized MIME types.
bool BLINK_COMMON_EXPORT IsSupportedImageMimeType(const std::string& mime_type);
bool BLINK_COMMON_EXPORT
IsSupportedNonImageMimeType(const std::string& mime_type);
bool BLINK_COMMON_EXPORT
IsUnsupportedTextMimeType(const std::string& mime_type);
bool BLINK_COMMON_EXPORT
IsSupportedJavascriptMimeType(const std::string& mime_type);

// Convenience function.
bool BLINK_COMMON_EXPORT IsSupportedMimeType(const std::string& mime_type);

#if defined(OS_TIZEN_TV_PRODUCT)
void BLINK_COMMON_EXPORT RegisterJavascriptPluginMimeTypes(const std::string&);
bool BLINK_COMMON_EXPORT
IsSupportedJavascriptPluginMimeType(const std::string&);
#endif

}  // namespace blink

#endif  // THIRD_PARTY_WEBKIT_COMMON_MIME_UTIL_MIME_UTIL_H__

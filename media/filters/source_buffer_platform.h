// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_SOURCE_BUFFER_PLATFORM_H_
#define MEDIA_FILTERS_SOURCE_BUFFER_PLATFORM_H_

#include <stddef.h>

#include "media/base/media_export.h"

namespace media {

// The maximum amount of data in bytes the stream will keep in memory.
MEDIA_EXPORT extern const size_t kSourceBufferAudioMemoryLimit;
#if defined(OS_TIZEN_TV_PRODUCT)
MEDIA_EXPORT extern const size_t kSourceBufferVideoMemoryLimitForFHD;
MEDIA_EXPORT extern const size_t kSourceBufferVideoMemoryLimitForUHD;
#else
MEDIA_EXPORT extern const size_t kSourceBufferVideoMemoryLimit;
#endif

}  // namespace media

#endif  // MEDIA_FILTERS_SOURCE_BUFFER_PLATFORM_H_

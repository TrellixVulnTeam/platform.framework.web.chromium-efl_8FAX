// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_AUDIO_DECODER_H_
#define CONTENT_RENDERER_MEDIA_AUDIO_DECODER_H_

#include <stddef.h>

namespace blink { class WebAudioBus; }

namespace content {

// Decode in-memory audio file data.
bool DecodeAudioFileData(blink::WebAudioBus* destination_bus, const char* data,
                         size_t data_size);

#if defined(TIZEN_MULTIMEDIA_SUPPORT)
void PlatformIsShutDown();
#endif

}  // namespace content

#endif  // CONTENT_RENDERER_MEDIA_AUDIO_DECODER_H_

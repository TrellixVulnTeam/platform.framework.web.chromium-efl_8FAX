// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// This file contains commonly used definitions of video capture.

#ifndef CONTENT_COMMON_MEDIA_VIDEO_CAPTURE_H_
#define CONTENT_COMMON_MEDIA_VIDEO_CAPTURE_H_

#include "base/callback.h"
#include "media/capture/video_capture_types.h"
#include "media/capture/video_capturer_source.h"

namespace content {

using VideoCaptureDeviceFormatsCB =
    media::VideoCapturerSource::VideoCaptureDeviceFormatsCB;

using VideoCaptureDeliverFrameCB =
    media::VideoCapturerSource::VideoCaptureDeliverFrameCB;

using VideoCaptureDecoderDataCB =
    media::VideoCapturerSource::VideoCaptureDecoderDataCB;

using SuspendingDecoderCB = media::VideoCapturerSource::SuspendingDecoderCB;

// Current status of the video capture device. It's used by multiple classes in
// browser process and renderer process. Browser process sends information about
// the current capture state and error to the renderer process using this type.
enum VideoCaptureState {
  VIDEO_CAPTURE_STATE_STARTING,
  VIDEO_CAPTURE_STATE_STARTED,
  VIDEO_CAPTURE_STATE_PAUSED,
  VIDEO_CAPTURE_STATE_RESUMED,
  VIDEO_CAPTURE_STATE_STOPPING,
  VIDEO_CAPTURE_STATE_STOPPED,
  VIDEO_CAPTURE_STATE_ERROR,
  VIDEO_CAPTURE_STATE_ENDED,
  VIDEO_CAPTURE_STATE_LAST = VIDEO_CAPTURE_STATE_ENDED
};

using VideoCaptureStateUpdateCB = base::Callback<void(VideoCaptureState)>;

}  // namespace content

#endif  // CONTENT_COMMON_MEDIA_VIDEO_CAPTURE_H_

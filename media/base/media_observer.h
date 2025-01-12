// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_MEDIA_OBSERVER_H_
#define MEDIA_BASE_MEDIA_OBSERVER_H_

#include "media/base/cdm_context.h"
#include "media/base/pipeline_metadata.h"
#include "url/gurl.h"

namespace media {

class MEDIA_EXPORT MediaObserverClient {
 public:
  virtual ~MediaObserverClient() {}

  // Requests to restart the media pipeline and create a new renderer as soon as
  // possible. When switching to remote renderer, all the optimizations that
  // might suspend the media pipeline should be disabled.
  // |remote_device_friendly_name| can be empty if the remote device is unknown.
  virtual void SwitchToRemoteRenderer(
      const std::string& remote_device_friendly_name) = 0;
  virtual void SwitchToLocalRenderer() = 0;

  // Requests to activate monitoring changes on viewport intersection.
  virtual void ActivateViewportIntersectionMonitoring(bool activate) = 0;

  // Reports the latest compatibility state of the element's source for remote
  // playback.
  virtual void UpdateRemotePlaybackCompatibility(bool is_compatible) = 0;

  // Gets the number of audio/video bytes/video frames decoded so far from the
  // media pipeline. All the counts keep increasing and will not be reset during
  // seek.
  virtual size_t AudioDecodedByteCount() const = 0;
  virtual size_t VideoDecodedByteCount() const = 0;
  virtual unsigned DecodedFrameCount() const = 0;
};

// This class is an observer of media player events.
class MEDIA_EXPORT MediaObserver {
 public:
  MediaObserver();
  virtual ~MediaObserver();

  // Called when the media element starts/stops being the dominant visible
  // content.
  virtual void OnBecameDominantVisibleContent(bool is_dominant) {}

  // Called when CDM is attached to the media element. The |cdm_context| is
  // only guaranteed to be valid in this call.
  virtual void OnSetCdm(CdmContext* cdm_context) = 0;

  // Called after demuxer is initialized.
  virtual void OnMetadataChanged(const PipelineMetadata& metadata) = 0;

  // Called to indicate whether the site requests that remote playback be
  // disabled. The "disabled" naming corresponds with the
  // "disableRemotePlayback" media element attribute, as described in the
  // Remote Playback API spec: https://w3c.github.io/remote-playback
  virtual void OnRemotePlaybackDisabled(bool disabled) = 0;

  // Called when the media is playing/paused.
  virtual void OnPlaying() = 0;
  virtual void OnPaused() = 0;

  // Called when the data source is asynchronously initialized.
  virtual void OnDataSourceInitialized(const GURL& url_after_redirects) = 0;

  // Set the MediaObserverClient.
  virtual void SetClient(MediaObserverClient* client) = 0;
};

}  // namespace media

#endif  // MEDIA_BASE_MEDIA_OBSERVER_H_

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DECODER_BUFFER_H_
#define MEDIA_BASE_DECODER_BUFFER_H_

#include <stddef.h>
#include <stdint.h>

#include <memory>
#include <string>
#include <utility>

#include "base/logging.h"
#include "base/macros.h"
#include "base/memory/aligned_memory.h"
#include "base/memory/ref_counted.h"
#include "base/scoped_generic.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "media/base/decrypt_config.h"
#include "media/base/media_export.h"
#include "media/base/timestamp_constants.h"

namespace media {

#if defined(OS_TIZEN_TV_PRODUCT)
// Trusted zone data handle. A structure used to encapsulate
// a handle to decrypted data content stored in trusted zone.
class TzDataHandle {
 public:
  TzDataHandle(int handle, int size) : handle_(handle), size_(size) {}

  int handle() const { return handle_; }
  int size() const { return size_; }

  bool operator==(const TzDataHandle& h) const {
    return handle() == h.handle() && size() == h.size();
  }

  bool operator!=(const TzDataHandle& h) const { return !(*this == h); }

 private:
  int handle_;
  int size_;
};

// Releases a handle from trusted zone.
int ReleaseTzHandle(int tz_handle, int size);

// Scoped trait releasing handle from trusted zone.
struct TzDataHandleScopedTraits {
  static TzDataHandle InvalidValue();
  static void Free(TzDataHandle& f);
};

typedef base::ScopedGeneric<TzDataHandle, TzDataHandleScopedTraits>
    TzDataHandleScoped;
#endif

// A specialized buffer for interfacing with audio / video decoders.
//
// Specifically ensures that data is aligned and padded as necessary by the
// underlying decoding framework.  On desktop platforms this means memory is
// allocated using FFmpeg with particular alignment and padding requirements.
//
// Also includes decoder specific functionality for decryption.
//
// NOTE: It is illegal to call any method when end_of_stream() is true.
class MEDIA_EXPORT DecoderBuffer
    : public base::RefCountedThreadSafe<DecoderBuffer> {
 public:
  enum {
    kPaddingSize = 32,
#if defined(ARCH_CPU_ARM_FAMILY)
    kAlignmentSize = 16
#else
    kAlignmentSize = 32
#endif
  };

  // Allocates buffer with |size| >= 0.  Buffer will be padded and aligned
  // as necessary, and |is_key_frame_| will default to false.
  explicit DecoderBuffer(size_t size);

  // Create a DecoderBuffer whose |data_| is copied from |data|.  Buffer will be
  // padded and aligned as necessary.  |data| must not be NULL and |size| >= 0.
  // The buffer's |is_key_frame_| will default to false.
  static scoped_refptr<DecoderBuffer> CopyFrom(const uint8_t* data,
                                               size_t size);

  // Create a DecoderBuffer whose |data_| is copied from |data| and |side_data_|
  // is copied from |side_data|. Buffers will be padded and aligned as necessary
  // Data pointers must not be NULL and sizes must be >= 0. The buffer's
  // |is_key_frame_| will default to false.
  static scoped_refptr<DecoderBuffer> CopyFrom(const uint8_t* data,
                                               size_t size,
                                               const uint8_t* side_data,
                                               size_t side_data_size);

  // Create a DecoderBuffer indicating we've reached end of stream.
  //
  // Calling any method other than end_of_stream() on the resulting buffer
  // is disallowed.
  static scoped_refptr<DecoderBuffer> CreateEOSBuffer();
#if defined(OS_TIZEN_TV_PRODUCT)
  static scoped_refptr<DecoderBuffer> CreateTzHandleBuffer(int handle,
                                                           int size);
#endif

  base::TimeDelta timestamp() const {
    DCHECK(!end_of_stream());
    return timestamp_;
  }

  // TODO(dalecurtis): This should be renamed at some point, but to avoid a yak
  // shave keep as a virtual with hacker_style() for now.
  virtual void set_timestamp(base::TimeDelta timestamp);

  base::TimeDelta duration() const {
    DCHECK(!end_of_stream());
    return duration_;
  }

  void set_duration(base::TimeDelta duration) {
    DCHECK(!end_of_stream());
    DCHECK(duration == kNoTimestamp ||
           (duration >= base::TimeDelta() && duration != kInfiniteDuration))
        << duration.InSecondsF();
    duration_ = duration;
  }

  const uint8_t* data() const {
    DCHECK(!end_of_stream());
    return data_.get();
  }

  uint8_t* writable_data() const {
    DCHECK(!end_of_stream());
    return data_.get();
  }

  size_t data_size() const {
    DCHECK(!end_of_stream());
    return size_;
  }

  const uint8_t* side_data() const {
    DCHECK(!end_of_stream());
    return side_data_.get();
  }

  size_t side_data_size() const {
    DCHECK(!end_of_stream());
    return side_data_size_;
  }

  // A discard window indicates the amount of data which should be discard from
  // this buffer after decoding.  The first value is the amount of the front and
  // the second the amount off the back.  A value of kInfiniteDuration for the
  // first value indicates the entire buffer should be discarded; the second
  // value must be base::TimeDelta() in this case.
  typedef std::pair<base::TimeDelta, base::TimeDelta> DiscardPadding;
  const DiscardPadding& discard_padding() const {
    DCHECK(!end_of_stream());
    return discard_padding_;
  }

  void set_discard_padding(const DiscardPadding& discard_padding) {
    DCHECK(!end_of_stream());
    discard_padding_ = discard_padding;
  }

  const DecryptConfig* decrypt_config() const {
    DCHECK(!end_of_stream());
    return decrypt_config_.get();
  }

  void set_decrypt_config(std::unique_ptr<DecryptConfig> decrypt_config) {
    DCHECK(!end_of_stream());
    decrypt_config_ = std::move(decrypt_config);
  }

  // If there's no data in this buffer, it represents end of stream.
  bool end_of_stream() const {
#if defined(OS_TIZEN_TV_PRODUCT)
    return data_ == NULL && !is_tz_handle_valid();
#else
    return data_ == NULL;
#endif
  }

  bool is_key_frame() const {
    DCHECK(!end_of_stream());
    return is_key_frame_;
  }

  void set_is_key_frame(bool is_key_frame) {
    DCHECK(!end_of_stream());
    is_key_frame_ = is_key_frame;
  }

  // Returns true if all fields in |buffer| matches this buffer
  // including |data_| and |side_data_|.
  bool MatchesForTesting(const DecoderBuffer& buffer) const;

#if defined(OS_TIZEN_TV_PRODUCT)
  void set_tz_handle(TzDataHandle tz_handle) { tz_handle_.reset(tz_handle); }

  const TzDataHandle& get_tz_handle() const { return tz_handle_.get(); }

  bool is_tz_handle_valid() const { return tz_handle_.is_valid(); }

  TzDataHandle pass_tz_handle() { return tz_handle_.release(); }
#endif

  // Returns a human-readable string describing |*this|.
  std::string AsHumanReadableString() const;

  // Replaces any existing side data with data copied from |side_data|.
  void CopySideDataFrom(const uint8_t* side_data, size_t side_data_size);

 protected:
  friend class base::RefCountedThreadSafe<DecoderBuffer>;

  // Allocates a buffer of size |size| >= 0 and copies |data| into it.  Buffer
  // will be padded and aligned as necessary.  If |data| is NULL then |data_| is
  // set to NULL and |buffer_size_| to 0.  |is_key_frame_| will default to
  // false.
  DecoderBuffer(const uint8_t* data,
                size_t size,
                const uint8_t* side_data,
                size_t side_data_size);
  virtual ~DecoderBuffer();

 private:
  base::TimeDelta timestamp_;
  base::TimeDelta duration_;

  size_t size_;
  std::unique_ptr<uint8_t, base::AlignedFreeDeleter> data_;
  size_t side_data_size_;
  std::unique_ptr<uint8_t, base::AlignedFreeDeleter> side_data_;
  std::unique_ptr<DecryptConfig> decrypt_config_;
  DiscardPadding discard_padding_;
#if defined(OS_TIZEN_TV_PRODUCT)
  TzDataHandleScoped tz_handle_;
#endif
  bool is_key_frame_;

  // Constructor helper method for memory allocations.
  void Initialize();

  DISALLOW_COPY_AND_ASSIGN(DecoderBuffer);
};

}  // namespace media

#endif  // MEDIA_BASE_DECODER_BUFFER_H_

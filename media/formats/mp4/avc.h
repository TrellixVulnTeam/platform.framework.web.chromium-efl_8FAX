// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FORMATS_MP4_AVC_H_
#define MEDIA_FORMATS_MP4_AVC_H_

#include <stddef.h>
#include <stdint.h>

#include <memory>
#include <vector>

#include "base/macros.h"
#include "media/base/media_export.h"
#include "media/formats/mp4/bitstream_converter.h"
#include "media/media_features.h"
#include "media/video/h264_parser.h"

namespace media {

struct SubsampleEntry;

namespace mp4 {

struct AVCDecoderConfigurationRecord;

class MEDIA_EXPORT AVC {
 public:
  static bool ConvertFrameToAnnexB(int length_size,
                                   std::vector<uint8_t>* buffer,
                                   std::vector<SubsampleEntry>* subsamples);

  // Inserts the SPS & PPS data from |avc_config| into |buffer|.
  // |buffer| is expected to contain AnnexB conformant data.
  // |subsamples| contains the SubsampleEntry info if |buffer| contains
  // encrypted data.
  // Returns true if the param sets were successfully inserted.
  static bool InsertParamSetsAnnexB(
      const AVCDecoderConfigurationRecord& avc_config,
      std::vector<uint8_t>* buffer,
      std::vector<SubsampleEntry>* subsamples);

  static bool ConvertConfigToAnnexB(
      const AVCDecoderConfigurationRecord& avc_config,
      std::vector<uint8_t>* buffer);

  static std::vector<SubsampleEntry> CreateNALSubsamples(std::vector<uint8_t>* buffer);
  static bool CreateSubsampleEncryptionEntries(
      media::H264Parser& parser,
      std::vector<uint8_t>* buffer,
      std::vector<SubsampleEntry>* subsamples,
      std::vector<SubsampleEntry>& nal_subsamples);

  // Verifies that the contents of |buffer| conform to
  // Section 7.4.1.2.3 of ISO/IEC 14496-10.
  // |subsamples| contains the information about what parts of the buffer are
  // encrypted and which parts are clear.
  // Returns true if |buffer| contains conformant Annex B data
  // TODO(acolwell): Remove the std::vector version when we can use,
  // C++11's std::vector<T>::data() method.
  static bool IsValidAnnexB(const std::vector<uint8_t>& buffer,
                            const std::vector<SubsampleEntry>& subsamples);
  static bool IsValidAnnexB(const uint8_t* buffer,
                            size_t size,
                            const std::vector<SubsampleEntry>& subsamples);

  // Given a |buffer| and |subsamples| information and |pts| pointer into the
  // |buffer| finds the index of the subsample |ptr| is pointing into.
  static int FindSubsampleIndex(const std::vector<uint8_t>& buffer,
                                const std::vector<SubsampleEntry>* subsamples,
                                const uint8_t* ptr);
};

// AVCBitstreamConverter converts AVC/H.264 bitstream from MP4 container format
// with embedded NALU lengths into AnnexB bitstream format (described in ISO/IEC
// 14496-10) with 4-byte start codes. It also knows how to handle CENC-encrypted
// streams and adjusts subsample data for those streams while converting.
class AVCBitstreamConverter : public BitstreamConverter {
 public:
  explicit AVCBitstreamConverter(
      std::unique_ptr<AVCDecoderConfigurationRecord> avc_config);

#if BUILDFLAG(ENABLE_DOLBY_VISION_DEMUXING)
  // TODO(erickung): Dolby Vision should have its own subclasses of the
  // bitstream converters so that the validation logic can properly accommodate
  // it.
  void disable_validation() { disable_validation_ = true; }
#endif  // BUILDFLAG(ENABLE_DOLBY_VISION_DEMUXING)

  // BitstreamConverter interface
  bool ConvertFrame(std::vector<uint8_t>* frame_buf,
                    bool is_keyframe,
                    std::vector<SubsampleEntry>* subsamples,
                    bool need_subsamples) const override;

  bool IsValid(std::vector<uint8_t>* frame_buf,
               std::vector<SubsampleEntry>* subsamples) const override;

 private:
  ~AVCBitstreamConverter() override;
  std::unique_ptr<AVCDecoderConfigurationRecord> avc_config_;

  // used by CreateSubsampleEncryptionEntries
  std::unique_ptr<media::H264Parser> parser_;

#if BUILDFLAG(ENABLE_DOLBY_VISION_DEMUXING)
  // Annex B validation is short-circuited when true.
  bool disable_validation_;
#endif  // BUILDFLAG(ENABLE_DOLBY_VISION_DEMUXING)

  DISALLOW_COPY_AND_ASSIGN(AVCBitstreamConverter);
};

}  // namespace mp4
}  // namespace media

#endif  // MEDIA_FORMATS_MP4_AVC_H_

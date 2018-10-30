/* Copyright (c) 2016 Samsung Electronics. All rights reserved.
 */

/* From samsung/pp_media_codecs_samsung.idl,
 *   modified Tue Nov 15 13:36:56 2016.
 */

#ifndef PPAPI_C_SAMSUNG_PP_MEDIA_CODECS_SAMSUNG_H_
#define PPAPI_C_SAMSUNG_PP_MEDIA_CODECS_SAMSUNG_H_

#include "ppapi/c/pp_macros.h"

/**
 * @file
 *
 * This file defines common audio and video codes.
 *
 * Below enums (FORMAT, PROFILE, TYPE) are based
 * on these defined in chromium's media namespace.
 */


/**
 * @addtogroup Enums
 * @{
 */
/* Enum describing audio codec types.
 * Do not change their ordinal values. When deleting a codec replace it
 * with a dummy value; when adding a codec, do so at the bottom
 * before PP_AUDIOCODEC_TYPE_SAMSUNG_MAX.
 */
typedef enum {
  PP_AUDIOCODEC_TYPE_SAMSUNG_UNKNOWN = 0,
  PP_AUDIOCODEC_TYPE_SAMSUNG_AAC = 1,
  PP_AUDIOCODEC_TYPE_SAMSUNG_MP3 = 2,
  PP_AUDIOCODEC_TYPE_SAMSUNG_PCM = 3,
  PP_AUDIOCODEC_TYPE_SAMSUNG_VORBIS = 4,
  PP_AUDIOCODEC_TYPE_SAMSUNG_FLAC = 5,
  PP_AUDIOCODEC_TYPE_SAMSUNG_AMR_NB = 6,
  PP_AUDIOCODEC_TYPE_SAMSUNG_AMR_WB = 7,
  PP_AUDIOCODEC_TYPE_SAMSUNG_PCM_MULAW = 8,
  PP_AUDIOCODEC_TYPE_SAMSUNG_GSM_MS = 9,
  PP_AUDIOCODEC_TYPE_SAMSUNG_PCM_S16BE = 10,
  PP_AUDIOCODEC_TYPE_SAMSUNG_PCM_S24BE = 11,
  PP_AUDIOCODEC_TYPE_SAMSUNG_OPUS = 12,
  PP_AUDIOCODEC_TYPE_SAMSUNG_EAC3 = 13,
  PP_AUDIOCODEC_TYPE_SAMSUNG_MP2 = 14,
  PP_AUDIOCODEC_TYPE_SAMSUNG_DTS = 15,
  PP_AUDIOCODEC_TYPE_SAMSUNG_AC3 = 16,
  PP_AUDIOCODEC_TYPE_SAMSUNG_WMAV1 = 17,
  PP_AUDIOCODEC_TYPE_SAMSUNG_WMAV2 = 18,
  /* Must always be last! */
  PP_AUDIOCODEC_TYPE_SAMSUNG_MAX
} PP_AudioCodec_Type_Samsung;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_AudioCodec_Type_Samsung, 4);

/* Enum  describing Audio Codec Profiles. */
typedef enum {
  PP_AUDIOCODEC_PROFILE_SAMSUNG_UNKNOWN = -1,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_AAC_MAIN = 0,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_AAC_LOW = 1,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_AAC_SSR = 2,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_AAC_LTP = 3,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_AAC_HE = 4,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_AAC_HE_V2 = 5,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_AAC_LD = 6,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_AAC_ELD = 7,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_MPEG2_AAC_LOW = 8,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_MPEG2_AAC_HE = 9,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_DTS = 10,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_DTS_ES = 11,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_DTS_96_24 = 12,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_DTS_HD_HRA = 13,
  PP_AUDIOCODEC_PROFILE_SAMSUNG_DTS_HD_MA = 14,
  /* this is the really last profile, could be used for checking supported
   * profile sets.
   */
  PP_AUDIOCODEC_PROFILE_SAMSUNG_LAST = PP_AUDIOCODEC_PROFILE_SAMSUNG_DTS_HD_MA
} PP_AudioCodec_Profile_Samsung;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_AudioCodec_Profile_Samsung, 4);

/* Enum describing Samples format. */
typedef enum {
  PP_SAMPLEFORMAT_SAMSUNG_UNKNOWN = 0,
  /* Unsigned 8-bit w/ bias of 128. */
  PP_SAMPLEFORMAT_SAMSUNG_U8 = 1,
  /* Signed 16-bit. */
  PP_SAMPLEFORMAT_SAMSUNG_S16 = 2,
  /* Signed 32-bit. */
  PP_SAMPLEFORMAT_SAMSUNG_S32 = 3,
  /* Float 32-bit. */
  PP_SAMPLEFORMAT_SAMSUNG_F32 = 4,
  /* Signed 16-bit planar. */
  PP_SAMPLEFORMAT_SAMSUNG_PLANARS16 = 5,
  /* Float 32-bit planar. */
  PP_SAMPLEFORMAT_SAMSUNG_PLANARF32 = 6,
  /* Must always be last! */
  PP_SAMPLEFORMAT_SAMSUNG_MAX
} PP_SampleFormat_Samsung;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_SampleFormat_Samsung, 4);

/* Enumerates the various representations of the ordering of audio channels.
 * Logged to UMA, so never reuse a value, always add new/greater ones!
 */
typedef enum {
  PP_CHANNEL_LAYOUT_SAMSUNG_NONE = 0,
  PP_CHANNEL_LAYOUT_SAMSUNG_UNSUPPORTED = 1,
  /* Front C */
  PP_CHANNEL_LAYOUT_SAMSUNG_MONO = 2,
  /* Front L, Front R */
  PP_CHANNEL_LAYOUT_SAMSUNG_STEREO = 3,
  /* Front L, Front R, Back C */
  PP_CHANNEL_LAYOUT_SAMSUNG_2_1 = 4,
  /* Front L, Front R, Front C */
  PP_CHANNEL_LAYOUT_SAMSUNG_SURROUND = 5,
  /* Front L, Front R, Front C, Back C */
  PP_CHANNEL_LAYOUT_SAMSUNG_4_0 = 6,
  /* Front L, Front R, Side L, Side R */
  PP_CHANNEL_LAYOUT_SAMSUNG_2_2 = 7,
  /* Front L, Front R, Back L, Back R */
  PP_CHANNEL_LAYOUT_SAMSUNG_QUAD = 8,
  /* Front L, Front R, Front C, Side L, Side R */
  PP_CHANNEL_LAYOUT_SAMSUNG_5_0 = 9,
  /* Front L, Front R, Front C, Side L, Side R, LFE */
  PP_CHANNEL_LAYOUT_SAMSUNG_5_1 = 10,
  /* Front L, Front R, Front C, Back L, Back R */
  PP_CHANNEL_LAYOUT_SAMSUNG_5_0_BACK = 11,
  /* Front L, Front R, Front C, Back L, Back R, LFE */
  PP_CHANNEL_LAYOUT_SAMSUNG_5_1_BACK = 12,
  /* Front L, Front R, Front C, Side L, Side R, Back L, Back R */
  PP_CHANNEL_LAYOUT_SAMSUNG_7_0 = 13,
  /* Front L, Front R, Front C, Side L, Side R, LFE, Back L, Back R */
  PP_CHANNEL_LAYOUT_SAMSUNG_7_1 = 14,
  /* Front L, Front R, Front C, Side L, Side R, LFE, Front LofC, Front RofC */
  PP_CHANNEL_LAYOUT_SAMSUNG_7_1_WIDE = 15,
  /* Stereo L, Stereo R */
  PP_CHANNEL_LAYOUT_SAMSUNG_STEREO_DOWNMIX = 16,
  /* Stereo L, Stereo R, LFE */
  PP_CHANNEL_LAYOUT_SAMSUNG_2POINT1 = 17,
  /* Stereo L, Stereo R, Front C, LFE */
  PP_CHANNEL_LAYOUT_SAMSUNG_3_1 = 18,
  /* Stereo L, Stereo R, Front C, Rear C, LFE */
  PP_CHANNEL_LAYOUT_SAMSUNG_4_1 = 19,
  /* Stereo L, Stereo R, Front C, Side L, Side R, Back C */
  PP_CHANNEL_LAYOUT_SAMSUNG_6_0 = 20,
  /* Stereo L, Stereo R, Side L, Side R, Front LofC, Front RofC */
  PP_CHANNEL_LAYOUT_SAMSUNG_6_0_FRONT = 21,
  /* Stereo L, Stereo R, Side L, Side R, Front C, Rear C. */
  PP_CHANNEL_LAYOUT_SAMSUNG_HEXAGONAL = 22,
  /* Stereo L, Stereo R, Side L, Side R, Front C, Rear Center, LFE */
  PP_CHANNEL_LAYOUT_SAMSUNG_6_1 = 23,
  /* Stereo L, Stereo R, Back L, Back R, Front C, Rear Center, LFE */
  PP_CHANNEL_LAYOUT_SAMSUNG_6_1_BACK = 24,
  /* Stereo L, Stereo R, Side L, Side R, Front LofC, Front RofC, LFE */
  PP_CHANNEL_LAYOUT_SAMSUNG_6_1_FRONT = 25,
  /* Front L, Front R, Front C, Side L, Side R, Front LofC, Front RofC */
  PP_CHANNEL_LAYOUT_SAMSUNG_7_0_FRONT = 26,
  /* Front L, Front R, Front C, Back L, Back R, LFE, Front LofC, Front RofC */
  PP_CHANNEL_LAYOUT_SAMSUNG_7_1_WIDE_BACK = 27,
  /* Front L, Front R, Front C, Side L, Side R, Rear C, Back L, Back R. */
  PP_CHANNEL_LAYOUT_SAMSUNG_OCTAGONAL = 28,
  /* Channels are not explicitly mapped to speakers. */
  PP_CHANNEL_LAYOUT_SAMSUNG_DISCRETE = 29,
  /* Total number of layouts.
   * Must always be last!
   */
  PP_CHANNEL_LAYOUT_SAMSUNG_MAX
} PP_ChannelLayout_Samsung;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_ChannelLayout_Samsung, 4);

/* Enum describing video codecs type. */
typedef enum {
  PP_VIDEOCODEC_TYPE_SAMSUNG_UNKNOWN = 0,
  PP_VIDEOCODEC_TYPE_SAMSUNG_H264 = 1,
  PP_VIDEOCODEC_TYPE_SAMSUNG_VC1 = 2,
  PP_VIDEOCODEC_TYPE_SAMSUNG_MPEG2 = 3,
  PP_VIDEOCODEC_TYPE_SAMSUNG_MPEG4 = 4,
  PP_VIDEOCODEC_TYPE_SAMSUNG_THEORA = 5,
  PP_VIDEOCODEC_TYPE_SAMSUNG_VP8 = 6,
  PP_VIDEOCODEC_TYPE_SAMSUNG_VP9 = 7,
  PP_VIDEOCODEC_TYPE_SAMSUNG_H263 = 8,
  PP_VIDEOCODEC_TYPE_SAMSUNG_WMV1 = 9,
  PP_VIDEOCODEC_TYPE_SAMSUNG_WMV2 = 10,
  PP_VIDEOCODEC_TYPE_SAMSUNG_WMV3 = 11,
  PP_VIDEOCODEC_TYPE_SAMSUNG_INDEO3 = 12,
  PP_VIDEOCODEC_TYPE_SAMSUNG_H265 = 13,
  /* PP_VIDEOCODEC_TYPE_SAMSUNG_MAX - last codec from chromium's set.
   */
  PP_VIDEOCODEC_TYPE_SAMSUNG_MAX = PP_VIDEOCODEC_TYPE_SAMSUNG_VP9,
  /* Must equal the last "real" codec above.
   * Used to determine absolute set size.
   */
  PP_VIDEOCODEC_TYPE_SAMSUNG_LAST = PP_VIDEOCODEC_TYPE_SAMSUNG_H265
} PP_VideoCodec_Type_Samsung;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VideoCodec_Type_Samsung, 4);

/* Video stream profile.
 * Keep the values in this enum unique, as they imply format (h.264 vs. VP8,
 * for example), and keep the values for a particular format grouped
 * together for clarity.
 */
typedef enum {
  PP_VIDEOCODEC_PROFILE_SAMSUNG_UNKNOWN = -1,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_MIN = 0,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_BASELINE =
    PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_MIN,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_MAIN = 1,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_EXTENDED = 2,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_HIGH = 3,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_HIGH10 = 4,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_HIGH422 = 5,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_HIGH444PREDICTIVE = 6,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_SCALABLEBASELINE = 7,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_SCALABLEHIGH = 8,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_STEREOHIGH = 9,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_MULTIVIEWHIGH = 10,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_MAX =
    PP_VIDEOCODEC_PROFILE_SAMSUNG_H264_MULTIVIEWHIGH,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VP8_MIN = 11,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VP8_MAIN =
    PP_VIDEOCODEC_PROFILE_SAMSUNG_VP8_MIN,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VP8_MAX =
    PP_VIDEOCODEC_PROFILE_SAMSUNG_VP8_MAIN,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VP9_MIN = 12,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VP9_MAIN =
    PP_VIDEOCODEC_PROFILE_SAMSUNG_VP9_MIN,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VP9_MAX =
    PP_VIDEOCODEC_PROFILE_SAMSUNG_VP9_MAIN,
  /* next profile means last from Chromium profiles set */
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MAX = PP_VIDEOCODEC_PROFILE_SAMSUNG_VP9_MAX,
  /* Next profiles expand Chromium set
   * and they are intentionally above PROFILE_MAX.
   */
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG2_MIN = 13,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG2_422 =
    PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG2_MIN,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG2_HIGH = 14,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG2_SS = 15,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG2_SNR_SCALABLE = 16,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG2_MAIN = 17,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG2_SIMPLE = 18,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VC1_SIMPLE = 19,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VC1_MAIN = 20,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VC1_COMPLEX = 21,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_VC1_ADVANCED = 22,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_SIMPLE = 23,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_SIMPLE_SCALABLE = 24,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_CORE = 25,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_MAIN = 26,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_N_BIT = 27,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_SCALABLE_TEXTURE = 28,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_SIMPLE_FACE_ANIMATION = 29,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_BASIC_ANIMATED_TEXTURE = 30,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_HYBRID = 31,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_ADVANCED_REAL_TIME = 32,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_CORE_SCALABLE = 33,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_ADVANCED_CODING = 34,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_ADVANCED_CORE = 35,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_ADVANCED_SCALABLE_TEXTURE = 36,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_SIMPLE_STUDIO = 37,
  PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_ADVANCED_SIMPLE = 38,
  /* this is the really last profile, it's could be used for checking
   * supported profile sets.
   */
  PP_VIDEOCODEC_PROFILE_SAMSUNG_LAST =
    PP_VIDEOCODEC_PROFILE_SAMSUNG_MPEG4_ADVANCED_SIMPLE
} PP_VideoCodec_Profile_Samsung;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VideoCodec_Profile_Samsung, 4);

/* Enum describing video frames formats. */
typedef enum {
  /* Invalid format value.  Used for error reporting. */
  PP_VIDEOFRAME_FORMAT_SAMSUNG_INVALID = 0,
  /* 32bpp RGB packed with extra byte 8:8:8 */
  PP_VIDEOFRAME_FORMAT_SAMSUNG_RGB32 = 4,
  /* 12bpp YVU planar 1x1 Y, 2x2 VU samples */
  PP_VIDEOFRAME_FORMAT_SAMSUNG_YV12 = 6,
  /* 16bpp YVU planar 1x1 Y, 2x1 VU samples */
  PP_VIDEOFRAME_FORMAT_SAMSUNG_YV16 = 7,
  /* An empty frame. */
  PP_VIDEOFRAME_FORMAT_SAMSUNG_EMPTY = 9,
  /* 12bpp YVU planar 1x1 Y, 2x2 UV samples. */
  PP_VIDEOFRAME_FORMAT_SAMSUNG_I420 = 11,
  /* Native texture.  Pixel-format agnostic. */
  PP_VIDEOFRAME_FORMAT_SAMSUNG_NATIVE_TEXTURE = 12,
  /* Hole frame. */
  PP_VIDEOFRAME_FORMAT_SAMSUNG_HOLE = 13,
  /* 20bpp YUVA planar 1x1 Y, 2x2 VU, 1x1 A samples. */
  PP_VIDEOFRAME_FORMAT_SAMSUNG_YV12A = 14,
  PP_VIDEOFRAME_FORMAT_SAMSUNG_LAST = PP_VIDEOFRAME_FORMAT_SAMSUNG_YV12A
} PP_VideoFrame_Format_Samsung;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VideoFrame_Format_Samsung, 4);
/**
 * @}
 */

#endif  /* PPAPI_C_SAMSUNG_PP_MEDIA_CODECS_SAMSUNG_H_ */

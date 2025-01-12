// Copyright 2014 Samsung Electronics Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// IPC messages for efl media player.
// Multiply-included message file, hence no include guard.

#include "content/common/content_export.h"
#include "content/common/media/media_param_traits_efl.h"
#include "content/common/media/media_player_init_config.h"
#include "content/common/media/media_player_messages_enums_efl.h"
#include "ipc/ipc_message_macros.h"
#include "media/base/ipc/media_param_traits_macros.h"
#include "media/base/ranges.h"
#include "media/base/tizen/demuxer_stream_player_params_efl.h"
#include "media/base/tizen/media_player_manager_efl.h"
#include "third_party/WebKit/public/platform/WebMediaPlayer.h"
#include "ui/gfx/geometry/rect_f.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START MediaPlayerMsgStart

IPC_ENUM_TRAITS(blink::WebMediaPlayer::ReadyState)
IPC_ENUM_TRAITS(blink::WebMediaPlayer::NetworkState)
IPC_ENUM_TRAITS(MediaPlayerHostMsg_Initialize_Type)

IPC_STRUCT_TRAITS_BEGIN(media::DemuxerConfigs)
IPC_STRUCT_TRAITS_MEMBER(audio_codec)
IPC_STRUCT_TRAITS_MEMBER(audio_channels)
IPC_STRUCT_TRAITS_MEMBER(audio_sampling_rate)
IPC_STRUCT_TRAITS_MEMBER(audio_bit_rate)
IPC_STRUCT_TRAITS_MEMBER(is_audio_encrypted)
IPC_STRUCT_TRAITS_MEMBER(audio_extra_data)

IPC_STRUCT_TRAITS_MEMBER(video_codec)
IPC_STRUCT_TRAITS_MEMBER(video_size)
IPC_STRUCT_TRAITS_MEMBER(is_video_encrypted)
IPC_STRUCT_TRAITS_MEMBER(video_extra_data)
#if defined(OS_TIZEN_TV_PRODUCT)
IPC_STRUCT_TRAITS_MEMBER(webm_hdr_info)
IPC_STRUCT_TRAITS_MEMBER(framerate_num)
IPC_STRUCT_TRAITS_MEMBER(framerate_den)
IPC_STRUCT_TRAITS_MEMBER(is_framerate_changed)
#endif

IPC_STRUCT_TRAITS_MEMBER(duration_ms)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(media::DemuxedBufferMetaData)
IPC_STRUCT_TRAITS_MEMBER(size)
IPC_STRUCT_TRAITS_MEMBER(end_of_stream)
IPC_STRUCT_TRAITS_MEMBER(timestamp)
IPC_STRUCT_TRAITS_MEMBER(time_duration)
IPC_STRUCT_TRAITS_MEMBER(type)
IPC_STRUCT_TRAITS_MEMBER(status)
#if defined(OS_TIZEN_TV_PRODUCT)
IPC_STRUCT_TRAITS_MEMBER(tz_handle)
#endif
IPC_STRUCT_TRAITS_END()

#if defined(TIZEN_TBM_SUPPORT)
IPC_STRUCT_TRAITS_BEGIN(gfx::TbmBufferHandle)
IPC_STRUCT_TRAITS_MEMBER(tbm_surface)
IPC_STRUCT_TRAITS_MEMBER(media_packet)
IPC_STRUCT_TRAITS_END()
#endif

#if defined(OS_TIZEN_TV_PRODUCT)
IPC_STRUCT_TRAITS_BEGIN(blink::WebMediaPlayer::register_timeline_cb_info_s)
IPC_STRUCT_TRAITS_MEMBER(timeline_selector)
IPC_STRUCT_TRAITS_MEMBER(units_per_tick)
IPC_STRUCT_TRAITS_MEMBER(units_per_second)
IPC_STRUCT_TRAITS_MEMBER(content_time)
IPC_STRUCT_TRAITS_MEMBER(timeline_state)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebMediaPlayer::audio_video_track_info_s)
IPC_STRUCT_TRAITS_MEMBER(id)
IPC_STRUCT_TRAITS_MEMBER(kind)
IPC_STRUCT_TRAITS_MEMBER(label)
IPC_STRUCT_TRAITS_MEMBER(language)
IPC_STRUCT_TRAITS_MEMBER(enabled)
IPC_STRUCT_TRAITS_END()
#endif

IPC_STRUCT_TRAITS_BEGIN(content::MediaPlayerInitConfig)
IPC_STRUCT_TRAITS_MEMBER(type)
IPC_STRUCT_TRAITS_MEMBER(url)
IPC_STRUCT_TRAITS_MEMBER(mime_type)
IPC_STRUCT_TRAITS_MEMBER(demuxer_client_id)
IPC_STRUCT_TRAITS_MEMBER(has_encrypted_listener_or_cdm)
IPC_STRUCT_TRAITS_END()

// Initialize Efl player.
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_Init,
                    int /* player_id */,
                    content::MediaPlayerInitConfig /* config */)

// Requests the player to enter fullscreen.
IPC_MESSAGE_ROUTED1(MediaPlayerEflHostMsg_EnteredFullscreen,
                    int /* player_id */)

// Requests the player to exit fullscreen.
IPC_MESSAGE_ROUTED1(MediaPlayerEflHostMsg_ExitedFullscreen, int /* player_id */)
// Initialize Efl player synchronously with result.
IPC_SYNC_MESSAGE_ROUTED5_1(MediaPlayerEflHostMsg_InitSync,
                           int /* player_id */,
                           MediaPlayerHostMsg_Initialize_Type /* type */,
                           GURL /* URL */,
                           std::string /*mime_type*/,
                           int /* demuxer client id */,
                           bool /* success */)

// Deinitialize Gst player.
IPC_MESSAGE_ROUTED1(MediaPlayerEflHostMsg_DeInit, int /* player_id */)

// Start playback.
IPC_MESSAGE_ROUTED1(MediaPlayerEflHostMsg_Play, int /* player_id */)

// Pause playback.
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_Pause,
                    int /* player_id */,
                    bool /* is_media_related_action */)

// Suspend media player.
IPC_MESSAGE_ROUTED1(MediaPlayerEflHostMsg_Suspend, int /* player_id */)

// Resume media player.
IPC_MESSAGE_ROUTED1(MediaPlayerEflHostMsg_Resume, int /* player_id*/)

// Player was activated by an user or an app.
IPC_MESSAGE_ROUTED1(MediaPlayerEflHostMsg_Activate, int /* player_id*/)

// Player should deactivate (ex. save power).
IPC_MESSAGE_ROUTED1(MediaPlayerEflHostMsg_Deactivate, int /* player_id*/)

// Set volume.
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_SetVolume,
                    int /* player_id */,
                    double /* volume */)

// Set playback rate.
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_SetRate,
                    int /* player_id */,
                    double /* rate */)

// Playback duration.
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_DurationChanged,
                    int /* player_id */,
                    base::TimeDelta /* time */)

// Current  duration.
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_TimeUpdate,
                    int /* player_id */,
                    base::TimeDelta /* time */)

// Pause state.
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_PauseStateChanged,
                    int /* player_id */,
                    bool /* state */)

// Seek state.
IPC_MESSAGE_ROUTED1(MediaPlayerEflMsg_OnSeekComplete, int /* player_id */)

// Current buffer range.
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_BufferUpdate,
                    int /* player_id */,
                    int /* buffering_percentage */)

// Playback completed.
IPC_MESSAGE_ROUTED1(MediaPlayerEflMsg_TimeChanged, int /* player_id */)

IPC_MESSAGE_ROUTED1(MediaPlayerEflMsg_PlayerDestroyed, int /* player_id */)
// Ready state change.
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_ReadyStateChange,
                    int /* player_id */,
                    blink::WebMediaPlayer::ReadyState /* state */)

// Network state change.
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_NetworkStateChange,
                    int /* player_id */,
                    blink::WebMediaPlayer::NetworkState /* state */)

// Gst media data has changed.
IPC_MESSAGE_ROUTED4(MediaPlayerEflMsg_MediaDataChanged,
                    int /* player_id */,
                    int /* width */,
                    int /* height */,
                    int /* media */)

IPC_MESSAGE_ROUTED1(MediaPlayerEflMsg_VideoSlotsAvailableChanged,
                    unsigned /* slots_available */)

// On new frame available.
IPC_MESSAGE_ROUTED4(MediaPlayerEflMsg_NewFrameAvailable,
                    int /* player_id */,
                    base::SharedMemoryHandle /* Handle */,
                    uint32_t /* length */,
                    base::TimeDelta /* time stamp */)

#if defined(TIZEN_TBM_SUPPORT)
#if defined(OS_TIZEN_TV_PRODUCT)
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_ControlMediaPacket,
                    int /* player_id */,
                    int /* control mode */)

IPC_MESSAGE_ROUTED4(MediaPlayerEflMsg_NewTbmBufferAvailable,
                    int /* player_id */,
                    gfx::TbmBufferHandle /* Handle */,
                    int /* physical index */,
                    base::TimeDelta /* time stamp */)
#else
IPC_MESSAGE_ROUTED3(MediaPlayerEflMsg_NewTbmBufferAvailable,
                    int /* player_id */,
                    gfx::TbmBufferHandle /* Handle */,
                    base::TimeDelta /* time stamp */)
#endif

// Requests to release tbm buffer.
IPC_MESSAGE_CONTROL1(MediaPlayerEflHostMsg_ReleaseTbmBuffer,
                     gfx::TbmBufferHandle /* Handle */)
#endif

#if defined(TIZEN_VIDEO_HOLE)
// Set geometry.
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_SetGeometry,
                    int /* player_id */,
                    gfx::RectF /* position and size */)
#endif

// Seek.
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_Seek,
                    int /* player_id */,
                    base::TimeDelta /* time */)

// For MSE internal seek request.
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_SeekRequest,
                    int /* player_id */,
                    base::TimeDelta /* time_to_seek */)

// Player has begun suspend procedure
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_PlayerSuspend,
                    int /* player_id */,
                    bool /* is_preempted */)

// Player has resumed
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_PlayerResumed,
                    int /* player_id */,
                    bool /* is_preempted */)

// Sent after the renderer demuxer has seeked.
IPC_MESSAGE_CONTROL3(MediaPlayerEflHostMsg_DemuxerSeekDone,
                     int /* demuxer_client_id */,
                     base::TimeDelta /* actual_browser_seek_time */,
                     base::TimeDelta /* video_key_frame */)

// Inform the media source player that the demuxer is ready.
IPC_MESSAGE_CONTROL2(MediaPlayerEflHostMsg_DemuxerReady,
                     int /* demuxer_client_id */,
                     media::DemuxerConfigs /* configs */)

// Sent when the data was read from the ChunkDemuxer.
IPC_MESSAGE_CONTROL3(MediaPlayerEflHostMsg_ReadFromDemuxerAck,
                     int /* demuxer_client_id */,
                     base::SharedMemoryHandle /* Handle */,
                     media::DemuxedBufferMetaData /* meta data of buffer*/)

// Inform the media source player of changed media duration from demuxer.
IPC_MESSAGE_CONTROL2(MediaPlayerEflHostMsg_DurationChanged,
                     int /* demuxer_client_id */,
                     base::TimeDelta /* duration */)

// The media source player reads data from demuxer
IPC_MESSAGE_CONTROL2(MediaPlayerEflMsg_ReadFromDemuxer,
                     int /* demuxer_client_id */,
                     media::DemuxerStream::Type /* type */)

// Requests renderer demuxer seek.
IPC_MESSAGE_CONTROL2(MediaPlayerEflMsg_DemuxerSeekRequest,
                     int /* demuxer_client_id */,
                     base::TimeDelta /* time_to_seek */)

IPC_MESSAGE_CONTROL2(MediaPlayerEflHostMsg_DemuxerBufferedChanged,
                     int /* demuxer_client_id */,
                     media::Ranges<base::TimeDelta> /* buffered */)

#if defined(OS_TIZEN_TV_PRODUCT)
// Inform Media PLayer about encrypted listener
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_HasEncryptedListener,
                    int /* player_id */,
                    bool /* ret */)

// Inform the media source player that the demuxer data and state change.
IPC_MESSAGE_CONTROL2(MediaPlayerEflHostMsg_DemuxerStateChanged,
                     int /* demuxer_client_id */,
                     media::DemuxerConfigs /* configs */)

// notify subtitle play track
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_GetPlayTrackInfo,
                    int /* player_id */,
                    int /* id */)

// notify subtitle play
IPC_MESSAGE_ROUTED4(MediaPlayerEflHostMsg_NotifySubtitlePlay,
                    int /* player_id */,
                    int /* id */,
                    std::string /* url */,
                    std::string /* lang */)
// Report drm error.
IPC_MESSAGE_ROUTED1(MediaPlayerEflMsg_DrmError, int /* player_id */)

// Playback seekable time change.
IPC_MESSAGE_ROUTED3(MediaPlayerEflMsg_SeekableTimeChanged,
                    int /* player_id */,
                    base::TimeDelta /* min time */,
                    base::TimeDelta /* max time */)
// regist timeline
IPC_SYNC_MESSAGE_ROUTED2_1(MediaPlayerEflHostMsg_RegisterTimeline,
                           int /* player_id */,
                           std::string /* timeline_selector */,
                           bool /*ret*/)

// unregister timeline
IPC_SYNC_MESSAGE_ROUTED2_1(MediaPlayerEflHostMsg_UnRegisterTimeline,
                           int /* player_id */,
                           std::string /* timeline_selector */,
                           bool /*ret*/)

#if defined(OS_TIZEN_TV_PRODUCT)
// push decryptor to browser
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_SetDecryptorHandle,
                    int /* player_id */,
                    eme::eme_decryptor_t /* decryptor */)

// init data from mmplayer to ieme
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_InitData,
                    int /* player_id */,
                    std::vector<unsigned char> /* init_data */)

// drm_eme is waiting for key for decryption
IPC_MESSAGE_ROUTED1(MediaPlayerEflMsg_WaitingForKey,
                    int /* player_id */)
#endif

// get timeline positions
IPC_SYNC_MESSAGE_ROUTED2_4(MediaPlayerEflHostMsg_GetTimelinePositions,
                           int /* player_id */,
                           std::string /* timeline_selector */,
                           uint32_t /*units_per_tick*/,
                           uint32_t /*units_per_second*/,
                           int64_t /*content_time*/,
                           bool /* paused*/)

#if defined(OS_TIZEN_TV_PRODUCT)
// Synchronously deinitialize Gst player.
IPC_SYNC_MESSAGE_ROUTED1_0(MediaPlayerEflHostMsg_DeInitSync,
                           int /* player_id */)
#endif

// get speed
IPC_SYNC_MESSAGE_ROUTED1_1(MediaPlayerEflHostMsg_GetSpeed,
                           int /* player_id */,
                           double /* speed */)

// get mrs url
IPC_SYNC_MESSAGE_ROUTED1_1(MediaPlayerEflHostMsg_GetMrsUrl,
                           int /* player_id */,
                           std::string /* url*/)

// get content id
IPC_SYNC_MESSAGE_ROUTED1_1(MediaPlayerEflHostMsg_GetContentId,
                           int /* player_id */,
                           std::string /* content id*/)

// sync timeline
IPC_SYNC_MESSAGE_ROUTED5_1(MediaPlayerEflHostMsg_SyncTimeline,
                           int /* player_id */,
                           std::string /* timeline_selector */,
                           int64_t /* timeline_pos*/,
                           int64_t /*wallclock_pos*/,
                           int /*tolerance*/,
                           bool /*ret*/)

// set wallclock
IPC_SYNC_MESSAGE_ROUTED2_1(MediaPlayerEflHostMsg_SetWallClock,
                           int /* player_id */,
                           std::string /*wallclock_url */,
                           bool /*ret*/)

// callback info of sync timeline
IPC_MESSAGE_ROUTED3(MediaPlayerEflMsg_SyncTimelineCb,
                    int /* player_id */,
                    std::string /* timeline_selector */,
                    int /*sync*/)

// callback info of register timeline
IPC_MESSAGE_ROUTED2(
    MediaPlayerEflMsg_RegisterTimelineCb,
    int /* player_id */,
    blink::WebMediaPlayer::register_timeline_cb_info_s /* cb info */)

IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_MrsUrlChange,
                    int /* player_id */,
                    std::string /*url */)

IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_ContentIdChange,
                    int /* player_id */,
                    std::string /*id */)

// Requests to add audio track.
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_AddAudioTrack,
                    int /* player_id */,
                    blink::WebMediaPlayer::audio_video_track_info_s)
// Requests to add video track.
IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_AddVideoTrack,
                    int /* player_id */,
                    blink::WebMediaPlayer::audio_video_track_info_s)
// Requests to add text track.
IPC_MESSAGE_ROUTED4(MediaPlayerEflMsg_AddTextTrack,
                    int /* player_id */,
                    std::string /* label */,
                    std::string /* language */,
                    std::string /* id */)
// Set Active Text Track
IPC_MESSAGE_ROUTED3(MediaPlayerEflHostMsg_SetActiveTextTrack,
                    int /* player_id */,
                    int /* id */,
                    bool /*is_in_band*/)
// Set Active Audio Track
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_SetActiveAudioTrack,
                    int /* player_id */,
                    int /* index */)
// Set Active Video Track
IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_SetActiveVideoTrack,
                    int /* player_id */,
                    int /* index */)
// Text Track
IPC_MESSAGE_ROUTED3(MediaPlayerEflMsg_InbandEventTextTrack,
                    int /* player_id */,
                    std::string /* info */,
                    int /*action*/)
// Text Track Cue
IPC_MESSAGE_ROUTED5(MediaPlayerEflMsg_InbandEventTextCue,
                    int /* player_id */,
                    std::string /* info */,
                    unsigned int /*id*/,
                    long long int /*startTimeMs*/,
                    long long int /*endTimeMs*/)

// Use sync ipc message to get the count of video frame drop.
IPC_SYNC_MESSAGE_ROUTED1_1(MediaPlayerEflHostMsg_GetDroppedFrameCount,
                           int /* player_id */,
                           unsigned /* droppedFrameCount */)

// Use sync ipc message to get the count of total frame.
IPC_SYNC_MESSAGE_ROUTED1_1(MediaPlayerEflHostMsg_GetDecodedFrameCount,
                           int /* player_id */,
                           unsigned /* decodedFrameCount */)

IPC_MESSAGE_ROUTED1(MediaPlayerEflHostMsg_NotifyElementRemove,
                    int /* player_id */)

IPC_MESSAGE_ROUTED2(MediaPlayerEflHostMsg_SetParentalRatingResult,
                    int /* player_id */,
                    bool /* is_pass */)

IPC_MESSAGE_ROUTED2(MediaPlayerEflMsg_IsPlayerStarted,
                    int /* player_id */,
                    bool /*is_play_started */)

IPC_SYNC_MESSAGE_ROUTED1_1(FrameHostMsg_CheckHLSSupport,
                           std::string /* url */,
                           bool /* support */)
#endif

// Use sync ipc message to get start date of current media.
IPC_SYNC_MESSAGE_ROUTED1_1(MediaPlayerEflHostMsg_GetStartDate,
                           int /* player_id */,
                           double /* start date */)

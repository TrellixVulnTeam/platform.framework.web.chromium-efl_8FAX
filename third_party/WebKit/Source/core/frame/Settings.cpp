/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2011, 2012 Apple Inc. All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/frame/Settings.h"

#include <memory>

#include "build/build_config.h"
#include "platform/scroll/ScrollbarTheme.h"
#include "platform/wtf/PtrUtil.h"

namespace blink {

// NOTEs
//  1) EditingMacBehavior comprises builds on Mac;
//  2) EditingWindowsBehavior comprises builds on Windows;
//  3) EditingUnixBehavior comprises all unix-based systems, but
//     Darwin/MacOS/Android (and then abusing the terminology);
//  4) EditingAndroidBehavior comprises Android builds.
// 99) MacEditingBehavior is used a fallback.
static EditingBehaviorType EditingBehaviorTypeForPlatform() {
  return
#if defined(OS_MACOSX)
      kEditingMacBehavior
#elif defined(OS_WIN)
      kEditingWindowsBehavior
#elif defined(OS_ANDROID)
      kEditingAndroidBehavior
#else  // Rest of the UNIX-like systems
      kEditingUnixBehavior
#endif
      ;
}

#if defined(OS_WIN)
static const bool kDefaultSelectTrailingWhitespaceEnabled = true;
#else
static const bool kDefaultSelectTrailingWhitespaceEnabled = false;
#endif

Settings::Settings()
    : text_autosizing_enabled_(false)
#if defined(OS_TIZEN)
      ,
      hw_keyboard_connected_(false)
#endif
#if defined(OS_TIZEN_TV_PRODUCT)
      ,
      media_playback_notification_enabled_(false),
      media_subtitle_notification_enabled_(false),
      gpu_vendor_kant2_used_(false)
#endif
          SETTINGS_INITIALIZER_LIST {
}

std::unique_ptr<Settings> Settings::Create() {
  return WTF::WrapUnique(new Settings);
}

SETTINGS_SETTER_BODIES

void Settings::SetDelegate(SettingsDelegate* delegate) {
  delegate_ = delegate;
}

void Settings::Invalidate(SettingsDelegate::ChangeType change_type) {
  if (delegate_)
    delegate_->SettingsChanged(change_type);
}

void Settings::SetTextAutosizingEnabled(bool text_autosizing_enabled) {
  if (text_autosizing_enabled_ == text_autosizing_enabled)
    return;

  text_autosizing_enabled_ = text_autosizing_enabled;
  Invalidate(SettingsDelegate::kTextAutosizingChange);
}

// FIXME: Move to Settings.in once make_settings can understand IntSize.
void Settings::SetTextAutosizingWindowSizeOverride(
    const IntSize& text_autosizing_window_size_override) {
  if (text_autosizing_window_size_override_ ==
      text_autosizing_window_size_override)
    return;

  text_autosizing_window_size_override_ = text_autosizing_window_size_override;
  Invalidate(SettingsDelegate::kTextAutosizingChange);
}

void Settings::SetMockScrollbarsEnabled(bool flag) {
  ScrollbarTheme::SetMockScrollbarsEnabled(flag);
}

bool Settings::MockScrollbarsEnabled() {
  return ScrollbarTheme::MockScrollbarsEnabled();
}

#if defined(TIZEN_VD_NATIVE_SCROLLBARS)
void Settings::SetUseNativeScrollbars(bool flag) {
  ScrollbarTheme::SetNativeScrollbarsEnabled(flag);
}

bool Settings::UseNativeScrollbars() {
  return ScrollbarTheme::NativeScrollbarsEnabled();
}
#endif

#if defined(USE_EFL)
void Settings::SetTizenVersion(
    unsigned major, unsigned minor, unsigned release) {
  if (tizen_compatibility_settings_.major == major
      && tizen_compatibility_settings_.minor == minor
      && tizen_compatibility_settings_.release == release)
    return;

  tizen_compatibility_settings_.major = major;
  tizen_compatibility_settings_.minor = minor;
  tizen_compatibility_settings_.release = release;

  Invalidate(SettingsDelegate::kTizenVersionChange);
}

bool Settings::TizenCompatibilityModeEnabled() const {
  return tizen_compatibility_settings_.TizenCompatibilityModeEnabled();
}
#endif

} // namespace blink

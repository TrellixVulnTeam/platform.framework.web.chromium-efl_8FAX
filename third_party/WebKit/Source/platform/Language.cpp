/*
 * Copyright (C) 2010, 2013 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "platform/Language.h"

#include "platform/text/PlatformLocale.h"
#include "platform/wtf/text/WTFString.h"
#include "public/platform/Platform.h"

#if defined(OS_TIZEN)
#include <vconf/vconf.h>
#endif

namespace blink {

namespace {

static String CanonicalizeLanguageIdentifier(const String& language_code) {
  String copied_code = language_code;
  // Platform::defaultLocale() might provide a language code with '_'.
  copied_code.Replace('_', '-');
#if defined(OS_TIZEN)
  size_t dot_position = copied_code.find('.');
  if (dot_position > 0)
    copied_code.Truncate(dot_position);
#endif
  return copied_code;
}

#if defined(OS_TIZEN)
static bool language_changed_ = false;
static bool language_changed_callback_registered_ = false;
static void platformLanguageChanged(keynode_t*, void* data) {
  language_changed_ = true;
}
#endif

// Main thread static AtomicString. This can be safely shared across threads.
const AtomicString* g_platform_language = nullptr;

const AtomicString& PlatformLanguage() {
  DCHECK(g_platform_language->Impl()->IsStatic())
      << "global language string is used from multiple threads, and must be "
         "static";
#if defined(OS_TIZEN)
  DEFINE_STATIC_LOCAL(AtomicString, g_vconf_language, ());
  if (language_changed_) {
    // Update the platform language from vconf
    char* lang = vconf_get_str(VCONFKEY_LANGSET);
    if (lang) {
      g_vconf_language = AtomicString(CanonicalizeLanguageIdentifier(lang));
      free(lang);
    }
    language_changed_ = false;
  }

  if (!g_vconf_language.IsEmpty())
    return g_vconf_language;
#endif
  return *g_platform_language;
}

Vector<AtomicString>& PreferredLanguagesOverride() {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(ThreadSpecific<Vector<AtomicString>>,
                                  thread_specific_languages, ());
  return *thread_specific_languages;
}

}  // namespace

#if defined(OS_TIZEN)
void StopListeningLanguageChange() {
  if (language_changed_callback_registered_)
    vconf_ignore_key_changed(VCONFKEY_LANGSET, platformLanguageChanged);

  language_changed_callback_registered_ = false;
}
#endif

void InitializePlatformLanguage() {
  DCHECK(IsMainThread());
#if defined(OS_TIZEN)
  if (!language_changed_callback_registered_) {
    // Register a callback to handle the platform language change
    vconf_notify_key_changed(VCONFKEY_LANGSET, platformLanguageChanged,
                             nullptr);
    language_changed_callback_registered_ = true;
    language_changed_ = true;
  }
#endif
  DEFINE_STATIC_LOCAL(
      // We add the platform language as a static string for two reasons:
      // 1. it can be shared across threads.
      // 2. since this is done very early on, we don't want to accidentally
      //    collide with a hard coded static string (like "fr" on SVG).
      const AtomicString, platform_language, (([]() {
        String canonicalized = CanonicalizeLanguageIdentifier(
            Platform::Current()->DefaultLocale());
        if (!canonicalized.IsEmpty()) {
          StringImpl* impl = StringImpl::CreateStatic(
              reinterpret_cast<const char*>(canonicalized.Characters8()),
              canonicalized.length(),
              StringHasher::ComputeHashAndMaskTop8Bits(
                  canonicalized.Characters8(), canonicalized.length()));

          return AtomicString(impl);
        }
        return AtomicString();
      })()));

  g_platform_language = &platform_language;
}

void OverrideUserPreferredLanguagesForTesting(
    const Vector<AtomicString>& override) {
  Vector<AtomicString>& canonicalized = PreferredLanguagesOverride();
  canonicalized.resize(0);
  canonicalized.ReserveCapacity(override.size());
  for (const auto& lang : override)
    canonicalized.push_back(CanonicalizeLanguageIdentifier(lang));
  Locale::ResetDefautlLocale();
}

AtomicString DefaultLanguage() {
  Vector<AtomicString>& override = PreferredLanguagesOverride();
  if (!override.IsEmpty())
    return override[0];
  return PlatformLanguage();
}

Vector<AtomicString> UserPreferredLanguages() {
  Vector<AtomicString>& override = PreferredLanguagesOverride();
  if (!override.IsEmpty())
    return override;

  Vector<AtomicString> languages;
  languages.ReserveInitialCapacity(1);
  languages.push_back(PlatformLanguage());
  return languages;
}

size_t IndexOfBestMatchingLanguageInList(
    const AtomicString& language,
    const Vector<AtomicString>& language_list) {
  AtomicString language_without_locale_match;
  AtomicString language_match_but_not_locale;
  size_t language_without_locale_match_index = 0;
  size_t language_match_but_not_locale_match_index = 0;
  bool can_match_language_only =
      (language.length() == 2 ||
       (language.length() >= 3 && language[2] == '-'));

  for (size_t i = 0; i < language_list.size(); ++i) {
    String canonicalized_language_from_list =
        CanonicalizeLanguageIdentifier(language_list[i]);

    if (language == canonicalized_language_from_list)
      return i;

    if (can_match_language_only &&
        canonicalized_language_from_list.length() >= 2) {
      if (language[0] == canonicalized_language_from_list[0] &&
          language[1] == canonicalized_language_from_list[1]) {
        if (!language_without_locale_match.length() &&
            canonicalized_language_from_list.length() == 2) {
          language_without_locale_match = language_list[i];
          language_without_locale_match_index = i;
        }
        if (!language_match_but_not_locale.length() &&
            canonicalized_language_from_list.length() >= 3) {
          language_match_but_not_locale = language_list[i];
          language_match_but_not_locale_match_index = i;
        }
      }
    }
  }

  // If we have both a language-only match and a languge-but-not-locale match,
  // return the languge-only match as is considered a "better" match. For
  // example, if the list provided has both "en-GB" and "en" and the user
  // prefers "en-US" we will return "en".
  if (language_without_locale_match.length())
    return language_without_locale_match_index;

  if (language_match_but_not_locale.length())
    return language_match_but_not_locale_match_index;

  return language_list.size();
}

}  // namespace blink

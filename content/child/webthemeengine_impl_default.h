// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_WEBTHEMEENGINE_IMPL_DEFAULT_H_
#define CONTENT_CHILD_WEBTHEMEENGINE_IMPL_DEFAULT_H_

#include <stdint.h>

#include "build/build_config.h"
#include "third_party/WebKit/public/platform/WebThemeEngine.h"

namespace content {

class WebThemeEngineImpl : public blink::WebThemeEngine {
 public:
  // WebThemeEngine methods:
  blink::WebSize GetSize(blink::WebThemeEngine::Part) override;
#if defined(TIZEN_VD_NATIVE_SCROLLBARS)
  blink::WebSize GetSize(blink::WebThemeEngine::Part, bool) override;
#endif

  void Paint(blink::WebCanvas* canvas,
             blink::WebThemeEngine::Part part,
             blink::WebThemeEngine::State state,
             const blink::WebRect& rect,
             const blink::WebThemeEngine::ExtraParams* extra_params) override;
  void GetOverlayScrollbarStyle(
      blink::WebThemeEngine::ScrollbarStyle*) override;
  bool SupportsNinePatch(Part part) const override;
  blink::WebSize NinePatchCanvasSize(Part part) const override;
  blink::WebRect NinePatchAperture(Part part) const override;
#if defined(OS_WIN)
  // Caches the scrollbar metrics. These are retrieved in the browser and passed
  // to the renderer in RendererPreferences because the required Windows system
  // calls cannot be made in sandboxed renderers.
  static void cacheScrollBarMetrics(int32_t vertical_scroll_bar_width,
                                    int32_t horizontal_scroll_bar_height,
                                    int32_t vertical_arrow_bitmap_height,
                                    int32_t horizontal_arrow_bitmap_width);
#endif
};

}  // namespace content

#endif  // CONTENT_CHILD_WEBTHEMEENGINE_IMPL_DEFAULT_H_

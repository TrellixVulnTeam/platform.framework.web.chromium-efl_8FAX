// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/renderer/android/disambiguation_popup_helper.h"

#include <stddef.h>

#include <algorithm>

#include "third_party/WebKit/public/platform/WebRect.h"
#include "ui/gfx/geometry/size_conversions.h"

using blink::WebRect;
using blink::WebVector;

namespace {

// The amount of padding to add to the disambiguation popup to show
// content around the possible elements, adding some context.
#if defined(S_TERRACE_SUPPORT)
const int kDisambiguationPopupPadding = 15;
#else
const int kDisambiguationPopupPadding = 8;
#endif

// Constants used for fitting the disambiguation popup inside the bounds of
// the view. Note that there are mirror constants in PopupZoomer.java.
#if defined(S_TERRACE_SUPPORT)
const int kDisambiguationPopupBoundsMargin = 17;
#else
const int kDisambiguationPopupBoundsMargin = 25;
#endif

// The smallest allowable touch target used for disambiguation popup.
// This value is used to determine the minimum amount we need to scale to
// make all targets touchable.
const int kDisambiguationPopupMinimumTouchSize = 40;
const float kDisambiguationPopupMaxScale = 5.0;
#if defined(S_TERRACE_SUPPORT)
const float kDisambiguationPopupMinScale = 2.5;
#else
const float kDisambiguationPopupMinScale = 2.0;
#endif

// Compute the scaling factor to ensure the smallest touch candidate reaches
// a certain clickable size after zooming
float FindOptimalScaleFactor(const WebVector<WebRect>& target_rects,
                             float total_scale) {
  DCHECK_GT(total_scale, 0.0f);
  if (!target_rects.size()) {
    NOTREACHED();
    return kDisambiguationPopupMinScale;
  }
  int smallest_target = std::min(target_rects[0].width, target_rects[0].height);
  for (size_t i = 1; i < target_rects.size(); i++) {
    smallest_target = std::min(
        {smallest_target, target_rects[i].width, target_rects[i].height});
  }
  const float smallest_target_f = std::max(smallest_target * total_scale, 1.0f);
  return std::min(kDisambiguationPopupMaxScale,
                  std::max(kDisambiguationPopupMinScale,
                           kDisambiguationPopupMinimumTouchSize /
                               smallest_target_f)) *
         total_scale;
}

void TrimEdges(int* e1, int* e2, int max_combined) {
  if (*e1 + *e2 <= max_combined)
    return;

  if (std::min(*e1, *e2) * 2 >= max_combined)
    *e1 = *e2 = max_combined / 2;
  else if (*e1 > *e2)
    *e1 = max_combined - *e2;
  else
    *e2 = max_combined - *e1;
}

// Ensure the disambiguation popup fits inside the screen,
// clip the edges farthest to the touch point if needed.
gfx::Rect CropZoomArea(const gfx::Rect& zoom_rect,
                       const gfx::Size& viewport_size,
                       const gfx::Point& touch_point,
#if defined(S_TERRACE_SUPPORT)
                       float device_scale_factor,
#endif
                       float scale) {
  gfx::Size max_size = viewport_size;
#if defined(S_TERRACE_SUPPORT)
  max_size.Enlarge(-2 * kDisambiguationPopupBoundsMargin * device_scale_factor,
                   -2 * kDisambiguationPopupBoundsMargin * device_scale_factor);
  max_size =
      gfx::ScaleToCeiledSize(max_size, (1.0 / scale) * device_scale_factor);
#else
  max_size.Enlarge(-2 * kDisambiguationPopupBoundsMargin,
                   -2 * kDisambiguationPopupBoundsMargin);
  max_size = gfx::ScaleToCeiledSize(max_size, 1.0 / scale);
#endif

  int left = touch_point.x() - zoom_rect.x();
  int right = zoom_rect.right() - touch_point.x();
  int top = touch_point.y() - zoom_rect.y();
  int bottom = zoom_rect.bottom() - touch_point.y();
#if defined(S_TERRACE_SUPPORT)
  int max_screen_width =
      (viewport_size.width() * device_scale_factor -
       kDisambiguationPopupBoundsMargin * device_scale_factor * 2) / scale;
  max_size.set_width(max_screen_width);
#endif
  TrimEdges(&left, &right, max_size.width());
  TrimEdges(&top, &bottom, max_size.height());

  return gfx::Rect(touch_point.x() - left,
                   touch_point.y() - top,
                   left + right,
                   top + bottom);
}

}  // namespace

namespace content {

// static
float DisambiguationPopupHelper::ComputeZoomAreaAndScaleFactor(
    const gfx::Rect& tap_rect,
    const WebVector<WebRect>& target_rects,
    const gfx::Size& screen_size,
    const gfx::Size& visible_content_size,
    float total_scale,
#if defined(S_TERRACE_SUPPORT)
    float device_scale_factor,
#endif
    gfx::Rect* zoom_rect) {
  *zoom_rect = tap_rect;
  for (size_t i = 0; i < target_rects.size(); i++)
    zoom_rect->Union(gfx::Rect(target_rects[i]));
  zoom_rect->Inset(-kDisambiguationPopupPadding, -kDisambiguationPopupPadding);

  zoom_rect->Intersect(gfx::Rect(visible_content_size));

  float new_total_scale =
      FindOptimalScaleFactor(target_rects, total_scale);
#if defined(S_TERRACE_SUPPORT)
  *zoom_rect = CropZoomArea(*zoom_rect, screen_size, tap_rect.CenterPoint(),
                            device_scale_factor, new_total_scale);
#else
  *zoom_rect = CropZoomArea(
      *zoom_rect, screen_size, tap_rect.CenterPoint(), new_total_scale);
#endif

  return new_total_scale;
}

}  // namespace content

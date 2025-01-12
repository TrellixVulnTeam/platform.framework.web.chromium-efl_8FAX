// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/ui/autofill/popup_view_common.h"

#include <algorithm>
#include <utility>

#include "ui/display/display.h"
#include "ui/display/screen.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/vector2d.h"

namespace autofill {

namespace {

std::pair<int, int> CalculatePopupXAndWidth(
    const display::Display& left_display,
    const display::Display& right_display,
    int popup_required_width,
    const gfx::Rect& element_bounds,
    bool is_rtl) {
  int leftmost_display_x = left_display.bounds().x();
  int rightmost_display_x =
      right_display.GetSizeInPixel().width() + right_display.bounds().x();

  // Calculate the start coordinates for the popup if it is growing right or
  // the end position if it is growing to the left, capped to screen space.
  int right_growth_start = std::max(
      leftmost_display_x, std::min(rightmost_display_x, element_bounds.x()));
  int left_growth_end =
      std::max(leftmost_display_x,
               std::min(rightmost_display_x, element_bounds.right()));

  int right_available = rightmost_display_x - right_growth_start;
  int left_available = left_growth_end - leftmost_display_x;

  int popup_width =
      std::min(popup_required_width, std::max(right_available, left_available));

  std::pair<int, int> grow_right(right_growth_start, popup_width);
  std::pair<int, int> grow_left(left_growth_end - popup_width, popup_width);

  // Prefer to grow towards the end (right for LTR, left for RTL). But if there
  // is not enough space available in the desired direction and more space in
  // the other direction, reverse it.
  if (is_rtl) {
    return left_available >= popup_width || left_available >= right_available
               ? grow_left
               : grow_right;
  }
  return right_available >= popup_width || right_available >= left_available
             ? grow_right
             : grow_left;
}

// Calculates the height of the popup and the y position of it. These values
// will stay on the screen.
std::pair<int, int> CalculatePopupYAndHeight(
    const display::Display& top_display,
    const display::Display& bottom_display,
    int popup_required_height,
    const gfx::Rect& element_bounds) {
  int topmost_display_y = top_display.bounds().y();
  int bottommost_display_y =
      bottom_display.GetSizeInPixel().height() + bottom_display.bounds().y();

  // Calculate the start coordinates for the popup if it is growing down or
  // the end position if it is growing up, capped to screen space.
  int top_growth_end = std::max(
      topmost_display_y, std::min(bottommost_display_y, element_bounds.y()));
  int bottom_growth_start =
      std::max(topmost_display_y,
               std::min(bottommost_display_y, element_bounds.bottom()));

  int top_available = bottom_growth_start - topmost_display_y;
  int bottom_available = bottommost_display_y - top_growth_end;

  // TODO(csharp): Restrict the popup height to what is available.
  if (bottom_available >= popup_required_height ||
      bottom_available >= top_available) {
    // The popup can appear below the field.
    return std::make_pair(bottom_growth_start, popup_required_height);
  } else {
    // The popup must appear above the field.
    return std::make_pair(top_growth_end - popup_required_height,
                          popup_required_height);
  }
}

}  // namespace

gfx::Rect PopupViewCommon::CalculatePopupBounds(int desired_width,
                                                int desired_height,
                                                const gfx::Rect& element_bounds,
                                                gfx::NativeView container_view,
                                                bool is_rtl) {
  // This is the top left point of the popup if the popup is above the element
  // and grows to the left (since that is the highest and furthest left the
  // popup go could).
  gfx::Point top_left_corner_of_popup =
      element_bounds.origin() +
      gfx::Vector2d(element_bounds.width() - desired_width, -desired_height);

  // This is the bottom right point of the popup if the popup is below the
  // element and grows to the right (since the is the lowest and furthest right
  // the popup could go).
  gfx::Point bottom_right_corner_of_popup =
      element_bounds.origin() +
      gfx::Vector2d(desired_width, element_bounds.height() + desired_height);

  display::Display top_left_display =
      GetDisplayNearestPoint(top_left_corner_of_popup, container_view);
  display::Display bottom_right_display =
      GetDisplayNearestPoint(bottom_right_corner_of_popup, container_view);

  std::pair<int, int> popup_x_and_width =
      CalculatePopupXAndWidth(top_left_display, bottom_right_display,
                              desired_width, element_bounds, is_rtl);
  std::pair<int, int> popup_y_and_height = CalculatePopupYAndHeight(
      top_left_display, bottom_right_display, desired_height, element_bounds);

  return gfx::Rect(popup_x_and_width.first, popup_y_and_height.first,
                   popup_x_and_width.second, popup_y_and_height.second);
}

display::Display PopupViewCommon::GetDisplayNearestPoint(
    const gfx::Point& point,
    gfx::NativeView container_view) {
  return display::Screen::GetScreen()->GetDisplayNearestPoint(point);
}

}  // namespace autofill

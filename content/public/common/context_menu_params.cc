// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/common/context_menu_params.h"

namespace content {

const int32_t CustomContextMenuContext::kCurrentRenderWidget = INT32_MAX;

CustomContextMenuContext::CustomContextMenuContext()
    : is_pepper_menu(false),
      request_id(0),
      render_widget_id(kCurrentRenderWidget) {
}

ContextMenuParams::ContextMenuParams()
    : media_type(blink::WebContextMenuData::kMediaTypeNone),
      x(0),
      y(0),
      has_image_contents(false),
#if defined(S_TERRACE_SUPPORT)
      image_width(0),
      image_height(0),
#endif
      media_flags(0),
      spellcheck_enabled(false),
      is_editable(false),
      writing_direction_default(
          blink::WebContextMenuData::kCheckableMenuItemDisabled),
      writing_direction_left_to_right(
          blink::WebContextMenuData::kCheckableMenuItemEnabled),
      writing_direction_right_to_left(
          blink::WebContextMenuData::kCheckableMenuItemEnabled),
      edit_flags(0),
      referrer_policy(blink::kWebReferrerPolicyDefault),
      source_type(ui::MENU_SOURCE_NONE),
      input_field_type(blink::WebContextMenuData::kInputFieldTypeNone) {}

ContextMenuParams::ContextMenuParams(const ContextMenuParams& other) = default;

ContextMenuParams::~ContextMenuParams() {
}

}  // namespace content

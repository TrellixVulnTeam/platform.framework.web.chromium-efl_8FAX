// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/accessibility/browser_accessibility_manager_auralinux.h"

#include "content/browser/accessibility/browser_accessibility_auralinux.h"
#include "content/common/accessibility_messages.h"

#if defined(TIZEN_ATK_SUPPORT)
#include "tizen/system_info.h"
#endif

namespace content {

#if !defined(TIZEN_ATK_SUPPORT)
// static
BrowserAccessibilityManager* BrowserAccessibilityManager::Create(
    const ui::AXTreeUpdate& initial_tree,
    BrowserAccessibilityDelegate* delegate,
    BrowserAccessibilityFactory* factory) {
  return new BrowserAccessibilityManagerAuraLinux(nullptr, initial_tree,
                                                  delegate, factory);
}
#endif

BrowserAccessibilityManagerAuraLinux*
BrowserAccessibilityManager::ToBrowserAccessibilityManagerAuraLinux() {
  return static_cast<BrowserAccessibilityManagerAuraLinux*>(this);
}

BrowserAccessibilityManagerAuraLinux::BrowserAccessibilityManagerAuraLinux(
    AtkObject* parent_object,
    const ui::AXTreeUpdate& initial_tree,
    BrowserAccessibilityDelegate* delegate,
    BrowserAccessibilityFactory* factory)
    : BrowserAccessibilityManager(delegate, factory),
      parent_object_(parent_object) {
  Initialize(initial_tree);
}

BrowserAccessibilityManagerAuraLinux::~BrowserAccessibilityManagerAuraLinux() {
}

// static
ui::AXTreeUpdate
    BrowserAccessibilityManagerAuraLinux::GetEmptyDocument() {
  ui::AXNodeData empty_document;
  empty_document.id = 0;
  empty_document.role = ui::AX_ROLE_ROOT_WEB_AREA;
  ui::AXTreeUpdate update;
  update.root_id = empty_document.id;
  update.nodes.push_back(empty_document);
  return update;
}

void BrowserAccessibilityManagerAuraLinux::NotifyAccessibilityEvent(
    BrowserAccessibilityEvent::Source source,
    ui::AXEvent event_type,
    BrowserAccessibility* node) {
  // TODO(shreeram.k) : Implement.
}

#if defined(TIZEN_ATK_SUPPORT)
void BrowserAccessibilityManagerAuraLinux::SetParentObject(
    AtkObject* parent_object) {
  if (IsMobileProfile())
    parent_object_ = parent_object;
}
#endif

}  // namespace content

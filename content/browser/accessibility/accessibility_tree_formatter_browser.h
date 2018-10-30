// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_ACCESSIBILITY_TREE_FORMATTER_BROWSER_H_
#define CONTENT_BROWSER_ACCESSIBILITY_ACCESSIBILITY_TREE_FORMATTER_BROWSER_H_

#include "content/browser/accessibility/accessibility_tree_formatter.h"

#include "base/macros.h"
#include "base/strings/string16.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "content/browser/accessibility/browser_accessibility.h"
#include "content/common/content_export.h"

namespace content {

// Inheritable class for tree formatters that rely on browser infrastructure for
// walking the tree and getting properties. Tree formatters that walk native
// objects should not inherit from this class.
class CONTENT_EXPORT AccessibilityTreeFormatterBrowser
    : public AccessibilityTreeFormatter {
 public:
  std::unique_ptr<base::DictionaryValue> BuildAccessibilityTree(
      BrowserAccessibility* root) override;

 protected:
  void RecursiveBuildAccessibilityTree(const BrowserAccessibility& node,
                                       base::DictionaryValue* dict);
  virtual uint32_t ChildCount(const BrowserAccessibility& node) const;

  virtual BrowserAccessibility* GetChild(const BrowserAccessibility& node,
                                         uint32_t i) const;

  // Add the attributes for each node into the given dict.
  virtual void AddProperties(const BrowserAccessibility& node,
                             base::DictionaryValue* dict) = 0;
};

}  // namespace content

#endif  // CONTENT_BROWSER_ACCESSIBILITY_ACCESSIBILITY_TREE_FORMATTER_BROWSER_H_
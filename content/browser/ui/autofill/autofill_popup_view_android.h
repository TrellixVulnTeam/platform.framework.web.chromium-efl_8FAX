// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_VIEW_ANDROID_H_
#define CONTENT_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_VIEW_ANDROID_H_

#include <jni.h>
#include <stddef.h>

#include "base/android/scoped_java_ref.h"
#include "base/compiler_specific.h"
#include "base/macros.h"
#include "content/browser/ui/autofill/autofill_popup_view.h"
#include "ui/android/view_android.h"

namespace autofill {

class AutofillPopupController;

class AutofillPopupViewAndroid : public AutofillPopupView {
 public:
  explicit AutofillPopupViewAndroid(AutofillPopupController* controller);

  // --------------------------------------------------------------------------
  // Methods called from Java via JNI
  // --------------------------------------------------------------------------
  // Called when an autofill item was selected.
  void SuggestionSelected(JNIEnv* env,
                          const base::android::JavaParamRef<jobject>& obj,
                          jint list_index);

  void DeletionRequested(JNIEnv* env,
                         const base::android::JavaParamRef<jobject>& obj,
                         jint list_index);

  void DeletionConfirmed(JNIEnv* env,
                         const base::android::JavaParamRef<jobject>& obj);

  void PopupDismissed(JNIEnv* env,
                      const base::android::JavaParamRef<jobject>& obj);

  static bool RegisterAutofillPopupViewAndroid(JNIEnv* env);

 protected:
  // AutofillPopupView implementation.
  void Show() override;
  void Hide() override;
  void OnSelectedRowChanged(base::Optional<int> previous_row_selection,
                            base::Optional<int> current_row_selection) override;
  void OnSuggestionsChanged() override;

 private:
  ~AutofillPopupViewAndroid() override;

  AutofillPopupController* controller_;  // weak.

  // The index of the last item the user long-pressed (they will be shown a
  // confirmation dialog).
  int deleting_index_;

  // The corresponding java object.
  base::android::ScopedJavaGlobalRef<jobject> java_object_;

  // Popup view
  ui::ViewAndroid::ScopedAnchorView popup_view_;

  DISALLOW_COPY_AND_ASSIGN(AutofillPopupViewAndroid);

  #include "autofill_popup_view_android_sec.h"
};

}  // namespace autofill

#endif  // CONTENT_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_VIEW_ANDROID_H_

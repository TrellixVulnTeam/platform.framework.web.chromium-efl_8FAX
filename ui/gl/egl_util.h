// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_EGL_UTIL_H_
#define UI_GL_EGL_UTIL_H_

#include <stdint.h>

#include "ui/gl/gl_export.h"

namespace ui {

#if defined(TIZEN_TBM_SUPPORT) && defined(OS_TIZEN_TV_PRODUCT)
enum TzGPUmode { NONSECURE_MODE, SECURE_MODE };

// Setup secure mode of the trust zone.
GL_EXPORT void SetSecureModeGPU(TzGPUmode gpu_mode);
#endif

GL_EXPORT const char* GetEGLErrorString(uint32_t error);

// Returns the last EGL error as a string.
GL_EXPORT const char* GetLastEGLErrorString();

}  // namespace ui

#endif  // UI_GL_EGL_UTIL_H_

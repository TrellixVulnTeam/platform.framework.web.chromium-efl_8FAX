// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MediaValues_h
#define MediaValues_h

#include "core/CoreExport.h"
#include "core/css/CSSPrimitiveValue.h"
#include "platform/heap/Handle.h"
#include "public/platform/PointerProperties.h"
#include "public/platform/ShapeProperties.h"
#include "public/platform/WebDisplayMode.h"

#if defined(USE_EFL)
#include "core/css/ViewMode.h"
#endif

namespace blink {

class Document;
class CSSPrimitiveValue;
class LocalFrame;
enum class ColorSpaceGamut;

class CORE_EXPORT MediaValues : public GarbageCollectedFinalized<MediaValues> {
 public:
  virtual ~MediaValues() {}
  DEFINE_INLINE_VIRTUAL_TRACE() {}

  static MediaValues* CreateDynamicIfFrameExists(LocalFrame*);
  virtual MediaValues* Copy() const = 0;

  static bool ComputeLengthImpl(double value,
                                CSSPrimitiveValue::UnitType,
                                unsigned default_font_size,
                                double viewport_width,
                                double viewport_height,
                                double& result);
  template <typename T>
  static bool ComputeLength(double value,
                            CSSPrimitiveValue::UnitType type,
                            unsigned default_font_size,
                            double viewport_width,
                            double viewport_height,
                            T& result) {
    double temp_result;
    if (!ComputeLengthImpl(value, type, default_font_size, viewport_width,
                           viewport_height, temp_result))
      return false;
    result = clampTo<T>(temp_result);
    return true;
  }
  virtual bool ComputeLength(double value,
                             CSSPrimitiveValue::UnitType,
                             int& result) const = 0;
  virtual bool ComputeLength(double value,
                             CSSPrimitiveValue::UnitType,
                             double& result) const = 0;

  virtual double ViewportWidth() const = 0;
  virtual double ViewportHeight() const = 0;
  virtual int DeviceWidth() const = 0;
  virtual int DeviceHeight() const = 0;
  virtual float DevicePixelRatio() const = 0;
  virtual int ColorBitsPerComponent() const = 0;
  virtual int MonochromeBitsPerComponent() const = 0;
  virtual PointerType PrimaryPointerType() const = 0;
  virtual int AvailablePointerTypes() const = 0;
  virtual HoverType PrimaryHoverType() const = 0;
  virtual int AvailableHoverTypes() const = 0;
  virtual bool ThreeDEnabled() const = 0;
  virtual const String MediaType() const = 0;
  virtual WebDisplayMode DisplayMode() const = 0;
  virtual bool StrictMode() const = 0;
  virtual Document* GetDocument() const = 0;
  virtual bool HasValues() const = 0;
#if defined(USE_EFL)
  virtual blink::ViewMode GetViewMode() const = 0;
#endif

  virtual void OverrideViewportDimensions(double width, double height) = 0;
  virtual DisplayShape GetDisplayShape() const = 0;
  virtual ColorSpaceGamut ColorGamut() const = 0;

 protected:
  static double CalculateViewportWidth(LocalFrame*);
  static double CalculateViewportHeight(LocalFrame*);
  static int CalculateDeviceWidth(LocalFrame*);
  static int CalculateDeviceHeight(LocalFrame*);
  static bool CalculateStrictMode(LocalFrame*);
  static float CalculateDevicePixelRatio(LocalFrame*);
  static int CalculateColorBitsPerComponent(LocalFrame*);
  static int CalculateMonochromeBitsPerComponent(LocalFrame*);
  static int CalculateDefaultFontSize(LocalFrame*);
  static const String CalculateMediaType(LocalFrame*);
  static WebDisplayMode CalculateDisplayMode(LocalFrame*);
  static bool CalculateThreeDEnabled(LocalFrame*);
  static PointerType CalculatePrimaryPointerType(LocalFrame*);
  static int CalculateAvailablePointerTypes(LocalFrame*);
  static HoverType CalculatePrimaryHoverType(LocalFrame*);
  static int CalculateAvailableHoverTypes(LocalFrame*);
  static DisplayShape CalculateDisplayShape(LocalFrame*);
  static ColorSpaceGamut CalculateColorGamut(LocalFrame*);
#if defined(USE_EFL)
  static blink::ViewMode CalculateViewMode(LocalFrame*);
#endif
};

}  // namespace blink

#endif  // MediaValues_h

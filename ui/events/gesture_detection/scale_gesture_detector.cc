// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/events/gesture_detection/scale_gesture_detector.h"

#include <limits.h>

#include <algorithm>
#include <cmath>

#include "base/logging.h"
#include "ui/events/gesture_detection/motion_event.h"
#include "ui/events/gesture_detection/scale_gesture_listeners.h"

using base::TimeDelta;
using base::TimeTicks;

namespace ui {
namespace {

// Using a small epsilon when comparing slop distances allows pixel perfect
// slop determination when using fractional DPI coordinates (assuming the slop
// region and DPI scale are reasonably proportioned).
const float kSlopEpsilon = .05f;

const float kScaleFactor = .5f;

// Samsung DA for multi-touch patent
#if defined(S_TERRACE_SUPPORT)
const float kAreaRateThreshold = 1.0f;
const int kNotScalePointerCount = 4;
#endif

}  // namespace

// Note: These constants were taken directly from the default (unscaled)
// versions found in Android's ViewConfiguration. Do not change these default
// values without explicitly consulting an OWNER.
ScaleGestureDetector::Config::Config()
    : span_slop(16),
      min_scaling_span(200),
      min_pinch_update_span_delta(0),
      stylus_scale_enabled(false) {}

ScaleGestureDetector::Config::~Config() {}

ScaleGestureDetector::ScaleGestureDetector(const Config& config,
                                           ScaleGestureListener* listener)
    : listener_(listener),
#if !defined(S_TERRACE_SUPPORT)
      stylus_scale_enabled_(config.stylus_scale_enabled),
#endif
      focus_x_(0),
      focus_y_(0),
      curr_span_(0),
      prev_span_(0),
      initial_span_(0),
      curr_span_x_(0),
      curr_span_y_(0),
      prev_span_x_(0),
      prev_span_y_(0),
      in_progress_(0),
      span_slop_(0),
      min_span_(0),
      anchored_scale_start_x_(0),
      anchored_scale_start_y_(0),
      anchored_scale_mode_(ANCHORED_SCALE_MODE_NONE),
#if defined(S_TERRACE_SUPPORT)
      // Samsung DA for multi-touch patent
      state_current_(),
      update_previous_(true),
      area_rate_calculating_(false),
      temp_len_before_sqrt_(0),
      curr_len_before_sqrt_(0),
      prev_len_before_sqrt_(0),
#endif
      event_before_or_above_starting_gesture_event_(false) {
  DCHECK(listener_);
  span_slop_ = config.span_slop + kSlopEpsilon;
  min_span_ = config.min_scaling_span + kSlopEpsilon;
}

ScaleGestureDetector::~ScaleGestureDetector() {}

bool ScaleGestureDetector::OnTouchEvent(const MotionEvent& event) {
  curr_time_ = event.GetEventTime();

  const int action = event.GetAction();

// Samsung DA for multi-touch patent
#if defined(S_TERRACE_SUPPORT)
  if (!in_progress_) {
    if (event.GetPointerCount() == kNotScalePointerCount ||
        action == MotionEvent::ACTION_UP ||
        action == MotionEvent::ACTION_CANCEL) {
      ResetArea();
    } else {
      GetArea(event);

      // initailize variable when a new gesture event starts
      const bool config_changed = action == MotionEvent::ACTION_DOWN ||
                                  action == MotionEvent::ACTION_POINTER_UP ||
                                  action == MotionEvent::ACTION_POINTER_DOWN;

      if (config_changed) {
        curr_span_x_ = state_current_.span_x;
        curr_span_y_ = state_current_.span_y;
        curr_len_before_sqrt_ = state_current_.len_before_sqrt;
      }

      if (state_current_.len_before_sqrt > kSlopEpsilon * kSlopEpsilon) {
        if (!area_rate_calculating_) {
          temp_len_before_sqrt_ = state_current_.len_before_sqrt;
          area_rate_calculating_ = true;
        }

        float area_rate;
        if (area_rate_calculating_) {
          area_rate =
              (state_current_.len_before_sqrt > temp_len_before_sqrt_) ?
              state_current_.len_before_sqrt / temp_len_before_sqrt_ :
              temp_len_before_sqrt_ / state_current_.len_before_sqrt;
        } else {
          area_rate =
              (state_current_.len_before_sqrt > prev_len_before_sqrt_) ?
              state_current_.len_before_sqrt / prev_len_before_sqrt_ :
              prev_len_before_sqrt_ / state_current_.len_before_sqrt;
        }

        const bool scale_decision =
            area_rate_calculating_ && area_rate > kAreaRateThreshold;
        if (scale_decision) {
          prev_span_x_ = curr_span_x_ = state_current_.span_x;
          prev_span_y_ = curr_span_y_ = state_current_.span_y;
          prev_time_ = curr_time_;
          prev_len_before_sqrt_ = curr_len_before_sqrt_ =
                                  state_current_.len_before_sqrt;
          in_progress_ = listener_->OnScaleBegin(*this, event);
          area_rate_calculating_ = false;
        }
      } else {
        if (action == MotionEvent::ACTION_MOVE) {
          if (update_previous_) {
            prev_span_x_ = curr_span_x_;
            prev_span_y_ = curr_span_y_;
            prev_len_before_sqrt_ = curr_len_before_sqrt_;
            prev_time_ = curr_time_;
          }
        }
      }
    }
  } else {
    if (action == MotionEvent::ACTION_MOVE) {
      GetArea(event);
      if (state_current_.len_before_sqrt <= 0)
        return true;
      curr_span_x_ = state_current_.span_x;
      curr_span_y_ = state_current_.span_y;
      curr_len_before_sqrt_ = state_current_.len_before_sqrt;
      update_previous_ = listener_->OnScale(*this, event);
    } else {
      listener_->OnScaleEnd(*this, event);
      ResetArea();
    }

    if (update_previous_) {
      prev_span_x_ = curr_span_x_;
      prev_span_y_ = curr_span_y_;
      prev_len_before_sqrt_ = curr_len_before_sqrt_;
      prev_time_ = curr_time_;
    }
  }
#else //S_TERRACE_SUPPORT
  const int count = static_cast<int>(event.GetPointerCount());
  const bool is_stylus_button_down =
      (event.GetButtonState() & MotionEvent::BUTTON_STYLUS_PRIMARY) != 0;

  const bool anchored_scale_cancelled =
      anchored_scale_mode_ == ANCHORED_SCALE_MODE_STYLUS &&
      !is_stylus_button_down;

  const bool stream_complete =
      action == MotionEvent::ACTION_UP ||
      action == MotionEvent::ACTION_CANCEL || anchored_scale_cancelled ||
      (action == MotionEvent::ACTION_POINTER_DOWN && InAnchoredScaleMode());

  if (action == MotionEvent::ACTION_DOWN || stream_complete) {
    // Reset any scale in progress with the listener.
    // If it's an ACTION_DOWN we're beginning a new event stream.
    // This means the app probably didn't give us all the events. Shame on it.
    if (in_progress_) {
      listener_->OnScaleEnd(*this, event);
      ResetScaleWithSpan(0);
    } else if (InAnchoredScaleMode() && stream_complete) {
      ResetScaleWithSpan(0);
    }

    if (stream_complete)
      return true;
  }

  if (!in_progress_ && stylus_scale_enabled_ && !InAnchoredScaleMode() &&
      !stream_complete && is_stylus_button_down) {
    // Start of a stylus scale gesture.
    anchored_scale_start_x_ = event.GetX();
    anchored_scale_start_y_ = event.GetY();
    anchored_scale_mode_ = ANCHORED_SCALE_MODE_STYLUS;
    initial_span_ = 0;
  }

  const bool config_changed = action == MotionEvent::ACTION_DOWN ||
                              action == MotionEvent::ACTION_POINTER_UP ||
                              action == MotionEvent::ACTION_POINTER_DOWN ||
                              anchored_scale_cancelled;

  const bool pointer_up = action == MotionEvent::ACTION_POINTER_UP;
  const int skip_index = pointer_up ? event.GetActionIndex() : -1;

  // Determine focal point.
  float sum_x = 0, sum_y = 0;
  const int unreleased_point_count = pointer_up ? count - 1 : count;
  const float inverse_unreleased_point_count = 1.0f / unreleased_point_count;

  float focus_x;
  float focus_y;
  if (InAnchoredScaleMode()) {
    // In double tap mode, the focal pt is always where the double tap
    // gesture started.
    focus_x = anchored_scale_start_x_;
    focus_y = anchored_scale_start_y_;
    if (event.GetY() < focus_y) {
      event_before_or_above_starting_gesture_event_ = true;
    } else {
      event_before_or_above_starting_gesture_event_ = false;
    }
  } else {
    for (int i = 0; i < count; i++) {
      if (skip_index == i)
        continue;
      sum_x += event.GetX(i);
      sum_y += event.GetY(i);
    }

    focus_x = sum_x * inverse_unreleased_point_count;
    focus_y = sum_y * inverse_unreleased_point_count;
  }

  // Determine average deviation from focal point.
  float dev_sum_x = 0, dev_sum_y = 0;
  for (int i = 0; i < count; i++) {
    if (skip_index == i)
      continue;

    dev_sum_x += std::abs(event.GetX(i) - focus_x);
    dev_sum_y += std::abs(event.GetY(i) - focus_y);
  }

  const float dev_x = dev_sum_x * inverse_unreleased_point_count;
  const float dev_y = dev_sum_y * inverse_unreleased_point_count;

  // Span is the average distance between touch points through the focal point;
  // i.e. the diameter of the circle with a radius of the average deviation from
  // the focal point.
  const float span_x = dev_x * 2;
  const float span_y = dev_y * 2;
  float span;
  if (InAnchoredScaleMode()) {
    span = span_y;
  } else {
    span = std::sqrt(span_x * span_x + span_y * span_y);
  }

  // Dispatch begin/end events as needed.
  // If the configuration changes, notify the app to reset its current state by
  // beginning a fresh scale event stream.
  const bool was_in_progress = in_progress_;
  focus_x_ = focus_x;
  focus_y_ = focus_y;
  if (!InAnchoredScaleMode() && in_progress_ &&
      (span < min_span_ || config_changed)) {
    listener_->OnScaleEnd(*this, event);
    ResetScaleWithSpan(span);
  }
  if (config_changed) {
    prev_span_x_ = curr_span_x_ = span_x;
    prev_span_y_ = curr_span_y_ = span_y;
    initial_span_ = prev_span_ = curr_span_ = span;
  }

  const float min_span = InAnchoredScaleMode() ? span_slop_ : min_span_;
  if (!in_progress_ && span >= min_span &&
      (was_in_progress || std::abs(span - initial_span_) > span_slop_)) {
    prev_span_x_ = curr_span_x_ = span_x;
    prev_span_y_ = curr_span_y_ = span_y;
    prev_span_ = curr_span_ = span;
    prev_time_ = curr_time_;
    in_progress_ = listener_->OnScaleBegin(*this, event);
  }

  // Handle motion; focal point and span/scale factor are changing.
  if (action == MotionEvent::ACTION_MOVE) {
    curr_span_x_ = span_x;
    curr_span_y_ = span_y;
    curr_span_ = span;

    bool update_prev = true;

    if (in_progress_) {
      update_prev = listener_->OnScale(*this, event);
    }

    if (update_prev) {
      prev_span_x_ = curr_span_x_;
      prev_span_y_ = curr_span_y_;
      prev_span_ = curr_span_;
      prev_time_ = curr_time_;
    }
  }
#endif //S_TERRACE_SUPPORT

  return true;
}

bool ScaleGestureDetector::IsInProgress() const { return in_progress_; }

bool ScaleGestureDetector::InAnchoredScaleMode() const {
  return anchored_scale_mode_ != ANCHORED_SCALE_MODE_NONE;
}

float ScaleGestureDetector::GetFocusX() const { return focus_x_; }

float ScaleGestureDetector::GetFocusY() const { return focus_y_; }

float ScaleGestureDetector::GetCurrentSpan() const { return curr_span_; }

float ScaleGestureDetector::GetCurrentSpanX() const { return curr_span_x_; }

float ScaleGestureDetector::GetCurrentSpanY() const { return curr_span_y_; }

float ScaleGestureDetector::GetPreviousSpan() const { return prev_span_; }

float ScaleGestureDetector::GetPreviousSpanX() const { return prev_span_x_; }

float ScaleGestureDetector::GetPreviousSpanY() const { return prev_span_y_; }

float ScaleGestureDetector::GetScaleFactor() const {
// Samsung DA for multi-touch patent
#if defined(S_TERRACE_SUPPORT)
  if (InAnchoredScaleMode()) {
    const bool scale_up = (event_before_or_above_starting_gesture_event_ &&
                          (curr_len_before_sqrt_ < prev_len_before_sqrt_)) ||
                          (!event_before_or_above_starting_gesture_event_ &&
                          (curr_len_before_sqrt_ > prev_len_before_sqrt_));
    const float span_diff =
        (std::abs(1.f -
                  std::sqrt(curr_len_before_sqrt_ / prev_len_before_sqrt_)) *
                  kScaleFactor);
    return prev_len_before_sqrt_ <= 0 ? 1.f
                           : (scale_up ? (1.f + span_diff) : (1.f - span_diff));
  }
  return prev_len_before_sqrt_ > 0 ?
    std::sqrt(curr_len_before_sqrt_ / prev_len_before_sqrt_) : 1;
#else
  if (InAnchoredScaleMode()) {
    // Drag is moving up; the further away from the gesture start, the smaller
    // the span should be, the closer, the larger the span, and therefore the
    // larger the scale.
    const bool scale_up = (event_before_or_above_starting_gesture_event_ &&
                           (curr_span_ < prev_span_)) ||
                          (!event_before_or_above_starting_gesture_event_ &&
                           (curr_span_ > prev_span_));
    const float span_diff =
        (std::abs(1.f - (curr_span_ / prev_span_)) * kScaleFactor);
    return prev_span_ <= 0 ? 1.f
                           : (scale_up ? (1.f + span_diff) : (1.f - span_diff));
  }
  return prev_span_ > 0 ? curr_span_ / prev_span_ : 1;
#endif
}

base::TimeDelta ScaleGestureDetector::GetTimeDelta() const {
  return curr_time_ - prev_time_;
}

base::TimeTicks ScaleGestureDetector::GetEventTime() const {
  return curr_time_;
}

void ScaleGestureDetector::GetArea(const MotionEvent& event) {
#if defined(S_TERRACE_SUPPORT)
  float x, y;
  state_current_.reset();

  if (InAnchoredScaleMode()) {
    // In double tap mode, the focal pt is always where the double tap
    // gesture started
    focus_x_ = anchored_scale_start_x_;
    focus_y_ = anchored_scale_start_y_;
    x = event.GetX();
    y = event.GetY();
    state_current_.span_x = std::abs(focus_x_ - x);
    state_current_.span_y = std::abs(focus_y_ - y);
    state_current_.len_before_sqrt =
        (state_current_.span_y * state_current_.span_y);
    event_before_or_above_starting_gesture_event_ = (y < focus_y_);
  } else {
    float focus_x = 0, focus_y = 0;
    float min_x = 0, min_y = 0, max_x = 0, max_y = 0;
    int count = event.GetPointerCount();
    for (int i = 0 ; i < count; i++) {
      x = event.GetX(i);
      y = event.GetY(i);

      focus_x += x;
      focus_y += y;

      if (i == 0) {
        min_x = max_x = x;
        min_y = max_y = y;
        continue;
      }
      if (min_x > x)
        min_x = x;
      if (min_y > y)
        min_y = y;
      if (max_x < x)
        max_x = x;
      if (max_y < y)
        max_y = y;
    }

    DCHECK(max_x != 0);
    focus_x_ = focus_x / count;
    focus_y_ = focus_y / count;
    state_current_.span_x = (max_x - min_x);
    state_current_.span_y = (max_y - min_y);
    state_current_.len_before_sqrt =
        (state_current_.span_x * state_current_.span_x) +
        (state_current_.span_y * state_current_.span_y);
  }
#endif
}

void ScaleGestureDetector::ResetArea() {
#if defined(S_TERRACE_SUPPORT)
  in_progress_ = false;
  update_previous_ = true;
  area_rate_calculating_ = false;
  anchored_scale_mode_ = ANCHORED_SCALE_MODE_NONE;
#endif
}

bool ScaleGestureDetector::OnDoubleTap(const MotionEvent& ev) {
  // Double tap: start watching for a swipe.
  anchored_scale_start_x_ = ev.GetX();
  anchored_scale_start_y_ = ev.GetY();
  anchored_scale_mode_ = ANCHORED_SCALE_MODE_DOUBLE_TAP;
  return true;
}

void ScaleGestureDetector::ResetScaleWithSpan(float span) {
  in_progress_ = false;
  initial_span_ = span;
  anchored_scale_mode_ = ANCHORED_SCALE_MODE_NONE;
}

}  // namespace ui

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOWNLOAD_INTERNAL_LOGGER_IMPL_H_
#define COMPONENTS_DOWNLOAD_INTERNAL_LOGGER_IMPL_H_

#include <memory>

#include "base/macros.h"
#include "base/observer_list.h"
#include "components/download/internal/log_sink.h"
#include "components/download/public/logger.h"

namespace base {
class Value;
}

namespace download {

class LogSource;

// The internal Logger implementation.  Note that this Logger will not do any
// actual work in response to LogSink requests if there are no Observers
// registered.  Any calls to the Logger API will still be honored though.
class LoggerImpl : public Logger, public LogSink {
 public:
  LoggerImpl();
  ~LoggerImpl() override;

  void SetLogSource(LogSource* log_source);

 private:
  // Logger implementation.
  void AddObserver(Observer* observer) override;
  void RemoveObserver(Observer* observer) override;
  base::Value GetServiceStatus() override;

  // LogSink implementation.
  void OnServiceStatusChanged() override;

  LogSource* log_source_;
  base::ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(LoggerImpl);
};

}  // namespace download

#endif  // COMPONENTS_DOWNLOAD_INTERNAL_LOGGER_IMPL_H_

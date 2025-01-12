# Copyright 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import json
import os
import subprocess
import tempfile

from telemetry.core import util
from telemetry.core.backends.chrome import android_browser_finder
from telemetry.core.platform import profiler

class SamsungMemReportProfiler(profiler.Profiler):
  """Android-specific, collects 'memreport' graphs."""

  def __init__(self, browser_backend, platform_backend, output_path, state):
    super(SamsungMemReportProfiler, self).__init__(
        browser_backend, platform_backend, output_path, state)
    self.results = None

    self._memreport_output = tempfile.TemporaryFile('a+')
    self._memreport = subprocess.Popen(
        [os.path.join(util.GetChromiumSrcDir(),
                      'tools', 'android', 'memdump', 'samsung_memreport.py'),
         '--package', browser_backend.package, '--interval', '1'],
         stdout=self._memreport_output,
         stdin=subprocess.PIPE)

  @classmethod
  def name(cls):
    return 'samsung-memreport'

  @classmethod
  def is_supported(cls, browser_type):
    if browser_type == 'any':
      return android_browser_finder.CanFindAvailableBrowsers()
    return browser_type.startswith('android')

  def CollectProfile(self):
    self._memreport.communicate('\n')
    self._memreport.wait()
    self._memreport_output.seek(0)
    self.results = json.loads(self._memreport_output.read())

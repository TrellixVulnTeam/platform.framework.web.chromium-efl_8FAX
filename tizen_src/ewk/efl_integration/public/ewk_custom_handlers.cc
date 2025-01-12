/*
 * Copyright (C) 2012-2016 Samsung Electronics. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY SAMSUNG ELECTRONICS. AND ITS CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SAMSUNG ELECTRONICS. OR ITS
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ewk_custom_handlers_internal.h"

Eina_Stringshare* ewk_custom_handlers_data_target_get(
    const Ewk_Custom_Handlers_Data*) {
  return nullptr;
}

Eina_Stringshare* ewk_custom_handlers_data_base_url_get(
    const Ewk_Custom_Handlers_Data*) {
  return nullptr;
}

Eina_Stringshare* ewk_custom_handlers_data_url_get(
    const Ewk_Custom_Handlers_Data*) {
  return nullptr;
}

Eina_Stringshare* ewk_custom_handlers_data_title_get(
    const Ewk_Custom_Handlers_Data*) {
  return nullptr;
}

void ewk_custom_handlers_data_result_set(Ewk_Custom_Handlers_Data*,
    Ewk_Custom_Handlers_State) {
}

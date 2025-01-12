// Copyright (c) 2015 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "browser/ssl_host_state_delegate_efl.h"

#include "net/base/hash_value.h"

using content::SSLHostStateDelegate;

namespace content {

namespace internal {
/* LCOV_EXCL_START */
net::SHA256HashValue getChainFingerprint256(const net::X509Certificate& cert) {
  net::SHA256HashValue fingerprint =
      net::X509Certificate::CalculateChainFingerprint256(
          cert.os_cert_handle(), cert.GetIntermediateCertificates());
  return fingerprint;
}

CertPolicy::CertPolicy() {
}

CertPolicy::~CertPolicy() {
}

// For an allowance, we consider a given |cert| to be a match to a saved
// allowed cert if the |error| is an exact match to or subset of the errors
// in the saved CertStatus.
bool CertPolicy::Check(const net::X509Certificate& cert,
                       net::CertStatus error) const {
  net::SHA256HashValue fingerprint = getChainFingerprint256(cert);
  std::map<net::SHA256HashValue, net::CertStatus,
           net::SHA256HashValueLessThan>::const_iterator allowed_iter =
      allowed_.find(fingerprint);
  if ((allowed_iter != allowed_.end()) && (allowed_iter->second & error) &&
      ((allowed_iter->second & error) == error)) {
    return true;
  }
  return false;
}

void CertPolicy::Allow(const net::X509Certificate& cert,
                       net::CertStatus error) {
  // If this same cert had already been saved with a different error status,
  // this will replace it with the new error status.
  net::SHA256HashValue fingerprint = getChainFingerprint256(cert);
  allowed_[fingerprint] = error;
}
/* LCOV_EXCL_STOP */

}  // namespace internal

SSLHostStateDelegateEfl::SSLHostStateDelegateEfl() {
}

SSLHostStateDelegateEfl::~SSLHostStateDelegateEfl() {
}

/* LCOV_EXCL_START */
void SSLHostStateDelegateEfl::HostRanInsecureContent(const std::string& host,
                                                     int child_id,
                                                     InsecureContentType content_type) {
  // Intentional no-op for efl WebView.
}
/* LCOV_EXCL_STOP */

bool SSLHostStateDelegateEfl::DidHostRunInsecureContent(const std::string& host,
                                                        int child_id,
                                                        InsecureContentType content_type) const {
  // Intentional no-op for efl WebView.
  return false;
}

/* LCOV_EXCL_START */
void SSLHostStateDelegateEfl::AllowCert(const std::string& host,
                                       const net::X509Certificate& cert,
                                       net::CertStatus error) {
  cert_policy_for_host_[host].Allow(cert, error);
}

void SSLHostStateDelegateEfl::Clear(
    const base::Callback<bool(const std::string&)>& host_filter) {
  cert_policy_for_host_.clear();
}

SSLHostStateDelegate::CertJudgment SSLHostStateDelegateEfl::QueryPolicy(
    const std::string& host,
    const net::X509Certificate& cert,
    net::CertStatus error,
    bool* expired_previous_decision) {
  return cert_policy_for_host_[host].Check(cert, error)
             ? SSLHostStateDelegate::ALLOWED
             : SSLHostStateDelegate::DENIED;
}

void SSLHostStateDelegateEfl::RevokeUserAllowExceptions(
    const std::string& host) {
  cert_policy_for_host_.erase(host);
}
/* LCOV_EXCL_STOP */

bool SSLHostStateDelegateEfl::HasAllowException(const std::string& host) const {
  return false;
}

}

// Copyright (c) 2015 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SSL_HOST_STATE_DELEGATE_EFL_H_
#define SSL_HOST_STATE_DELEGATE_EFL_H_

#include <map>
#include <string>

#include "content/public/browser/ssl_host_state_delegate.h"
#include "net/base/hash_value.h"
#include "net/cert/cert_status_flags.h"
#include "net/cert/x509_certificate.h"

namespace content {

namespace internal {
// This class maintains the policy for storing actions on certificate errors.
class CertPolicy {
 public:
  CertPolicy();
  ~CertPolicy();
  // Returns true if the user has decided to proceed through the ssl error
  // before. For a certificate to be allowed, it must not have any
  // *additional* errors from when it was allowed.
  bool Check(const net::X509Certificate& cert, net::CertStatus error) const;

  // Causes the policy to allow this certificate for a given |error|. And
  // remember the user's choice.
  void Allow(const net::X509Certificate& cert, net::CertStatus error);

 private:
  // The set of fingerprints of allowed certificates.
  std::map<net::SHA256HashValue, net::CertStatus, net::SHA256HashValueLessThan>
      allowed_;
};

}  // namespace internal

class SSLHostStateDelegateEfl : public content::SSLHostStateDelegate {
 public:
  SSLHostStateDelegateEfl();
  ~SSLHostStateDelegateEfl() override;

  // Records that |cert| is permitted to be used for |host| in the future, for
  // a specified |error| type.
  void AllowCert(const std::string& host,
                 const net::X509Certificate& cert,
                 net::CertStatus error) override;

  void Clear(const base::Callback<bool(const std::string&)>& host_filter) override;

  // Queries whether |cert| is allowed or denied for |host| and |error|.
  content::SSLHostStateDelegate::CertJudgment QueryPolicy(
      const std::string& host,
      const net::X509Certificate& cert,
      net::CertStatus error,
      bool* expired_previous_decision) override;

  // Records that a host has run insecure content.
  void HostRanInsecureContent(const std::string& host,
                              int child_id,
                              InsecureContentType content_type) override;

  // Returns whether the specified host ran insecure content.
  bool DidHostRunInsecureContent(const std::string& host,
                                 int child_id,
                                 InsecureContentType content_type) const override;

  // Revokes all SSL certificate error allow exceptions made by the user for
  // |host|.
  void RevokeUserAllowExceptions(const std::string& host) override;

  // Returns whether the user has allowed a certificate error exception for
  // |host|. This does not mean that *all* certificate errors are allowed, just
  // that there exists an exception. To see if a particular certificate and
  // error combination exception is allowed, use QueryPolicy().
  bool HasAllowException(const std::string& host) const override;

 private:
  // Certificate policies for each host.
  std::map<std::string, internal::CertPolicy> cert_policy_for_host_;

  DISALLOW_COPY_AND_ASSIGN(SSLHostStateDelegateEfl);
};

}

#endif  // SSL_HOST_STATE_DELEGATE_EFL_H_

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_SIGNIN_CORE_BROWSER_TEST_SIGNIN_CLIENT_H_
#define COMPONENTS_SIGNIN_CORE_BROWSER_TEST_SIGNIN_CLIENT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/scoped_temp_dir.h"
#include "base/memory/ref_counted.h"
#include "components/signin/core/browser/signin_client.h"
#include "net/url_request/url_request_test_util.h"

// An implementation of SigninClient for use in unittests. Instantiates test
// versions of the various objects that SigninClient is required to provide as
// part of its interface.
class TestSigninClient : public SigninClient {
 public:
  TestSigninClient();
  virtual ~TestSigninClient();

  // SigninClient implementation that is specialized for unit tests.

  // Returns NULL.
  // NOTE: This should be changed to return a properly-initalized PrefService
  // once there is a unit test that requires it.
  virtual PrefService* GetPrefs() OVERRIDE;

  // Returns a pointer to a loaded database.
  virtual scoped_refptr<TokenWebData> GetDatabase() OVERRIDE;

  // Returns true.
  virtual bool CanRevokeCredentials() OVERRIDE;

  // Returns a TestURLRequestContextGetter.
  virtual net::URLRequestContextGetter* GetURLRequestContext() OVERRIDE;

 private:
  // Loads the token database.
  void LoadDatabase();

  base::ScopedTempDir temp_dir_;
  scoped_refptr<net::TestURLRequestContextGetter> request_context_;
  scoped_refptr<TokenWebData> database_;

  DISALLOW_COPY_AND_ASSIGN(TestSigninClient);
};

#endif  // COMPONENTS_SIGNIN_CORE_BROWSER_TEST_SIGNIN_CLIENT_H_

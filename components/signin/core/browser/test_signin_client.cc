// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/logging.h"
#include "components/signin/core/browser/test_signin_client.h"
#include "components/signin/core/browser/webdata/token_service_table.h"
#include "components/webdata/common/web_data_service_base.h"
#include "components/webdata/common/web_database_service.h"
#include "testing/gtest/include/gtest/gtest.h"

TestSigninClient::TestSigninClient()
    : request_context_(new net::TestURLRequestContextGetter(
          base::MessageLoopProxy::current())) {
  LoadDatabase();
}

TestSigninClient::~TestSigninClient() {}

PrefService* TestSigninClient::GetPrefs() { return NULL; }

scoped_refptr<TokenWebData> TestSigninClient::GetDatabase() {
  return database_;
}

bool TestSigninClient::CanRevokeCredentials() { return true; }

net::URLRequestContextGetter* TestSigninClient::GetURLRequestContext() {
  return request_context_;
}

void TestSigninClient::LoadDatabase() {
  ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
  base::FilePath path = temp_dir_.path().AppendASCII("TestWebDB");
  scoped_refptr<WebDatabaseService> web_database =
      new WebDatabaseService(path,
                             base::MessageLoopProxy::current(),
                             base::MessageLoopProxy::current());
  web_database->AddTable(scoped_ptr<WebDatabaseTable>(new TokenServiceTable()));
  web_database->LoadDatabase();
  database_ = new TokenWebData(web_database,
                               base::MessageLoopProxy::current(),
                               base::MessageLoopProxy::current(),
                               WebDataServiceBase::ProfileErrorCallback());
  database_->Init();
}

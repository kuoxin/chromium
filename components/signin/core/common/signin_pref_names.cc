// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/signin/core/common/signin_pref_names.h"

namespace prefs {

// String the identifies the last user that logged into sync and other
// google services. As opposed to kGoogleServicesUsername, this value is not
// cleared on signout, but while the user is signed in the two values will
// be the same.
const char kGoogleServicesLastUsername[] = "google.services.last_username";

// Obfuscated account ID that identifies the current user logged into sync and
// other google services.
const char kGoogleServicesUserAccountId[] = "google.services.user_account_id";

// String that identifies the current user logged into sync and other google
// services.
const char kGoogleServicesUsername[] = "google.services.username";

// Local state pref containing a string regex that restricts which accounts
// can be used to log in to chrome (e.g. "*@google.com"). If missing or blank,
// all accounts are allowed (no restrictions).
const char kGoogleServicesUsernamePattern[] =
    "google.services.username_pattern";

// Boolean which stores if the user is allowed to signin to chrome.
const char kSigninAllowed[] = "signin.allowed";

}  // namespace prefs

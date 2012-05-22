// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_FILEAPI_FILE_WRITER_H_
#define WEBKIT_FILEAPI_FILE_WRITER_H_
#pragma once

#include "base/basictypes.h"
#include "net/base/completion_callback.h"

namespace net {
class IOBuffer;
}

namespace fileapi {

// A generic interface for writing to a file-like object.
//
// TODO(kinuko): Consider better naming. (http://crbug.com/128483)
// Note: this does not directly correspond to FileWriter in File API (which
// is implemented by WebCore::FileWriter), though this class is used to
// implement a part of it.  FileWriterDelegate is NOT a delegate of this
// class either.
class FileWriter {
 public:
  // Closes the file. If there's an in-flight operation, it is canceled (i.e.,
  // the callback function associated with the operation is not called).
  virtual ~FileWriter() {}

  // Writes to the current cursor position asynchronously.
  //
  // Up to buf_len bytes will be written.  (In other words, partial
  // writes are allowed.) If the write completed synchronously, it returns
  // the number of bytes written. If the operation could not be performed, it
  // returns an error code. Otherwise, net::ERR_IO_PENDING is returned, and the
  // callback will be run on the thread where Write() was called when the write
  // has completed.
  //
  // This errors out (either synchronously or via callback) with:
  //   net::ERR_FILE_NOT_FOUND: When the target file is not found.
  //   net::ERR_ACCESS_DENIED: When the target file is a directory or
  //      the writer has no permission on the file.
  //   net::ERR_FILE_NO_SPACE: When the write will result in out of quota
  //      or there is not enough room left on the disk.
  //
  // It is invalid to call Write while there is an in-flight async operation.
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    const net::CompletionCallback& callback) = 0;

  // Cancels an in-flight async operation.
  //
  // If the cancel is finished synchronously, it returns net::OK. If the
  // cancel could not be performed, it returns an error code. Especially when
  // there is no in-flight operation, net::ERR_UNEXPECTED is returned.
  // Otherwise, net::ERR_IO_PENDING is returned, and the callback will be run on
  // the thread where Cancel() was called when the cancel has completed. It is
  // invalid to call Cancel() more than once on the same async operation.
  //
  // In either case, the callback function passed to the in-flight async
  // operation is dismissed immediately when Cancel() is called, and thus
  // will never be called.
  virtual int Cancel(const net::CompletionCallback& callback) = 0;
};

}  // namespace fileapi

#endif  // WEBKIT_FILEAPI_FILE_WRITER_H_

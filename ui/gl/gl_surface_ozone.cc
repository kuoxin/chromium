// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_surface.h"

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/ozone/surface_factory_ozone.h"
#include "ui/gfx/ozone/surface_ozone.h"
#include "ui/gl/gl_implementation.h"
#include "ui/gl/gl_surface_egl.h"
#include "ui/gl/gl_surface_osmesa.h"
#include "ui/gl/gl_surface_stub.h"

namespace gfx {

namespace {

// A thin wrapper around GLSurfaceEGL that owns the EGLNativeWindow
class GL_EXPORT GLSurfaceOzoneEGL : public NativeViewGLSurfaceEGL {
 public:
  GLSurfaceOzoneEGL(scoped_ptr<SurfaceOzone> ozone_surface)
      : NativeViewGLSurfaceEGL(ozone_surface->GetEGLNativeWindow()),
        ozone_surface_(ozone_surface.Pass()) {}

  virtual ~GLSurfaceOzoneEGL() {
    Destroy();  // EGL surface must be destroyed before SurfaceOzone
  }

 private:
  // The native surface. Deleting this is allowed to free the EGLNativeWindow.
  scoped_ptr<SurfaceOzone> ozone_surface_;

  DISALLOW_COPY_AND_ASSIGN(GLSurfaceOzoneEGL);
};

}  // namespace

// static
bool GLSurface::InitializeOneOffInternal() {
  switch (GetGLImplementation()) {
    case kGLImplementationEGLGLES2:
      if (gfx::SurfaceFactoryOzone::GetInstance()->InitializeHardware() !=
          gfx::SurfaceFactoryOzone::INITIALIZED) {
        LOG(ERROR) << "Ozone failed to initialize hardware";
        return false;
      }

      if (!GLSurfaceEGL::InitializeOneOff()) {
        LOG(ERROR) << "GLSurfaceEGL::InitializeOneOff failed.";
        return false;
      }

      return true;
    case kGLImplementationOSMesaGL:
    case kGLImplementationMockGL:
      return true;
    default:
      return false;
  }
}

// static
scoped_refptr<GLSurface> GLSurface::CreateViewGLSurface(
    gfx::AcceleratedWidget window) {
  if (GetGLImplementation() == kGLImplementationOSMesaGL) {
    scoped_refptr<GLSurface> surface(new GLSurfaceOSMesaHeadless());
    if (!surface->Initialize())
      return NULL;
    return surface;
  }
  DCHECK(GetGLImplementation() == kGLImplementationEGLGLES2);
  if (window != kNullAcceleratedWidget) {
    scoped_ptr<SurfaceOzone> surface_ozone =
        SurfaceFactoryOzone::GetInstance()->CreateSurfaceForWidget(window);
    if (!surface_ozone->InitializeEGL())
      return NULL;
    scoped_refptr<GLSurfaceOzoneEGL> surface =
        new GLSurfaceOzoneEGL(surface_ozone.Pass());
    if (!surface->Initialize(surface_ozone->CreateVSyncProvider()))
      return NULL;
    return surface;
  } else {
    scoped_refptr<GLSurface> surface = new GLSurfaceStub();
    if (surface->Initialize())
      return surface;
  }
  return NULL;
}

// static
scoped_refptr<GLSurface> GLSurface::CreateOffscreenGLSurface(
    const gfx::Size& size) {
  switch (GetGLImplementation()) {
    case kGLImplementationOSMesaGL: {
      scoped_refptr<GLSurface> surface(new GLSurfaceOSMesa(1, size));
      if (!surface->Initialize())
        return NULL;

      return surface;
    }
    case kGLImplementationEGLGLES2: {
      scoped_refptr<GLSurface> surface;
      if (GLSurfaceEGL::IsEGLSurfacelessContextSupported() &&
          (size.width() == 0 && size.height() == 0)) {
        surface = new SurfacelessEGL(size);
      } else
        surface = new PbufferGLSurfaceEGL(size);

      if (!surface->Initialize())
        return NULL;
      return surface;
    }
    default:
      NOTREACHED();
      return NULL;
  }
}

EGLNativeDisplayType GetPlatformDefaultEGLNativeDisplay() {
  return SurfaceFactoryOzone::GetInstance()->GetNativeDisplay();
}

}  // namespace gfx

/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Thomas Perl <thomas.perl@jolla.com>
**
** Based on hwcomposer_backend.h
**
** Copyright (C) 2016 Michael Serpieri <mickybart@pygoscelis.org>
**
** This file is part of the surfaceflinger plugin.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SURFACEFLINGER_BACKEND_H
#define SURFACEFLINGER_BACKEND_H

#include <sys/types.h>
#include <sync/sync.h>

#include <android-config.h>
#include <hybris/surface_flinger/surface_flinger_compatibility_layer.h>
#include <hardware/hwcomposer_defs.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <qdebug.h>


// Evaluate "x", if it doesn't return zero, print a warning
#define SF_PLUGIN_EXPECT_ZERO(x) \
    { int res; if ((res = (x)) != 0) \
        qWarning("QPA-SF: %s in %s returned %i", (#x), __func__, res); }

// Evaluate "x", if it isn't NULL, print a warning
#define SF_PLUGIN_EXPECT_NULL(x) \
    { void *res; if ((res = (x)) != NULL) \
        qWarning("QPA-SF: %s in %s returned %x", (#x), __func__, (unsigned int)res); }

// Evaluate "x", if it is NULL, exit with a fatal error
#define SF_PLUGIN_FATAL(x) \
    qFatal("QPA-SF: %s in %s", x, __func__)

// Evaluate "x", if it is NULL, exit with a fatal error
#define SF_PLUGIN_ASSERT_NOT_NULL(x) \
    { void *res; if ((res = (x)) == NULL) \
        qFatal("QPA-SF: %s in %s returned %x", (#x), __func__, (unsigned int)res); }

// Evaluate "x", if it doesn't return zero, exit with a fatal error
#define SF_PLUGIN_ASSERT_ZERO(x) \
    { int res; if ((res = (x)) != 0) \
        qFatal("QPA-SF: %s in %s returned %i", (#x), __func__, res); }


class SurfaceFlingerBackend {
public:
    static SurfaceFlingerBackend *create();
    static void destroy(SurfaceFlingerBackend *backend);

    EGLNativeDisplayType display();
    EGLNativeWindowType createWindow(int width, int height);
    void destroyWindow(EGLNativeWindowType window);
    void swap(EGLNativeDisplayType display, EGLSurface surface);
    void sleepDisplay(bool sleep);
    float refreshRate();

protected:
    SurfaceFlingerBackend(struct SfClient* sf_client);
    virtual ~SurfaceFlingerBackend();

    struct SfClient* sf_client;
    struct SfSurface* sf_surface;
};

#endif /* SURFACEFLINGER_BACKEND_H */

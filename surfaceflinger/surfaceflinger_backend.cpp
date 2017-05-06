/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Thomas Perl <thomas.perl@jolla.com>
**
** Based on hwcomposer_backend.cpp
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

#include "surfaceflinger_backend.h"
#include "hybris_nativebufferext.h"

SurfaceFlingerBackend::SurfaceFlingerBackend(struct SfClient* sf_client)
    : sf_client(sf_client), sf_surface(NULL)
{
    sleepDisplay(false);
    screen_info = new SurfaceFlingerScreenInfo(0);
}

SurfaceFlingerBackend::~SurfaceFlingerBackend()
{
    // TODO: Add support to release resources
    delete screen_info;
}

SurfaceFlingerBackend *
SurfaceFlingerBackend::create()
{
    struct SfClient* sf_client = sf_client_create();
    SF_PLUGIN_ASSERT_NOT_NULL(sf_client);
    
    return new SurfaceFlingerBackend(sf_client);
}

void
SurfaceFlingerBackend::destroy(SurfaceFlingerBackend *backend)
{
    delete backend;
}

EGLNativeDisplayType
SurfaceFlingerBackend::display()
{
    //EGL_DEFAULT_DISPLAY
    return sf_client_get_egl_display(sf_client);
}

SurfaceFlingerScreenInfo*
SurfaceFlingerBackend::screenInfo()
{
    return screen_info;
}

EGLNativeWindowType
SurfaceFlingerBackend::createWindow(int width, int height)
{
    SfSurfaceCreationParameters params = {
            0,
            0,
            width,
            height,
            HYBRIS_PIXEL_FORMAT_RGBA_8888, //PIXEL_FORMAT_RGBA_8888
            INT_MAX, //layer position
            1.0f, //opacity
            0, //create_egl_window_surface
            "qt5-qpa-plugin"
    };

    sf_surface = sf_surface_create(sf_client, &params);
    SF_PLUGIN_ASSERT_NOT_NULL(sf_surface);
    
    //sf_surface_make_current(sf_surface);
    
    return sf_surface_get_egl_native_window(sf_surface);
}

void
SurfaceFlingerBackend::destroyWindow(EGLNativeWindowType window)
{
    Q_UNUSED(window);
}

void
SurfaceFlingerBackend::swap(EGLNativeDisplayType display, EGLSurface surface)
{
    eglSwapBuffers(display, surface);
}

void
SurfaceFlingerBackend::sleepDisplay(bool sleep)
{
    if (sleep) {
        sf_blank(0);
    } else {
        sf_unblank(0);
    }
}


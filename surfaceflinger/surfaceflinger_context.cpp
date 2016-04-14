/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** Based on qeglfshooks_stub.cpp, part of the plugins of the Qt Toolkit.
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Thomas Perl <thomas.perl@jolla.com>
**
** Based on hwcomposer_context.cpp
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

#include "surfaceflinger_context.h"

#include "qeglfscontext.h"
#include "surfaceflinger_screeninfo.h"
#include "surfaceflinger_backend.h"

#include <qcoreapplication.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <inttypes.h>
#include <unistd.h>
#include <signal.h>


QT_BEGIN_NAMESPACE


static void exit_qt_gracefully(int sig)
{
    qDebug("Exiting on signal: %d", sig);
    QCoreApplication::exit(0);
}

SurfaceFlingerContext::SurfaceFlingerContext()
    : backend(NULL)
    , display_off(false)
    , window_created(false)
{
    // We need to catch the SIGTERM and SIGINT signals, so that we can do a
    // proper shutdown of Qt and the plugin, and avoid crashes, hangs and
    // reboots in cases where we don't properly close the hwcomposer.
    struct sigaction new_action;
    new_action.sa_handler = exit_qt_gracefully;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGTERM, &new_action, NULL);
    sigaction(SIGINT, &new_action, NULL);

    // Create SurfaceFlinger Client Backend
    backend = SurfaceFlingerBackend::create();
    SF_PLUGIN_ASSERT_NOT_NULL(backend);
}

SurfaceFlingerContext::~SurfaceFlingerContext()
{
    // Turn display off
    sleepDisplay(true);

    // Properly clean up surfaceflinger backend
    SurfaceFlingerBackend::destroy(backend);
}

EGLNativeDisplayType SurfaceFlingerContext::platformDisplay() const
{
    return backend->display();
}

QSizeF SurfaceFlingerContext::physicalScreenSize() const
{
    return backend->screenInfo()->physicalScreenSize();
}

int SurfaceFlingerContext::screenDepth() const
{
    return backend->screenInfo()->screenDepth();
}

QSize SurfaceFlingerContext::screenSize() const
{
    return backend->screenInfo()->screenSize();
}

QSurfaceFormat SurfaceFlingerContext::surfaceFormatFor(const QSurfaceFormat &inputFormat) const
{
    QSurfaceFormat newFormat = inputFormat; 
    if (screenDepth() == 16) {
        newFormat.setRedBufferSize(5);
        newFormat.setGreenBufferSize(6);
        newFormat.setBlueBufferSize(5);
    } else {
        newFormat.setStencilBufferSize(8);
        newFormat.setAlphaBufferSize(8);
        newFormat.setRedBufferSize(8);
        newFormat.setGreenBufferSize(8);
        newFormat.setBlueBufferSize(8);
    }

    return newFormat;
}

EGLNativeWindowType SurfaceFlingerContext::createNativeWindow(const QSurfaceFormat &format)
{
    Q_UNUSED(format);

    if (window_created) {
        SF_PLUGIN_FATAL("There can only be one window, someone tried to create more.");
    }

    window_created = true;
    QSize size = screenSize();
    return backend->createWindow(size.width(), size.height());
}

void SurfaceFlingerContext::destroyNativeWindow(EGLNativeWindowType window)
{
    return backend->destroyWindow(window);
}

void SurfaceFlingerContext::swapToWindow(QEglFSContext *context, QPlatformSurface *surface)
{
    if (display_off) {
        qWarning("Swap requested while display is off");
        return;
    }

    EGLDisplay egl_display = context->eglDisplay();
    EGLSurface egl_surface = context->eglSurfaceForPlatformSurface(surface);
    return backend->swap(egl_display, egl_surface);
}

void SurfaceFlingerContext::sleepDisplay(bool sleep)
{
    if (sleep) {
        qDebug("sleepDisplay");
        display_off = true;
    } else {
        qDebug("unsleepDisplay");
        display_off = false;
    }

    backend->sleepDisplay(sleep);
}

qreal SurfaceFlingerContext::refreshRate() const
{
    return backend->screenInfo()->refreshRate();
}

QT_END_NAMESPACE

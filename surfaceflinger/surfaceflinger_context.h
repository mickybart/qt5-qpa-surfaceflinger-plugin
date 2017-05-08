/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** Based on qeglfshooks.h, part of the plugins of the Qt Toolkit.
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Thomas Perl <thomas.perl@jolla.com>
**
** Based on hwcomposer_context.h
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

#ifndef SURFACEFLINGER_CONTEXT_H
#define SURFACEFLINGER_CONTEXT_H

#include <qpa/qplatformintegration.h>
#include <qpa/qplatformscreen.h>
#include <QtGui/QSurfaceFormat>
#include <QtGui/QImage>
#include <EGL/egl.h>
#include <QtEglSupport/private/qeglplatformcontext_p.h>

QT_BEGIN_NAMESPACE

class QEglFSContext;
class SurfaceFlingerScreenInfo;
class SurfaceFlingerBackend;

class SurfaceFlingerContext
{
public:
    SurfaceFlingerContext();
    ~SurfaceFlingerContext();

    QSizeF physicalScreenSize() const;
    QSize screenSize() const;
    int screenDepth() const;

    QSurfaceFormat surfaceFormatFor(const QSurfaceFormat &inputFormat) const;

    EGLNativeDisplayType platformDisplay() const;
    EGLNativeWindowType createNativeWindow(const QSurfaceFormat &format);
    void destroyNativeWindow(EGLNativeWindowType window);

    void swapToWindow(QEglFSContext *context, QPlatformSurface *surface);

    void sleepDisplay(bool sleep);
    qreal refreshRate() const;

private:
    SurfaceFlingerBackend *backend;
    bool display_off;
    bool window_created;
};

QT_END_NAMESPACE

#endif // SURFACEFLINGER_CONTEXT_H

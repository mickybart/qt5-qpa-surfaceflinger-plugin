TARGET = surfaceflinger

PLUGIN_TYPE = platforms
PLUGIN_CLASS_NAME = QEglFSsfIntegrationPlugin
load(qt_plugin)

INCLUDEPATH += .
DEPENDPATH += .

SOURCES += main.cpp

SOURCES += surfaceflinger_context.cpp
HEADERS += surfaceflinger_context.h

SOURCES += surfaceflinger_screeninfo.cpp
HEADERS += surfaceflinger_screeninfo.h

SOURCES += surfaceflinger_backend.cpp
HEADERS += surfaceflinger_backend.h

QT += core-private gui-private platformsupport-private

DEFINES += QEGL_EXTRA_DEBUG
CONFIG += egl qpa/genericunixfontdatabase

CONFIG += link_pkgconfig

# For linking against libQt5PlatformSupport.a
PKGCONFIG += libudev glib-2.0 mtdev

# libhybris / droid integration
PKGCONFIG += android-headers libsf hybris-egl-platform

# Avoid X11 header collision
DEFINES += MESA_EGL_NO_X11_HEADERS

SOURCES +=  $$PWD/qeglfsintegration.cpp \
            $$PWD/qeglfswindow.cpp \
            $$PWD/qeglfsbackingstore.cpp \
            $$PWD/qeglfsscreen.cpp \
            $$PWD/qeglfscontext.cpp

HEADERS +=  $$PWD/qeglfsintegration.h \
            $$PWD/qeglfswindow.h \
            $$PWD/qeglfsbackingstore.h \
            $$PWD/qeglfsscreen.h \
            $$PWD/qeglfscontext.h

QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF

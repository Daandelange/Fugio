#-------------------------------------------------
#
# Project created by QtCreator 2013-04-16T09:48:22
#
#-------------------------------------------------

include( ../../FugioGlobal.pri )

QT += gui widgets

TARGET = $$qtLibraryTarget(fugio-colour)
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

DESTDIR = $$DESTDIR/plugins

SOURCES += \
    colourbutton.cpp \
    colourpin.cpp \
    colourplugin.cpp \
    splitcolourhslanode.cpp \
    colourlistpin.cpp \
    splitcolourrgbanode.cpp \
    joincolourrgbanode.cpp \
    joincolourhslanode.cpp \
    colourbuttonnode.cpp

HEADERS += \
    ../../include/fugio/colour/uuid.h \
    ../../include/fugio/colour/colour_interface.h \
    ../../include/fugio/nodecontrolbase.h \
    ../../include/fugio/pincontrolbase.h \
    colourbutton.h \
    colourpin.h \
    colourplugin.h \
    splitcolourhslanode.h \
    colourlistpin.h \
    splitcolourrgbanode.h \
    joincolourrgbanode.h \
    joincolourhslanode.h \
    colourbuttonnode.h

#------------------------------------------------------------------------------
# OSX plugin bundle

macx {
    DEFINES += TARGET_OS_MAC
    CONFIG -= x86
    CONFIG += lib_bundle

    BUNDLEDIR    = $$DESTDIR/$$TARGET".bundle"
    INSTALLBASE  = $$FUGIO_ROOT/deploy-installer-$$QMAKE_HOST.arch
    INSTALLDIR   = $$INSTALLBASE/packages/com.bigfug.fugio
    INSTALLDEST  = $$INSTALLDIR/data/plugins
    INCLUDEDEST  = $$INSTALLDIR/data/include/fugio

    DESTDIR = $$BUNDLEDIR/Contents/MacOS
    DESTLIB = $$DESTDIR/"lib"$$TARGET".dylib"

    CONFIG(release,debug|release) {
        QMAKE_POST_LINK += echo

        LIBCHANGEDEST = $$DESTLIB

        QMAKE_POST_LINK += $$qtLibChange( QtWidgets )
        QMAKE_POST_LINK += $$qtLibChange( QtGui )
        QMAKE_POST_LINK += $$qtLibChange( QtCore )

        QMAKE_POST_LINK += && defaults write $$absolute_path( "Contents/Info", $$BUNDLEDIR ) CFBundleExecutable "lib"$$TARGET".dylib"

        QMAKE_POST_LINK += && macdeployqt $$BUNDLEDIR -always-overwrite -no-plugins

        QMAKE_POST_LINK += && mkdir -pv $$INSTALLDIR/meta
        QMAKE_POST_LINK += && mkdir -pv $$INSTALLDEST
        QMAKE_POST_LINK += && mkdir -pv $$INCLUDEDEST

        QMAKE_POST_LINK += && rm -rf $$INSTALLDEST/$$TARGET".bundle"

        QMAKE_POST_LINK += && cp -a $$BUNDLEDIR $$INSTALLDEST
    }
}

windows {
    INSTALLBASE  = $$FUGIO_ROOT/deploy-installer-$$QMAKE_HOST.arch
    INSTALLDIR   = $$INSTALLBASE/packages/com.bigfug.fugio

    CONFIG(release,debug|release) {
        QMAKE_POST_LINK += echo

        QMAKE_POST_LINK += & mkdir $$shell_path( $$INSTALLDIR/data/plugins )

        QMAKE_POST_LINK += & copy /V /Y $$shell_path( $$DESTDIR/$$TARGET".dll" ) $$shell_path( $$INSTALLDIR/data/plugins )
    }
}

#------------------------------------------------------------------------------
# API

INCLUDEPATH += $$PWD/../../include

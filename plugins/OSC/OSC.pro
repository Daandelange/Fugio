#-------------------------------------------------
#
# Project created by QtCreator 2014-07-28T10:04:45
#
#-------------------------------------------------

include( ../../FugioGlobal.pri )

QT       += widgets

TARGET = $$qtLibraryTarget(fugio-osc)
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

DESTDIR = $$DESTDIR/plugins

SOURCES += \
    opensoundcontrolplugin.cpp \
    decodernode.cpp \
    splitpin.cpp \
    encodernode.cpp \
    joinnode.cpp \
    joinpin.cpp \
    bundlernode.cpp \
    splitnode.cpp \
    namespacepin.cpp

HEADERS += \
    ../../include/fugio/nodecontrolbase.h \
    ../../include/fugio/pincontrolbase.h \
    ../../include/fugio/osc/join_interface.h \
    ../../include/fugio/osc/split_interface.h \
    ../../include/fugio/osc/namespace_interface.h \
    ../../include/fugio/osc/uuid.h \
    opensoundcontrolplugin.h \
    decodernode.h \
    splitpin.h \
    encodernode.h \
    joinnode.h \
    joinpin.h \
    bundlernode.h \
    splitnode.h \
    namespacepin.h

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

#------------------------------------------------------------------------------
# Windows Install

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


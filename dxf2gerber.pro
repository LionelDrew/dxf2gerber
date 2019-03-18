QT += core gui widgets

CONFIG += c++11

TARGET = dxf2gerber

TEMPLATE = app

SOURCES += main.cpp \
    thirdparty/dxflib/dl_dxf.cpp \
    thirdparty/dxflib/dl_writer_ascii.cpp \
    beziercurve2arcs/beziercurvetoarcs.cpp \
    beziercurve2arcs/cubicbeziertools.cpp \
    beziercurve2arcs/mathtools.cpp \
    dxfcreationadapter.cpp \
    painterpath2gerber.cpp \
    pdmalgorithmutil.cpp

HEADERS += \
    thirdparty/dxflib/dl_attributes.h \
    thirdparty/dxflib/dl_codes.h \
    thirdparty/dxflib/dl_creationadapter.h \
    thirdparty/dxflib/dl_creationinterface.h \
    thirdparty/dxflib/dl_dxf.h \
    thirdparty/dxflib/dl_entities.h \
    thirdparty/dxflib/dl_exception.h \
    thirdparty/dxflib/dl_extrusion.h \
    thirdparty/dxflib/dl_global.h \
    thirdparty/dxflib/dl_writer.h \
    thirdparty/dxflib/dl_writer_ascii.h \
    beziercurve2arcs/beziercurvetoarcs.h \
    beziercurve2arcs/cubicbeziertools.h \
    beziercurve2arcs/mathtools.h \
    dxfcreationadapter.h \
    painterpath2gerber.h \
    pdmalgorithmutil.h

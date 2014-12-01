TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -gdwarf-3

QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_DEBUG += -std=c++0x
QMAKE_CXXFLAGS_DEBUG += -Wparentheses
QMAKE_CXXFLAGS_DEBUG += -Wreturn-type
QMAKE_CXXFLAGS_DEBUG += -Wshadow
QMAKE_CXXFLAGS_DEBUG += -Wextra
QMAKE_CXXFLAGS_DEBUG += -Wunused-parameter
QMAKE_CXXFLAGS_DEBUG -= -Wwrite-strings
QMAKE_CXXFLAGS_DEBUG += -Wno-unused-variable
QMAKE_CXXFLAGS_DEBUG += -Weffc++

QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS_RELEASE += -std=c++0x
QMAKE_CXXFLAGS_RELEASE += -Wparentheses
QMAKE_CXXFLAGS_RELEASE += -Wreturn-type
QMAKE_CXXFLAGS_RELEASE += -Wshadow
QMAKE_CXXFLAGS_RELEASE += -Wextra
QMAKE_CXXFLAGS_RELEASE += -Wunused-parameter
QMAKE_CXXFLAGS_RELEASE -= -Wwrite-strings
QMAKE_CXXFLAGS_RELEASE += -Wno-unused-variable
QMAKE_CXXFLAGS_RELEASE += -Weffc++
QMAKE_CXXFLAGS_RELEASE += -msse

#DESTDIR += bin
CONFIG  += x86_64


OBJECTS_DIR = .tmp
MOC_DIR = .tmp
UI_DIR = .tmp
RCC_DIR = .tmp


message("qmake xmlparser1")


SOURCES += src/main.cpp \
    src/parser/src/xml_exception.cpp \
    src/parser/src/xml_builder.cpp \
    src/parser/src/txml_utl.cpp \
    src/parser/src/txml_interfaceimpl.cpp \
    src/parser/src/txml_fs.cpp \
    src/parser/src/raw_position.cpp \
    src/parser/src/keyentries.cpp \
    src/parser/src/xml_element.cpp \
    src/parser/src/xml_comment.cpp \
    src/parser/src/xml_declaration.cpp \
    src/parser/src/xml_document.cpp \
    src/parser/src/xml_locator.cpp \
    src/parser/src/xml_attribute.cpp \
    src/parser/src/xml_node.cpp \
    src/parser/src/xml_printer.cpp \
    src/parser/src/xml_text.cpp \
    src/filefn/t_filename.cpp \
    src/hstring/stringhelper.cpp \
    src/hstring/gettokens.cpp \
    src/log/tLogImpl.cpp \
    src/log/tLog.cpp \
    src/demo.cpp \
    src/ubuntu32_create.cpp \
    src/other_demo.cpp \
    src/log/alloccheck.cpp


HEADERS += \
    src/examples.h \
    src/parser/interface/xml_interface.h \
    src/parser/src/xml_utl.h \
    src/parser/src/xml_fs.h \
    src/parser/src/xml_builder.h \
    src/parser/src/win32.h \
    src/parser/src/rawxml_position.h \
    src/parser/src/keyentries.h \
    src/parser/src/xml_element.h \
    src/parser/src/xml_attribute.h \
    src/parser/src/xml_comment.h \
    src/parser/src/xml_declaration.h \
    src/parser/src/xml_document.h \
    src/parser/src/xml_locator.h \
    src/parser/src/xml_node.h \
    src/parser/src/xml_printer.h \
    src/parser/src/xml_text.h \
    src/parser/src/xml_visitor.h \
    src/parser/interface/xml_exception.h \
    src/filefn/t_filename.h \
    src/filefn/boost_cstdint.h \
    src/hstring/stringhelper.h \
    src/hstring/gettokens.h \
    src/log/tLogImpl.h \
    src/log/tLogEnum.h \
    src/log/tLogCategories.h \
    src/log/tLog.h \
    src/log/tLog_Category_default.h \
    src/log/tLog_Category_A.h \
    src/log/tLfmCL.h \
    src/log/tLfm.h \
    src/txtrw/wList.h \
    src/txtrw/rList.h \
    src/log/alloccheck.h



LIBS += -lpthread
LIBS += -lboost_system #-lboost_system-mt
LIBS += -lboost_filesystem #-lboost_filesystem-mt

cache()

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/filefn
INCLUDEPATH += $$PWD/src/hstring
INCLUDEPATH += $$PWD/src/log
INCLUDEPATH += $$PWD/src/parser
INCLUDEPATH += $$PWD/src/parser/interface
INCLUDEPATH += $$PWD/src/parser/src
INCLUDEPATH += $$PWD/src/txtrw


#LIBS += -L$$PWD/../libtxml/ -ltxml
#LIBS += /usr/lib/libboost_system-mt.a
#LIBS += /usr/lib/libboost_filesystem-mt.a

LIBS = -lboost_system -lboost_filesystem

#INCLUDEPATH += $$PWD/../libtxml
#DEPENDPATH += $$PWD/../libtxml

#PRE_TARGETDEPS += $$PWD/../libtxml/libtxml.a

OTHER_FILES += \
    appsettings.xml \
    README.md \
    lgpl-3.0.txt








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

QMAKE_CXXFLAGS_DEBUG += -Wwrite-strings
QMAKE_CXXFLAGS_DEBUG += -Wno-unused-variable
QMAKE_CXXFLAGS_DEBUG += -Wpedantic
QMAKE_CXXFLAGS_DEBUG += -Wno-unused-but-set-variable
QMAKE_CXXFLAGS_DEBUG += -Werror
QMAKE_CXXFLAGS_DEBUG += -Wall
QMAKE_CXXFLAGS_DEBUG += -Wcast-align
QMAKE_CXXFLAGS_DEBUG += -Wcast-qual
QMAKE_CXXFLAGS_DEBUG += -Woverloaded-virtual
QMAKE_CXXFLAGS_DEBUG += -Wzero-as-null-pointer-constant
QMAKE_CXXFLAGS_DEBUG += -Wwrite-strings
QMAKE_CXXFLAGS_DEBUG += -Wconversion
QMAKE_CXXFLAGS_DEBUG += -Wold-style-cast
QMAKE_CXXFLAGS_DEBUG += -Wdisabled-optimization
QMAKE_CXXFLAGS_DEBUG += -Weffc++



#gcc -Werror -Wall -Wextra -Wpedantic -Wcast-align
#-Wcast-qual -Wconversion -Wctor-dtor-privacy
#-Wdisabled-optimization -Wdouble-promotion
#-Wfloat-equal -Wformat=2 -Winit-self -Winvalid-
#pch -Wlogical-op -Wmissing-declarations
#-Wmissing-include-dirs -Wnoexcept -Wold-style-
#cast -Woverloaded-virtual -Wredundant-decls
#-Wshadow -Wsign-conversion -Wsign-promo
#-Wstrict-null-sentinel -Wstrict-overflow=5
#-Wtrampolines -Wundef -Wunsafe-loop-
#optimizations -Wvector-operation-performance
#-Wzero-as-null-pointer-constant


QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE += -std=c++0x
QMAKE_CXXFLAGS_RELEASE += -Wparentheses
QMAKE_CXXFLAGS_RELEASE += -Wreturn-type
QMAKE_CXXFLAGS_RELEASE += -Wshadow
QMAKE_CXXFLAGS_RELEASE += -Wextra
QMAKE_CXXFLAGS_RELEASE += -Wunused-parameter
QMAKE_CXXFLAGS_RELEASE += -Wwrite-strings
QMAKE_CXXFLAGS_RELEASE += -Wno-unused-variable

QMAKE_CXXFLAGS_RELEASE += -Wpedantic
QMAKE_CXXFLAGS_RELEASE += -Wno-unused-but-set-variable
QMAKE_CXXFLAGS_RELEASE += -Werror
QMAKE_CXXFLAGS_RELEASE += -Wall
QMAKE_CXXFLAGS_RELEASE += -Wcast-align
QMAKE_CXXFLAGS_RELEASE += -Wcast-qual
QMAKE_CXXFLAGS_RELEASE += -Woverloaded-virtual
QMAKE_CXXFLAGS_RELEASE += -Wzero-as-null-pointer-constant
QMAKE_CXXFLAGS_RELEASE += -Wwrite-strings
QMAKE_CXXFLAGS_RELEASE += -Wconversion
QMAKE_CXXFLAGS_RELEASE += -Wold-style-cast
QMAKE_CXXFLAGS_RELEASE += -Wdisabled-optimization

#QMAKE_CXXFLAGS_RELEASE += -Weffc++
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
    src/log/alloccheck.cpp \
    src/parser/src/element_locator.cpp \
    src/parser/src/key_path.cpp \
    src/parser/src/key_path_element.cpp


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
    src/log/tLfmCL.h \
    src/log/tLfm.h \
    src/txtrw/wList.h \
    src/txtrw/rList.h \
    src/log/alloccheck.h \
    src/parser/src/tPointers.h \
    src/parser/src/enum_macro.h \
    src/parser/src/element_locator.h \
    src/parser/src/visitor_ret.h



LIBS += -lpthread
LIBS += -lboost_system
LIBS += -lboost_filesystem

cache()

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/filefn
INCLUDEPATH += $$PWD/src/hstring
INCLUDEPATH += $$PWD/src/log
INCLUDEPATH += $$PWD/src/parser
INCLUDEPATH += $$PWD/src/parser/interface
INCLUDEPATH += $$PWD/src/parser/src
INCLUDEPATH += $$PWD/src/txtrw

LIBS = -lboost_system -lboost_filesystem


OTHER_FILES += \
    appsettings.xml \
    README.md \
    lgpl-3.0.txt








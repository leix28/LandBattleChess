QT              +=  core gui widgets

TARGET          =   Chess
TEMPLATE        =   app

SOURCES         +=  src/main.cpp \
    src/mainwindow.cpp \
    src/chessboard.cpp \
    src/chessmodel.cpp \
    src/ezsocket.cpp \
    src/guessdialog.cpp

HEADERS         += \    
    src/mainwindow.h \
    src/chessboard.h \
    src/chessmodel.h \
    src/ezsocket.h \
    src/guessdialog.h


LIBS            +=  -stdlib=libc++ -L/usr/lib -L/usr/local/lib
QMAKE_CXXFLAGS  +=  -std=c++11 #-O2
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

OBJECTS_DIR     =   build
MOC_DIR         =   build
RCC_DIR         =   build

INCLUDEPATH     +=  /usr/local/include /usr/include

RESOURCES       += \  
    resource/resource.qrc

ICON            =   

FORMS +=


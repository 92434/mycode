QT += widgets serialport network

TARGET = kingly_printer
TEMPLATE = app

message($$escape_expand("QT:$${QT}"))

FORMS += \
    mainwindow.ui \
    settingdialogs/textsettingdialog.ui \
    settingdialogs/qrsetttingdialog.ui

HEADERS += \
    mainwindow.h \
    ../../app/application.h \
    scene/graphicsscene.h \
    scene/graphicsarrowitem.h \
    scene/graphicspolygonitem.h \
    scene/graphicstextitem.h \
    scene/graphicsqritem.h \
    qr/qzint.h \
    settingdialogs/textsettingdialog.h \
    settingdialogs/qrsetttingdialog.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../../app/application.cpp \
    scene/graphicsarrowitem.cpp \
    scene/graphicsscene.cpp \
    scene/graphicspolygonitem.cpp \
    scene/graphicstextitem.cpp \
    scene/graphicsqritem.cpp \
    qr/qzint.cpp \
    settingdialogs/textsettingdialog.cpp \
    settingdialogs/qrsetttingdialog.cpp

unix:!macx: LIBS += -L$$PWD/qr/libs -lzint
unix:!macx: QMAKE_CXXFLAGS += -I$$PWD/qr
unix:!macx: QMAKE_CXXFLAGS += -I$$PWD/../../utils
unix:!macx: QMAKE_CXXFLAGS += -I$$PWD/../../app
unix:!macx: QMAKE_CXXFLAGS += -I$$PWD/scene
unix:!macx: QMAKE_CXXFLAGS += -I$$PWD/settingdialogs


RESOURCES += \
    resource.qrc

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
    qr/zint-master/backend_qt4/qzint.h \
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
    qr/zint-master/backend_qt4/qzint.cpp \
    settingdialogs/textsettingdialog.cpp \
    settingdialogs/qrsetttingdialog.cpp

unix:!macx: LIBS += -L$$PWD/qr/zint-master/build/backend -lzint
unix:!macx: QMAKE_CXXFLAGS += -I$$PWD/qr/zint-master/backend
unix:!macx: QMAKE_CXXFLAGS += -I$$PWD/qr/zint-master/backend_qt4
unix:!macx: QMAKE_CXXFLAGS += -I$$PWD/../../utils


RESOURCES += \
    resource.qrc

QT += widgets serialport network

TARGET = kingly_printer
TEMPLATE = app

message($$escape_expand("QT:$${QT}"))

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    ../../app/application.h \
    scene/graphicsscene.h \
    scene/graphicsarrowitem.h \
    scene/graphicspolygonitem.h \
    scene/graphicstextitem.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../../app/application.cpp \
    scene/graphicsarrowitem.cpp \
    scene/graphicsscene.cpp \
    scene/graphicspolygonitem.cpp \
    scene/graphicstextitem.cpp

RESOURCES += \
    resource.qrc

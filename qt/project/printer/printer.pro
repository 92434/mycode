QT += widgets serialport network

TARGET = kingly_printer
TEMPLATE = app

message($$escape_expand("QT:$${QT}"))

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    scene/arrow.h \
    scene/diagramitem.h \
    scene/diagramscene.h \
    scene/diagramtextitem.h \
    ../../app/single_application.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scene/diagramitem.cpp \
    scene/arrow.cpp \
    scene/diagramscene.cpp \
    scene/diagramtextitem.cpp \
    ../../app/single_application.cpp

RESOURCES += \
    resource.qrc

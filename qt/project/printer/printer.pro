QT += widgets serialport network

TARGET = kingly_printer
TEMPLATE = app

message($$escape_expand("QT:$${QT}"))

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    single_application.h \
    scene/arrow.h \
    scene/diagramitem.h \
    scene/diagramscene.h \
    scene/diagramtextitem.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    single_application.cpp \
    scene/diagramitem.cpp \
    scene/arrow.cpp \
    scene/diagramscene.cpp \
    scene/diagramtextitem.cpp

RESOURCES += \
    resource.qrc

QT += widgets serialport

TARGET = form
TEMPLATE = app

message($$escape_expand("QT:$${QT}"))

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h

SOURCES += \
    main.cpp \
    mainwindow.cpp

RESOURCES += \
    resource.qrc

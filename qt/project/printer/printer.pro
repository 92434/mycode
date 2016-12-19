QT += widgets serialport network

TARGET = form
TEMPLATE = app

message($$escape_expand("QT:$${QT}"))

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    single_application.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    single_application.cpp

RESOURCES += \
    resource.qrc

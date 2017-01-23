QT += widgets network printsupport

TARGET = test
TEMPLATE = app

message($$escape_expand("QT:$${QT}"))

SOURCES += \
    main.cpp \
    ../app/application.cpp \
    dialog.cpp

HEADERS += \
    ../app/application.h \
    dialog.h

DISTFILES +=

FORMS += \
    dialog.ui

RESOURCES += \
    resource.qrc


TEMPLATE = subdirs

SUBDIRS += project/printer

message($$escape_expand("QT:$${QT}"))

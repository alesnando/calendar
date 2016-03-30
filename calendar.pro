QT += qml quick sql
TARGET = calendar

!contains(sql-drivers, sqllite): QTPLUGIN += qsqlite

include(src/src.pri)
include(../shared/shared.pri)

OTHER_FILES += qml/main.qml

RESOURCES += resources.qrc

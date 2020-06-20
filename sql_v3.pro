#-------------------------------------------------
#
# Project created by QtCreator 2019-07-21T19:53:47
#
#-------------------------------------------------


greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

QT += core gui sql widgets

TARGET = sql_v3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17


SOURCES += \
    ActionTable.cpp \
    Paper_Model.cpp \
    Sql.cpp \
    SqlTableView.cpp \
    actionarealist.cpp \
    areaview.cpp \
    columncombobox.cpp \
#    logic.cpp \
    main.cpp \
    sqlForeignKeyDelegate.cpp \
    sqllistdelegate.cpp \
    sqltabledelegate.cpp \
    sqltablehheader.cpp

HEADERS += \
    ActionTable.h \
    Paper_Model.h \
    Sql.h \
    SqlTableView.h \
    actionarealist.h \
    areaview.h \
    columncombobox.h \
    constants.h \
    sqlForeignKeyDelegate.h \
    sqllistdelegate.h \
    sqltabledelegate.h \
    sqltablehheader.h \
#    ui_library.h \
    sqlsetup.h

PRECOMPILED_HEADER = sqlsetup.h

FORMS += \
    area_editor.ui \
    library.ui \
#    newreference_popup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

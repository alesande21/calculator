QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../models/s21_ModelCredit.cc \
    ../models/s21_ModelDeposit.cc \
    ../models/s21_ModelGraph.cc \
    ../models/s21_ModelPolish.cc \
    credit.cpp \
    deposit.cpp \
    graph.cpp \
    ../../main.cpp \
    mainwindow.cpp \
    ../qcustomplot/qcustomplot.cpp \

HEADERS += \
    ../controllers/s21_ControllerCredit.h \
    ../controllers/s21_ControllerDeposit.h \
    ../controllers/s21_ControllerGraph.h \
    ../controllers/s21_ControllerPolish.h \
    ../models/s21_ModelCredit.h \
    ../models/s21_ModelDeposit.h \
    ../models/s21_ModelGraph.h \
    ../models/s21_ModelPolish.h \
    credit.h \
    deposit.h \
    graph.h \
    mainwindow.h \
    ../qcustomplot/qcustomplot.h \

FORMS += \
    credit.ui \
    deposit.ui \
    graph.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

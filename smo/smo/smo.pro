QT += core gui widgets charts

CONFIG += c++14
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp mainwindow.cpp BMS.cpp BMS1.cpp Buffer.cpp nextwindow.cpp properies.cpp\
            automode.cpp BMS2.cpp BMS3.cpp Device.cpp settingswindow.cpp Source.cpp properies.cpp Request.cpp Source.cpp

HEADERS += Source.h Request.h properties.h Source.h settingswindow.h\
            mainwindow.h BMS.h BMS1.h Buffer.h nextwindow.h properties.h automode.h BMS2.h BMS3.h Device.h

FORMS += settingswindow.ui mainwindow.ui nextwindow.ui automode.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

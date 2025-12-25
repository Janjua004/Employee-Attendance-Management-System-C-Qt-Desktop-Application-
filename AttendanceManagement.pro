QT += core widgets charts

CONFIG += c++17
CONFIG += qt
CONFIG -= app_bundle

TARGET = AttendanceManagement
TEMPLATE = app

# Windows console for debug output
win32:CONFIG(debug, debug|release) {
    CONFIG += console
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    loginDialog.cpp \
    adminInterface.cpp \
    employeeInterface.cpp \
    employee.cpp \
    leaveRequest.cpp \
    attendanceRecord.cpp \
    DataStructures.cpp

HEADERS += \
    mainwindow.h \
    loginDialog.h \
    adminInterface.h \
    employeeInterface.h \
    employee.h \
    leaveRequest.h \
    attendanceRecord.h \
    DataStructures.h

FORMS += \
    mainwindow.ui \
    loginDialog.ui \
    adminInterface.ui \
    employeeInterface.ui

#include <QApplication>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Employee Attendance Management System");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("EAS Solutions");
    
    // Set working directory to application directory for data files
    QString appDir = QCoreApplication::applicationDirPath();
    QDir::setCurrent(appDir);
    
    try {
        MainWindow window;
        window.show();
        
        return app.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Application Error", 
                             QString("An error occurred: %1").arg(e.what()));
        return 1;
    } catch (...) {
        QMessageBox::critical(nullptr, "Application Error", 
                             "An unknown error occurred.");
        return 1;
    }
}

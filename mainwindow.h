#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
class QStackedWidget;
QT_END_NAMESPACE

class LoginDialog;
class AdminInterface;
class EmployeeInterface;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAdminLogin();
    void showEmployeeLogin();
    void onAdminLoginSuccess();
    void onEmployeeLoginSuccess(int employeeId);
    void logout();

private:
    void setupUI();
    void setupStyles();

    QStackedWidget *m_stackedWidget;
    QWidget *m_welcomeWidget;
    QPushButton *m_adminButton;
    QPushButton *m_employeeButton;
    QPushButton *m_logoutButton;
    
    LoginDialog *m_loginDialog;
    AdminInterface *m_adminInterface;
    EmployeeInterface *m_employeeInterface;
    
    int m_currentEmployeeId;
};

#endif // MAINWINDOW_H

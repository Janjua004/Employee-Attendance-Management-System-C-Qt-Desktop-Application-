#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    enum LoginMode {
        AdminMode,
        EmployeeMode
    };

    explicit LoginDialog(QWidget *parent = nullptr);

    void setLoginMode(LoginMode mode);

signals:
    void adminLoginSuccess();
    void employeeLoginSuccess(int employeeId);

private slots:
    void handleLogin();
    void resetForm();

private:
    void setupUI();
    void setupStyles();
    bool validateAdminLogin(const QString& username, const QString& password);
    bool validateEmployeeLogin(int employeeId, const QString& password);

    LoginMode m_mode;
    QLabel *m_titleLabel;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    QPushButton *m_cancelButton;
    QLabel *m_modeIndicator;
};

#endif // LOGINDIALOG_H

#include "loginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>
#include <QFont>
#include <QFrame>
#include <QSpacerItem>
#include <QFile>
#include <QTextStream>
#include "employee.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), m_mode(AdminMode)
{
    setupUI();
    setupStyles();
    
    // Set dialog properties
    setWindowTitle("EAS - Employee Attendance System");
    setModal(true);
    setFixedSize(480, 420);
    
    // Connect signals
    connect(m_loginButton, &QPushButton::clicked, this, &LoginDialog::handleLogin);
    connect(m_cancelButton, &QPushButton::clicked, this, &LoginDialog::reject);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::handleLogin);
    connect(m_usernameEdit, &QLineEdit::returnPressed, [this]() { m_passwordEdit->setFocus(); });
}

void LoginDialog::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Header section
    QFrame *headerFrame = new QFrame();
    headerFrame->setObjectName("headerFrame");
    headerFrame->setFixedHeight(80);
    
    QVBoxLayout *headerLayout = new QVBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 15, 20, 15);
    
    // Title label with proper styling
    m_titleLabel = new QLabel("Employee Attendance System", headerFrame);
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    
    // Subtitle label
    QLabel *subtitleLabel = new QLabel("Secure Login Portal", headerFrame);
    subtitleLabel->setObjectName("subtitleLabel");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    
    // Add to header layout
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addWidget(subtitleLabel);
    
    // Content section
    QFrame *contentFrame = new QFrame();
    contentFrame->setObjectName("contentFrame");
    
    QVBoxLayout *contentLayout = new QVBoxLayout(contentFrame);
    contentLayout->setContentsMargins(40, 30, 40, 30);
    contentLayout->setSpacing(20);
    
    // Mode indicator
    m_modeIndicator = new QLabel("Administrator Access");
    m_modeIndicator->setObjectName("modeIndicator");
    m_modeIndicator->setAlignment(Qt::AlignCenter);
    
    // Login form
    QFrame *formFrame = new QFrame();
    formFrame->setObjectName("formFrame");
    
    QGridLayout *formLayout = new QGridLayout(formFrame);
    formLayout->setContentsMargins(20, 20, 20, 20);
    formLayout->setVerticalSpacing(15);
    formLayout->setHorizontalSpacing(10);
    
    // Username field
    QLabel *usernameLabel = new QLabel("Username:");
    usernameLabel->setObjectName("fieldLabel");
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setObjectName("inputField");
    m_usernameEdit->setPlaceholderText("Enter your username");
    m_usernameEdit->setFixedHeight(35);
    
    // Password field
    QLabel *passwordLabel = new QLabel("Password:");
    passwordLabel->setObjectName("fieldLabel");
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setObjectName("inputField");
    m_passwordEdit->setPlaceholderText("Enter your password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setFixedHeight(35);
    
    // Add form fields
    formLayout->addWidget(usernameLabel, 0, 0);
    formLayout->addWidget(m_usernameEdit, 0, 1);
    formLayout->addWidget(passwordLabel, 1, 0);
    formLayout->addWidget(m_passwordEdit, 1, 1);
    
    // Buttons section
    QFrame *buttonFrame = new QFrame();
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(15);
    
    // Add spacer
    buttonLayout->addStretch();
    
    // Cancel button
    m_cancelButton = new QPushButton("Cancel");
    m_cancelButton->setObjectName("cancelButton");
    m_cancelButton->setFixedSize(100, 35);
    
    // Login button
    m_loginButton = new QPushButton("Login");
    m_loginButton->setObjectName("loginButton");
    m_loginButton->setFixedSize(100, 35);
    m_loginButton->setDefault(true);
    
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_loginButton);
    
    // Assemble content
    contentLayout->addWidget(m_modeIndicator);
    contentLayout->addWidget(formFrame);
    contentLayout->addStretch();
    contentLayout->addWidget(buttonFrame);
    
    // Assemble main layout
    mainLayout->addWidget(headerFrame);
    mainLayout->addWidget(contentFrame);
}

void LoginDialog::setupStyles()
{
    QString styleSheet = R"(
        /* Main dialog */
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #f8f9fa, stop:1 #e9ecef);
            border-radius: 8px;
        }        /* Header - LIGHTER BACKGROUND FOR BETTER TEXT VISIBILITY */
        #headerFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #34495e, stop:1 #2c3e50);
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            border-bottom: 2px solid #1a252f;
        }
        
        /* Title Label - MAXIMUM CONTRAST */
        #titleLabel {
            color: #FFFFFF;
            font-size: 28px;
            font-weight: bold;
            font-family: 'Arial', sans-serif;
            background: transparent;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.8);
        }
        
        #subtitleLabel {
            color: #DDDDDD;
            font-size: 14px;
            font-family: 'Arial', sans-serif;
            text-shadow: 1px 1px 2px rgba(0,0,0,0.5);
        }
        
        /* Content area */
        #contentFrame {
            background: white;
            border-bottom-left-radius: 8px;
            border-bottom-right-radius: 8px;
        }
        
        /* Mode indicator */
        #modeIndicator {
            color: #2c3e50;
            font-size: 14px;
            font-weight: bold;
            background: #ecf0f1;
            padding: 8px 16px;
            border-radius: 4px;
            border: 1px solid #bdc3c7;
        }
        
        /* Form frame */
        #formFrame {
            background: #f8f9fa;
            border: 1px solid #dee2e6;
            border-radius: 6px;
        }
        
        /* Field labels */
        #fieldLabel {
            color: #495057;
            font-size: 13px;
            font-weight: 600;
            font-family: 'Segoe UI', Arial, sans-serif;
            min-width: 80px;
        }
        
        /* Input fields */
        #inputField {
            background: white;
            border: 2px solid #dee2e6;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 13px;
            font-family: 'Segoe UI', Arial, sans-serif;
            color: #495057;
        }
        
        #inputField:focus {
            border-color: #007bff;
            outline: none;
            background: #f8f9ff;
        }
        
        #inputField:hover {
            border-color: #adb5bd;
        }
        
        /* Login button */
        #loginButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #007bff, stop:1 #0056b3);
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 13px;
            font-weight: 600;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        
        #loginButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #0056b3, stop:1 #004085);
        }
        
        #loginButton:pressed {
            background: #004085;
        }
        
        /* Cancel button */
        #cancelButton {
            background: #f8f9fa;
            color: #6c757d;
            border: 1px solid #dee2e6;
            border-radius: 4px;
            font-size: 13px;
            font-weight: 600;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        
        #cancelButton:hover {
            background: #e9ecef;
            border-color: #adb5bd;
        }
        
        #cancelButton:pressed {
            background: #dee2e6;
        }
    )";
    
    setStyleSheet(styleSheet);
}

void LoginDialog::setLoginMode(LoginMode mode)
{
    m_mode = mode;
    
    if (mode == AdminMode) {
        m_modeIndicator->setText("Administrator Access");
        m_usernameEdit->setPlaceholderText("Enter admin username");
        m_passwordEdit->setPlaceholderText("Enter admin password");
        setWindowTitle("EAS - Administrator Login");
    } else {
        m_modeIndicator->setText("Employee Access");
        m_usernameEdit->setPlaceholderText("Enter employee ID");
        m_passwordEdit->setPlaceholderText("Enter employee password");
        setWindowTitle("EAS - Employee Login");
    }
    
    // Clear fields when mode changes
    resetForm();
}

void LoginDialog::handleLogin()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", 
                           "Please enter both username and password.");
        return;
    }
    
    if (m_mode == AdminMode) {
        if (validateAdminLogin(username, password)) {
            emit adminLoginSuccess();
            accept();
        } else {
            QMessageBox::critical(this, "Login Failed", 
                                "Invalid administrator credentials.");
            resetForm();
        }
    } else {
        bool ok;
        int employeeId = username.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Login Error", 
                               "Employee ID must be a number.");
            return;
        }
        
        if (validateEmployeeLogin(employeeId, password)) {
            emit employeeLoginSuccess(employeeId);
            accept();
        } else {
            QMessageBox::critical(this, "Login Failed", 
                                "Invalid employee credentials.");
            resetForm();
        }
    }
}

void LoginDialog::resetForm()
{
    m_usernameEdit->clear();
    m_passwordEdit->clear();
    m_usernameEdit->setFocus();
}

bool LoginDialog::validateAdminLogin(const QString& username, const QString& password)
{
    // Simple validation - in real application, this would check against database
    return (username == "admin" && password == "admin123");
}

bool LoginDialog::validateEmployeeLogin(int employeeId, const QString& password)
{
    // Check against employees file
    QFile file("employees.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Fallback validation for demo
        return (employeeId >= 1000 && employeeId <= 9999 && password == "emp123");
    }
    
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        Employee emp = Employee::fromFileString(line);
        if (emp.getId() == employeeId && emp.getPassword() == password) {
            return true;
        }
    }
    
    return false;
}

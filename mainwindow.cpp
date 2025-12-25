#include "mainwindow.h"
#include "loginDialog.h"
#include "adminInterface.h"
#include "employeeInterface.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(nullptr)
    , m_welcomeWidget(nullptr)
    , m_adminButton(nullptr)
    , m_employeeButton(nullptr)
    , m_logoutButton(nullptr)
    , m_loginDialog(nullptr)
    , m_adminInterface(nullptr)
    , m_employeeInterface(nullptr)
    , m_currentEmployeeId(0)
{
    setupUI();
    setupStyles();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("Employee Attendance Management System");
    setMinimumSize(800, 600);
    resize(1000, 700);

    // Create central widget with stacked layout
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // Create welcome screen
    m_welcomeWidget = new QWidget;
    QVBoxLayout *welcomeLayout = new QVBoxLayout(m_welcomeWidget);
    
    QLabel *titleLabel = new QLabel("Employee Attendance Management System");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    QLabel *subtitleLabel = new QLabel("Please select your role to continue");
    subtitleLabel->setObjectName("subtitleLabel");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    
    // Role selection buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    m_adminButton = new QPushButton("Admin Login");
    m_employeeButton = new QPushButton("Employee Login");
    
    m_adminButton->setObjectName("roleButton");
    m_employeeButton->setObjectName("roleButton");
    m_adminButton->setMinimumSize(150, 50);
    m_employeeButton->setMinimumSize(150, 50);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_adminButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(m_employeeButton);
    buttonLayout->addStretch();
    
    welcomeLayout->addStretch();
    welcomeLayout->addWidget(titleLabel);
    welcomeLayout->addSpacing(10);
    welcomeLayout->addWidget(subtitleLabel);
    welcomeLayout->addSpacing(30);
    welcomeLayout->addLayout(buttonLayout);
    welcomeLayout->addStretch();
    
    m_stackedWidget->addWidget(m_welcomeWidget);
    
    // Create login dialog
    m_loginDialog = new LoginDialog(this);
    
    // Create interfaces
    m_adminInterface = new AdminInterface(this);
    m_employeeInterface = new EmployeeInterface(this);
    
    m_stackedWidget->addWidget(m_adminInterface);
    m_stackedWidget->addWidget(m_employeeInterface);
    
    // Create menu bar
    QMenuBar *menuBar = this->menuBar();
    QMenu *fileMenu = menuBar->addMenu("&File");
    
    QAction *logoutAction = fileMenu->addAction("&Logout");
    logoutAction->setShortcut(QKeySequence::Close);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::logout);
    
    QAction *exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Status bar
    statusBar()->showMessage("Ready");
      // Connect signals
    connect(m_adminButton, &QPushButton::clicked, this, &MainWindow::showAdminLogin);
    connect(m_employeeButton, &QPushButton::clicked, this, &MainWindow::showEmployeeLogin);
    connect(m_loginDialog, &LoginDialog::adminLoginSuccess, this, &MainWindow::onAdminLoginSuccess);
    connect(m_loginDialog, &LoginDialog::employeeLoginSuccess, this, &MainWindow::onEmployeeLoginSuccess);
    
    // Connect logout signals from interfaces
    connect(m_adminInterface, &AdminInterface::logoutRequested, this, &MainWindow::logout);
    connect(m_employeeInterface, &EmployeeInterface::logoutRequested, this, &MainWindow::logout);
}

void MainWindow::setupStyles()
{
    QString style = R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        
        #titleLabel {
            font-size: 28px;
            font-weight: bold;
            color: #2c3e50;
            margin: 20px;
        }
        
        #subtitleLabel {
            font-size: 16px;
            color: #7f8c8d;
            margin: 10px;
        }
        
        #roleButton {
            background-color: #3498db;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 14px;
            font-weight: bold;
            padding: 15px 30px;
        }
        
        #roleButton:hover {
            background-color: #2980b9;
        }
        
        #roleButton:pressed {
            background-color: #21618c;
        }
        
        QMenuBar {
            background-color: #34495e;
            color: white;
            border-bottom: 1px solid #2c3e50;
        }
        
        QMenuBar::item {
            background: transparent;
            padding: 8px 12px;
        }
        
        QMenuBar::item:selected {
            background-color: #2c3e50;
        }
        
        QStatusBar {
            background-color: #ecf0f1;
            border-top: 1px solid #bdc3c7;
        }
    )";
    
    setStyleSheet(style);
}

void MainWindow::showAdminLogin()
{
    m_loginDialog->setLoginMode(LoginDialog::AdminMode);
    m_loginDialog->exec();
}

void MainWindow::showEmployeeLogin()
{
    m_loginDialog->setLoginMode(LoginDialog::EmployeeMode);
    m_loginDialog->exec();
}

void MainWindow::onAdminLoginSuccess()
{
    m_stackedWidget->setCurrentWidget(m_adminInterface);
    statusBar()->showMessage("Logged in as Administrator");
}

void MainWindow::onEmployeeLoginSuccess(int employeeId)
{
    m_currentEmployeeId = employeeId;
    m_employeeInterface->setCurrentEmployee(employeeId);
    m_stackedWidget->setCurrentWidget(m_employeeInterface);
    statusBar()->showMessage(QString("Logged in as Employee ID: %1").arg(employeeId));
}

void MainWindow::logout()
{
    m_stackedWidget->setCurrentWidget(m_welcomeWidget);
    m_currentEmployeeId = 0;
    statusBar()->showMessage("Logged out");
}

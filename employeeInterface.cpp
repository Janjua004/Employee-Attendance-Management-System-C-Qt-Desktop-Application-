#include "employeeInterface.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QHeaderView>
#include <QSplitter>

EmployeeInterface::EmployeeInterface(QWidget *parent)
    : QWidget(parent)
    , m_currentEmployeeId(0)
    , m_dataManager(DataManager::getInstance())  // Initialize DataManager
{
    setupUI();
    setupStyles();
}

void EmployeeInterface::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // Title and logout header
    QHBoxLayout *headerLayout = new QHBoxLayout;
    
    QLabel *titleLabel = new QLabel("Employee Dashboard");
    titleLabel->setObjectName("employeeTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    QPushButton *logoutButton = new QPushButton("Sign Out");
    logoutButton->setObjectName("logoutButton");
    logoutButton->setMaximumWidth(100);
    logoutButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 8px 15px; border: none; border-radius: 5px; font-weight: bold; } QPushButton:hover { background-color: #c0392b; }");
    
    // Add datetime label with styled background
    QLabel* dateTimeLabel = new QLabel();
    dateTimeLabel->setObjectName("dateTimeLabel");
    dateTimeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    dateTimeLabel->setStyleSheet("QLabel { color: #fff; font-size: 14px; padding: 8px 15px; background-color: #3498db; border-radius: 5px; font-weight: bold; }");
    
    // Update date time initially
    dateTimeLabel->setText(QDateTime::currentDateTime().toString("dddd, MMMM d, yyyy hh:mm:ss AP"));
    
    // Setup timer for datetime updates
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        dateTimeLabel->setText(QDateTime::currentDateTime().toString("dddd, MMMM d, yyyy hh:mm:ss AP"));
    });
    timer->start(1000); // Update every second
    
    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(dateTimeLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(logoutButton);
    
    // Main content in a splitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    
    // Left panel
    QWidget *leftPanel = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    setupDashboard();
    setupAttendanceSection();
    
    leftLayout->addWidget(m_dashboardGroup);
    leftLayout->addWidget(m_attendanceGroup);
    leftLayout->addStretch();
    
    // Right panel
    QWidget *rightPanel = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    
    setupLeaveSection();
    setupFeedbackSection();
    
    rightLayout->addWidget(m_leaveGroup);
    rightLayout->addWidget(m_feedbackGroup);
    
    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setSizes({1, 1});
    
    // Refresh button
    m_refreshButton = new QPushButton("Refresh Data");
    
    QHBoxLayout *bottomLayout = new QHBoxLayout;    bottomLayout->addStretch();
    bottomLayout->addWidget(m_refreshButton);
    
    m_mainLayout->addLayout(headerLayout);
    m_mainLayout->addWidget(splitter);
    m_mainLayout->addLayout(bottomLayout);
    
    // Connect signals
    connect(m_refreshButton, &QPushButton::clicked, this, &EmployeeInterface::refreshData);
    connect(logoutButton, &QPushButton::clicked, this, &EmployeeInterface::logout);
}

void EmployeeInterface::setupDashboard()
{
    m_dashboardGroup = new QGroupBox("Dashboard");
    QVBoxLayout *layout = new QVBoxLayout(m_dashboardGroup);
    
    m_welcomeLabel = new QLabel("Welcome!");
    m_welcomeLabel->setObjectName("welcomeLabel");
    
    m_employeeInfoLabel = new QLabel();
    m_employeeInfoLabel->setObjectName("infoLabel");
    
    m_salaryLabel = new QLabel();
    m_salaryLabel->setObjectName("salaryLabel");
    
    m_attendanceStatusLabel = new QLabel();
    m_attendanceStatusLabel->setObjectName("statusLabel");
    
    layout->addWidget(m_welcomeLabel);
    layout->addWidget(m_employeeInfoLabel);
    layout->addWidget(m_salaryLabel);
    layout->addWidget(m_attendanceStatusLabel);
    layout->addStretch();
}

void EmployeeInterface::setupAttendanceSection()
{
    m_attendanceGroup = new QGroupBox("Attendance");
    QVBoxLayout *layout = new QVBoxLayout(m_attendanceGroup);
    
    m_markAttendanceButton = new QPushButton("Mark Attendance");
    m_markAttendanceButton->setObjectName("attendanceButton");
    m_markAttendanceButton->setMinimumHeight(40);
    
    m_lastAttendanceLabel = new QLabel("Last attendance: Not recorded");
    m_lastAttendanceLabel->setObjectName("lastAttendanceLabel");
    
    layout->addWidget(m_markAttendanceButton);
    layout->addWidget(m_lastAttendanceLabel);
    
    connect(m_markAttendanceButton, &QPushButton::clicked, this, &EmployeeInterface::markAttendance);
}

void EmployeeInterface::setupLeaveSection()
{
    m_leaveGroup = new QGroupBox("Leave Request");
    QVBoxLayout *layout = new QVBoxLayout(m_leaveGroup);
    
    // Leave request form
    QFormLayout *formLayout = new QFormLayout;
    
    m_startDateEdit = new QDateEdit;
    m_startDateEdit->setDate(QDate::currentDate());
    m_startDateEdit->setCalendarPopup(true);
    
    m_endDateEdit = new QDateEdit;
    m_endDateEdit->setDate(QDate::currentDate().addDays(1));
    m_endDateEdit->setCalendarPopup(true);
    
    m_reasonEdit = new QLineEdit;
    m_reasonEdit->setPlaceholderText("Enter reason for leave");
    
    formLayout->addRow("Start Date:", m_startDateEdit);
    formLayout->addRow("End Date:", m_endDateEdit);
    formLayout->addRow("Reason:", m_reasonEdit);
    
    m_submitLeaveButton = new QPushButton("Submit Leave Request");
    
    // Leave status table
    QLabel *statusLabel = new QLabel("My Leave Requests:");
    statusLabel->setObjectName("sectionSubLabel");
    
    m_leaveStatusTable = new QTableWidget;
    m_leaveStatusTable->setColumnCount(4);
    QStringList headers = {"Start Date", "End Date", "Reason", "Status"};
    m_leaveStatusTable->setHorizontalHeaderLabels(headers);
    m_leaveStatusTable->horizontalHeader()->setStretchLastSection(true);
    m_leaveStatusTable->setMaximumHeight(150);
    m_leaveStatusTable->setAlternatingRowColors(true);
    
    layout->addLayout(formLayout);
    layout->addWidget(m_submitLeaveButton);
    layout->addSpacing(10);
    layout->addWidget(statusLabel);
    layout->addWidget(m_leaveStatusTable);
    
    connect(m_submitLeaveButton, &QPushButton::clicked, this, &EmployeeInterface::submitLeaveRequest);
}

void EmployeeInterface::setupFeedbackSection()
{
    m_feedbackGroup = new QGroupBox("Feedback");
    QVBoxLayout *layout = new QVBoxLayout(m_feedbackGroup);
    
    QLabel *feedbackLabel = new QLabel("Share your feedback:");
    feedbackLabel->setObjectName("sectionSubLabel");
    
    m_feedbackEdit = new QTextEdit;
    m_feedbackEdit->setMaximumHeight(100);
    m_feedbackEdit->setPlaceholderText("Enter your feedback here...");
    
    m_submitFeedbackButton = new QPushButton("Submit Feedback");
    
    layout->addWidget(feedbackLabel);
    layout->addWidget(m_feedbackEdit);
    layout->addWidget(m_submitFeedbackButton);
    
    connect(m_submitFeedbackButton, &QPushButton::clicked, this, &EmployeeInterface::submitFeedback);
}

void EmployeeInterface::setupStyles()
{    QString style = R"(
        #employeeTitle {
            font-size: 24px;
            font-weight: bold;
            color: #2c3e50;
            margin: 15px;
        }
        #dateTimeLabel {
            font-size: 14px;
            color: #34495e;
            margin-right: 15px;
            font-family: Arial;
        }
        
        #welcomeLabel {
            font-size: 18px;
            font-weight: bold;
            color: #27ae60;
            margin: 5px 0px;
        }
        
        #infoLabel {
            font-size: 14px;
            color: #34495e;
            margin: 3px 0px;
        }
        
        #salaryLabel {
            font-size: 14px;
            font-weight: bold;
            color: #e67e22;
            margin: 3px 0px;
        }
        
        #statusLabel {
            font-size: 14px;
            font-weight: bold;
            margin: 3px 0px;
        }
        
        #sectionSubLabel {
            font-size: 12px;
            font-weight: bold;
            color: #34495e;
        }
        
        #lastAttendanceLabel {
            font-size: 12px;
            color: #7f8c8d;
            margin: 5px 0px;
        }
        
        #attendanceButton {
            background-color: #27ae60;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
        }
        
        #attendanceButton:hover {
            background-color: #229954;
        }
        
        #attendanceButton:pressed {
            background-color: #1e8449;
        }
        
        QGroupBox {
            font-weight: bold;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 15px;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 15px;
            padding: 0 8px 0 8px;
            color: #2c3e50;
            font-size: 14px;
        }
        
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
            min-width: 120px;
        }
        
        QPushButton:hover {
            background-color: #2980b9;
        }
        
        QPushButton:pressed {
            background-color: #21618c;
        }
        
        #logoutButton {
            background-color: #e74c3c;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
            min-width: 80px;
        }
        
        #logoutButton:hover {
            background-color: #c0392b;
        }
        
        #logoutButton:pressed {
            background-color: #a93226;
        }
        
        QLineEdit, QDateEdit {
            padding: 6px 8px;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            font-size: 12px;
        }
        
        QLineEdit:focus, QDateEdit:focus {
            border-color: #3498db;
        }
        
        QTextEdit {
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            font-size: 12px;
        }
        
        QTableWidget {
            border: 1px solid #bdc3c7;
            gridline-color: #ecf0f1;
            background-color: white;
            alternate-background-color: #f8f9fa;
        }
        
        QTableWidget::item {
            padding: 6px;
        }
        
        QTableWidget::item:selected {
            background-color: #3498db;
            color: white;
        }
        
        QSplitter::handle {
            background-color: #bdc3c7;
        }
    )";
    
    setStyleSheet(style);
}

void EmployeeInterface::setCurrentEmployee(int employeeId)
{
    m_currentEmployeeId = employeeId;
    
    // Load employee data using BST search (O(log n))
    Employee* emp = m_dataManager->findEmployee(employeeId);
    if (emp) {
        m_currentEmployee = *emp;
    }
    
    updateDashboardInfo();
    loadAttendanceData();
    loadLeaveRequests();
}

void EmployeeInterface::loadEmployeeData()
{
    // Use DataManager BST for O(log n) employee search
    Employee* emp = m_dataManager->findEmployee(m_currentEmployeeId);
    if (emp) {
        m_currentEmployee = *emp;
    }
}

Employee EmployeeInterface::findEmployeeById(int id)
{
    // Use BST search from DataManager - O(log n) complexity
    Employee* emp = m_dataManager->findEmployee(id);
    if (emp) {
        return *emp;
    }
    
    // Return default employee if not found
    return Employee(id, "Unknown Employee", "Unknown", 0.0, "Unknown");
}

void EmployeeInterface::updateDashboardInfo()
{
    if (m_currentEmployee.getId() > 0) {
        m_welcomeLabel->setText(QString("Welcome, %1!").arg(m_currentEmployee.getName()));
        m_employeeInfoLabel->setText(QString("ID: %1 | Department: %2 | Position: %3")
                                   .arg(m_currentEmployee.getId())
                                   .arg(m_currentEmployee.getDepartment())
                                   .arg(m_currentEmployee.getPosition()));
        m_salaryLabel->setText(QString("Salary: $%1").arg(m_currentEmployee.getSalary(), 0, 'f', 2));
        
        // Check attendance status
        if (hasAttendanceToday()) {
            m_attendanceStatusLabel->setText("✓ Attendance marked for today");
            m_attendanceStatusLabel->setStyleSheet("color: #27ae60;");
        } else {
            m_attendanceStatusLabel->setText("⚠ Attendance not marked for today");
            m_attendanceStatusLabel->setStyleSheet("color: #e74c3c;");
        }
    }
}

void EmployeeInterface::loadAttendanceData()
{
    // Get employee attendance using AVL Tree from DataManager
    QList<AttendanceRecord> records = m_dataManager->getEmployeeAttendance(m_currentEmployeeId);
    
    QString lastAttendance = "Not recorded";
    QDate today = QDate::currentDate();
    
    // Find the most recent attendance (AVL tree returns sorted by date)
    for (const AttendanceRecord& record : records) {
        if (record.getDate() == today) {
            lastAttendance = QString("Today at %1").arg(record.getTimeIn().toString("hh:mm"));
        } else {
            lastAttendance = QString("%1 at %2").arg(record.getDate().toString("yyyy-MM-dd"))
                                                .arg(record.getTimeIn().toString("hh:mm"));
        }
    }
    
    m_lastAttendanceLabel->setText(QString("Last attendance: %1").arg(lastAttendance));
}

bool EmployeeInterface::hasAttendanceToday()
{
    // Use AVL Tree search from DataManager - O(log n) complexity
    return m_dataManager->hasAttendanceToday(m_currentEmployeeId);
}

void EmployeeInterface::loadLeaveRequests()
{
    // Get employee's leave requests from DataManager (Doubly Linked List)
    QList<LeaveRequest> myRequests = m_dataManager->getEmployeeLeaveRequests(m_currentEmployeeId);
    
    m_leaveStatusTable->setRowCount(myRequests.size());
    
    for (int i = 0; i < myRequests.size(); ++i) {
        const LeaveRequest& request = myRequests[i];
        m_leaveStatusTable->setItem(i, 0, new QTableWidgetItem(request.getStartDate().toString("yyyy-MM-dd")));
        m_leaveStatusTable->setItem(i, 1, new QTableWidgetItem(request.getEndDate().toString("yyyy-MM-dd")));
        m_leaveStatusTable->setItem(i, 2, new QTableWidgetItem(request.getReason()));
        
        QTableWidgetItem* statusItem = new QTableWidgetItem(request.getStatusString());
        if (request.getStatus() == LeaveStatus::Approved) {
            statusItem->setBackground(QBrush(QColor(46, 204, 113, 50)));
        } else if (request.getStatus() == LeaveStatus::Rejected) {
            statusItem->setBackground(QBrush(QColor(231, 76, 60, 50)));
        } else {
            statusItem->setBackground(QBrush(QColor(241, 196, 15, 50)));
        }
        m_leaveStatusTable->setItem(i, 3, statusItem);
    }
}

void EmployeeInterface::markAttendance()
{
    // Check using AVL Tree - O(log n)
    if (m_dataManager->hasAttendanceToday(m_currentEmployeeId)) {
        QMessageBox::information(this, "Attendance", "You have already marked attendance for today.");
        return;
    }
    
    QDateTime now = QDateTime::currentDateTime();
    AttendanceRecord record(m_currentEmployeeId, m_currentEmployee.getName(), 
                           now.date(), now.time());
    
    // Add to AVL Tree via DataManager - O(log n) with auto-balancing
    m_dataManager->addAttendanceRecord(record);
    m_dataManager->saveAttendanceRecords();
    
    QMessageBox::information(this, "Success", 
        QString("Attendance marked successfully at %1").arg(now.time().toString("hh:mm")));
    
    loadAttendanceData();
    updateDashboardInfo();
}

void EmployeeInterface::submitLeaveRequest()
{
    // Validate start date is not in the past
    if (m_startDateEdit->date() < QDate::currentDate()) {
        QMessageBox::warning(this, "Invalid Dates", "Start date cannot be in the past.");
        return;
    }
    
    if (m_startDateEdit->date() > m_endDateEdit->date()) {
        QMessageBox::warning(this, "Invalid Dates", "End date must be after start date.");
        return;
    }
    
    if (m_reasonEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please provide a reason for leave.");
        m_reasonEdit->setFocus();
        return;
    }
    
    LeaveRequest request(m_currentEmployeeId, m_currentEmployee.getName(),
                        m_startDateEdit->date(), m_endDateEdit->date(),
                        m_reasonEdit->text().trimmed());
    
    // Add to DataManager Queue - FIFO processing by admin
    m_dataManager->submitLeaveRequest(request);
    m_dataManager->saveLeaveRequests();
    
    // Clear form
    m_startDateEdit->setDate(QDate::currentDate());
    m_endDateEdit->setDate(QDate::currentDate().addDays(1));
    m_reasonEdit->clear();
    
    loadLeaveRequests();
    
    QMessageBox::information(this, "Success", "Leave request submitted successfully!");
}

void EmployeeInterface::saveLeaveRequest(const LeaveRequest& request)
{
    QFile file("leave_requests.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << request.toFileString() << "\n";
    }
}

void EmployeeInterface::submitFeedback()
{
    QString feedback = m_feedbackEdit->toPlainText().trimmed();
    if (feedback.isEmpty()) {
        QMessageBox::warning(this, "Empty Feedback", "Please enter your feedback.");
        return;
    }
    
    QString timestampedFeedback = QString("[%1] Employee %2 (%3): %4\n")
                                 .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"))
                                 .arg(m_currentEmployee.getId())
                                 .arg(m_currentEmployee.getName())
                                 .arg(feedback);
    
    saveFeedback(timestampedFeedback);
    m_feedbackEdit->clear();
    
    QMessageBox::information(this, "Success", "Feedback submitted successfully.");
}

void EmployeeInterface::saveFeedback(const QString& feedback)
{
    QFile file("feedback.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << feedback;
    }
}

void EmployeeInterface::logout()
{
    int ret = QMessageBox::question(this, "Sign Out", 
                                   "Are you sure you want to sign out?",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        emit logoutRequested();
    }
}

void EmployeeInterface::refreshData()
{
    // Reload data from DataManager
    m_dataManager->loadAllData();
    
    // Refresh employee using BST search
    Employee* emp = m_dataManager->findEmployee(m_currentEmployeeId);
    if (emp) {
        m_currentEmployee = *emp;
    }
    
    updateDashboardInfo();
    loadAttendanceData();
    loadLeaveRequests();
    
    QMessageBox::information(this, "Refresh", "Data refreshed successfully!");
}

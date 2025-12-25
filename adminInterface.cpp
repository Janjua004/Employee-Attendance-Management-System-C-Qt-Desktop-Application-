#include "adminInterface.h"
#include "ui_adminInterface.h"
#include "employee.h"
#include "leaveRequest.h"
#include "DataStructures.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QtCharts>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QSet>

AdminInterface::AdminInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminInterface)
    , m_dateTimeLabel(new QLabel(this))
    , m_dateTimeTimer(new QTimer(this))
    , m_tabWidget(new QTabWidget)
    , m_employeeTab(new QWidget)
    , m_leaveTab(new QWidget)
    , m_feedbackTab(new QWidget)
    , attendanceChart(new QChart)
    , chartView(new QChartView(attendanceChart))
    , m_attendanceSeries(new QPieSeries)
    , m_fileWatcher(new QFileSystemWatcher(this))
    , m_undoButton(nullptr)
    , m_redoButton(nullptr)
    , m_deleteEmpButton(nullptr)
    , m_dataManager(DataManager::getInstance())  // Initialize DataManager
{
    ui->setupUi(this);
    
    // Create main layout container
    QWidget* mainContainer = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainContainer);
    setCentralWidget(mainContainer);

    // Create header with date/time and logout button
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("Administration Panel", this);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; }");
    m_dateTimeLabel->setStyleSheet("QLabel { color: #fff; font-size: 14px; padding: 8px 15px; background-color: #3498db; border-radius: 5px; font-weight: bold; }");
    m_dateTimeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    QPushButton* logoutButton = new QPushButton("Sign Out", this);
    logoutButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 8px 15px; border: none; border-radius: 5px; font-weight: bold; } QPushButton:hover { background-color: #c0392b; }");
    connect(logoutButton, &QPushButton::clicked, this, &AdminInterface::logout);
    
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_dateTimeLabel);
    headerLayout->addWidget(logoutButton);
    
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(m_tabWidget);

    // Setup UI components
    setupUI();
    setupStyles();
    setupAttendanceChart();

    // Connect timer for date/time updates
    connect(m_dateTimeTimer, &QTimer::timeout, this, &AdminInterface::updateDateTime);
    m_dateTimeTimer->start(1000);
    updateDateTime(); // Initial update

    // Watch files for changes
    m_fileWatcher->addPath("employees.txt");
    m_fileWatcher->addPath("leave_requests.txt");
    m_fileWatcher->addPath("feedback.txt");
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &AdminInterface::onFileChanged);

    // Initialize data using DataManager (uses DSA internally)
    m_dataManager->loadAllData();

    // Initial refresh of views
    refreshEmployeeList();
    refreshLeaveRequests();
    refreshFeedback();
}

AdminInterface::~AdminInterface()
{
    delete ui;
}

void AdminInterface::setupUI()
{
    // Setup individual tabs
    setupEmployeeTab();
    setupLeaveTab();
    setupFeedbackTab();

    // Add tabs to widget
    m_tabWidget->addTab(m_employeeTab, "Employee Management");
    m_tabWidget->addTab(m_leaveTab, "Leave Requests");
    m_tabWidget->addTab(m_feedbackTab, "Feedback");
    
    // Set tab style
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #cccccc; }"
        "QTabWidget::tab-bar { left: 5px; }"
        "QTabBar::tab { background: #f0f0f0; padding: 8px 20px; margin: 2px; }"
        "QTabBar::tab:selected { background: #ffffff; border: 1px solid #cccccc; border-bottom: none; }"
    );
}

void AdminInterface::setupEmployeeTab()
{
    QVBoxLayout* layout = new QVBoxLayout(m_employeeTab);
    QHBoxLayout* contentLayout = new QHBoxLayout();
    
    // Left side - Employee list and form
    QVBoxLayout* leftLayout = new QVBoxLayout();
    m_employeeTable = new QTableWidget(this);
    m_employeeTable->setColumnCount(5);
    m_employeeTable->setHorizontalHeaderLabels({"ID", "Name", "Department", "Position", "Salary"});
    
    // Create form widgets
    m_empNameEdit = new QLineEdit(this);
    m_empDepartmentEdit = new QLineEdit(this);
    m_empPositionEdit = new QLineEdit(this);
    m_empSalaryEdit = new QDoubleSpinBox(this);
    m_empSalaryEdit->setRange(0, 1000000);
    m_empSalaryEdit->setDecimals(2);
    m_empPasswordEdit = new QLineEdit(this);
    m_empPasswordEdit->setEchoMode(QLineEdit::Password);
    
    // Add form widgets to layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Name:", m_empNameEdit);
    formLayout->addRow("Department:", m_empDepartmentEdit);
    formLayout->addRow("Position:", m_empPositionEdit);
    formLayout->addRow("Salary:", m_empSalaryEdit);
    formLayout->addRow("Password:", m_empPasswordEdit);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_addEmpButton = new QPushButton("Add Employee", this);
    m_editEmpButton = new QPushButton("Update Employee", this);
    m_deleteEmpButton = new QPushButton("Delete Employee", this);
    m_deleteEmpButton->setStyleSheet("background-color: #e74c3c;");
    
    // DSA Operation Buttons
    m_undoButton = new QPushButton("↶ Undo", this);
    m_redoButton = new QPushButton("↷ Redo", this);
    
    m_undoButton->setEnabled(false);
    m_redoButton->setEnabled(false);
    
    buttonLayout->addWidget(m_addEmpButton);
    buttonLayout->addWidget(m_editEmpButton);
    buttonLayout->addWidget(m_deleteEmpButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_undoButton);
    buttonLayout->addWidget(m_redoButton);
    
    leftLayout->addWidget(m_employeeTable);
    leftLayout->addLayout(formLayout);
    leftLayout->addLayout(buttonLayout);
    
    // Right side - Attendance chart
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->addWidget(chartView);
    
    // Add left and right layouts to content
    contentLayout->addLayout(leftLayout, 2);
    contentLayout->addLayout(rightLayout, 1);
    
    // Add content to main layout
    layout->addLayout(contentLayout);
    
    // Connect signals
    connect(m_employeeTable, &QTableWidget::itemSelectionChanged,
            this, &AdminInterface::onEmployeeSelectionChanged);
    connect(m_addEmpButton, &QPushButton::clicked,
            this, &AdminInterface::addEmployee);
    connect(m_editEmpButton, &QPushButton::clicked,
            this, &AdminInterface::editEmployee);
    connect(m_deleteEmpButton, &QPushButton::clicked,
            this, &AdminInterface::deleteEmployee);
    // DSA Button Connections
    connect(m_undoButton, &QPushButton::clicked,
            this, &AdminInterface::undoAction);
    connect(m_redoButton, &QPushButton::clicked,
            this, &AdminInterface::redoAction);
}

void AdminInterface::setupAttendanceChart()
{
    attendanceChart->addSeries(m_attendanceSeries);
    attendanceChart->setTitle("Monthly Attendance");
    attendanceChart->setAnimationOptions(QChart::AllAnimations);
    attendanceChart->legend()->setAlignment(Qt::AlignBottom);
    
    chartView->setRenderHint(QPainter::Antialiasing);
}

void AdminInterface::calculateAttendanceData(const QString& employeeId, int& present, int& absent)
{
    present = 0;
    absent = 0;
    
    QFile file("attendance.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QDateTime currentDate = QDateTime::currentDateTime();
        QSet<int> daysPresent;
        
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split("|");  // File uses | delimiter
            if (parts.size() >= 3 && parts[0] == employeeId) {
                QDate attendanceDate = QDate::fromString(parts[2], "yyyy-MM-dd");
                if (attendanceDate.month() == currentDate.date().month() &&
                    attendanceDate.year() == currentDate.date().year()) {
                    daysPresent.insert(attendanceDate.day());
                }
            }
        }
        file.close();
        
        present = daysPresent.size();
        // Calculate working days in the current month up to today
        int daysInMonth = qMin(currentDate.date().day(), currentDate.date().daysInMonth());
        int workingDays = 0;
        for (int d = 1; d <= daysInMonth; d++) {
            QDate date(currentDate.date().year(), currentDate.date().month(), d);
            if (date.dayOfWeek() <= 5) {  // Monday to Friday
                workingDays++;
            }
        }
        absent = qMax(0, workingDays - present);
    }
}

void AdminInterface::updateAttendanceChart(const QString& employeeId)
{
    int present = 0, absent = 0;
    calculateAttendanceData(employeeId, present, absent);
    
    m_attendanceSeries->clear();
    
    if (present > 0)
        m_attendanceSeries->append("Present (" + QString::number(present) + ")", present);
    if (absent > 0)
        m_attendanceSeries->append("Absent (" + QString::number(absent) + ")", absent);
    
    int total = present + absent;
    if (total > 0 && !m_attendanceSeries->slices().isEmpty()) {
        for (auto slice : m_attendanceSeries->slices()) {
            if (slice && slice->label().startsWith("Present")) {
                slice->setBrush(QColor("#4CAF50")); // Green
            } else if (slice) {
                slice->setBrush(QColor("#F44336")); // Red
            }
            
            // Calculate percentage safely
            if (slice) {
                double percentage = (slice->value() / static_cast<double>(total)) * 100.0;
                slice->setLabel(slice->label() + QString(" - %1%").arg(percentage, 0, 'f', 1));
            }
        }
    }
    
    attendanceChart->update();
}

void AdminInterface::setupLeaveTab()
{
    // m_leaveTab is already created in constructor
    QVBoxLayout *layout = new QVBoxLayout(m_leaveTab);
    
    QLabel *titleLabel = new QLabel("Leave Request Management");
    titleLabel->setObjectName("sectionLabel");
    
    m_leaveTable = new QTableWidget;
    m_leaveTable->setColumnCount(6);
    QStringList headers = {"Employee ID", "Employee Name", "Start Date", "End Date", "Reason", "Status"};
    m_leaveTable->setHorizontalHeaderLabels(headers);
    m_leaveTable->horizontalHeader()->setStretchLastSection(true);
    m_leaveTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_leaveTable->setAlternatingRowColors(true);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    m_approveButton = new QPushButton("Approve Selected");
    m_rejectButton = new QPushButton("Reject Selected");
    
    m_approveButton->setEnabled(false);
    m_rejectButton->setEnabled(false);
    
    buttonLayout->addWidget(m_approveButton);
    buttonLayout->addWidget(m_rejectButton);
    buttonLayout->addStretch();
    
    layout->addWidget(titleLabel);
    layout->addWidget(m_leaveTable);
    layout->addLayout(buttonLayout);
    
    // Connect signals
    connect(m_leaveTable, &QTableWidget::itemSelectionChanged,
            this, &AdminInterface::onLeaveRequestSelectionChanged);
    connect(m_approveButton, &QPushButton::clicked,
            this, &AdminInterface::approveLeaveRequest);
    connect(m_rejectButton, &QPushButton::clicked,
            this, &AdminInterface::rejectLeaveRequest);
}

void AdminInterface::setupFeedbackTab()
{
    // m_feedbackTab is already created in constructor
    QVBoxLayout *layout = new QVBoxLayout(m_feedbackTab);
    
    QLabel *titleLabel = new QLabel("Employee Feedback");
    titleLabel->setObjectName("sectionLabel");
    
    m_feedbackDisplay = new QTextEdit;
    m_feedbackDisplay->setReadOnly(true);
    
    QLabel* autoUpdateLabel = new QLabel("(Updates automatically when feedback changes)");
    autoUpdateLabel->setStyleSheet("color: gray; font-style: italic;");
    autoUpdateLabel->setAlignment(Qt::AlignRight);
    
    layout->addWidget(titleLabel);
    layout->addWidget(m_feedbackDisplay);
    layout->addWidget(autoUpdateLabel);
}

void AdminInterface::setupStyles()
{
    QString style = R"(
        #adminTitle {
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
        
        #sectionLabel {
            font-size: 16px;
            font-weight: bold;
            color: #34495e;
            margin: 10px 0px;
        }
        
        QTabWidget::pane {
            border: 1px solid #bdc3c7;
            background-color: white;
        }
        
        QTabWidget::tab-bar {
            alignment: left;
        }
        
        QTabBar::tab {
            background-color: #ecf0f1;
            padding: 10px 20px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        
        QTabBar::tab:selected {
            background-color: white;
            border-bottom: 2px solid #3498db;
        }
        
        QTableWidget {
            border: 1px solid #bdc3c7;
            gridline-color: #ecf0f1;
            background-color: white;
            alternate-background-color: #f8f9fa;
        }
        
        QTableWidget::item {
            padding: 8px;
        }
        
        QTableWidget::item:selected {
            background-color: #3498db;
            color: white;
        }
        
        QGroupBox {
            font-weight: bold;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
            min-width: 100px;
        }
        
        QPushButton:hover {
            background-color: #2980b9;
        }
        
        QPushButton:pressed {
            background-color: #21618c;
        }
          QPushButton:disabled {
            background-color: #95a5a6;
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
        
        QLineEdit, QDoubleSpinBox {
            padding: 6px;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
        }
        
        QLineEdit:focus, QDoubleSpinBox:focus {
            border-color: #3498db;
        }
        
        QTextEdit {
            border: 1px solid #bdc3c7;
            border-radius: 4px;
        }
    )";
    
    setStyleSheet(style);
}

void AdminInterface::loadEmployees()
{
    // Data is loaded via DataManager using Doubly Linked List + BST
    m_dataManager->loadEmployees();
}

void AdminInterface::saveEmployees()
{
    // Save via DataManager (uses Doubly Linked List traversal)
    m_dataManager->saveEmployees();
}

void AdminInterface::loadLeaveRequests()
{
    // Reload data from file via DataManager
    m_dataManager->loadLeaveRequests();
}

void AdminInterface::saveLeaveRequests()
{
    // Save via DataManager (uses Doubly Linked List traversal)
    m_dataManager->saveLeaveRequests();
}

void AdminInterface::loadFeedback()
{
    QString feedbackText;
    QFile file("feedback.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        feedbackText = in.readAll();
    }
    
    if (feedbackText.isEmpty()) {
        feedbackText = "No feedback available.";
    }
    
    m_feedbackDisplay->setPlainText(feedbackText);
}

void AdminInterface::refreshEmployeeList()
{
    loadEmployees();
    
    // Get employees from DataManager (stored in Doubly Linked List)
    QList<Employee> employees = m_dataManager->getAllEmployees();
    
    m_employeeTable->setRowCount(employees.size());
    
    for (int i = 0; i < employees.size(); ++i) {
        const Employee& emp = employees[i];
        m_employeeTable->setItem(i, 0, new QTableWidgetItem(QString::number(emp.getId())));
        m_employeeTable->setItem(i, 1, new QTableWidgetItem(emp.getName()));
        m_employeeTable->setItem(i, 2, new QTableWidgetItem(emp.getDepartment()));
        m_employeeTable->setItem(i, 3, new QTableWidgetItem(emp.getPosition()));
        m_employeeTable->setItem(i, 4, new QTableWidgetItem(QString::number(emp.getSalary(), 'f', 2)));
    }
    
    clearEmployeeForm();
    
    // Update undo/redo button states
    m_undoButton->setEnabled(m_dataManager->canUndo());
    m_redoButton->setEnabled(m_dataManager->canRedo());
}

void AdminInterface::addEmployee()
{
    // Validate required fields
    if (m_empNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter employee name.");
        m_empNameEdit->setFocus();
        return;
    }
    
    if (m_empDepartmentEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter department.");
        m_empDepartmentEdit->setFocus();
        return;
    }
    
    if (m_empPositionEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter position.");
        m_empPositionEdit->setFocus();
        return;
    }
    
    // Find next available ID using BST max operation
    QList<Employee> employees = m_dataManager->getAllEmployees();
    int maxId = 1000;
    for (const Employee& emp : employees) {
        if (emp.getId() > maxId) {
            maxId = emp.getId();
        }
    }
    
    Employee newEmp(maxId + 1, 
                   m_empNameEdit->text().trimmed(),
                   m_empDepartmentEdit->text().trimmed(),
                   m_empSalaryEdit->value(),
                   m_empPositionEdit->text().trimmed());
    
    if (!m_empPasswordEdit->text().isEmpty()) {
        newEmp.setPassword(m_empPasswordEdit->text());
    }
    
    // Add employee using DataManager (stores in Doubly Linked List + BST)
    m_dataManager->addEmployee(newEmp);
    m_dataManager->saveEmployees();
    refreshEmployeeList();
    
    QMessageBox::information(this, "Success", "Employee added successfully!");
}

void AdminInterface::editEmployee()
{
    int row = m_employeeTable->currentRow();
    QList<Employee> employees = m_dataManager->getAllEmployees();
    
    if (row < 0 || row >= employees.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select an employee to update.");
        return;
    }
    
    // Validate required fields
    if (m_empNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Employee name cannot be empty.");
        m_empNameEdit->setFocus();
        return;
    }
    
    int empId = employees[row].getId();
    
    Employee updatedEmp(empId,
                       m_empNameEdit->text().trimmed(),
                       m_empDepartmentEdit->text().trimmed(),
                       m_empSalaryEdit->value(),
                       m_empPositionEdit->text().trimmed());
    
    if (!m_empPasswordEdit->text().isEmpty()) {
        updatedEmp.setPassword(m_empPasswordEdit->text());
    } else {
        updatedEmp.setPassword(employees[row].getPassword());
    }
    
    // Update using DataManager (updates in both Doubly Linked List and BST)
    m_dataManager->updateEmployee(empId, updatedEmp);
    m_dataManager->saveEmployees();
    refreshEmployeeList();
    
    QMessageBox::information(this, "Success", "Employee updated successfully!");
}

void AdminInterface::deleteEmployee()
{
    int row = m_employeeTable->currentRow();
    QList<Employee> employees = m_dataManager->getAllEmployees();
    
    if (row < 0 || row >= employees.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select an employee to delete.");
        return;
    }
    
    const Employee& emp = employees[row];
    int ret = QMessageBox::question(this, "Confirm Delete", 
                                   QString("Are you sure you want to delete employee %1?").arg(emp.getName()),
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        // Delete using DataManager (removes from both Doubly Linked List and BST)
        m_dataManager->deleteEmployee(emp.getId());
        m_dataManager->saveEmployees();
        refreshEmployeeList();
        
        QMessageBox::information(this, "Success", "Employee deleted successfully!");
    }
}

void AdminInterface::onEmployeeSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = m_employeeTable->selectedItems();
    if (!selectedItems.isEmpty()) {
        QString employeeId = selectedItems[0]->text(); // First column is ID
        updateAttendanceChart(employeeId);
        
        // Populate form using BST search (O(log n))
        int id = employeeId.toInt();
        Employee* emp = m_dataManager->findEmployee(id);
        if (emp) {
            populateEmployeeForm(*emp);
            m_editEmpButton->setEnabled(true);
        }
    }
}

void AdminInterface::clearEmployeeForm()
{
    m_empNameEdit->clear();
    m_empDepartmentEdit->clear();
    m_empSalaryEdit->setValue(0);
    m_empPositionEdit->clear();
    m_empPasswordEdit->clear();
    m_editEmpButton->setEnabled(false);
}

void AdminInterface::populateEmployeeForm(const Employee& emp)
{
    m_empNameEdit->setText(emp.getName());
    m_empDepartmentEdit->setText(emp.getDepartment());
    m_empSalaryEdit->setValue(emp.getSalary());
    m_empPositionEdit->setText(emp.getPosition());
    m_empPasswordEdit->clear(); // Don't show password for security
}

void AdminInterface::refreshLeaveRequests()
{
    loadLeaveRequests();
    
    // Get all leave requests from DataManager (stored in Doubly Linked List)
    QList<LeaveRequest> leaveRequests = m_dataManager->getAllLeaveRequests();
    
    m_leaveTable->setRowCount(leaveRequests.size());
    
    for (int i = 0; i < leaveRequests.size(); ++i) {
        const LeaveRequest& request = leaveRequests[i];
        m_leaveTable->setItem(i, 0, new QTableWidgetItem(QString::number(request.getEmployeeId())));
        m_leaveTable->setItem(i, 1, new QTableWidgetItem(request.getEmployeeName()));
        m_leaveTable->setItem(i, 2, new QTableWidgetItem(request.getStartDate().toString("yyyy-MM-dd")));
        m_leaveTable->setItem(i, 3, new QTableWidgetItem(request.getEndDate().toString("yyyy-MM-dd")));
        m_leaveTable->setItem(i, 4, new QTableWidgetItem(request.getReason()));
        
        QTableWidgetItem* statusItem = new QTableWidgetItem(request.getStatusString());
        if (request.getStatus() == LeaveStatus::Approved) {
            statusItem->setBackground(QBrush(QColor(46, 204, 113, 100)));
        } else if (request.getStatus() == LeaveStatus::Rejected) {
            statusItem->setBackground(QBrush(QColor(231, 76, 60, 100)));
        } else {
            statusItem->setBackground(QBrush(QColor(241, 196, 15, 100)));
        }
        m_leaveTable->setItem(i, 5, statusItem);
    }
    
    // Update undo/redo button states
    m_undoButton->setEnabled(m_dataManager->canUndo());
    m_redoButton->setEnabled(m_dataManager->canRedo());
}

void AdminInterface::approveLeaveRequest()
{
    int row = m_leaveTable->currentRow();
    QList<LeaveRequest> leaveRequests = m_dataManager->getAllLeaveRequests();
    
    if (row >= 0 && row < leaveRequests.size()) {
        const LeaveRequest& request = leaveRequests[row];
        
        // Process leave request using DataManager (uses Queue for FIFO processing)
        m_dataManager->processLeaveRequest(request.getEmployeeId(), 
                                           request.getStartDate(), 
                                           LeaveStatus::Approved);
        m_dataManager->saveLeaveRequests();
        refreshLeaveRequests();
        
        QMessageBox::information(this, "Success", "Leave request approved!");
    }
}

void AdminInterface::rejectLeaveRequest()
{
    int row = m_leaveTable->currentRow();
    QList<LeaveRequest> leaveRequests = m_dataManager->getAllLeaveRequests();
    
    if (row >= 0 && row < leaveRequests.size()) {
        const LeaveRequest& request = leaveRequests[row];
        
        // Process leave request using DataManager
        m_dataManager->processLeaveRequest(request.getEmployeeId(), 
                                           request.getStartDate(), 
                                           LeaveStatus::Rejected);
        m_dataManager->saveLeaveRequests();
        refreshLeaveRequests();
        
        QMessageBox::information(this, "Success", "Leave request rejected.");
    }
}

void AdminInterface::onLeaveRequestSelectionChanged()
{
    int row = m_leaveTable->currentRow();
    QList<LeaveRequest> leaveRequests = m_dataManager->getAllLeaveRequests();
    
    bool hasSelection = (row >= 0 && row < leaveRequests.size());
    bool isPending = hasSelection && leaveRequests[row].getStatus() == LeaveStatus::Pending;
    
    m_approveButton->setEnabled(isPending);
    m_rejectButton->setEnabled(isPending);
}

void AdminInterface::refreshFeedback()
{
    loadFeedback();
}

void AdminInterface::logout()
{
    int ret = QMessageBox::question(this, "Sign Out",
                                  "Are you sure you want to sign out?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        emit logoutRequested();
    }
}

void AdminInterface::onFileChanged(const QString& path)
{
    // Re-add the file to the watcher as it might have been recreated
    m_fileWatcher->addPath(path);
    
    // Auto-refresh leave requests and feedback when their files change
    if (path.endsWith("leave_requests.txt")) {
        loadLeaveRequests();
        refreshLeaveRequests();
        
        // Update the button states
        QList<QTableWidgetItem*> selectedItems = m_leaveTable->selectedItems();
        bool hasSelection = !selectedItems.isEmpty();
        m_approveButton->setEnabled(hasSelection);
        m_rejectButton->setEnabled(hasSelection);
    }
    else if (path.endsWith("feedback.txt")) {
        loadFeedback();
        refreshFeedback();
    }
}

void AdminInterface::updateDateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss AP");
    m_dateTimeLabel->setText(formattedDateTime);
}

// ============================================================================
// DSA OPERATIONS - Undo, Redo, and Statistics
// ============================================================================

void AdminInterface::undoAction()
{
    if (m_dataManager->undo()) {
        m_dataManager->saveAllData();
        refreshEmployeeList();
        refreshLeaveRequests();
        
        QMessageBox::information(this, "Undo Successful", "Last action has been undone.");
    } else {
        QMessageBox::information(this, "Undo", "Nothing to undo.");
    }
}

void AdminInterface::redoAction()
{
    if (m_dataManager->redo()) {
        m_dataManager->saveAllData();
        refreshEmployeeList();
        refreshLeaveRequests();
        
        QMessageBox::information(this, "Redo Successful", "Action has been redone.");
    } else {
        QMessageBox::information(this, "Redo", "Nothing to redo.");
    }
}

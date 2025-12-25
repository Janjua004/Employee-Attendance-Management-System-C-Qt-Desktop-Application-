#ifndef EMPLOYEEINTERFACE_H
#define EMPLOYEEINTERFACE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QGroupBox>
#include <QTableWidget>
#include <QFrame>
#include <QTimer>
#include <QDateTime>
#include "employee.h"
#include "leaveRequest.h"
#include "attendanceRecord.h"
#include "DataStructures.h"  // Include DSA Header

class DataManager;  // Forward declaration

class EmployeeInterface : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeeInterface(QWidget *parent = nullptr);
    void setCurrentEmployee(int employeeId);

private slots:
    void markAttendance();
    void submitLeaveRequest();
    void submitFeedback();
    void refreshData();
    void logout();

signals:
    void logoutRequested();

private:
    void setupUI();
    void setupDashboard();
    void setupAttendanceSection();
    void setupLeaveSection();
    void setupFeedbackSection();
    void setupStyles();
    
    void loadEmployeeData();
    void loadAttendanceData();
    void loadLeaveRequests();
    void saveLeaveRequest(const LeaveRequest& request);
    void saveFeedback(const QString& feedback);
    void updateDashboardInfo();
    
    Employee findEmployeeById(int id);
    bool hasAttendanceToday();
    
    int m_currentEmployeeId;
    Employee m_currentEmployee;
    
    // DataManager for DSA operations
    DataManager* m_dataManager;
    
    // UI Components
    QVBoxLayout *m_mainLayout;
    
    // Dashboard
    QGroupBox *m_dashboardGroup;
    QLabel *m_welcomeLabel;
    QLabel *m_employeeInfoLabel;
    QLabel *m_salaryLabel;
    QLabel *m_attendanceStatusLabel;
    
    // Attendance Section
    QGroupBox *m_attendanceGroup;
    QPushButton *m_markAttendanceButton;
    QLabel *m_lastAttendanceLabel;
    
    // Leave Request Section
    QGroupBox *m_leaveGroup;
    QDateEdit *m_startDateEdit;
    QDateEdit *m_endDateEdit;
    QLineEdit *m_reasonEdit;
    QPushButton *m_submitLeaveButton;
    QTableWidget *m_leaveStatusTable;
    
    // Feedback Section
    QGroupBox *m_feedbackGroup;
    QTextEdit *m_feedbackEdit;
    QPushButton *m_submitFeedbackButton;
    
    QPushButton *m_refreshButton;
};

#endif // EMPLOYEEINTERFACE_H

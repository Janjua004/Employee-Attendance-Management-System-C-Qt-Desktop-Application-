#ifndef ADMININTERFACE_H
#define ADMININTERFACE_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QFileSystemWatcher>
#include <QTextEdit>
#include <QTabWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include "DataStructures.h"  // Include DSA Header

QT_BEGIN_NAMESPACE
namespace Ui {
class AdminInterface;
}
QT_END_NAMESPACE

class Employee;
class LeaveRequest;
class DataManager;  // Forward declaration

class AdminInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminInterface(QWidget *parent = nullptr);
    ~AdminInterface();

signals:
    void logoutRequested();

private slots:
    void updateDateTime();
    void onFileChanged(const QString& path);
    void onEmployeeSelectionChanged();
    void onLeaveRequestSelectionChanged();
    void addEmployee();
    void editEmployee();
    void deleteEmployee();
    void approveLeaveRequest();
    void rejectLeaveRequest();
    void logout();
    // DSA Operations
    void undoAction();
    void redoAction();

private:
    void setupUI();
    void setupEmployeeTab();
    void setupLeaveTab();
    void setupFeedbackTab();
    void setupStyles();
    void setupAttendanceChart();
    void clearEmployeeForm();
    void populateEmployeeForm(const Employee& emp);
    void calculateAttendanceData(const QString& employeeId, int& present, int& absent);
    void updateAttendanceChart(const QString& employeeId);
    void loadEmployees();
    void saveEmployees();
    void loadLeaveRequests();
    void saveLeaveRequests();
    void loadFeedback();
    void refreshEmployeeList();
    void refreshLeaveRequests();
    void refreshFeedback();

private:
    Ui::AdminInterface *ui;
    QLabel* m_dateTimeLabel;
    QTimer* m_dateTimeTimer;
    QVBoxLayout* topLayout;
    
    // Main UI elements
    QTabWidget* m_tabWidget;
    QWidget* m_employeeTab;
    QWidget* m_leaveTab;
    QWidget* m_feedbackTab;

    // Chart elements
    QChart* attendanceChart;
    QChartView* chartView;
    QPieSeries* m_attendanceSeries;
    
    // Employee tab elements
    QTableWidget* m_employeeTable;
    QLineEdit* m_empNameEdit;
    QLineEdit* m_empDepartmentEdit;
    QDoubleSpinBox* m_empSalaryEdit;
    QLineEdit* m_empPositionEdit;
    QLineEdit* m_empPasswordEdit;
    QPushButton* m_addEmpButton;
    QPushButton* m_editEmpButton;
    
    // Leave tab elements
    QTableWidget* m_leaveTable;
    QPushButton* m_approveButton;
    QPushButton* m_rejectButton;

    // Feedback tab elements
    QTextEdit* m_feedbackDisplay;
    
    // File watcher
    QFileSystemWatcher* m_fileWatcher;
    
    // DSA Buttons
    QPushButton* m_undoButton;
    QPushButton* m_redoButton;
    QPushButton* m_deleteEmpButton;
    
    // DataManager for DSA operations
    DataManager* m_dataManager;
};

#endif // ADMININTERFACE_H

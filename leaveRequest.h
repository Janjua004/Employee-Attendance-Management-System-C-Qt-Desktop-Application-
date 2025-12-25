#ifndef LEAVEREQUEST_H
#define LEAVEREQUEST_H

#include <QString>
#include <QDate>

enum class LeaveStatus {
    Pending,
    Approved,
    Rejected
};

class LeaveRequest
{
public:
    LeaveRequest();
    LeaveRequest(int employeeId, const QString& employeeName, 
                 const QDate& startDate, const QDate& endDate, 
                 const QString& reason);

    // Getters
    int getEmployeeId() const { return m_employeeId; }
    QString getEmployeeName() const { return m_employeeName; }
    QDate getStartDate() const { return m_startDate; }
    QDate getEndDate() const { return m_endDate; }
    QString getReason() const { return m_reason; }
    LeaveStatus getStatus() const { return m_status; }
    QString getStatusString() const;

    // Setters
    void setStatus(LeaveStatus status) { m_status = status; }

    // File operations
    QString toFileString() const;
    static LeaveRequest fromFileString(const QString& line);

private:
    int m_employeeId;
    QString m_employeeName;
    QDate m_startDate;
    QDate m_endDate;
    QString m_reason;
    LeaveStatus m_status;
};

#endif // LEAVEREQUEST_H

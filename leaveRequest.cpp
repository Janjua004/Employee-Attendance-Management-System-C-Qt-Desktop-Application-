#include "leaveRequest.h"
#include <QStringList>

LeaveRequest::LeaveRequest()
    : m_employeeId(0), m_status(LeaveStatus::Pending)
{
}

LeaveRequest::LeaveRequest(int employeeId, const QString& employeeName, 
                          const QDate& startDate, const QDate& endDate, 
                          const QString& reason)
    : m_employeeId(employeeId), m_employeeName(employeeName),
      m_startDate(startDate), m_endDate(endDate), m_reason(reason),
      m_status(LeaveStatus::Pending)
{
}

QString LeaveRequest::getStatusString() const
{
    switch (m_status) {
        case LeaveStatus::Pending: return "Pending";
        case LeaveStatus::Approved: return "Approved";
        case LeaveStatus::Rejected: return "Rejected";
        default: return "Unknown";
    }
}

QString LeaveRequest::toFileString() const
{
    return QString("%1|%2|%3|%4|%5|%6")
           .arg(m_employeeId)
           .arg(m_employeeName)
           .arg(m_startDate.toString("yyyy-MM-dd"))
           .arg(m_endDate.toString("yyyy-MM-dd"))
           .arg(m_reason)
           .arg(static_cast<int>(m_status));
}

LeaveRequest LeaveRequest::fromFileString(const QString& line)
{
    QStringList parts = line.split('|');
    if (parts.size() >= 6) {
        LeaveRequest request;
        request.m_employeeId = parts[0].toInt();
        request.m_employeeName = parts[1];
        request.m_startDate = QDate::fromString(parts[2], "yyyy-MM-dd");
        request.m_endDate = QDate::fromString(parts[3], "yyyy-MM-dd");
        request.m_reason = parts[4];
        request.m_status = static_cast<LeaveStatus>(parts[5].toInt());
        return request;
    }
    return LeaveRequest();
}

#include "attendanceRecord.h"
#include <QStringList>

AttendanceRecord::AttendanceRecord()
    : m_employeeId(0)
{
}

AttendanceRecord::AttendanceRecord(int employeeId, const QString& employeeName, 
                                  const QDate& date, const QTime& timeIn, 
                                  const QTime& timeOut)
    : m_employeeId(employeeId), m_employeeName(employeeName),
      m_date(date), m_timeIn(timeIn), m_timeOut(timeOut)
{
}

QString AttendanceRecord::toFileString() const
{
    return QString("%1|%2|%3|%4|%5")
           .arg(m_employeeId)
           .arg(m_employeeName)
           .arg(m_date.toString("yyyy-MM-dd"))
           .arg(m_timeIn.toString("hh:mm"))
           .arg(m_timeOut.isValid() ? m_timeOut.toString("hh:mm") : "");
}

AttendanceRecord AttendanceRecord::fromFileString(const QString& line)
{
    QStringList parts = line.split('|');
    if (parts.size() >= 4) {
        AttendanceRecord record;
        record.m_employeeId = parts[0].toInt();
        record.m_employeeName = parts[1];
        record.m_date = QDate::fromString(parts[2], "yyyy-MM-dd");
        record.m_timeIn = QTime::fromString(parts[3], "hh:mm");
        if (parts.size() > 4 && !parts[4].isEmpty()) {
            record.m_timeOut = QTime::fromString(parts[4], "hh:mm");
        }
        return record;
    }
    return AttendanceRecord();
}

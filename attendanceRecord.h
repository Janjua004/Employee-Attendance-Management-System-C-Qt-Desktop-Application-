#ifndef ATTENDANCERECORD_H
#define ATTENDANCERECORD_H

#include <QString>
#include <QDate>
#include <QTime>

class AttendanceRecord
{
public:
    AttendanceRecord();
    AttendanceRecord(int employeeId, const QString& employeeName, 
                     const QDate& date, const QTime& timeIn, 
                     const QTime& timeOut = QTime());

    // Getters
    int getEmployeeId() const { return m_employeeId; }
    QString getEmployeeName() const { return m_employeeName; }
    QDate getDate() const { return m_date; }
    QTime getTimeIn() const { return m_timeIn; }
    QTime getTimeOut() const { return m_timeOut; }
    bool isComplete() const { return m_timeOut.isValid(); }

    // Setters
    void setTimeOut(const QTime& timeOut) { m_timeOut = timeOut; }

    // File operations
    QString toFileString() const;
    static AttendanceRecord fromFileString(const QString& line);

private:
    int m_employeeId;
    QString m_employeeName;
    QDate m_date;
    QTime m_timeIn;
    QTime m_timeOut;
};

#endif // ATTENDANCERECORD_H

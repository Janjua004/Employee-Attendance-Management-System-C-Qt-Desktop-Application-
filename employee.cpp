#include "employee.h"
#include <QStringList>

Employee::Employee()
    : m_id(0), m_salary(0.0)
{
}

Employee::Employee(int id, const QString& name, const QString& department, 
                   double salary, const QString& position)
    : m_id(id), m_name(name), m_department(department), 
      m_salary(salary), m_position(position)
{
    // Default password is employee ID
    m_password = QString::number(id);
}

QString Employee::toFileString() const
{
    return QString("%1|%2|%3|%4|%5|%6")
           .arg(m_id)
           .arg(m_name)
           .arg(m_department)
           .arg(m_salary)
           .arg(m_position)
           .arg(m_password);
}

Employee Employee::fromFileString(const QString& line)
{
    QStringList parts = line.split('|');
    if (parts.size() >= 5) {
        Employee emp;
        emp.setId(parts[0].toInt());
        emp.setName(parts[1]);
        emp.setDepartment(parts[2]);
        emp.setSalary(parts[3].toDouble());
        emp.setPosition(parts[4]);
        if (parts.size() > 5) {
            emp.setPassword(parts[5]);
        } else {
            emp.setPassword(QString::number(emp.getId()));
        }
        return emp;
    }
    return Employee();
}

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QDate>

class Employee
{
public:
    Employee();
    Employee(int id, const QString& name, const QString& department, 
             double salary, const QString& position);

    // Getters
    int getId() const { return m_id; }
    QString getName() const { return m_name; }
    QString getDepartment() const { return m_department; }
    double getSalary() const { return m_salary; }
    QString getPosition() const { return m_position; }
    QString getPassword() const { return m_password; }

    // Setters
    void setId(int id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setDepartment(const QString& department) { m_department = department; }
    void setSalary(double salary) { m_salary = salary; }
    void setPosition(const QString& position) { m_position = position; }
    void setPassword(const QString& password) { m_password = password; }

    // File operations
    QString toFileString() const;
    static Employee fromFileString(const QString& line);

private:
    int m_id;
    QString m_name;
    QString m_department;
    double m_salary;
    QString m_position;
    QString m_password;
};

#endif // EMPLOYEE_H

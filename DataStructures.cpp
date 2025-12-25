#include "DataStructures.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

// Initialize static instance
DataManager* DataManager::instance = nullptr;

// ============================================================================
// EMPLOYEE OPERATIONS (Doubly Linked List + BST)
// ============================================================================

void DataManager::addEmployee(const Employee& emp, bool recordAction) {
    // Add to doubly linked list (maintains insertion order)
    m_employeeList.insertAtEnd(emp);
    
    // Add to BST (for fast search by ID)
    m_employeeBST->insert(emp);
    
    // Record action for undo
    if (recordAction) {
        ActionRecord action;
        action.type = ActionType::ADD_EMPLOYEE;
        action.employee = emp;
        m_undoStack.push(action);
        m_redoStack.clear(); // Clear redo stack on new action
    }
}

bool DataManager::updateEmployee(int id, const Employee& emp, bool recordAction) {
    // Find old employee for undo
    Employee* oldEmp = findEmployee(id);
    if (!oldEmp) return false;
    
    Employee previousEmp = *oldEmp;
    
    // Update in doubly linked list
    m_employeeList.update([id](const Employee& e) { return e.getId() == id; }, emp);
    
    // Update in BST
    m_employeeBST->update(id, emp);
    
    // Record action for undo
    if (recordAction) {
        ActionRecord action;
        action.type = ActionType::EDIT_EMPLOYEE;
        action.employee = emp;
        action.previousEmployee = previousEmp;
        m_undoStack.push(action);
        m_redoStack.clear();
    }
    
    return true;
}

bool DataManager::deleteEmployee(int id, bool recordAction) {
    // Find employee for undo
    Employee* emp = findEmployee(id);
    if (!emp) return false;
    
    Employee deletedEmp = *emp;
    
    // Delete from doubly linked list
    m_employeeList.deleteByValue(deletedEmp, [](const Employee& a, const Employee& b) {
        return a.getId() == b.getId();
    });
    
    // Delete from BST
    m_employeeBST->remove(id);
    
    // Record action for undo
    if (recordAction) {
        ActionRecord action;
        action.type = ActionType::DELETE_EMPLOYEE;
        action.employee = deletedEmp;
        m_undoStack.push(action);
        m_redoStack.clear();
    }
    
    return true;
}

Employee* DataManager::findEmployee(int id) {
    // Use BST for O(log n) search
    return m_employeeBST->search(id);
}

QList<Employee> DataManager::getAllEmployees() {
    // Return from doubly linked list (maintains order)
    return m_employeeList.toQList();
}

int DataManager::getEmployeeCount() {
    return m_employeeList.size();
}


// ============================================================================
// LEAVE REQUEST OPERATIONS (Queue + Doubly Linked List)
// ============================================================================

void DataManager::submitLeaveRequest(const LeaveRequest& request) {
    // Add to pending queue (FIFO processing)
    if (request.getStatus() == LeaveStatus::Pending) {
        m_pendingLeaveQueue.enqueue(request);
    }
    
    // Add to all leave requests list
    m_allLeaveRequests.insertAtEnd(request);
}

LeaveRequest* DataManager::getNextPendingRequest() {
    LeaveRequest request;
    if (m_pendingLeaveQueue.peek(request)) {
        // Find in the main list and return pointer
        return m_allLeaveRequests.find([&request](const LeaveRequest& r) {
            return r.getEmployeeId() == request.getEmployeeId() &&
                   r.getStartDate() == request.getStartDate() &&
                   r.getStatus() == LeaveStatus::Pending;
        });
    }
    return nullptr;
}

bool DataManager::processLeaveRequest(int employeeId, const QDate& startDate, 
                                       LeaveStatus newStatus, bool recordAction) {
    // Find the request in all leave requests
    LeaveRequest* request = m_allLeaveRequests.find([employeeId, &startDate](const LeaveRequest& r) {
        return r.getEmployeeId() == employeeId && r.getStartDate() == startDate;
    });
    
    if (!request) return false;
    
    LeaveStatus previousStatus = request->getStatus();
    
    // Update the status
    LeaveRequest updatedRequest = *request;
    updatedRequest.setStatus(newStatus);
    
    m_allLeaveRequests.update([employeeId, &startDate](const LeaveRequest& r) {
        return r.getEmployeeId() == employeeId && r.getStartDate() == startDate;
    }, updatedRequest);
    
    // Remove from pending queue if was pending
    if (previousStatus == LeaveStatus::Pending) {
        m_pendingLeaveQueue.removeByCondition([employeeId, &startDate](const LeaveRequest& r) {
            return r.getEmployeeId() == employeeId && r.getStartDate() == startDate;
        });
    }
    
    // Record action for undo
    if (recordAction) {
        ActionRecord action;
        action.type = (newStatus == LeaveStatus::Approved) ? 
                      ActionType::APPROVE_LEAVE : ActionType::REJECT_LEAVE;
        action.leaveRequest = updatedRequest;
        action.previousStatus = previousStatus;
        m_undoStack.push(action);
        m_redoStack.clear();
    }
    
    return true;
}

QList<LeaveRequest> DataManager::getAllLeaveRequests() {
    return m_allLeaveRequests.toQList();
}

QList<LeaveRequest> DataManager::getPendingRequests() {
    return m_pendingLeaveQueue.toQList();
}

QList<LeaveRequest> DataManager::getEmployeeLeaveRequests(int employeeId) {
    QList<LeaveRequest> result;
    m_allLeaveRequests.traverseForward([&result, employeeId](const LeaveRequest& r) {
        if (r.getEmployeeId() == employeeId) {
            result.append(r);
        }
    });
    return result;
}


// ============================================================================
// ATTENDANCE OPERATIONS (AVL Tree)
// ============================================================================

void DataManager::addAttendanceRecord(const AttendanceRecord& record) {
    m_attendanceAVL->insert(record);
}

AttendanceRecord* DataManager::findAttendanceRecord(int employeeId, const QDate& date) {
    QString key = date.toString("yyyy-MM-dd") + "_" + QString::number(employeeId);
    return m_attendanceAVL->search(key);
}

QList<AttendanceRecord> DataManager::getAttendanceByDateRange(const QDate& start, const QDate& end) {
    QList<AttendanceRecord> result;
    
    // Use AVL range search for efficient date range queries
    QString lowKey = start.toString("yyyy-MM-dd") + "_0";
    QString highKey = end.toString("yyyy-MM-dd") + "_999999";
    
    m_attendanceAVL->rangeSearch(lowKey, highKey, [&result](const AttendanceRecord& r) {
        result.append(r);
    });
    
    return result;
}

QList<AttendanceRecord> DataManager::getEmployeeAttendance(int employeeId) {
    return m_attendanceAVL->findAll([employeeId](const AttendanceRecord& r) {
        return r.getEmployeeId() == employeeId;
    });
}

bool DataManager::hasAttendanceToday(int employeeId) {
    QDate today = QDate::currentDate();
    return findAttendanceRecord(employeeId, today) != nullptr;
}


// ============================================================================
// UNDO/REDO OPERATIONS (Stack)
// ============================================================================

bool DataManager::undo() {
    ActionRecord action;
    if (!m_undoStack.pop(action)) return false;
    
    switch (action.type) {
        case ActionType::ADD_EMPLOYEE:
            // Undo add = delete
            deleteEmployee(action.employee.getId(), false);
            break;
            
        case ActionType::EDIT_EMPLOYEE:
            // Undo edit = restore previous
            updateEmployee(action.employee.getId(), action.previousEmployee, false);
            break;
            
        case ActionType::DELETE_EMPLOYEE:
            // Undo delete = add back
            addEmployee(action.employee, false);
            break;
            
        case ActionType::APPROVE_LEAVE:
        case ActionType::REJECT_LEAVE:
            // Undo status change = restore previous status
            processLeaveRequest(action.leaveRequest.getEmployeeId(),
                               action.leaveRequest.getStartDate(),
                               action.previousStatus, false);
            break;
    }
    
    // Push to redo stack
    m_redoStack.push(action);
    return true;
}

bool DataManager::redo() {
    ActionRecord action;
    if (!m_redoStack.pop(action)) return false;
    
    switch (action.type) {
        case ActionType::ADD_EMPLOYEE:
            addEmployee(action.employee, false);
            break;
            
        case ActionType::EDIT_EMPLOYEE:
            updateEmployee(action.employee.getId(), action.employee, false);
            break;
            
        case ActionType::DELETE_EMPLOYEE:
            deleteEmployee(action.employee.getId(), false);
            break;
            
        case ActionType::APPROVE_LEAVE:
            processLeaveRequest(action.leaveRequest.getEmployeeId(),
                               action.leaveRequest.getStartDate(),
                               LeaveStatus::Approved, false);
            break;
            
        case ActionType::REJECT_LEAVE:
            processLeaveRequest(action.leaveRequest.getEmployeeId(),
                               action.leaveRequest.getStartDate(),
                               LeaveStatus::Rejected, false);
            break;
    }
    
    // Push back to undo stack
    m_undoStack.push(action);
    return true;
}

bool DataManager::canUndo() {
    ActionRecord temp;
    return m_undoStack.peek(temp);
}

bool DataManager::canRedo() {
    ActionRecord temp;
    return m_redoStack.peek(temp);
}


// ============================================================================
// FILE OPERATIONS
// ============================================================================

void DataManager::loadAllData() {
    loadEmployees();
    loadLeaveRequests();
    loadAttendanceRecords();
}

void DataManager::saveAllData() {
    saveEmployees();
    saveLeaveRequests();
    saveAttendanceRecords();
}

void DataManager::loadEmployees() {
    // Clear existing data
    m_employeeList.clear();
    m_employeeBST->clear();
    
    QFile file("employees.txt");
    
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                Employee emp = Employee::fromFileString(line);
                if (emp.getId() > 0) {
                    m_employeeList.insertAtEnd(emp);
                    m_employeeBST->insert(emp);
                }
            }
        }
        file.close();
    }
}

void DataManager::saveEmployees() {
    QFile file("employees.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        
        // Traverse doubly linked list and save
        m_employeeList.traverseForward([&out](const Employee& emp) {
            out << emp.toFileString() << "\n";
        });
        
        file.close();
    }
}

void DataManager::loadLeaveRequests() {
    // Clear existing data
    m_allLeaveRequests.clear();
    m_pendingLeaveQueue.clear();
    
    QFile file("leave_requests.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                LeaveRequest request = LeaveRequest::fromFileString(line);
                if (request.getEmployeeId() > 0) {
                    m_allLeaveRequests.insertAtEnd(request);
                    
                    // Add pending requests to queue
                    if (request.getStatus() == LeaveStatus::Pending) {
                        m_pendingLeaveQueue.enqueue(request);
                    }
                }
            }
        }
        file.close();
    }
}

void DataManager::saveLeaveRequests() {
    QFile file("leave_requests.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        
        m_allLeaveRequests.traverseForward([&out](const LeaveRequest& request) {
            out << request.toFileString() << "\n";
        });
        
        file.close();
    }
}

void DataManager::loadAttendanceRecords() {
    m_attendanceAVL->clear();
    
    QFile file("attendance.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                AttendanceRecord record = AttendanceRecord::fromFileString(line);
                if (record.getEmployeeId() > 0) {
                    m_attendanceAVL->insert(record);
                }
            }
        }
        file.close();
    }
}

void DataManager::saveAttendanceRecords() {
    QFile file("attendance.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        
        m_attendanceAVL->inorderTraversal([&out](const AttendanceRecord& record) {
            out << record.toFileString() << "\n";
        });
        
        file.close();
    }
}


// ============================================================================
// STATISTICS
// ============================================================================

int DataManager::getBSTHeight() {
    return m_employeeBST->getHeight();
}

int DataManager::getAVLHeight() {
    return m_attendanceAVL->getHeight();
}

bool DataManager::isAVLBalanced() {
    return m_attendanceAVL->isBalanced();
}

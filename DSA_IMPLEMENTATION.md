# Data Structures & Algorithms Implementation

## 📊 Overview

This Employee Attendance Management System has been enhanced with custom implementations of fundamental Data Structures and Algorithms (DSA). Each data structure serves a specific purpose in the application.

---

## 🔗 1. Doubly Linked List

### Implementation: `DoublyLinkedList<T>` template class

### Used For: **Employee Management**

### Features:
- **Bidirectional Traversal**: Navigate employees forward and backward
- **O(1) Insertion**: At beginning and end
- **O(n) Search**: Linear traversal for finding elements
- **Dynamic Size**: No fixed capacity limit

### Operations:
```cpp
insertAtBeginning(T value)   // O(1)
insertAtEnd(T value)         // O(1)
insertAt(int position, T)    // O(n)
deleteFromBeginning()        // O(1)
deleteFromEnd()              // O(1)
deleteAt(int position)       // O(n)
find(condition)              // O(n)
traverseForward(action)      // O(n)
traverseBackward(action)     // O(n)
```

### Why Doubly Linked List?
- Maintains insertion order of employees
- Efficient deletion without shifting elements
- Can traverse in both directions (useful for navigation)

---

## 📚 2. Stack

### Implementation: `Stack<T>` template class

### Used For: **Undo/Redo Operations**

### Features:
- **LIFO (Last In, First Out)**: Perfect for undo functionality
- **Maximum Capacity**: Configurable limit (default: 50 operations)
- **O(1) Operations**: Push and Pop

### Operations:
```cpp
push(T value)    // O(1) - Add action to stack
pop(T& value)    // O(1) - Remove and get top action
peek(T& value)   // O(1) - View top without removing
isEmpty()        // O(1) - Check if stack is empty
size()           // O(1) - Get number of elements
```

### Usage in Application:
- **Undo Stack**: Stores previous actions (add, edit, delete employee)
- **Redo Stack**: Stores undone actions for redo functionality
- Actions stored: Employee additions, modifications, deletions, leave approvals/rejections

---

## 📥 3. Queue

### Implementation: `Queue<T>` template class

### Used For: **Leave Request Processing (FIFO)**

### Features:
- **FIFO (First In, First Out)**: Fair processing order
- **O(1) Enqueue/Dequeue**: Constant time operations
- **Dynamic Size**: No fixed limit

### Operations:
```cpp
enqueue(T value)           // O(1) - Add request to queue
dequeue(T& value)          // O(1) - Remove from front
peek(T& value)             // O(1) - View front without removing
removeByCondition(cond)    // O(n) - Remove specific element
traverse(action)           // O(n) - Process all elements
```

### Why Queue for Leave Requests?
- **Fairness**: First submitted = First processed
- **Natural workflow**: Admin processes requests in order received
- Pending requests automatically queued for processing

---

## 🌳 4. Binary Search Tree (BST)

### Implementation: `BST<T, K>` template class

### Used For: **Fast Employee Search by ID**

### Features:
- **O(log n) Search**: Much faster than linear search
- **Sorted Order**: In-order traversal gives sorted employees
- **Key-based Operations**: Search, insert, delete by employee ID

### Operations:
```cpp
insert(T value)                 // O(log n) average
search(K key)                   // O(log n) average
remove(K key)                   // O(log n) average
update(K key, T newValue)       // O(log n) average
inorderTraversal(action)        // O(n) - Sorted order
preorderTraversal(action)       // O(n)
postorderTraversal(action)      // O(n)
getMin() / getMax()             // O(log n)
getHeight()                     // O(n)
```

### Why BST for Employees?
- **Fast Lookup**: Find employee by ID in O(log n) time
- **Sorted Access**: Get employees in ID order
- **Efficient Updates**: Quick modifications

---

## ⚖️ 5. AVL Tree (Self-Balancing BST)

### Implementation: `AVLTree<T, K>` template class

### Used For: **Attendance Records by Date**

### Features:
- **Guaranteed O(log n)**: Always balanced
- **Self-Balancing**: Automatic rotations on insert/delete
- **Range Queries**: Efficient date range searches

### Operations:
```cpp
insert(T value)                     // O(log n) guaranteed
search(K key)                       // O(log n) guaranteed
remove(K key)                       // O(log n) guaranteed
rangeSearch(low, high, action)      // O(log n + k) where k = results
findAll(condition)                  // O(n)
isBalanced()                        // O(n) - Verify balance
getHeight()                         // O(1) - Stored in nodes
```

### Balancing Operations:
- **Left Rotation**: For right-heavy subtrees
- **Right Rotation**: For left-heavy subtrees
- **Left-Right Rotation**: Double rotation for LR case
- **Right-Left Rotation**: Double rotation for RL case

### Why AVL for Attendance?
- **Guaranteed Performance**: Always O(log n), even with unbalanced insertions
- **Date Range Queries**: Efficiently find attendance in date ranges
- **Many Records**: Handles large attendance data efficiently

---

## 🏗️ Architecture: DataManager Class

The `DataManager` class serves as the central data management hub:

```cpp
class DataManager {
    // Doubly Linked List for maintaining employee order
    DoublyLinkedList<Employee> m_employeeList;
    
    // BST for fast employee search by ID
    BST<Employee, int>* m_employeeBST;
    
    // Queue for pending leave requests (FIFO)
    Queue<LeaveRequest> m_pendingLeaveQueue;
    
    // Doubly Linked List for all leave requests
    DoublyLinkedList<LeaveRequest> m_allLeaveRequests;
    
    // AVL Tree for attendance (balanced by date)
    AVLTree<AttendanceRecord, QString>* m_attendanceAVL;
    
    // Stacks for undo/redo
    Stack<ActionRecord> m_undoStack;
    Stack<ActionRecord> m_redoStack;
};
```

---

## 📈 Complexity Analysis

| Operation | Data Structure | Time Complexity |
|-----------|---------------|-----------------|
| Add Employee | DLL + BST | O(1) + O(log n) |
| Find Employee | BST | O(log n) |
| Delete Employee | DLL + BST | O(n) + O(log n) |
| Submit Leave | Queue + DLL | O(1) + O(1) |
| Process Leave | Queue | O(1) |
| Mark Attendance | AVL Tree | O(log n) |
| Find Attendance | AVL Tree | O(log n) |
| Date Range Query | AVL Tree | O(log n + k) |
| Undo Action | Stack | O(1) |
| Redo Action | Stack | O(1) |

---

## 🎯 Key Benefits

1. **Efficient Search**: BST and AVL provide O(log n) lookups
2. **Fair Processing**: Queue ensures FIFO for leave requests
3. **Undo/Redo**: Stack enables action reversal
4. **Flexible Navigation**: Doubly Linked List allows bidirectional traversal
5. **Balanced Performance**: AVL Tree guarantees O(log n) even with skewed data
6. **Educational Value**: Demonstrates practical DSA applications

---

## 🔧 Usage Examples

### Adding an Employee (Uses DLL + BST)
```cpp
DataManager* dm = DataManager::getInstance();
Employee emp(1001, "John", "IT", 50000, "Developer");
dm->addEmployee(emp);  // O(1) + O(log n)
```

### Finding an Employee (Uses BST)
```cpp
Employee* emp = dm->findEmployee(1001);  // O(log n)
```

### Submitting Leave Request (Uses Queue)
```cpp
LeaveRequest request(1001, "John", startDate, endDate, "Vacation");
dm->submitLeaveRequest(request);  // Enqueued in FIFO order
```

### Marking Attendance (Uses AVL Tree)
```cpp
AttendanceRecord record(1001, "John", today, currentTime);
dm->addAttendanceRecord(record);  // O(log n) with auto-balancing
```

### Undo Last Action (Uses Stack)
```cpp
dm->undo();  // O(1) - Pops from undo stack, pushes to redo
```

---

## 📊 DSA Statistics Panel

The application includes a "DSA Stats" button that displays:
- Number of employees in Doubly Linked List
- BST height and node count
- AVL Tree height and balance status
- Queue size (pending requests)
- Stack status (can undo/redo)

This helps visualize how the data structures are being utilized in real-time.

---

## 👨‍💻 Author

Enhanced with DSA implementations for educational purposes.
Demonstrates practical applications of:
- Doubly Linked List
- Stack
- Queue
- Binary Search Tree (BST)
- AVL Tree (Self-Balancing BST)

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QString>
#include <functional>
#include "employee.h"
#include "leaveRequest.h"
#include "attendanceRecord.h"

// ============================================================================
// DOUBLY LINKED LIST - For Employee Management
// ============================================================================
template<typename T>
class DoublyLinkedList {
public:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

private:
    Node* head;
    Node* tail;
    int count;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    
    ~DoublyLinkedList() {
        clear();
    }
    
    // Copy constructor
    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node* current = other.head;
        while (current) {
            insertAtEnd(current->data);
            current = current->next;
        }
    }
    
    // Assignment operator
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current) {
                insertAtEnd(current->data);
                current = current->next;
            }
        }
        return *this;
    }
    
    // Insert at beginning
    void insertAtBeginning(const T& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }
    
    // Insert at end
    void insertAtEnd(const T& value) {
        Node* newNode = new Node(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }
    
    // Insert at position
    void insertAt(int position, const T& value) {
        if (position <= 0) {
            insertAtBeginning(value);
            return;
        }
        if (position >= count) {
            insertAtEnd(value);
            return;
        }
        
        Node* current = head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }
        
        Node* newNode = new Node(value);
        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;
        count++;
    }
    
    // Delete from beginning
    bool deleteFromBeginning() {
        if (!head) return false;
        
        Node* temp = head;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
        count--;
        return true;
    }
    
    // Delete from end
    bool deleteFromEnd() {
        if (!tail) return false;
        
        Node* temp = tail;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete temp;
        count--;
        return true;
    }
    
    // Delete at position
    bool deleteAt(int position) {
        if (position < 0 || position >= count) return false;
        if (position == 0) return deleteFromBeginning();
        if (position == count - 1) return deleteFromEnd();
        
        Node* current = head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
        
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
        count--;
        return true;
    }
    
    // Delete by value using comparator
    bool deleteByValue(const T& value, std::function<bool(const T&, const T&)> compare) {
        Node* current = head;
        int position = 0;
        while (current) {
            if (compare(current->data, value)) {
                return deleteAt(position);
            }
            current = current->next;
            position++;
        }
        return false;
    }
    
    // Find by condition
    T* find(std::function<bool(const T&)> condition) {
        Node* current = head;
        while (current) {
            if (condition(current->data)) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }
    
    // Update element
    bool update(std::function<bool(const T&)> condition, const T& newValue) {
        Node* current = head;
        while (current) {
            if (condition(current->data)) {
                current->data = newValue;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Get at position
    T* getAt(int position) {
        if (position < 0 || position >= count) return nullptr;
        
        Node* current = head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
        return &(current->data);
    }
    
    // Forward traversal
    void traverseForward(std::function<void(const T&)> action) const {
        Node* current = head;
        while (current) {
            action(current->data);
            current = current->next;
        }
    }
    
    // Backward traversal
    void traverseBackward(std::function<void(const T&)> action) const {
        Node* current = tail;
        while (current) {
            action(current->data);
            current = current->prev;
        }
    }
    
    // Get size
    int size() const { return count; }
    
    // Check if empty
    bool isEmpty() const { return count == 0; }
    
    // Clear list
    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }
    
    // Get head
    Node* getHead() const { return head; }
    
    // Get tail
    Node* getTail() const { return tail; }
    
    // Convert to QList for compatibility
    QList<T> toQList() const {
        QList<T> list;
        Node* current = head;
        while (current) {
            list.append(current->data);
            current = current->next;
        }
        return list;
    }
};


// ============================================================================
// STACK - For Undo/Redo Operations
// ============================================================================
template<typename T>
class Stack {
public:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& value) : data(value), next(nullptr) {}
    };

private:
    Node* topNode;
    int count;
    int maxSize;

public:
    Stack(int maxCapacity = 50) : topNode(nullptr), count(0), maxSize(maxCapacity) {}
    
    ~Stack() {
        clear();
    }
    
    // Push element
    void push(const T& value) {
        // If stack is full, remove bottom element
        if (count >= maxSize) {
            removeBottom();
        }
        
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }
    
    // Pop element
    bool pop(T& value) {
        if (!topNode) return false;
        
        value = topNode->data;
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        count--;
        return true;
    }
    
    // Peek top element
    bool peek(T& value) const {
        if (!topNode) return false;
        value = topNode->data;
        return true;
    }
    
    // Check if empty
    bool isEmpty() const { return count == 0; }
    
    // Get size
    int size() const { return count; }
    
    // Clear stack
    void clear() {
        while (topNode) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
        count = 0;
    }

private:
    void removeBottom() {
        if (!topNode) return;
        if (!topNode->next) {
            delete topNode;
            topNode = nullptr;
            count = 0;
            return;
        }
        
        Node* current = topNode;
        while (current->next->next) {
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
        count--;
    }
};


// ============================================================================
// QUEUE - For Leave Request Processing (FIFO)
// ============================================================================
template<typename T>
class Queue {
public:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& value) : data(value), next(nullptr) {}
    };

private:
    Node* front;
    Node* rear;
    int count;

public:
    Queue() : front(nullptr), rear(nullptr), count(0) {}
    
    ~Queue() {
        clear();
    }
    
    // Enqueue element
    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }
    
    // Dequeue element
    bool dequeue(T& value) {
        if (!front) return false;
        
        value = front->data;
        Node* temp = front;
        front = front->next;
        if (!front) {
            rear = nullptr;
        }
        delete temp;
        count--;
        return true;
    }
    
    // Peek front element
    bool peek(T& value) const {
        if (!front) return false;
        value = front->data;
        return true;
    }
    
    // Check if empty
    bool isEmpty() const { return count == 0; }
    
    // Get size
    int size() const { return count; }
    
    // Clear queue
    void clear() {
        while (front) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
        count = 0;
    }
    
    // Get front node for iteration
    Node* getFront() const { return front; }
    
    // Traverse queue
    void traverse(std::function<void(const T&)> action) const {
        Node* current = front;
        while (current) {
            action(current->data);
            current = current->next;
        }
    }
    
    // Convert to QList
    QList<T> toQList() const {
        QList<T> list;
        Node* current = front;
        while (current) {
            list.append(current->data);
            current = current->next;
        }
        return list;
    }
    
    // Remove by condition
    bool removeByCondition(std::function<bool(const T&)> condition) {
        if (!front) return false;
        
        // Check front
        if (condition(front->data)) {
            Node* temp = front;
            front = front->next;
            if (!front) rear = nullptr;
            delete temp;
            count--;
            return true;
        }
        
        // Check rest
        Node* current = front;
        while (current->next) {
            if (condition(current->next->data)) {
                Node* temp = current->next;
                current->next = temp->next;
                if (temp == rear) rear = current;
                delete temp;
                count--;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Update by condition
    bool updateByCondition(std::function<bool(const T&)> condition, const T& newValue) {
        Node* current = front;
        while (current) {
            if (condition(current->data)) {
                current->data = newValue;
                return true;
            }
            current = current->next;
        }
        return false;
    }
};


// ============================================================================
// BINARY SEARCH TREE (BST) - For Employee Search by ID
// ============================================================================
template<typename T, typename K>
class BST {
public:
    struct Node {
        T data;
        K key;
        Node* left;
        Node* right;
        
        Node(const T& value, const K& k) : data(value), key(k), left(nullptr), right(nullptr) {}
    };

private:
    Node* root;
    int count;
    std::function<K(const T&)> keyExtractor;

public:
    BST(std::function<K(const T&)> extractor) : root(nullptr), count(0), keyExtractor(extractor) {}
    
    ~BST() {
        clear();
    }
    
    // Insert element
    void insert(const T& value) {
        K key = keyExtractor(value);
        root = insertRecursive(root, value, key);
        count++;
    }
    
    // Search by key
    T* search(const K& key) {
        Node* result = searchRecursive(root, key);
        return result ? &(result->data) : nullptr;
    }
    
    // Delete by key
    bool remove(const K& key) {
        if (!search(key)) return false;
        root = deleteRecursive(root, key);
        count--;
        return true;
    }
    
    // Update element
    bool update(const K& key, const T& newValue) {
        Node* node = searchRecursive(root, key);
        if (node) {
            node->data = newValue;
            return true;
        }
        return false;
    }
    
    // In-order traversal (sorted order)
    void inorderTraversal(std::function<void(const T&)> action) const {
        inorderRecursive(root, action);
    }
    
    // Pre-order traversal
    void preorderTraversal(std::function<void(const T&)> action) const {
        preorderRecursive(root, action);
    }
    
    // Post-order traversal
    void postorderTraversal(std::function<void(const T&)> action) const {
        postorderRecursive(root, action);
    }
    
    // Get size
    int size() const { return count; }
    
    // Check if empty
    bool isEmpty() const { return count == 0; }
    
    // Get minimum
    T* getMin() {
        if (!root) return nullptr;
        Node* current = root;
        while (current->left) {
            current = current->left;
        }
        return &(current->data);
    }
    
    // Get maximum
    T* getMax() {
        if (!root) return nullptr;
        Node* current = root;
        while (current->right) {
            current = current->right;
        }
        return &(current->data);
    }
    
    // Get height
    int getHeight() const {
        return heightRecursive(root);
    }
    
    // Clear tree
    void clear() {
        clearRecursive(root);
        root = nullptr;
        count = 0;
    }
    
    // Convert to QList (in-order)
    QList<T> toQList() const {
        QList<T> list;
        inorderTraversal([&list](const T& item) {
            list.append(item);
        });
        return list;
    }

private:
    Node* insertRecursive(Node* node, const T& value, const K& key) {
        if (!node) {
            return new Node(value, key);
        }
        
        if (key < node->key) {
            node->left = insertRecursive(node->left, value, key);
        } else if (key > node->key) {
            node->right = insertRecursive(node->right, value, key);
        } else {
            // Key exists, update value
            node->data = value;
            count--; // Compensate for the increment in insert()
        }
        
        return node;
    }
    
    Node* searchRecursive(Node* node, const K& key) const {
        if (!node || node->key == key) {
            return node;
        }
        
        if (key < node->key) {
            return searchRecursive(node->left, key);
        }
        return searchRecursive(node->right, key);
    }
    
    Node* deleteRecursive(Node* node, const K& key) {
        if (!node) return nullptr;
        
        if (key < node->key) {
            node->left = deleteRecursive(node->left, key);
        } else if (key > node->key) {
            node->right = deleteRecursive(node->right, key);
        } else {
            // Node to delete found
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            
            // Node has two children
            Node* minRight = node->right;
            while (minRight->left) {
                minRight = minRight->left;
            }
            
            node->data = minRight->data;
            node->key = minRight->key;
            node->right = deleteRecursive(node->right, minRight->key);
        }
        
        return node;
    }
    
    void inorderRecursive(Node* node, std::function<void(const T&)> action) const {
        if (!node) return;
        inorderRecursive(node->left, action);
        action(node->data);
        inorderRecursive(node->right, action);
    }
    
    void preorderRecursive(Node* node, std::function<void(const T&)> action) const {
        if (!node) return;
        action(node->data);
        preorderRecursive(node->left, action);
        preorderRecursive(node->right, action);
    }
    
    void postorderRecursive(Node* node, std::function<void(const T&)> action) const {
        if (!node) return;
        postorderRecursive(node->left, action);
        postorderRecursive(node->right, action);
        action(node->data);
    }
    
    int heightRecursive(Node* node) const {
        if (!node) return 0;
        int leftHeight = heightRecursive(node->left);
        int rightHeight = heightRecursive(node->right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }
    
    void clearRecursive(Node* node) {
        if (!node) return;
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }
};


// ============================================================================
// AVL TREE - Self-Balancing BST for Attendance Records by Date
// ============================================================================
template<typename T, typename K>
class AVLTree {
public:
    struct Node {
        T data;
        K key;
        Node* left;
        Node* right;
        int height;
        
        Node(const T& value, const K& k) 
            : data(value), key(k), left(nullptr), right(nullptr), height(1) {}
    };

private:
    Node* root;
    int count;
    std::function<K(const T&)> keyExtractor;

public:
    AVLTree(std::function<K(const T&)> extractor) : root(nullptr), count(0), keyExtractor(extractor) {}
    
    ~AVLTree() {
        clear();
    }
    
    // Insert element with auto-balancing
    void insert(const T& value) {
        K key = keyExtractor(value);
        root = insertRecursive(root, value, key);
        count++;
    }
    
    // Search by key
    T* search(const K& key) {
        Node* result = searchRecursive(root, key);
        return result ? &(result->data) : nullptr;
    }
    
    // Delete by key with auto-balancing
    bool remove(const K& key) {
        if (!search(key)) return false;
        root = deleteRecursive(root, key);
        count--;
        return true;
    }
    
    // Range search (useful for date ranges)
    void rangeSearch(const K& low, const K& high, std::function<void(const T&)> action) const {
        rangeSearchRecursive(root, low, high, action);
    }
    
    // In-order traversal (sorted by key)
    void inorderTraversal(std::function<void(const T&)> action) const {
        inorderRecursive(root, action);
    }
    
    // Get size
    int size() const { return count; }
    
    // Check if empty
    bool isEmpty() const { return count == 0; }
    
    // Get tree height
    int getHeight() const {
        return root ? root->height : 0;
    }
    
    // Check if balanced
    bool isBalanced() const {
        return isBalancedRecursive(root);
    }
    
    // Clear tree
    void clear() {
        clearRecursive(root);
        root = nullptr;
        count = 0;
    }
    
    // Convert to QList
    QList<T> toQList() const {
        QList<T> list;
        inorderTraversal([&list](const T& item) {
            list.append(item);
        });
        return list;
    }
    
    // Find all matching a condition
    QList<T> findAll(std::function<bool(const T&)> condition) const {
        QList<T> results;
        inorderTraversal([&results, &condition](const T& item) {
            if (condition(item)) {
                results.append(item);
            }
        });
        return results;
    }

private:
    int getNodeHeight(Node* node) const {
        return node ? node->height : 0;
    }
    
    int getBalance(Node* node) const {
        return node ? getNodeHeight(node->left) - getNodeHeight(node->right) : 0;
    }
    
    void updateHeight(Node* node) {
        if (node) {
            int leftHeight = getNodeHeight(node->left);
            int rightHeight = getNodeHeight(node->right);
            node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
        }
    }
    
    // Right rotation
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }
    
    // Left rotation
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }
    
    Node* insertRecursive(Node* node, const T& value, const K& key) {
        // Standard BST insert
        if (!node) {
            return new Node(value, key);
        }
        
        if (key < node->key) {
            node->left = insertRecursive(node->left, value, key);
        } else if (key > node->key) {
            node->right = insertRecursive(node->right, value, key);
        } else {
            // Duplicate key - update or add to list
            node->data = value;
            count--;
            return node;
        }
        
        // Update height
        updateHeight(node);
        
        // Get balance factor
        int balance = getBalance(node);
        
        // Left Left Case
        if (balance > 1 && key < node->left->key) {
            return rightRotate(node);
        }
        
        // Right Right Case
        if (balance < -1 && key > node->right->key) {
            return leftRotate(node);
        }
        
        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    Node* searchRecursive(Node* node, const K& key) const {
        if (!node || node->key == key) {
            return node;
        }
        
        if (key < node->key) {
            return searchRecursive(node->left, key);
        }
        return searchRecursive(node->right, key);
    }
    
    Node* getMinNode(Node* node) const {
        Node* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }
    
    Node* deleteRecursive(Node* node, const K& key) {
        if (!node) return nullptr;
        
        if (key < node->key) {
            node->left = deleteRecursive(node->left, key);
        } else if (key > node->key) {
            node->right = deleteRecursive(node->right, key);
        } else {
            // Node to delete found
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                Node* temp = getMinNode(node->right);
                node->key = temp->key;
                node->data = temp->data;
                node->right = deleteRecursive(node->right, temp->key);
            }
        }
        
        if (!node) return nullptr;
        
        // Update height
        updateHeight(node);
        
        // Balance the tree
        int balance = getBalance(node);
        
        // Left Left Case
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rightRotate(node);
        }
        
        // Left Right Case
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        // Right Right Case
        if (balance < -1 && getBalance(node->right) <= 0) {
            return leftRotate(node);
        }
        
        // Right Left Case
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    void rangeSearchRecursive(Node* node, const K& low, const K& high, 
                              std::function<void(const T&)> action) const {
        if (!node) return;
        
        if (low < node->key) {
            rangeSearchRecursive(node->left, low, high, action);
        }
        
        if (low <= node->key && node->key <= high) {
            action(node->data);
        }
        
        if (high > node->key) {
            rangeSearchRecursive(node->right, low, high, action);
        }
    }
    
    void inorderRecursive(Node* node, std::function<void(const T&)> action) const {
        if (!node) return;
        inorderRecursive(node->left, action);
        action(node->data);
        inorderRecursive(node->right, action);
    }
    
    bool isBalancedRecursive(Node* node) const {
        if (!node) return true;
        
        int balance = getBalance(node);
        if (balance < -1 || balance > 1) return false;
        
        return isBalancedRecursive(node->left) && isBalancedRecursive(node->right);
    }
    
    void clearRecursive(Node* node) {
        if (!node) return;
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }
};


// ============================================================================
// ACTION RECORD - For Undo/Redo Stack
// ============================================================================
enum class ActionType {
    ADD_EMPLOYEE,
    EDIT_EMPLOYEE,
    DELETE_EMPLOYEE,
    APPROVE_LEAVE,
    REJECT_LEAVE
};

struct ActionRecord {
    ActionType type;
    Employee employee;          // For employee actions
    Employee previousEmployee;  // For edit operations (to restore)
    LeaveRequest leaveRequest; // For leave actions
    LeaveStatus previousStatus; // For leave status changes
    
    ActionRecord() : type(ActionType::ADD_EMPLOYEE), previousStatus(LeaveStatus::Pending) {}
};


// ============================================================================
// DATA MANAGER - Central Data Management with DSA
// ============================================================================
class DataManager {
private:
    // Doubly Linked List for Employee Management
    DoublyLinkedList<Employee> m_employeeList;
    
    // BST for fast employee search by ID
    BST<Employee, int>* m_employeeBST;
    
    // Queue for Leave Request Processing
    Queue<LeaveRequest> m_pendingLeaveQueue;
    
    // Doubly Linked List for all leave requests
    DoublyLinkedList<LeaveRequest> m_allLeaveRequests;
    
    // AVL Tree for Attendance Records (by date)
    AVLTree<AttendanceRecord, QString>* m_attendanceAVL;  // Key: date_employeeId
    
    // Stack for Undo operations
    Stack<ActionRecord> m_undoStack;
    
    // Stack for Redo operations
    Stack<ActionRecord> m_redoStack;
    
    static DataManager* instance;
    
    DataManager() {
        m_employeeBST = new BST<Employee, int>([](const Employee& e) { return e.getId(); });
        m_attendanceAVL = new AVLTree<AttendanceRecord, QString>([](const AttendanceRecord& r) {
            return r.getDate().toString("yyyy-MM-dd") + "_" + QString::number(r.getEmployeeId());
        });
    }

public:
    static DataManager* getInstance() {
        if (!instance) {
            instance = new DataManager();
        }
        return instance;
    }
    
    ~DataManager() {
        delete m_employeeBST;
        delete m_attendanceAVL;
    }
    
    // Employee Operations using Doubly Linked List + BST
    void addEmployee(const Employee& emp, bool recordAction = true);
    bool updateEmployee(int id, const Employee& emp, bool recordAction = true);
    bool deleteEmployee(int id, bool recordAction = true);
    Employee* findEmployee(int id);
    QList<Employee> getAllEmployees();
    int getEmployeeCount();
    
    // Leave Request Operations using Queue
    void submitLeaveRequest(const LeaveRequest& request);
    LeaveRequest* getNextPendingRequest();
    bool processLeaveRequest(int employeeId, const QDate& startDate, LeaveStatus newStatus, bool recordAction = true);
    QList<LeaveRequest> getAllLeaveRequests();
    QList<LeaveRequest> getPendingRequests();
    QList<LeaveRequest> getEmployeeLeaveRequests(int employeeId);
    
    // Attendance Operations using AVL Tree
    void addAttendanceRecord(const AttendanceRecord& record);
    AttendanceRecord* findAttendanceRecord(int employeeId, const QDate& date);
    QList<AttendanceRecord> getAttendanceByDateRange(const QDate& start, const QDate& end);
    QList<AttendanceRecord> getEmployeeAttendance(int employeeId);
    bool hasAttendanceToday(int employeeId);
    
    // Undo/Redo using Stack
    bool undo();
    bool redo();
    bool canUndo();
    bool canRedo();
    
    // File Operations
    void loadAllData();
    void saveAllData();
    void loadEmployees();
    void saveEmployees();
    void loadLeaveRequests();
    void saveLeaveRequests();
    void loadAttendanceRecords();
    void saveAttendanceRecords();
    
    // Statistics
    int getBSTHeight();
    int getAVLHeight();
    bool isAVLBalanced();
};

#endif // DATASTRUCTURES_H

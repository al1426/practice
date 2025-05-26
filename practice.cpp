#include <iostream>     
#include <stdexcept>    
#include <string>       
#include <cstddef>      
#include <vector>      
#include <stack>        
#include <queue>        
#include <algorithm>    
#include <cctype>       
#include <utility> 

void print_task_step(int task_num, const std::string& description) {
    std::cout << "\n// --- Задание " << task_num << ": " << description << " --- //\n";
}

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& val, Node* p = nullptr, Node* n = nullptr)
            : data(val), prev(p), next(n) {}
    };

    Node* head;
    Node* tail;
    size_t count;

    void clear_nodes() {
        Node* current = head;
        while (current != nullptr) {
            Node* next_node = current->next;
            delete current;
            current = next_node;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~DoublyLinkedList() {
        clear_nodes();
    }

    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node* current_other = other.head;
        while (current_other != nullptr) {
            push_back(current_other->data);
            current_other = current_other->next;
        }
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this == &other) {
            return *this;
        }
        
        clear_nodes();
        Node* current_other = other.head;
        while (current_other != nullptr) {
            push_back(current_other->data); 
            current_other = current_other->next;
        }
        return *this;
    }

    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head(other.head), tail(other.tail), count(other.count) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        clear_nodes(); // Release own resources

        head = other.head;
        tail = other.tail;
        count = other.count;

        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
        return *this;
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (isEmpty()) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        count++;
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value);
        if (isEmpty()) {
            head = tail = new_node;
        } else {
            head->prev = new_node;
            new_node->next = head;
            head = new_node;
        }
        count++;
    }

    T pop_back() {
        if (isEmpty()) {
            throw std::out_of_range("Cannot pop_back from an empty list.");
        }
        Node* node_to_delete = tail;
        T value = node_to_delete->data;

        if (head == tail) { 
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete node_to_delete;
        count--;
        return value;
    }

    T pop_front() {
        if (isEmpty()) {
            throw std::out_of_range("Cannot pop_front from an empty list.");
        }
        Node* node_to_delete = head;
        T value = node_to_delete->data;

        if (head == tail) { // List has only one element
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        delete node_to_delete;
        count--;
        return value;
    }

    const T& front() const {
        if (isEmpty()) {
            throw std::out_of_range("Cannot get front from an empty list.");
        }
        return head->data;
    }
     T& front() {
        if (isEmpty()) {
            throw std::out_of_range("Cannot get front from an empty list.");
        }
        return head->data;
    }

    const T& back() const {
        if (isEmpty()) {
            throw std::out_of_range("Cannot get back from an empty list.");
        }
        return tail->data;
    }
    T& back() {
        if (isEmpty()) {
            throw std::out_of_range("Cannot get back from an empty list.");
        }
        return tail->data;
    }

    bool isEmpty() const noexcept { 
        return count == 0;
    }

    size_t getSize() const noexcept { 
        return count;
    }

    void print() const {
        Node* current = head;
        std::cout << "List (size " << count << "): ";
        if (isEmpty()) {
            std::cout << "empty";
        }
        while (current != nullptr) {
            std::cout << current->data << (current->next ? " <-> " : "");
            current = current->next;
        }
        std::cout << std::endl;
    }
};

template <typename T>
class IContainer {
public:
    virtual ~IContainer() {}
    virtual void addElement(const T& value) = 0;
    virtual T removeElement() = 0;
    virtual bool isEmpty() const noexcept = 0; // Added noexcept
};

template <typename T>
class Stack : public IContainer<T> {
private:
    DoublyLinkedList<T> list;

public:
    Stack() = default;

    void addElement(const T& value) override {
        list.push_back(value); 
    }

    T removeElement() override { 
        if (list.isEmpty()) {
            throw std::logic_error("Stack is empty, cannot pop (removeElement).");
        }
        return list.pop_back();
    }

    bool isEmpty() const noexcept override { // Added noexcept
        return list.isEmpty();
    }

    const T& top() const { 
        if (list.isEmpty()) {
            throw std::logic_error("Stack is empty, cannot peek top.");
        }
        return list.back();
    }
     T& top() { 
        if (list.isEmpty()) {
            throw std::logic_error("Stack is empty, cannot peek top.");
        }
        return list.back();
    }

    size_t getSize() const noexcept { 
        return list.getSize();
    }
};

// Задача 2  В текстовом файле записана формула. За один проход определите корректность расстановки скобок...
bool areBracketsBalanced(const std::string& formula) {
    std::stack<char> s;
    for (char ch : formula) {
        if (ch == '(' || ch == '{' || ch == '[' || ch == '<') {
            s.push(ch);
        } else if (ch == ')') {
            if (s.empty() || s.top() != '(') return false;
            s.pop();
        } else if (ch == '}') {
            if (s.empty() || s.top() != '{') return false;
            s.pop();
        } else if (ch == ']') {
            if (s.empty() || s.top() != '[') return false;
            s.pop();
        } else if (ch == '>') {
            if (s.empty() || s.top() != '<') return false;
            s.pop();
        }
    }
    return s.empty();
}
int main() {
    // --- Демонстрация пользовательских структур данных ---
    print_task_step(1, "Демонстрация DoublyLinkedList");
    DoublyLinkedList<int> myList;
    myList.push_back(10);
    myList.push_back(20);
    myList.push_front(5);
    myList.print(); 

    std::cout << "Извлечено из начала: " << myList.pop_front() << std::endl; 
    myList.print(); 

    std::cout << "Извлечено с конца: " << myList.pop_back() << std::endl; 
    myList.print(); 

    std::cout << "Список пуст? " << (myList.isEmpty() ? "Да" : "Нет") << std::endl; 
    std::cout << "Извлечено с конца: " << myList.pop_back() << std::endl; 
    std::cout << "Список пуст после последнего извлечения? " << (myList.isEmpty() ? "Да" : "Нет") << std::endl; 

    DoublyLinkedList<std::string> list1;
    list1.push_back("альфа");
    list1.push_back("бета");

    DoublyLinkedList<std::string> list2 = list1; 
    std::cout << "List2 (скопирован из list1): ";
    list2.print(); 

    DoublyLinkedList<std::string> list3;
    list3.push_back("гамма");
    list3 = list1; 
    std::cout << "List3 (присвоен из list1): ";
    list3.print(); 

    DoublyLinkedList<int> list_move_src;
    list_move_src.push_back(100);
    list_move_src.push_back(200);
    std::cout << "Исходный list_move_src: ";
    list_move_src.print(); 

    DoublyLinkedList<int> list_move_dest_ctor = std::move(list_move_src); 
    std::cout << "list_move_dest_ctor (после конструктора перемещения): ";
    list_move_dest_ctor.print(); 
    std::cout << "list_move_src (после конструктора перемещения, должен быть пуст): ";
    list_move_src.print(); 

    list_move_src.push_back(300); 
    list_move_src.push_back(400);
    std::cout << "Восстановленный list_move_src: ";
    list_move_src.print(); 

    DoublyLinkedList<int> list_move_dest_assign;
    list_move_dest_assign.push_back(50); 
    std::cout << "list_move_dest_assign (до оператора присваивания перемещением): ";
    list_move_dest_assign.print(); 
    list_move_dest_assign = std::move(list_move_src); 
    std::cout << "list_move_dest_assign (после оператора присваивания перемещением): ";
    list_move_dest_assign.print(); 
    std::cout << "list_move_src (после оператора присваивания перемещением, должен быть пуст): ";
    list_move_src.print(); 

    IContainer<std::string>* container = new Stack<std::string>();
    container->addElement("Привет");
    container->addElement("Мир");
    std::cout << "Контейнер (реализованный как Стек) пуст? " << (container->isEmpty() ? "Да" : "Нет") << std::endl; 
    std::cout << "Удалено из контейнера: " << container->removeElement() << std::endl; 
    std::cout << "Удалено из контейнера: " << container->removeElement() << std::endl; 
    delete container;

    print_task_step(2, "Задача: Проверка корректности расстановки скобок"); 
    std::string f1 = "({<[]>})";
    std::cout << "Формула: " << f1 << " - Корректно: " << (areBracketsBalanced(f1) ? "Да" : "Нет") << std::endl; 
    std::string f2 = "([)]";
    std::cout << "Формула: " << f2 << " - Корректно: " << (areBracketsBalanced(f2) ? "Да" : "Нет") << std::endl; 
    std::string f3 = "{()";
    std::cout << "Формула: " << f3 << " - Корректно: " << (areBracketsBalanced(f3) ? "Да" : "Нет") << std::endl; 
    std::string f4 = "())({}{()})";
    std::cout << "Формула: " << f4 << " - Корректно: " << (areBracketsBalanced(f4) ? "Да" : "Нет") << std::endl; 
    std::string f5 = "{<O{}>}"; 
    std::cout << "Формула: " << f5 << " - Корректно: " << (areBracketsBalanced(f5) ? "Да" : "Нет") << std::endl; 
    std::string f6 = "()({<O{}>})";
    std::cout << "Формула: " << f6 << " - Корректно: " << (areBracketsBalanced(f6) ? "Да" : "Нет") << std::endl; 
    std::string f7 = "()({(O{})})";
    std::cout << "Формула: " << f7 << " - Корректно: " << (areBracketsBalanced(f7) ? "Да" : "Нет") << std::endl; 
    std::string f8 = "()({(O{)} })";
    std::cout << "Формула: " << f8 << " - Корректно: " << (areBracketsBalanced(f8) ? "Да" : "Нет") << std::endl; 
    std::string f9 = "([{}])";
    std::cout << "Формула: " << f9 << " - Корректно: " << (areBracketsBalanced(f9) ? "Да" : "Нет") << std::endl; 
    std::string f10 = "";
    std::cout << "Формула: " << f10 << " - Корректно: " << (areBracketsBalanced(f10) ? "Да" : "Нет") << std::endl; 
    std::string f11 = ")";
    std::cout << "Формула: " << f11 << " - Корректно: " << (areBracketsBalanced(f11) ? "Да" : "Нет") << std::endl; 



    // --- Обоснование выбора стандартных контейнеров ---
    std::cout << "Для решения прикладных задач были использованы стандартные контейнеры C++:" << std::endl;
    std::cout << "   Использовался std::stack. Стек идеально подходит для этой задачи благодаря своей LIFO (Last-In, First-Out) природе." << std::endl;
    std::cout << "   - Для скобок: последняя открывающая скобка должна быть первой закрыта соответствующей закрывающей." << std::endl;

    return 0;
}
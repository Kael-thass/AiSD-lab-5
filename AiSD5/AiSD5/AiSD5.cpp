#include <iostream>
#include <stdexcept>
#include <locale>
#include <string>
//7 вариант средний уровень

template <typename T> //стек на массиве
class Stack {
private:
    T* data;
    int topIndex;
    int capacity;

    void resize() { //расширение
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i <= topIndex; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Stack(int initialCapacity = 10) : capacity(initialCapacity), topIndex(-1) { //конструктор
        data = new T[capacity];
    }

    ~Stack() { delete[] data; } //деструктор

    void push(const T& value) { //добавление
        if (topIndex >= capacity - 1) resize();
        data[++topIndex] = value;
    }

    T pop() { //удаление
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return data[topIndex--];
    }

    T top() const { //просмотр верхнего
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return data[topIndex];
    }

    bool isEmpty() const { return topIndex == -1; }
    int size() const { return topIndex + 1; }

    void removeEven() { //задание: удалить чётные
        Stack<T> temp1, temp2;
        while (!isEmpty()) temp1.push(pop());
        while (!temp1.isEmpty()) {
            T val = temp1.pop();
            if (val % 2 != 0) temp2.push(val);
        }
        while (!temp2.isEmpty()) push(temp2.pop());
    }
};

template <typename T> //очередь на кольцевом буфере
class Queue {
private:
    T* data;
    int frontIndex;
    int rearIndex;
    int count;
    int capacity;

    void resize() { //расширение
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; i++) newData[i] = data[(frontIndex + i) % capacity];
        delete[] data;
        data = newData;
        frontIndex = 0;
        rearIndex = count;
        capacity = newCapacity;
    }

public:
    Queue(int initialCapacity = 10) : capacity(initialCapacity), frontIndex(0), rearIndex(0), count(0) { //конструктор
        data = new T[capacity];
    }

    ~Queue() { delete[] data; } //деструктор

    void enqueue(const T& value) { //добавление
        if (count >= capacity) resize();
        data[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity;
        count++;
    }

    T dequeue() { //удаление
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        T value = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return value;
    }

    T front() const { //просмотр первого
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        return data[frontIndex];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    void sort() { //задание: сортировка
        Queue<T> q2;
        while (!isEmpty()) {
            T minVal = front();
            int minIdx = 0;
            int n = count;
            for (int i = 0; i < n; i++) { //поиск минимума
                T val = dequeue();
                if (val < minVal) { minVal = val; minIdx = i; }
                enqueue(val);
            }
            for (int i = 0; i < n; i++) { //удаление минимума
                T val = dequeue();
                if (i != minIdx) enqueue(val);
            }
            q2.enqueue(minVal);
        }
        while (!q2.isEmpty()) enqueue(q2.dequeue());
    }
};

template <typename T> //дек на двусвязном списке
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int count;

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {} //конструктор

    ~Deque() { //деструктор
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_front(const T& value) { //добавление в начало
        Node* newNode = new Node(value);
        if (head == nullptr) head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    void push_back(const T& value) { //добавление в конец
        Node* newNode = new Node(value);
        if (tail == nullptr) head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void pop_front() { //удаление из начала
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        Node* temp = head;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        count--;
    }

    void pop_back() { //удаление из конца
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        Node* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        count--;
    }

    T front() const {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        return head->data;
    }

    T back() const {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        return tail->data;
    }

    void print() const { //вывод
        Node* current = head;
        std::cout << "[";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) std::cout << ", ";
            current = current->next;
        }
        std::cout << "]" << std::endl;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    void removeEven() { //задание: удалить чётные
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            if (current->data % 2 == 0) {
                if (current == head) pop_front();
                else if (current == tail) pop_back();
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    delete current;
                    count--;
                }
            }
            current = nextNode;
        }
    }
};

struct Task { //структура для планировщика
    std::string name;
    int priority;
    bool operator>(const Task& other) const { return priority > other.priority; }
};

template <typename T> //приоритетная очередь на куче
class PriorityQueue {
private:
    T* heap;
    int count;
    int capacity;

    void resize() { //расширение
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity];
        for (int i = 0; i < count; i++) newHeap[i] = heap[i];
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    void heapifyUp(int index) { //просеивание вверх
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] > heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            }
            else break;
        }
    }

    void heapifyDown(int index) { //просеивание вниз
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;
            if (left < count && heap[left] > heap[largest]) largest = left;
            if (right < count && heap[right] > heap[largest]) largest = right;
            if (largest == index) break;
            std::swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue(int initialCapacity = 10) : capacity(initialCapacity), count(0) { //конструктор
        heap = new T[capacity];
    }

    ~PriorityQueue() { delete[] heap; } //деструктор

    void push(const T& value) { //добавление
        if (count >= capacity) resize();
        heap[count] = value;
        heapifyUp(count);
        count++;
    }

    T pop() { //удаление
        if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
        T maxValue = heap[0];
        heap[0] = heap[count - 1];
        count--;
        heapifyDown(0);
        return maxValue;
    }

    T top() const { //просмотр максимального
        if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
        return heap[0];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

using namespace std; //разрешаем std в main

int main() {
    setlocale(LC_ALL, "ru");

    // 1. Стек: удалить чётные
    cout << "== Стек (удаление чётных) ==" << endl;
    Stack<int> stack;
    for (int i = 1; i <= 6; i++) stack.push(i);
    cout << "Исходный: 1 2 3 4 5 6" << endl;
    stack.removeEven();
    cout << "Результат: ";
    while (!stack.isEmpty()) cout << stack.pop() << " ";
    cout << endl << endl;

    // 2. Очередь: сортировка
    cout << "== Очередь (сортировка) ==" << endl;
    Queue<int> queue;
    queue.enqueue(30);
    queue.enqueue(10);
    queue.enqueue(20);
    cout << "Исходная: 30 10 20" << endl;
    queue.sort();
    cout << "Результат: ";
    while (!queue.isEmpty()) cout << queue.dequeue() << " ";
    cout << endl << endl;

    // 3. Дек: удалить чётные
    cout << "== Дек (удаление чётных) ==" << endl;
    Deque<int> deque;
    for (int i = 1; i <= 6; i++) deque.push_back(i);
    cout << "Исходный: ";
    deque.print();
    deque.removeEven();
    cout << "Результат: ";
    deque.print();
    cout << endl;

    // 4. Приоритетная очередь: планировщик задач
    cout << "== Планировщик задач ==" << endl;
    PriorityQueue<Task> pq;
    pq.push({ "Задача A", 1 });
    pq.push({ "Задача B", 3 });
    pq.push({ "Задача C", 2 });
    cout << "Порядок выполнения: ";
    while (!pq.isEmpty()) {
        Task t = pq.pop();
        cout << t.name << " ";
    }
    cout << endl;

    return 0;
}
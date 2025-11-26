#include <iostream>
using namespace std;
class SinglyList {
private:
    struct Node {
        int data;
        Node* next;
        Node(int val) : data(val), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int size;
public:
    SinglyList() : head(nullptr), tail(nullptr), size(0) {}
    ~SinglyList() {
        Node* current = head;
        while (current) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
    void push_back(int value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }
    void pop_back() {
        if (!head) return;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        size--;
    }
    int get(int index) {
        if (index < 0 || index >= size) return -1;
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }
    void set(int index, int value) {
        if (index < 0 || index >= size) return;
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        current->data = value;
    }
    int length() { return size; }
    bool empty() { return size == 0; }
    void print() {
        Node* current = head;
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};
int main() {
    SinglyList sl;
    sl.push_back(40);
    sl.push_back(20);
    sl.push_back(30);
    sl.print();  // 10 20 30
    
    sl.set(1, 25);
    cout << sl.get(1) << endl;  // 25
    sl.pop_back();
    sl.print();  // 10 25
    
    return 0;
}
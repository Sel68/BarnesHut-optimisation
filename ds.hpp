#pragma once
#include <cstddef>
#include<iostream>
using namespace std;

namespace ds {

// Singly Linked List
class SList {
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
    SList() : head(nullptr), tail(nullptr), size(0) {}
    ~SList() {
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

//Dynamic Array
class DynamicArray {
private:
    int* arr;
    int capacity;
    int size;

    void resize() {
        capacity *= 2;
        int* newArr = new int[capacity];
        for (int i = 0; i < size; i++) newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

public:
    DynamicArray(int cap = 2) : capacity(cap), size(0) {
        arr = new int[capacity];
    }

    ~DynamicArray() {
        delete[] arr;
    }

    void push(int value) {
        if (size == capacity) resize();
        arr[size++] = value;
    }

    void pop() {
        if (size > 0) size--;
    }

    int get(int index) {
        return (index >= 0 && index < size) ? arr[index] : -1;
    }

    void set(int index, int value) {
        if (index >= 0 && index < size) arr[index] = value;
    }

    int length() { return size; }
    bool empty() { return size == 0; }

    void print() {
        for (int i = 0; i < size; i++) cout << arr[i] << " ";
        cout << endl;
    }
};

}

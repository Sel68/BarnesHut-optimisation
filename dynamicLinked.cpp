#include <iostream>
using namespace std;

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

int main() {
    DynamicArray da;
    da.push(10);
    da.push(20);
    da.push(30);
    da.print();  // 10 20 30
    
    da.set(1, 25);
    cout << da.get(1) << endl;  // 25
    da.pop();
    da.print();  // 10 25
    
    return 0;
}
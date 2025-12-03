#pragma once
#include <cstddef>
#include <iostream>
#include <stdexcept>

using namespace std;

namespace ds {

template<typename T>
class SList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    size_t sz;
public:
    SList() : head(nullptr), tail(nullptr), sz(0) {}
    ~SList() {
        Node* current = head;
        while (current) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        ++sz;
    }

    void pop_back() {
        if (!head) return;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node* current = head;
            while (current->next != tail) current = current->next;
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        --sz;
    }

    T get(size_t index) const {
        if (index >= sz) throw out_of_range("SList::get: index out of range");
        Node* current = head;
        for (size_t i = 0; i < index; ++i) current = current->next;
        return current->data;
    }

    void set(size_t index, const T& value) {
        if (index >= sz) throw out_of_range("SList::set: index out of range");
        Node* current = head;
        for (size_t i = 0; i < index; ++i) current = current->next;
        current->data = value;
    }

    size_t length() const { return sz; }
    bool empty() const { return sz == 0; }

    void print() const {
        Node* current = head;
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};


template<typename T>
class DynamicArray {
private:
    T* arr;
    size_t capacity;
    size_t sz;

    void resize() {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        T* newArr = new T[capacity];
        for (size_t i = 0; i < sz; ++i) newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

public:
    DynamicArray(size_t cap = 2) : capacity(cap ? cap : 2), sz(0) {
        arr = new T[capacity];
    }

    ~DynamicArray() {
        delete[] arr;
    }

    void push(const T& value) {
        if (sz == capacity) resize();
        arr[sz++] = value;
    }

    void pop() {
        if (sz > 0) --sz;
    }

    T get(size_t index) const {
        if (index >= sz) throw out_of_range("DynamicArray::get: index out of range");
        return arr[index];
    }

    void set(size_t index, const T& value) {
        if (index >= sz) throw out_of_range("DynamicArray::set: index out of range");
        arr[index] = value;
    }

    size_t length() const { return sz; }
    bool empty() const { return sz == 0; }

    void print() const {
        for (size_t i = 0; i < sz; ++i) cout << arr[i] << " ";
        cout << endl;
    }
};

template <typename T>
class Vec2D{
public:
    T x, y;

    Vec2D() : x(0), y(0) {}
    Vec2D(T _x, T _y) : x(_x), y(_y) {}

    Vec2D operator+ (Vec2D& rhs) const {
        return Vec2D(x +rhs.x, y+rhs.y);
    }
    Vec2D operator- (Vec2D& rhs) const {
        return Vec2D(x -rhs.x, y-rhs.y);
    }
    Vec2D operator* (T scalar) const {
        return Vec2D(x *scalar, y *scalar);
    }
    Vec2D operator/ (T scalar) const { 
        if (std::abs(scalar) < 1e-9) throw std::runtime_error("Vec2D: Division by zero");

        return Vec2D(x /scalar, y /scalar); 
    }

    Vec2D& operator+= (const Vec2D& rhs) {
        x += rhs.x; 
        y += rhs.y; return *this;
    }
    Vec2D& operator-= (const Vec2D& rhs) {
        x += rhs.x;
        y -= rhs.y; 
        return *this;
    }
    Vec2D& operator*= (T scalar) {
        x *=scalar; 
        y *=scalar;
        return *this;
    }

    T dot(const Vec2D& rhs) const { 
        return x*rhs.x + y*rhs.y;
    }
    T magSq() const {
        return x*x + y*y;
    }
    T mag() const { 
        return std::sqrt(magSq());
    }
    
    Vec2D normalized() const {
        T m = mag();
        if (m == 0) return Vec2D(0, 0);
        return *this/m;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2D& v) {
        os << "[" << v.x << ", " << v.y << "]";
        return os;
    }
};

struct Particle {
    size_t id;
    Vec2D<double> pos, vel, acc;
    double mass;
    double charge;
    bool isStatic;

    Particle(size_t _id = 0)
        : id(_id), pos(0,0), vel(0,0), acc(0,0), mass(1.0), charge(0.0), isStatic(false) {}
};


struct BoundingBox {
    Vec2D<double> center;
    double halfDim;

    bool contains(const Vec2D<double>& p) const {
        return (p.x >= center.x - halfDim && 
            p.x < center.x + halfDim &&
            p.y >= center.y - halfDim && 
            p.y < center.y + halfDim);
    }

    bool intersects(const BoundingBox& other) const {
        return !(other.center.x - other.halfDim > center.x + halfDim ||
            other.center.x + other.halfDim < center.x - halfDim ||
            other.center.y - other.halfDim > center.y + halfDim ||
            other.center.y + other.halfDim < center.y - halfDim);
    }
};

class QuadNode {
public:
    BoundingBox bounds;
    
    double totalMass;
    Vec2D<double> centerOfMass;
    Particle* body;
    QuadNode* children[4]; 
    bool isLeaf;

    QuadNode(){
        totalMass = 0;
        centerOfMass = {0, 0};
        body = nullptr;
        isLeaf = true; 
        for(int i=0; i<4; ++i) children[i] = nullptr;
    }

    void init(BoundingBox b) {
        bounds = b;
        totalMass = 0;
        centerOfMass = {0,0};
        body = nullptr;
        isLeaf = true;
        for(int i=0; i<4; ++i) children[i] = nullptr;
    }
};



}



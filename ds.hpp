#pragma once
#include <memory>
#include <cstddef>
#include <iostream>
#include <stdexcept>


using namespace std;


namespace ds {

constexpr double G_CONST = 6.67430e-11;
constexpr double COULOMB_K = 8.98755e9;
constexpr double THETA_DEFAULT = 0.5;
constexpr double SOFTENING = 1e-5;
constexpr double eps = 1e-8;

enum class ForceType { GRAVITY, ELECTRIC, LENNARD_JONES, CUSTOM };
enum class IntegratorType { EULER, SYMPLECTIC_EULER, VERLET, RK4 };

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

    Vec2D operator+ (const Vec2D& rhs) const { 
        return Vec2D(x + rhs.x, y + rhs.y);
    }
    Vec2D operator- (const Vec2D& rhs) const { 
        return Vec2D(x - rhs.x, y - rhs.y); 
    }
    Vec2D operator* (T scalar) const { 
        return Vec2D(x * scalar, y * scalar); 
    }
    Vec2D operator/ (T scalar) const {
        if (abs(scalar) < eps) throw std::runtime_error("Vec2D: Division by zero");
        return Vec2D(x/scalar, y/scalar);
    }

    Vec2D& operator+= (const Vec2D& rhs) { 
        x += rhs.x; y += rhs.y; 
        return *this; 
    }
    Vec2D& operator-= (const Vec2D& rhs) { 
        x -= rhs.x; y -= rhs.y; 
        return *this; 
    }
    Vec2D& operator*= (T scalar) { 
        x *= scalar; y *= scalar; 
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

// Memory allocator
template<typename T>
class BlockAllocator {
private:
    std::vector<T> memory_pool;
    size_t current_index;

public:
    BlockAllocator(size_t capacity){
        current_index = 0;
        memory_pool.resize(capacity);
    }

    void reset() {
        current_index = 0;
    }

    T* allocate() {
        if (current_index >= memory_pool.size()) {
            throw std::overflow_error("Allocator: Out of memory block");
        }
        return &memory_pool[current_index++];
    }
    
    size_t used_memory() const {
        return current_index * sizeof(T);
    }
};

class BarnesHutTree {
private:
    QuadNode* root;
    BlockAllocator<QuadNode> allocator;
    double theta;

    //index (NW=0, NE=1, SW=2, SE=3)
    int getQuadrant(const BoundingBox& b, const Vec2D<double>& p)const{
        bool right = p.x > b.center.x;
        bool top = p.y > b.center.y;
        if (!right && top) return 0;
        if (right && top) return 1;
        if (!right && !top) return 2;
        return 3;
    }

    void subdivide(QuadNode* node) {
        double half = node->bounds.halfDim / 2.0;
        Vec2D c = node->bounds.center;
        
        node->children[0] = allocator.allocate();
        node->children[0]->init({ Vec2D(c.x-half, c.y+half), half});

        node->children[1] = allocator.allocate();
        node->children[1]->init({ Vec2D(c.x+half, c.y+half), half});

        node->children[2] = allocator.allocate();
        node->children[2]->init({ Vec2D(c.x-half, c.y-half), half});

        node->children[3] = allocator.allocate();
        node->children[3]->init({ Vec2D(c.x+half, c.y-half), half});
        
        node->isLeaf = false;
    }

    void insertRecursive(QuadNode* node, Particle* p) {
        //empty leaf, pyt body here
        if (node->isLeaf && node->body == nullptr) {
            node->body = p;
            node->totalMass = p->mass;
            node->centerOfMass = p->pos;
            return;
        }

        // occupied leaf, subdivide and re-insert both
        if (node->isLeaf && node->body != nullptr) {
            if ((node->body->pos - p->pos).magSq() < eps) {
                p->pos.x += 1e-5; 
            }

            Particle* oldBody = node->body;
            node->body = nullptr;
            subdivide(node);
            
            insertRecursive(node->children[getQuadrant(node->bounds, oldBody->pos)], oldBody);
            insertRecursive(node->children[getQuadrant(node->bounds, p->pos)], p);
            
            // Update internal node aggregates, CoM
            node->totalMass = oldBody->mass + p->mass;
            node->centerOfMass = ((oldBody->pos * oldBody->mass) + (p->pos * p->mass)) / node->totalMass;
            return;
        }

        // recurse on internal node
        if (!node->isLeaf) {
            // Update Center of Mass: R_new = (M*R + m*r) / (M+m)
            double newMass = node->totalMass + p->mass;
            node->centerOfMass = (node->centerOfMass * node->totalMass + p->pos * p->mass) / newMass;
            node->totalMass = newMass;

            insertRecursive(node->children[getQuadrant(node->bounds, p->pos)], p);
        }
    }

    Vec2D<double> computeForceRecursive(QuadNode* node, const Particle* p, double k, double power) const {
        if (!node || node->totalMass <= 0) return Vec2D(0.0,0.0);
        
        Vec2D<double> rVec = node->centerOfMass - p->pos;
        double rSq = rVec.magSq();
        double r = sqrt(rSq);
        double s = node->bounds.halfDim * 2.0;

        // Barnes-Hut MAC: If far enough (s/d < theta), treat as single body
        if (node->isLeaf || (s / r < theta)) {
            if (node->body == p) return Vec2D(0.0,0.0); // Self
            
            double dist = max(r, SOFTENING); // 
            double fMag = (k * p->mass * node->totalMass) / pow(dist, power);
            return rVec * (fMag / dist);
        }

        // Otherwise, recurse deeper
        Vec2D totalForce(0.0,0.0);
        for(int i=0; i<4; ++i) {
            totalForce += computeForceRecursive(node->children[i], p, k, power);
        }
        return totalForce;
    }

public:
    // Allocator size = Est. Particles * 2 (for safety)
    BarnesHutTree(size_t maxParticles, double _theta = THETA_DEFAULT) 
        : allocator(maxParticles * 4), theta(_theta), root(nullptr) {}

    void build(std::vector<Particle>& particles, BoundingBox worldBounds) {
        allocator.reset();
        root = allocator.allocate();
        root->init(worldBounds);

        for (auto& p : particles) {
            // Bounds check
            if (worldBounds.contains(p.pos)) {
                insertRecursive(root, &p);
            }
        }
    }
    Vec2D<double> getForceOn(const Particle* p, double k, double power) {
        return computeForceRecursive(root, p, k, power);
    }
};
template<typename T>
class Stack {
private:
    T* arr;
    size_t capacity;
    size_t topIndex;

    void resize() {
        capacity *= 2;
        T* newArr = new T[capacity];
        for (size_t i = 0; i < topIndex; ++i)
            newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

public:
    Stack(size_t cap = 10) : capacity(cap), topIndex(0) {
        arr = new T[capacity];
    }

    ~Stack() {
        delete[] arr;
    }

    void push(const T& value) {
        if (topIndex == capacity)
            resize();
        arr[topIndex++] = value;
    }

    void pop() {
        if (empty())
            throw runtime_error("Stack underflow");
        --topIndex;
    }

    T top() const {
        if (empty())
            throw runtime_error("Stack is empty");
        return arr[topIndex - 1];
    }

    bool empty() const {
        return topIndex == 0;
    }

    size_t size() const {
        return topIndex;
    }

    void print() const {
        cout << "Stack (top -> bottom): ";
        for (int i = topIndex - 1; i >= 0; --i)
            cout << arr[i] << " ";
        cout << endl;
    }
};

template<typename T>
class Queue {
private:
    T* arr;
    size_t capacity;
    size_t frontIndex;
    size_t rearIndex;
    size_t count;

    void resize() {
        size_t newCap = capacity * 2;
        T* newArr = new T[newCap];

        for (size_t i = 0; i < count; ++i)
            newArr[i] = arr[(frontIndex + i) % capacity];

        delete[] arr;
        arr = newArr;
        capacity = newCap;
        frontIndex = 0;
        rearIndex = count;
    }

public:
    Queue(size_t cap = 10)
        : capacity(cap), frontIndex(0), rearIndex(0), count(0) {
        arr = new T[capacity];
    }

    ~Queue() {
        delete[] arr;
    }

    void enqueue(const T& value) {
        if (count == capacity)
            resize();

        arr[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity;
        ++count;
    }

    void dequeue() {
        if (empty())
            throw runtime_error("Queue underflow");

        frontIndex = (frontIndex + 1) % capacity;
        --count;
    }

    T front() const {
        if (empty())
            throw runtime_error("Queue is empty");
        return arr[frontIndex];
    }

    bool empty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    }

    void print() const {
        cout << "Queue (front -> rear): ";
        for (size_t i = 0; i < count; ++i)
            cout << arr[(frontIndex + i) % capacity] << " ";
        cout << endl;
    }
};

template<class It, class Comp>
void merge_sort(It l, It r, Comp cmp){
    auto n = r - l;
    if(n <= 1) return;

    It m = l + n/2;
    merge_sort(l, m, cmp);
    merge_sort(m, r, cmp);

    using T = typename iterator_traits<It>::value_type;
    vector<T> tmp;
    tmp.reserve(n);

    It i = l, j = m;
    while(i < m && j < r){
        if(cmp(*j, *i)) tmp.push_back(*j++);
        else tmp.push_back(*i++);
    }
    while(i < m) tmp.push_back(*i++);
    while(j < r) tmp.push_back(*j++);

    for (int k = 0; k<n; k++) *(l + k) = tmp[k];
}


}



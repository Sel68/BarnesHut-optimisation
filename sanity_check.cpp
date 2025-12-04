#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include <string>

#include "ds.hpp"

using namespace std;
using namespace ds;

bool almostEqual(double a, double b, double epsilon = 1e-6) {
    return abs(a - b) < epsilon;
}

void testSList() {
    cout << "[Running SList Test]..." << endl;
    SList<int> list;
    
    assert(list.empty());
    assert(list.length() == 0);

    // Push Back
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    assert(list.length() == 3);
    assert(list.get(0) == 10);
    assert(list.get(1) == 20);
    assert(list.get(2) == 30);

    // Set
    list.set(1, 99);
    assert(list.get(1) == 99);

    // Pop Back
    list.pop_back();
    assert(list.length() == 2);
    assert(list.get(1) == 99); // 10, 99

    list.pop_back(); 
    list.pop_back();
    assert(list.empty());

    // Out of range check
    try {
        list.get(0);
        assert(false && "Should have thrown out_of_range");
    } catch (const std::out_of_range&) {
        // Expected
    }

    cout << "PASSED" << endl;
}

void testDynamicArray() {
    cout << "[Running DynamicArray Test]..." << endl;
    DynamicArray<int> arr(2); // small cap for later resize

    assert(arr.empty());
    
    arr.push(1);
    arr.push(2);
    assert(arr.length() == 2);

    // Trigger resize
    arr.push(3);
    assert(arr.length() == 3);
    assert(arr.get(2) == 3);

    arr.set(0, 100);
    assert(arr.get(0) == 100);

    arr.pop();
    assert(arr.length() == 2);
    assert(arr.get(1) == 2);

    cout << "PASSED" << endl;
}

void testVec2D() {
    cout << "[Running Vec2D Test]..." << endl;
    Vec2D<double> v1(3.0, 4.0);
    Vec2D<double> v2(1.0, 2.0);

    // Magnitude
    assert(almostEqual(v1.mag(), 5.0));
    assert(almostEqual(v1.magSq(), 25.0));

    // Arithmetic
    Vec2D<double> vSum = v1 + v2;
    assert(almostEqual(vSum.x, 4.0) && almostEqual(vSum.y, 6.0));

    Vec2D<double> vDiff = v1 - v2;
    assert(almostEqual(vDiff.x, 2.0) && almostEqual(vDiff.y, 2.0));

    Vec2D<double> vScale = v1 * 2.0;
    assert(almostEqual(vScale.x, 6.0) && almostEqual(vScale.y, 8.0));

    // Dot product, 11
    assert(almostEqual(v1.dot(v2), 11.0));

    // Normalization
    Vec2D<double> vNorm = v1.normalized();
    assert(almostEqual(vNorm.x, 0.6) && almostEqual(vNorm.y, 0.8));
    assert(almostEqual(vNorm.mag(), 1.0));

    cout << "PASSED" << endl;
}

void testPhysicsStructs() {
    cout << "[Running Physics/QuadTree Structs Test]..." << endl;
    
    // Bounding Box
    BoundingBox box;
    box.center = {0, 0};
    box.halfDim = 10; // Range: [-10, 10]

    assert(box.contains({5, 5}));
    assert(box.contains({-9, -9}));
    assert(!box.contains({11, 0}));

    BoundingBox other;
    other.center = {15, 0};
    other.halfDim = 6; // Range: [9, 21]. Overlaps with [ -10, 10] at x=9..10
    
    assert(box.intersects(other));

    other.center = {25, 0}; // Range: [19, 31]. No overlap
    assert(!box.intersects(other));

    // QuadNode Init
    QuadNode node;
    node.init(box);
    assert(node.isLeaf);
    assert(node.body == nullptr);
    assert(node.totalMass == 0);

    cout << "PASSED" << endl;
}

void testAllocator() {
    cout << "[Running BlockAllocator Test]..." << endl;
    
    BlockAllocator<QuadNode> allocator(10);
    
    // Allocate 1
    QuadNode* q1 = allocator.allocate();
    assert(q1 != nullptr);
    assert(allocator.used_memory() == sizeof(QuadNode));

    // Allocate 2
    QuadNode* q2 = allocator.allocate();
    assert(q2 != nullptr);
    assert(q2 != q1);
    assert(allocator.used_memory() == 2 * sizeof(QuadNode));

    // Reset
    allocator.reset();
    assert(allocator.used_memory() == 0); 
    
    // Reuse
    QuadNode* q3 = allocator.allocate();
    // q3 should theoretically point to the same memory slot q1 did
    q3->totalMass = 50.0;
    assert(q3->totalMass == 50.0);

    // Overflow test
    BlockAllocator<int> smallAlloc(2);
    smallAlloc.allocate();
    smallAlloc.allocate();
    try {
        smallAlloc.allocate();
        assert(false && "Should have thrown overflow_error");
    } catch (const std::overflow_error&) {
        // Expected
    }

    cout << "PASSED" << endl;
}

int main() {
    cout << "Starting Unit Tests..." << endl << endl;

    try {
        testSList();
        testDynamicArray();
        testVec2D();
        testPhysicsStructs();
        testAllocator();
    } catch (const exception& e) {
        cerr << "Test FAILED with exception: " << e.what() << endl;
        return 1;
    }

    cout << endl << "All unit tests passed successfully!" << endl;
    return 0;
}
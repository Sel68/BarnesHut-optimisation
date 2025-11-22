#pragma once
#include <cstddef>

namespace ds {

// Singly Linked List
template<typename T>
class SList {

private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n) : data(d), next(n) {}
        Node(T&& d, Node* n) : data(std::move(d)), next(n) {}
    };

    Node* head_;
    std::size_t sz_;

public:
    SList() noexcept;
    ~SList() noexcept;

    void push_front(const T& v);
    void push_front(T&& v);

    void pop_front();

    T& front();
    const T& front() const;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

};

//Dynamic Array
template<typename T>
class DynArray {
private:
    T* data_;
    std::size_t sz_;
    std::size_t cap_;

public:
    DynArray() noexcept;
    explicit DynArray(std::size_t n);
    ~DynArray() noexcept;

    void push_back(const T& v);
    void push_back(T&& v);

    void pop_back();

    T& operator[](std::size_t i);
    const T& operator[](std::size_t i) const;

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    void reserve(std::size_t new_cap);

};

}

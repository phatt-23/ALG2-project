#ifndef __BINARY_HEAP_H
#define __BINARY_HEAP_H

#include <ostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>

#define MACRO_BINARY_HEAP_TEMPLATE_CLASS(type_letter, comp_func) \
    template <typename type_letter, typename comp_func = std::less<type_letter>>

#define MACRO_BINARY_HEAP_TEMPLATE(type_letter, comp_func) \
    template <typename type_letter, typename comp_func>

/// Binary heap for numerical types or 
/// any type implementing comparison operations.
MACRO_BINARY_HEAP_TEMPLATE_CLASS(T,F)
class BinaryHeap
{
private:
    std::vector<T> heap_vec;
    F comp;

    void heapify(int parent_index);
    void build_heap();

public:
    BinaryHeap(F compare = F()) 
        : heap_vec{std::vector<T>()}, comp(compare) {}

    BinaryHeap(std::vector<T>& inputData, F compare = F())
        : heap_vec{inputData}, comp(compare) {}

    ~BinaryHeap() = default;
    
    const std::vector<T>& HeapVec() const;

    void Insert(T elem);
    bool Empty() const;
    T Poll();
    const T& Peek() const;
    
    MACRO_BINARY_HEAP_TEMPLATE(U,C)
    friend std::ostream& operator << (std::ostream& os, const BinaryHeap<U,C>& bh);
};

template<typename T, typename F>
const std::vector<T>& BinaryHeap<T,F>::HeapVec() const { return this->heap_vec; }

MACRO_BINARY_HEAP_TEMPLATE(T,F)
bool BinaryHeap<T,F>::Empty() const { return heap_vec.empty(); }

MACRO_BINARY_HEAP_TEMPLATE(T,F)
T BinaryHeap<T,F>::Poll() 
{
    if (heap_vec.empty()) 
        throw std::out_of_range("Heap empty. Nothing to poll");

    std::swap(heap_vec.front(), heap_vec.back());
    T min = heap_vec.back();
    heap_vec.pop_back();
    this->build_heap();
    return min;
}

MACRO_BINARY_HEAP_TEMPLATE(T,F)
const T& BinaryHeap<T,F>::Peek() const
{
    if (heap_vec.empty()) 
        throw std::out_of_range("Heap empty. Nothing to poll");

    return heap_vec.front();
}

MACRO_BINARY_HEAP_TEMPLATE(T,F)
void BinaryHeap<T,F>::Insert(T elem)
{
    this->heap_vec.push_back(elem);
    int current_index = this->heap_vec.size() - 1;

    while (current_index > 0)
    {
        int parent_index = (current_index - 1) / 2; // floored
        if (comp(this->heap_vec[parent_index], this->heap_vec[current_index]))
            break;
        std::swap(this->heap_vec[parent_index], this->heap_vec[current_index]);
        current_index = parent_index;
    }
}

MACRO_BINARY_HEAP_TEMPLATE(T,F)
void BinaryHeap<T,F>::build_heap()
{
    // go from bottom upward to the root
    for (int i = this->heap_vec.size() - 1; i >= 0; i--)
        this->heapify(i);
}

MACRO_BINARY_HEAP_TEMPLATE(T,F)
void BinaryHeap<T,F>::heapify(int parent_index)
{
    int maximum_index = parent_index;
    int left_child_index = 2 * parent_index + 1;
    int right_child_index = 2 * parent_index + 2;

    if (left_child_index < (int)this->heap_vec.size() && 
        comp(this->heap_vec[left_child_index], this->heap_vec[maximum_index]))
        maximum_index = left_child_index;

    if (right_child_index < (int)this->heap_vec.size() && 
        comp(this->heap_vec[right_child_index], this->heap_vec[maximum_index]))
        maximum_index = right_child_index;

    if (maximum_index != parent_index) {
        std::swap(this->heap_vec[maximum_index], this->heap_vec[parent_index]);
        this->heapify(maximum_index);
    }
}

MACRO_BINARY_HEAP_TEMPLATE(T,F)
std::ostream& operator << (std::ostream& os, const BinaryHeap<T,F>& bh)
{
    const auto& heap = bh.get_heap();
    int len = heap.size();
    std::stringstream max_elem_string {}; 
    T max_elem = *std::max_element(heap.begin(), heap.end());

    max_elem_string << max_elem;
    int padding_width = max_elem_string.str().length() + 1;

    os << "[ ";

    for (int i = 0; i < len - 1; ++i) {
        os << std::setw(padding_width) << std::right
           << heap[i] 
           << ", ";
    }

    os << std::setw(padding_width) << std::right
       << heap[len - 1]
       << std::setw(padding_width) << std::right
       << " ]";

    return os;
}

#endif//__BINARY_HEAP_H


//
// Created by phatt on 12/6/24.
//

#ifndef VECTOR_H
#define VECTOR_H

#include <cstdint>
#include <iostream>
#include <new>
#include <utility>
#include <iterator>


template <typename T>
class Vector
{
private:
    size_t _capacity = 0;
    size_t _size = 0;
    T* _block = nullptr;

public:
    using ValueType = T;
    // using Iterator = VectorIterator<Vector<T>>;

    Vector() {
        Resize(2);
    }

    Vector(size_t size) {
        Resize(size);
    }

    Vector(const Vector<T>& other) {
        Resize(other._capacity); 
        _size = other._size;
        for (size_t i = 0; i < _size; i++) {
            new(&_block[i]) T(other._block[i]); // Copy construct
        }
    }


    Vector(Vector<T>&& other) noexcept
        : _capacity(other._capacity), _size(other._size), _block(other._block) 
    {
        other._capacity = 0;
        other._size = 0;
        other._block = nullptr;
    }

    Vector<T>& operator=(const Vector<T>& other) {
        std::cout << "Vector<T>& operator=(const Vector<T>& other) {\n";
        if (this == &other) return *this;

        Clear();
        ::operator delete(_block, _capacity * sizeof(T));

        Resize(other._capacity);
        _size = other._size;
        for (size_t i = 0; i < _size; i++) {
            new(&_block[i]) T(other._block[i]);
        }
        return *this;
    }

    Vector<T>& operator=(Vector<T>&& other) noexcept {
        std::cout << "Vector<T>& operator=(Vector<T>&& other) noexcept {\n";
        if (this == &other) return *this;

        Clear();
        ::operator delete(_block, _capacity * sizeof(T));

        _capacity = other._capacity;
        _size = other._size;
        _block = other._block;
        other._capacity = 0;
        other._size = 0;
        other._block = nullptr;
        return *this;
    }

    ~Vector() {
        Clear();
        ::operator delete(_block, _capacity * sizeof(T));
    }

    T& operator[](size_t index) {
        if (index >= _size) {
            throw std::out_of_range(std::format("Index out of bounds. INDEX: {}, SIZE: {}", index, _size));
        }
        return _block[index];
    }

    const T& operator[](size_t index) const {
        if (index >= _size) {
            throw std::out_of_range(std::format("Index out of bounds. INDEX: {}, SIZE: {}", index, _size));
        }
        return _block[index];
    }

    [[nodiscard]] size_t Size() const { return _size; }

    void Resize(const size_t newCapacity) {
        T* newBlock = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

        for (size_t i = 0; i < _size; i++) {
            new(&newBlock[i]) T(std::move(_block[i])); // Move construct
            _block[i].~T(); // Explicitly destroy old object
        }

        ::operator delete(_block, _capacity * sizeof(T));
        _block = newBlock;
        _capacity = newCapacity;

        if (newCapacity < _size) _size = newCapacity;

    }

    void Insert(size_t index, const T& value) {
        if (index >= _capacity) {
            Resize(_capacity << 1); // double the size
        }

        _block[index] = value;

        if (index >= _size) {
            _size = index + 1;
        }

    }

    void PushBack(const T& value) {
        if (_size >= _capacity) {
            Resize(_capacity * 2);
        }
        new(&_block[_size++]) T(value); 
    }

    void PushBack(T&& value) {
        if (_size >= _capacity) {
            Resize(_capacity * 2);
        }
        new(&_block[_size++]) T(std::move(value));
    }

    template <typename ...Args>
    T& EmplaceBack(Args&&... args) {
        if (_size >= _capacity) {
            Resize(_capacity * 2); // double the size
        }
        // _block[_size] = T(std::forward<Args>(args)...);
        new(&_block[_size]) T(std::forward<Args>(args)...);
        return _block[_size++];
    }
    
    void PopBack() {
        if (_size > 0) {
            _size--;
            _block[_size].~T();
        }
    }
    
    void Clear() {
        for (size_t i = 0; i < _size; i++) {
            _block[i].~T();
        }
        _size = 0;
    }

    const T& Front() const { return *(_block); }
    const T& Back() const { return *(_block + _size); }

    T* begin() { return _block; }
    const T* begin() const { return _block; }
    T* end() { return _block + _size; }
    const T* end() const { return _block + _size; }

};

#endif //VECTOR_H



#ifndef _MATRIX_H
#define _MATRIX_H

#include "i_to_string.h"
#include "comparable_type.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iomanip>
 
template <Comparable T>
class Matrix : public IToString
{
    size_t rows;
    size_t columns;
    T* elements;

public:
    Matrix() = default;
    Matrix(size_t rows, size_t columns);
    Matrix(size_t rows, size_t columns, T* elements);
    ~Matrix() override;

    size_t Rows() const { return rows; }
    size_t Columns() const { return columns; }
    const T& Elements() const { return elements; }

    Matrix& Allocate();
    Matrix& Zero();
    Matrix& Randomize(double low, double high);    
    T& At(size_t row, size_t column);
    const T& Get(size_t row, size_t column) const;
    
    Matrix& SetElements(T* elems);

    std::string ToString() const override;
};

// implementation

template <Comparable T>
Matrix<T>::Matrix(size_t rows, size_t columns)
    : IToString()
    , rows(rows)
    , columns(columns)
    , elements(nullptr)
{
}

template <Comparable T>
Matrix<T>::Matrix(size_t rows, size_t columns, T* elements)
    : IToString()
    , rows(rows)
    , columns(columns)
    , elements(elements)
{
}

template <Comparable T>
Matrix<T>::~Matrix()
{
    if (this->elements != nullptr)
        delete[] elements;
}

template <Comparable T>
Matrix<T>& Matrix<T>::Allocate()
{
    if (this->elements != nullptr)
        throw std::runtime_error("Can't allocate elements for matrix that has already been allocated!");

    elements = new T[this->rows * this->columns];

    return this;
}

template <Comparable T>
Matrix<T>& Matrix<T>::Zero()
{
    if (this->elements == nullptr)
        throw std::runtime_error("Can't zero a matrix that doesnt have any elements!");

    memset(elements, 0, rows * columns);

    return this;
}

template <Comparable T>
Matrix<T>& Matrix<T>::Randomize(double low, double high)
{
    if (this->elements == nullptr)
        throw std::runtime_error("Can't randomize a matrix that doesnt have any elements!");

    int diff = high - low;
    
    if (diff < 0)
         throw std::runtime_error("High is lower than Low");

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            this->elements[i * rows + j] = 
                (double)std::rand() / (double)RAND_MAX * diff - low;
        }
    }

    return this;
}

template <Comparable T>
T& Matrix<T>::At(size_t row, size_t column)
{
    if (this->elements == nullptr)
        throw std::runtime_error("Can't read from matrix with initialized elements (memory)!");

    size_t index = row * columns + column;

    if (index < 0)
        throw std::runtime_error("Can't read from matrix's negative index!");
        
    if (index >= columns * rows)
        throw std::runtime_error("Out of bounds access");

    return &this->elements[row * this->columns + column];
}
    
template <Comparable T>
const T& Matrix<T>::Get(size_t row, size_t column) const
{
    if (this->elements == nullptr)
        throw std::runtime_error("Can't read from matrix with initialized elements (memory)!");

    size_t index = row * columns + column;

    if (index < 0)
        throw std::runtime_error("Can't read from matrix's negative index!");
        
    if (index >= columns * rows)
        throw std::runtime_error("Out of bounds access");

    return this->elements[row * this->columns + column];
}
   

template <Comparable T>
std::string Matrix<T>::ToString() const
{
    using namespace std;
    if (this->elements == nullptr)
        throw std::runtime_error("Can't convert matrix to string with uninitialized elements (memory)!");

    stringstream string_stream;

    std::stringstream max_ss {}; 
    max_ss << *std::max_element(elements, elements + rows*columns);

    size_t padding = max_ss.str().length() + 1;

    string_stream << std::setprecision(3);

    string_stream << "[ " << this->rows << "x" << this->columns << "\n";

    for (size_t i = 0; i < this->rows; ++i) {
        for (size_t j = 0; j < this->columns; ++j) {
            string_stream << setw(padding) << right 
                          << this->elements[i * this->columns + j];
        }
        string_stream << std::endl;
    }

    string_stream << "]";

    return string_stream.str();
}

template <Comparable T>
Matrix<T>& Matrix<T>::SetElements(T* elems)
{
    if (this->elements != nullptr)
        delete[] this->elements;

    this->elements = elems;

    return this;
}

#endif // _MATRIX_H


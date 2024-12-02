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

/// @brief A templated class representing a 2D matrix of type T.
/// 
/// This class provides various utilities for matrix manipulation, such as 
/// allocation, element access, initialization, randomization, and 
/// string representation.
template <Comparable T>
class Matrix : public IToString
{
    size_t rows;     ///< Number of rows in the matrix.
    size_t columns;  ///< Number of columns in the matrix.
    T* elements;     ///< Pointer to the matrix elements.

public:
    // Default constructor
    Matrix() = default;

    /// @brief Constructs a Matrix with the specified number of rows and columns.
    /// @param rows The number of rows in the matrix.
    /// @param columns The number of columns in the matrix.
    Matrix(size_t rows, size_t columns);

    /// @brief Constructs a Matrix with the specified number of rows, columns, and elements.
    /// @param rows The number of rows in the matrix.
    /// @param columns The number of columns in the matrix.
    /// @param elements Pointer to an array of elements to initialize the matrix.
    Matrix(size_t rows, size_t columns, T* elements);

    // Virtual destructor
    ~Matrix() override;

    size_t Rows() const { return rows; }                   ///< Returns the number of rows.
    size_t Columns() const { return columns; }             ///< Returns the number of columns.
    const T* Elements() const { return elements; }         ///< Returns a pointer to the elements.

    /// @brief Allocates memory for the matrix elements.
    /// @return reference to the current matrix.
    Matrix& Allocate();

    /// @brief Initializes all elements of the matrix to zero.
    /// @return reference to the current matrix.
    Matrix& Zero();

    /// @brief Randomizes the matrix elements with values in the specified range.
    /// @param low The lower bound for random values.
    /// @param high The upper bound for random values.
    /// @return reference to the current matrix.
    Matrix& Randomize(double low, double high);

    /// @brief Accesses the element at the specified row and column.
    /// @param row The row index of the element.
    /// @param column The column index of the element.
    /// @return Reference to the element at the specified position.
    T& At(size_t row, size_t column);

    /// @brief Retrieves the element at the specified row and column.
    /// @param row The row index of the element.
    /// @param column The column index of the element.
    /// @return Constant reference to the element at the specified position.
    const T& Get(size_t row, size_t column) const;

    /// @brief Sets the elements of the matrix with the provided array.
    /// @param elems Pointer to an array of elements.
    /// @return reference to the current matrix.
    Matrix& SetElements(T* elems);

    /// @brief Converts the matrix to a string representation.
    /// @return A string representing the matrix.
    std::string ToString() const override;
};

// Implementation

template <Comparable T>
Matrix<T>::Matrix(size_t rows, size_t columns)
    : IToString(), rows(rows), columns(columns), elements(nullptr)
{
}

template <Comparable T>
Matrix<T>::Matrix(size_t rows, size_t columns, T* elements)
    : IToString(), rows(rows), columns(columns), elements(elements)
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
    return *this;
}

template <Comparable T>
Matrix<T>& Matrix<T>::Zero()
{
    if (this->elements == nullptr)
        throw std::runtime_error("Can't zero a matrix that doesn't have any elements!");

    memset(elements, 0, rows * columns * sizeof(T));
    return *this;
}

template <Comparable T>
Matrix<T>& Matrix<T>::Randomize(double low, double high)
{
    if (this->elements == nullptr)
        throw std::runtime_error("Can't randomize a matrix that doesn't have any elements!");

    if (low > high)
        throw std::runtime_error("High is lower than Low");

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            elements[i * columns + j] = static_cast<T>((std::rand() / static_cast<double>(RAND_MAX)) * (high - low) + low);
        }
    }

    return *this;
}

template <Comparable T>
T& Matrix<T>::At(size_t row, size_t column)
{
    if (this->elements == nullptr)
        throw std::runtime_error("Can't read from a matrix with uninitialized elements!");

    if (row >= rows || column >= columns)
        throw std::runtime_error("Out of bounds access");

    return this->elements[row * this->columns + column];
}

template <Comparable T>
const T& Matrix<T>::Get(size_t row, size_t column) const
{
    if (this->elements == nullptr)
        throw std::runtime_error("Can't read from a matrix with uninitialized elements!");

    if (row >= rows || column >= columns)
        throw std::runtime_error("Out of bounds access");

    return this->elements[row * this->columns + column];
}

template <Comparable T>
std::string Matrix<T>::ToString() const
{
    using namespace std;
    if (this->elements == nullptr)
        throw std::runtime_error("Can't convert matrix to string with uninitialized elements!");

    stringstream string_stream;

    // Find the maximum element's string length for padding
    size_t padding = std::to_string(*std::max_element(elements, elements + rows * columns)).length() + 1;

    string_stream << std::setprecision(3);
    string_stream << "[ " << this->rows << "x" << this->columns << "\n";

    for (size_t i = 0; i < this->rows; ++i) {
        for (size_t j = 0; j < this->columns; ++j) {
            string_stream << setw(padding) << right << this->elements[i * this->columns + j];
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
    return *this;
}

#endif // _MATRIX_H

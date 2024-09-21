#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>
#include <stdexcept>
#include <iostream>

// Matrix class template
template <size_t Rows, size_t Cols>
class Matrix {
    private:
        std::array<std::array<int, Cols>, Rows> data;

    public:
        // Constructor to initialize the Matrix
        Matrix();

        // Function to set an element
        void set(size_t row, size_t col, int value);

        // Function to get an element
        int get(size_t row, size_t col) const;

        // Function to print the Matrix
        void print() const;

        // Function to add another Matrix (Matrix addition)
        Matrix<Rows, Cols> add(const Matrix<Rows, Cols>& other) const;

        // Function to multiply with another Matrix (Matrix multiplication)
        template <size_t OtherCols>
        Matrix<Rows, Cols> multiply(const Matrix<Cols, OtherCols>& other) const;

        // Function to transpose the Matrix (Matrix transpose)
        Matrix<Rows, Cols> transpose() const;

        // Function to compute the determinant of n x n Matrix
        long determinant() const;

};

// Specialization for 1x1 and 2x2 determinants
template<>
long Matrix<1, 1>::determinant() const;

template<>
long Matrix<2, 2>::determinant() const;

template <size_t Rows, size_t Cols>
long Matrix<Rows, Cols>::determinant() const;

#endif
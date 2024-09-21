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
    Matrix() {
        for (auto& row : data) {
            row.fill(0);
        }
    }

    // Set and get functions
    void set(size_t row, size_t col, int value) {
        if (row >= Rows || col >= Cols) {
            throw std::out_of_range("Index out of range\n");
        }
        data[row][col] = value;
    }

    int get(size_t row, size_t col) const {
        if (row >= Rows || col >= Cols) {
            throw std::out_of_range("Index out of range\n");
        }
        return data[row][col];
    }

    // Print function
    void print() const {
        for (const auto& row : data) {
            for (const auto& element : row) {
                std::cout << element << " ";
            }
            std::cout << '\n';
        }
    }

    // Add function
    Matrix<Rows, Cols> add(const Matrix<Rows, Cols>& other) const {
        Matrix<Rows, Cols> result;
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Cols; ++j) {
                result.set(i, j, this->get(i, j) + other.get(i, j));
            }
        }
        return result;
    }

    // Multiply function
    template <size_t OtherCols>
    Matrix<Rows, OtherCols> multiply(const Matrix<Cols, OtherCols>& other) const {
        Matrix<Rows, OtherCols> result;
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < OtherCols; ++j) {
                int sum = 0;
                for (size_t k = 0; k < Cols; ++k) {
                    sum += this->get(i, k) * other.get(k, j);
                }
                result.set(i, j, sum);
            }
        }
        return result;
    }

    // Transpose function
    Matrix<Cols, Rows> transpose() const {
        Matrix<Cols, Rows> result;
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Cols; ++j) {
                result.set(j, i, this->get(i, j));
            }
        }
        return result;
    }

    // Declaration of the determinant function
    long determinant() const;
};

// Specialization for 1x1 determinant
template<>
long Matrix<1, 1>::determinant() const {
    return data[0][0];
}

// Specialization for 2x2 determinant
template<>
long Matrix<2, 2>::determinant() const {
    return data[0][0]*data[1][1] - data[0][1] * data[1][0];
}

// General determinant implementation
template <size_t Rows, size_t Cols>
long Matrix<Rows, Cols>::determinant() const {
    static_assert(Rows == Cols, "Determinant is only defined for square matrices\n");

    long det = 0;
    for(size_t j = 0; j < Cols; ++j) {
        if(Rows > 2) {
            Matrix<Rows - 1, Cols - 1> submatrix;
            for(size_t sub_i = 0; sub_i < Rows - 1; ++sub_i) {
                for(size_t sub_j = 0; sub_j < Cols - 1; ++sub_j) {
                    submatrix.set(sub_i, sub_j, this->get(sub_i + 1, (sub_j >= j ? sub_j + 1 : sub_j)));
                }
            }
            det += (j % 2 == 0 ? 1 : -1) * data[0][j] * submatrix.determinant();
        }
    }

    return det;
}

#endif
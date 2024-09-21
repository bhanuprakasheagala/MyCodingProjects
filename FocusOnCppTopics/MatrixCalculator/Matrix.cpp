#include "Matrix.hpp"

// Constructor
template <size_t Rows, size_t Cols>
Matrix<Rows, Cols>::Matrix() {
    for(auto& row : data) {
        row.fill(0);
    }
}

// Set element
template <size_t Rows, size_t Cols>
void Matrix<Rows, Cols>::set(size_t row, size_t col, int value) {
    if(row >= Rows || col >= Cols) {
        throw std::out_of_range("Index out of range\n");
    }
    data[row][col] = value;
}

// Get element
template <size_t Rows, size_t Cols>
int Matrix<Rows, Cols>::get(size_t row, size_t col) const {
    if(row >= Rows || col >= Cols) {
        throw std::out_of_range("Index out of range\n");
    }
    return data[row][col];
}

// Print Matrix
template <size_t Rows, size_t Cols>
void Matrix<Rows, Cols>::print() const {
    for(const auto& row : data) {
        for(const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << '\n';
    }
}

// Matrix Addition
template <size_t Rows, size_t Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::add(const Matrix<Rows, Cols>& other) const {
    Matrix<Rows, Cols> result;
    for(size_t i = 0; i < Rows; ++i) {
        for(size_t j = 0; j < Cols; ++j) {
            result.set(i, j, this->get(i, j) + other.get(i, j));
        }
    }

    return result;
}

// Matrix Multiplication
template <size_t Rows, size_t Cols>
template <size_t OtherCols>
Matrix<Rows, OtherCols> Matrix<Rows, Cols>::multiply(const Matrix<Rows, OtherCols>& other) const {
    Matrix<Rows, OtherCols> result;
    for(size_t i = 0; i < Rows; ++i) {
        for(size_t j = 0; j < OtherCols; ++j) {
            int sum = 0;
            for(size_t k = 0; k < Cols; ++k) {
                sum += this->get(i, k) * other.get(k, j);
            }

            result.set(i, j, sum);
        }
    }

    return result;
}

// Matrix Transponse
template <size_t Rows, size_t Cols>
Matrix<Cols, Rows> Matrix<Rows, Cols>::transpose() const {
    Matrix<Cols, Rows> result;
    for(size_t i = 0; i < Rows; ++i) {
        for(size_t j = 0; j < Cols; ++j) {
            result.set(j, i, this->get(i, j));
        }
    }
    return result;
}


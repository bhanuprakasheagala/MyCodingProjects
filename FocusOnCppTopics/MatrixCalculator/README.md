# Matrix Class Template in C++

## Overview

This project provides a robust implementation of a matrix class in C++ using templates, allowing users to perform various matrix operations such as addition, multiplication, transposition, and determinant calculation. It is designed to be flexible, efficient, and easy to use, making it suitable for both educational purposes and practical applications in linear algebra.

## Features

- **Template-Based**: Define matrices of any size at compile-time.
- **Basic Operations**: Perform addition, multiplication, and transposition of matrices.
- **Determinant Calculation**: Calculate the determinant for square matrices using recursion.
- **Error Handling**: Implements exception handling for out-of-bounds access and ensures safety through static assertions.

## Getting Started

To get started with the project, clone this repository to your local machine:

```bash
git clone https://github.com/yourusername/matrix-template.git
cd matrix-template
```

### Prerequisites

Make sure you have a C++ compiler (like `g++`) installed. This project is compatible with C++11 and above.

## Project Structure

- `Matrix.hpp`: Contains the full implementation of the `Matrix` class template.
- `Matrix.cpp`: A demonstration of how to use the `Matrix` class, showcasing its features through various operations.

## How It Works

### Code Flow

1. **Creating a Matrix**: You instantiate a matrix using `Matrix<Rows, Cols>`, where `Rows` and `Cols` specify the matrix dimensions. The constructor initializes all elements to zero.

2. **Setting and Getting Elements**: Use `set(row, col, value)` to assign a value at a specific position, and `get(row, col)` to retrieve it. Both methods include bounds checking to prevent invalid access.

3. **Printing the Matrix**: Call `print()` to display the matrix elements in a formatted way.

4. **Performing Operations**:
   - **Addition**: Use `add(otherMatrix)` to add another matrix of the same dimensions.
   - **Multiplication**: Use `multiply(otherMatrix)` to multiply with another matrix, ensuring the number of columns in the first matrix matches the number of rows in the second.
   - **Transposition**: Call `transpose()` to obtain a new matrix where rows and columns are swapped.
   - **Determinant**: Use `determinant()` to calculate the determinant of square matrices, with special handling for 1x1 and 2x2 cases.

### Example Usage

Here’s a brief example demonstrating how to use the `Matrix` class in `main.cpp`:

```cpp
int main() {
    Matrix<2, 3> mat1;
    mat1.set(0, 0, 1);
    mat1.set(0, 1, 2);
    mat1.set(0, 2, 3);
    mat1.set(1, 0, 4);
    mat1.set(1, 1, 5);
    mat1.set(1, 2, 6);
    
    Matrix<2, 3> mat2;
    mat2.set(0, 0, 7);
    mat2.set(0, 1, 8);
    mat2.set(0, 2, 9);
    mat2.set(1, 0, 1);
    mat2.set(1, 1, 2);
    mat2.set(1, 2, 3);
    
    Matrix<2, 3> matSum = mat1.add(mat2);
    matSum.print(); // Outputs the result of matrix addition
}
```
## Error Handling

- Out-of-bounds access is managed using exceptions. Attempting to access an invalid index will throw `std::out_of_range`.
- Compile-time checks ensure that operations like determinant calculations are only performed on square matrices, preventing logical errors.

# Key Concepts in the Matrix Class Implementation

### 1. C++ Templates

**Concept**: Templates in C++ allow you to write generic and reusable code. The `Matrix` class is defined using template parameters, enabling it to handle matrices of any specified size.

**How It Works**: The template parameters `Rows` and `Cols` are specified at compile-time, allowing the class to generate a specific instance of `Matrix` for those dimensions.

**Why It Matters**: This flexibility means you can create matrices of different sizes without duplicating code. For instance, `Matrix<3, 3>` creates a 3x3 matrix, while `Matrix<2, 4>` creates a 2x4 matrix. If you omitted templates and hard-coded sizes, you'd have to create separate classes for each matrix size, leading to code duplication and maintenance challenges.

**Potential Issues**: If you forget to use `template` or define a method without it, the compiler will not understand the function as a template, leading to compilation errors.

### 2. Static Assertions

**Concept**: `static_assert` is used to perform compile-time checks.

**How It Works**: The line `static_assert(Rows == Cols, "Determinant is only defined for square matrices");` ensures that the determinant function is only called for square matrices.

**Why It Matters**: This prevents runtime errors by catching mistakes at compile time. If someone tries to calculate the determinant of a non-square matrix, they’ll receive a clear error message immediately.

**Potential Issues**: If you remove or modify this assertion, you could end up attempting to calculate the determinant for a non-square matrix, leading to incorrect results or undefined behavior.

### 3. Exception Handling

**Concept**: The `set` and `get` methods include checks for out-of-bounds access, throwing exceptions if the indices are invalid.

**How It Works**: The code checks if the specified row and column indices are within the valid range. If not, it throws an `std::out_of_range` exception.

**Why It Matters**: This safeguards against accessing invalid memory, which could lead to crashes or corrupted data. Proper exception handling makes the code more robust and user-friendly.

**Potential Issues**: If you were to omit these checks, users could access elements outside the bounds of the matrix, leading to undefined behavior. This could cause crashes or incorrect calculations elsewhere in the program.

### 4. Matrix Operations

#### Addition

**Concept**: The `add` method iterates through each element of the two matrices and sums corresponding elements.

**How It Works**: The method creates a new `Matrix` object to hold the results, using a nested loop to access each element.

**Why It Matters**: This straightforward implementation is intuitive and aligns with how matrix addition is defined mathematically.

**Potential Issues**: If you forget to check the dimensions of the two matrices before addition, you could end up with a runtime error or incorrect results. Always ensure both matrices are of the same size.

#### Multiplication

**Concept**: The `multiply` method implements matrix multiplication, which requires the number of columns in the first matrix to equal the number of rows in the second matrix.

**How It Works**: A nested loop structure calculates the dot product for each element in the resulting matrix.

**Why It Matters**: Matrix multiplication is not commutative, and understanding the relationship between dimensions is crucial for correct operations.

**Potential Issues**: If you accidentally try to multiply matrices with incompatible dimensions (e.g., a 2x3 matrix with a 2x2 matrix), you would either need to implement additional checks or risk undefined behavior. Proper checks or static assertions would help avoid such issues.

### 5. Determinant Calculation

**Concept**: The determinant function is recursively defined, especially for matrices larger than 2x2.

**How It Works**: It creates submatrices and applies the Laplace expansion, which is a classic method for calculating determinants.

**Why It Matters**: This method showcases the recursive nature of determinant calculation and emphasizes understanding matrix properties.

**Potential Issues**: Recursive implementations can lead to performance issues for large matrices due to excessive function calls and stack depth. An iterative approach or optimization techniques could be beneficial for larger matrices.

### 6. Transpose Operation

**Concept**: The `transpose` function switches rows and columns.

**How It Works**: It creates a new matrix and sets each element according to its transposed position.

**Why It Matters**: Transposing is a fundamental operation in linear algebra, relevant for various applications, such as solving systems of equations and transforming data.

**Potential Issues**: Forgetting to create a new matrix for the result could lead to data being overwritten. Always ensure that operations that alter dimensions return new matrix instances.
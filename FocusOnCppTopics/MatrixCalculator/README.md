# Matrix Operations using Templates

## Overview

This project implements a **Matrix class template** in C++, supporting common matrix operations such as addition, multiplication, transposition, and determinant calculation. The code demonstrates key C++ template concepts, offering flexibility to handle matrices of arbitrary sizes (rows and columns) and is entirely type-safe.

This project is designed to help beginners understand how template programming can be applied to real-world problems. Special attention is given to the `determinant` function, which showcases advanced template techniques such as **template specialization** and **recursive template instantiation**.

## Key Features
- **Matrix creation** for any dimension defined at compile-time using templates.
- **Matrix operations** including addition, multiplication, and transposition.
- **Determinant calculation** for square matrices, including specializations for `1x1` and `2x2` matrices.
- Detailed error handling for invalid operations such as index out of bounds or non-square matrices in determinant calculation.

---

## Getting Started

### Prerequisites
- C++11 or later is required for the code to compile.
- A compiler supporting C++ templates (e.g., GCC or Clang).

### Execution

You can compile the project using:
```bash
g++ -std=c++11 Matrix.cpp -o matrix
./matrix
```

### Usage

Here is an example of how you can use the `Matrix` class:
```cpp
Matrix<2, 3> mat1;
mat1.set(0, 0, 1);
mat1.set(0, 1, 2);
mat1.set(0, 2, 3);
mat1.set(1, 0, 4);
mat1.set(1, 1, 5);
mat1.set(1, 2, 6);

// Printing the matrix
mat1.print();
```

---

## Template Concepts Explained

### 1. Class Templates
The `Matrix` class is a **class template** that allows you to create matrices of any size, where the dimensions are provided as template parameters:
```cpp
template <size_t Rows, size_t Cols>
class Matrix {
    std::array<std::array<int, Cols>, Rows> data;
    // ...
};
```
Here, `Rows` and `Cols` are non-type template parameters that define the number of rows and columns in the matrix. This makes the `Matrix` class flexible for any matrix size, known at compile time.

### 2. Template Specialization
The **determinant** function is only meaningful for square matrices. Additionally, calculating the determinant is straightforward for `1x1` and `2x2` matrices, so we specialize the determinant function for these cases:

```cpp
template<>
long Matrix<1, 1>::determinant() const {
    return data[0][0];
}

template<>
long Matrix<2, 2>::determinant() const {
    return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}
```
**Why use specialization?**
- **Specialization** allows us to provide a specific implementation for certain template arguments. In this case, it simplifies the determinant calculation for `1x1` and `2x2` matrices.
- This avoids redundant code or unnecessary complexity, improving both readability and performance.

### 3. Recursive Template Instantiation for Determinant

**What is Recursive Template Instantiation?**

Recursive template instantiation occurs when a template function or class calls itself with modified template arguments. This concept is particularly powerful in C++ because it allows problems to be broken down into smaller, easier-to-solve parts—much like traditional recursion in programming.

#### Determinant Example

In the matrix determinant function, recursive template instantiation is used to calculate the determinant of a square matrix larger than `2x2`. The general determinant calculation is done by recursively reducing the matrix into smaller submatrices, eventually reaching the base case (`1x1` or `2x2` matrix), which has direct solutions.

**How does it work?**
1. **Base Case (1x1 and 2x2 matrices)**: We define specialized versions of the determinant for `1x1` and `2x2` matrices. These are simple cases where the determinant is straightforward to calculate.
    - `1x1` determinant: the only element in the matrix.
    - `2x2` determinant: `ad - bc` for a matrix of the form:
      ```
      | a b |
      | c d |
      ```

2. **Recursive Case for Larger Matrices**: For an `NxN` matrix (`N > 2`), the determinant is computed by expanding along the first row (Laplace expansion). For each element in the first row, a smaller submatrix is formed by removing that element's row and column. The process repeats until we reach the `1x1` or `2x2` base case.

Here's the recursive instantiation concept in action:
```cpp
template <size_t Rows, size_t Cols>
long Matrix<Rows, Cols>::determinant() const {
    static_assert(Rows == Cols, "Determinant is only defined for square matrices");

    long det = 0;
    for (size_t j = 0; j < Cols; ++j) {
        Matrix<Rows - 1, Cols - 1> submatrix;
        // Create submatrix by removing first row and j-th column
        for (size_t sub_i = 0; sub_i < Rows - 1; ++sub_i) {
            for (size_t sub_j = 0; sub_j < Cols - 1; ++sub_j) {
                submatrix.set(sub_i, sub_j, this->get(sub_i + 1, (sub_j >= j ? sub_j + 1 : sub_j)));
            }
        }
        // Recursive call to determinant
        det += (j % 2 == 0 ? 1 : -1) * this->get(0, j) * submatrix.determinant();
    }

    return det;
}
```

**Step-by-Step Breakdown**:
1. **Recursive Function Call**: In each recursive step, a smaller submatrix is formed and passed to the `determinant()` function recursively.
2. **Base Case Handling**: When the recursive call reaches a matrix size of `1x1` or `2x2`, the specialized versions of the determinant are invoked, ending the recursion.
3. **Recursive Expansion**: The result of each recursive call contributes to the final determinant calculation through the formula:
   \[
   \text{determinant} = \sum_{j=0}^{N-1} (-1)^j \times \text{matrix}[0][j] \times \text{determinant of submatrix}
   \]
   
This approach breaks down the problem recursively until it reaches a solvable base case. 

**Why use recursive template instantiation?**
- **Efficiency**: The recursion allows you to handle matrices of arbitrary size without hardcoding each case.
- **Compile-time guarantees**: Through `static_assert`, the recursion is only triggered for valid (square) matrices, reducing runtime errors.

### 4. Template Function Overloading

**What is Template Function Overloading?**

Template function overloading allows multiple template functions to be defined with the same name but with different parameter types or numbers of parameters. In this context, the compiler decides which version of the function to invoke based on the arguments passed at compile time.

#### Multiply Function Example

In the matrix multiplication function, we overload the template function based on the matrix dimensions. Matrix multiplication is only valid when the number of columns in the first matrix matches the number of rows in the second matrix. The template enforces this relationship by parameterizing the number of columns in the second matrix.

```cpp
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
```

In this case, `OtherCols` is a new template parameter for the number of columns in the second matrix. The `multiply` function:
- Takes another matrix as an argument where the number of rows equals the number of columns of the first matrix.
- Returns a result matrix where the dimensions are `Rows x OtherCols`.

**How does function overloading work here?**

Let's consider an example:

```cpp
Matrix<2, 3> mat1;
Matrix<3, 2> mat2;
Matrix<2, 2> result = mat1.multiply(mat2);
```

Here:
- `mat1` is a `2x3` matrix.
- `mat2` is a `3x2` matrix.
- The function signature `multiply(const Matrix<Cols, OtherCols>&)` will match `mat2` because the template parameters `Cols` (from `mat1`) and `Rows` (from `mat2`) are compatible.

The function template ensures at compile-time that the matrices can be multiplied, producing a `2x2` result. If you tried multiplying incompatible matrices (e.g., `2x3` with `2x3`), the compiler would catch this as an error, ensuring that the operation is mathematically valid.

**Why is this useful?**
- **Compile-time checking**: You can ensure matrix multiplication only occurs between compatible matrices, reducing the risk of runtime errors.
- **Code reuse**: The template allows you to write the multiplication logic once, yet apply it to any combination of compatible matrices, regardless of their dimensions.

**Beginners’ Tip**:
Think of template function overloading as a way to give a function different behaviors based on the types or number of arguments passed to it. C++ will figure out the correct version to use based on the types involved.

For larger square matrices (`n x n`, where `n > 2`), the determinant is computed using a **recursive expansion** along the first row. This is achieved by using recursive template instantiation:

```cpp
template <size_t Rows, size_t Cols>
long Matrix<Rows, Cols>::determinant() const {
    static_assert(Rows == Cols, "Determinant is only defined for square matrices");
    long det = 0;
    
    // Recursive expansion
    for (size_t j = 0; j < Cols; ++j) {
        Matrix<Rows - 1, Cols - 1> submatrix;
        // Fill the submatrix by excluding the current row and column
        for (size_t sub_i = 0; sub_i < Rows - 1; ++sub_i) {
            for (size_t sub_j = 0; sub_j < Cols - 1; ++sub_j) {
                submatrix.set(sub_i, sub_j, this->get(sub_i + 1, (sub_j >= j ? sub_j + 1 : sub_j)));
            }
        }
        det += (j % 2 == 0 ? 1 : -1) * data[0][j] * submatrix.determinant();
    }
    return det;
}
```
**How does this work?**
- This implementation is recursive and relies on the **minor matrix expansion** technique, where smaller submatrices are created by excluding a row and column at each step.
- The recursion continues until it hits a base case (`2x2` or `1x1` matrix), for which the specialized templates are called.
  
**Key Points**:
- `static_assert` is used to ensure that determinant calculation is only applied to square matrices.
- Each recursive call generates a smaller matrix and eventually reduces to cases handled by the template specializations.
The **multiply** function template accepts another matrix as a parameter, with a compile-time check that the matrices can be multiplied based on their dimensions:
```cpp
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
```
This function:
- Ensures matrices with compatible dimensions are multiplied.
- Uses the compile-time sizes `Rows`, `Cols`, and `OtherCols` to statically ensure correctness.
- The result is also a template matrix, allowing flexible dimensions for the resulting matrix.

### 5. Static Assertions
We use `static_assert` in several places to enforce **compile-time constraints**. For example, the determinant function uses this to ensure it is only called on square matrices:
```cpp
static_assert(Rows == Cols, "Determinant is only defined for square matrices");
```
This prevents misuse of the function, ensuring errors are caught early during compilation.

---

## Conclusion

This project showcases the power and flexibility of **templates in C++**. The matrix operations are statically checked, ensuring correctness at compile-time, while still providing a flexible framework to handle matrices of varying sizes. By using template specialization and recursion, the code efficiently handles different matrix operations without sacrificing readability.

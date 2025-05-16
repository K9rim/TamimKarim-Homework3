#include <iostream>
#include <memory>

template<typename T, size_t Rows, size_t Cols>
class Matrix {
private:
    std::unique_ptr<T[]> data;

public:
    Matrix() {
        data = std::make_unique<T[]>(Rows * Cols);
    }

    Matrix(const Matrix& other) {
        data = std::make_unique<T[]>(Rows * Cols);
        for (size_t i = 0; i < Rows * Cols; ++i)
            data[i] = other.data[i];
    }

    Matrix(Matrix&& other) noexcept {
        data = std::move(other.data);
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            data = std::make_unique<T[]>(Rows * Cols);
            for (size_t i = 0; i < Rows * Cols; ++i)
                data[i] = other.data[i];
        }
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other)
            data = std::move(other.data);
        return *this;
    }

    ~Matrix() {}

    T& operator()(size_t i, size_t j) {
        return data[i * Cols + j];
    }

    const T& operator()(size_t i, size_t j) const {
        return data[i * Cols + j];
    }

    Matrix operator+(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; ++i)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; ++i)
            result.data[i] = data[i] - other.data[i];
        return result;
    }

    template<size_t OtherCols>
    Matrix<T, Rows, OtherCols> operator*(const Matrix<T, Cols, OtherCols>& other) const {
        Matrix<T, Rows, OtherCols> result;
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < OtherCols; ++j) {
                T sum = T();
                for (size_t k = 0; k < Cols; ++k)
                    sum += (*this)(i, k) * other(k, j);
                result(i, j) = sum;
            }
        }
        return result;
    }
};

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Cols, Rows> transpose(Matrix<T, Rows, Cols>&& mat) {
    Matrix<T, Cols, Rows> result;
    for (size_t i = 0; i < Rows; ++i)
        for (size_t j = 0; j < Cols; ++j)
            result(j, i) = mat(i, j);
    return result;
}

int main() {
    Matrix<int, 3, 3> A;
    Matrix<int, 3, 3> B;

    int val = 1;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            A(i, j) = val;
            B(i, j) = val + 1;
            val++;
        }
    }

    Matrix<int, 3, 3> sum = A + B;
    Matrix<int, 3, 3> diff = B - A;
    Matrix<int, 3, 3> prod = A * B;

    auto At = transpose(std::move(A));

    auto trace = [](const auto& mat) {
        int t = 0;
        for (size_t i = 0; i < 3; ++i)
            t += mat(i, i);
        return t;
    };

    std::cout << "Trace of B: " << trace(B) << std::endl;

    return 0;
}

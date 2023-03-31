#include <vector>
#include <stdexcept>

template<typename T>
class Matrix {
public:
    Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows* cols) {}

    Matrix(const std::initializer_list<std::initializer_list<T>>& list)
        : rows_(list.size()), cols_(list.begin()->size()), data_(rows_* cols_)
    {
        int i = 0;
        for (const auto& row : list) {
            if (row.size() != cols_) {
                throw std::invalid_argument("Invalid initializer list.");
            }
            std::copy(row.begin(), row.end(), data_.begin() + i * cols_);
            ++i;
        }
    }

    T& operator()(int row, int col) {
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
            throw std::out_of_range("Matrix indices out of range.");
        }
        return data_[row * cols_ + col];
    }

    const T& operator()(int row, int col) const {
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
            throw std::out_of_range("Matrix indices out of range.");
        }
        return data_[row * cols_ + col];
    }

    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions do not match.");
        }
        Matrix result(rows_, cols_);
        for (int i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] + other.data_[i];
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions do not match.");
        }
        Matrix result(rows_, cols_);
        for (int i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] - other.data_[i];
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        //if (cols_ != other.rows_) {
        //    throw std::invalid_argument("Matrix dimensions do not match.");
        //}
        Matrix result(rows_, other.cols_);
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < other.cols_; ++j) {
                for (int k = 0; k < cols_; ++k) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }
        return result;
    }

    Matrix operator*(const T& scalar) const {
        Matrix result(rows_, cols_);
        for (int i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] * scalar;
        }
        return result;
    }

    Matrix operator/(const T& scalar) const {
        Matrix result(rows_, cols_);
        for (int i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] / scalar;
        }
        return result;
    }

    Matrix inverse() const {
        if (rows_ != cols_) {
            throw std::invalid_argument("Matrix must be square to find its inverse.");
        }
        Matrix inv(rows_, cols_);

        // Create an augmented matrix
        Matrix aug(rows_, cols_ * 2);
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                aug(i, j) = (*this)(i, j);
                aug(i, j + cols_) = (i == j) ? 1 : 0;
            }
        }

        // Perform row operations to transform the left half of the matrix into the identity matrix
        for (int i = 0; i < rows_; ++i) {
            // Find a pivot element for the current row
            int pivot_row = i;
            while (pivot_row < rows_ && aug(pivot_row, i) == 0) {
                ++pivot_row;
            }
            if (pivot_row == rows_) {
                throw std::runtime_error("Matrix is singular and has no inverse.");
            }
            if (pivot_row != i) {
                // Swap the current row with the pivot row
                for (int j = 0; j < cols_ * 2; ++j) {
                    std::swap(aug(i, j), aug(pivot_row, j));
                }
            }
            // Scale the pivot row to have a leading 1
            T pivot = aug(i, i);
            for (int j = 0; j < cols_ * 2; ++j) {
                aug(i, j) /= pivot;
            }
            // Use the pivot row to eliminate the pivot column in the other rows
            for (int j = 0; j < rows_; ++j) {
                if (j != i) {
                    T factor = aug(j, i);
                    for (int k = 0; k < cols_ * 2; ++k) {
                        aug(j, k) -= factor * aug(i, k);
                    }
                }
            }
        }

        // The right half of the augmented matrix now contains the inverse of the original matrix
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                inv(i, j) = aug(i, j + cols_);
            }
        }
        return inv;
    }

    int rows() { return rows_; }
    int cols() { return cols_; }

private:
    int rows_;
    int cols_;
    std::vector<T> data_;
};
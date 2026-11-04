#include "matrix.h"

namespace Math {
    Matrix::Matrix(size_t rows, size_t cols, float value) : rows(rows), cols(cols), data(rows*cols, value) {};
    Matrix::Matrix(size_t rows, size_t cols, std::vector<float>& data) : rows(rows), cols(cols), data(data) {};

    // Mathematical operator overrides

    Matrix Matrix::operator+(const Matrix& B) const {
        Matrix out(rows, cols);

        for (size_t i = 0; i < data.size(); i++) {
            out.data[i] = data[i] + B.data[i];
        }

        return out;
    }

    Matrix Matrix::operator+(const std::vector<float>& b) const {
        Matrix out(rows, cols);
        
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                out(i,j) = (*this)(i, j) + b[j];
            }
        }

        return out;
    }

    Matrix& Matrix::operator+=(const Matrix& B) {
        if (rows != B.getRows() || cols != B.getCols()) return *this;
        *this = (*this) + B;
        return *this;
    }

    Matrix& Matrix::operator+=(const std::vector<float>& b) {
        if (cols != b.size()) return *this;
        *this = (*this) + b;
        return *this;
    }

    Matrix Matrix::operator*(const Matrix& B) const {
        Matrix out(rows, B.getCols(), 0.0f);
        if (cols != B.getRows()) return out;

        size_t out_cols = B.getCols();

        for (int i = 0; i < static_cast<int>(rows); i++) {
            for (size_t k = 0; k < cols; k++) {
                float r_val = (*this)(i, k);
                for (size_t j = 0; j < out_cols; j++) {
                    out(i, j) += r_val * B(k, j);
                }
            }
        }

        return out;
    }

    Matrix Matrix::operator*(const float b) const {
        Matrix out(rows, cols);
        
        for (size_t i = 0; i < out.data.size(); i++) {
            out.data[i] = data[i] * b;
        }

        return out;
    }

    Matrix& Matrix::operator*=(const Matrix& B) {
        if (cols != B.getRows()) return *this;
        *this = (*this) * B;
        return *this;
    }

    Matrix& Matrix::operator*=(float b) {
        *this = (*this) * b;
        return *this;
    }

    // Access operator overrides

    float& Matrix::operator()(size_t r, size_t c) {
        return data[(r * cols) + c];
    }

    float Matrix::operator()(size_t r, size_t c) const {
        return data[(r * cols) + c];
    }

    void Matrix::fill(float value) {
        for (size_t i = 0; i < data.size(); i++) data[i] = value;
    }

    Matrix Matrix::transpose() const {
        Matrix out(cols, rows);

        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                out(j,i) += (*this)(i,j);
            }
        }

        return out;
    }

    std::vector<EigenPair> Matrix::get2x2EigenVectorsAndValues() const {
        std::vector<EigenPair> eigenData;
        if (rows != 2 || cols != 2) return eigenData;

        float a = 1.0f;
        float b = -data[0] - data[3];
        float c = (data[0] * data[3]) - (data[1] * data[2]);

        float det = b*b - 4*a*c;
        if (det < 0.0f) return eigenData; // Complex roots - spirals or circles

        float sqrtDet = std::sqrt(det);
        float lambda1 = (-b + sqrtDet) / (2.0f * a);
        float lambda2 = (-b - sqrtDet) / (2.0f * a);

        // Compute eigenvectors (A - lambda*I)v = 0
        EigenPair eigen1;
        EigenPair eigen2;

        eigen1.eigenValue = lambda1;
        eigen2.eigenValue = lambda2;

        if (std::abs(data[1]) > 0.0f) {
            eigen1.eigenVector[1] = (lambda1 - data[0]) / data[1];
            eigen2.eigenVector[1] = (lambda2 - data[0]) / data[1];
        } else if (std::abs(data[2]) > 0.0f) {
            eigen1.eigenVector[0] = (lambda1 - data[3]) / data[2];
            eigen2.eigenVector[0] = (lambda2 - data[3]) / data[2];
        }

        eigenData.push_back(eigen1);
        eigenData.push_back(eigen2);

        return eigenData;
    }
}
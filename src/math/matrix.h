#pragma once

#include "complex_number.h"

#include <vector>
#include <cmath>

namespace Math {

    struct EigenPair {
        ComplexNumber eigenVector[2] = {ComplexNumber(1.0f, 0.0f), ComplexNumber(1.0f, 0.0f)};
        ComplexNumber eigenValue = ComplexNumber(0.0f, 0.0f);
    };

    class Matrix {
        public:
            Matrix(size_t rows, size_t cols, float value = 0.0f);
            Matrix(size_t rows, size_t cols, std::vector<float>& data);

            size_t getRows() const { return rows; }
            size_t getCols() const { return cols; }

            // Mathematical operator overrides
            Matrix operator+(const Matrix& B) const;
            Matrix operator+(const std::vector<float>& b) const;

            Matrix& operator+=(const Matrix& B);
            Matrix& operator+=(const std::vector<float>& b);

            Matrix operator*(const Matrix& B) const;
            Matrix operator*(const float b) const;

            Matrix& operator*=(const Matrix& B);
            Matrix& operator*=(float b);

            // Access operator overrides
            float& operator()(size_t row, size_t col);
            float operator()(size_t row, size_t col) const;

            // Fills the matrix with a given value
            void fill(float value);

            // Returns the transpose of the matrix
            Matrix transpose() const;

            // Computes a 2x2 matrices eigenvalues and eigenvectors
            std::vector<EigenPair> get2x2EigenVectorsAndValues() const;

        private:
            size_t rows;
            size_t cols;

            std::vector<float> data;
    };
}
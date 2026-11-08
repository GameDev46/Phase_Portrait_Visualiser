#pragma once

#include <cmath>

namespace Math {
    class ComplexNumber {
        public:
            ComplexNumber(float real = 0.0f, float imaginary = 0.0f);

            static ComplexNumber sqrt(float a);

            float getRealComponent() const { return re; };
            float getImaginaryComponent() const { return img; };

            // Mathematical operator overrides
            ComplexNumber operator+(const ComplexNumber& c) const;
            ComplexNumber operator+(float b) const;

            ComplexNumber& operator+=(const ComplexNumber& c);
            ComplexNumber& operator+=(float b);

            ComplexNumber operator-(const ComplexNumber& c) const;
            ComplexNumber operator-(float b) const;

            ComplexNumber& operator-=(const ComplexNumber& c);
            ComplexNumber& operator-=(float b);

            ComplexNumber operator*(const ComplexNumber& c) const;
            ComplexNumber operator*(float b) const;

            ComplexNumber& operator*=(const ComplexNumber& c);
            ComplexNumber& operator*=(float b);

            ComplexNumber operator/(const ComplexNumber& c) const;
            ComplexNumber operator/(float b) const;

            ComplexNumber& operator/=(const ComplexNumber& c);
            ComplexNumber& operator/=(float b);

            // Returns the conjugate of the complex number
            ComplexNumber conjugate() const;

        private:
            float re = 0.0f;
            float img = 0.0f;
    };
}
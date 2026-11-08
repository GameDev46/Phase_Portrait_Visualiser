#include "complex_number.h"

namespace Math {
    ComplexNumber::ComplexNumber(float real, float imaginary) : re(real), img(imaginary) {}

    ComplexNumber ComplexNumber::sqrt(float a) {
        ComplexNumber out(0.0f, 0.0f);

        if (a < 0.0f) out.img = std::sqrt(-a);
        else out.re = std::sqrt(a);

        return out;
    }

    ComplexNumber ComplexNumber::operator+(const ComplexNumber& c) const {
        return ComplexNumber(re + c.re, img + c.img);
    };

    ComplexNumber ComplexNumber::operator+(float b) const {
        return ComplexNumber(re + b, img);
    };

    ComplexNumber& ComplexNumber::operator+=(const ComplexNumber& c) {
        *this = (*this) + c;
        return *this;
    };

    ComplexNumber& ComplexNumber::operator+=(float b) {
        *this = (*this) + b;
        return *this;
    };

    ComplexNumber ComplexNumber::operator-(const ComplexNumber& c) const {
        return ComplexNumber(re - c.re, img - c.img);
    };

    ComplexNumber ComplexNumber::operator-(float b) const {
        return ComplexNumber(re - b, img);
    };

    ComplexNumber& ComplexNumber::operator-=(const ComplexNumber& c) {
        *this = (*this) - c;
        return *this;
    };

    ComplexNumber& ComplexNumber::operator-=(float b) {
        *this = (*this) - b;
        return *this;
    };

    ComplexNumber ComplexNumber::operator*(const ComplexNumber& c) const {
        return ComplexNumber((re * c.re) - (img * c.img), (re * c.img) + (img * c.re));;
    };

    ComplexNumber ComplexNumber::operator*(float b) const {
        return ComplexNumber(re * b, img * b);;
    };

    ComplexNumber& ComplexNumber::operator*=(const ComplexNumber& c) {
        *this = (*this) * c;
        return *this;
    };

    ComplexNumber& ComplexNumber::operator*=(float b) {
        *this = (*this) * b;
        return *this;
    };

    ComplexNumber ComplexNumber::operator/(const ComplexNumber& c) const {
        ComplexNumber out(0.0f, 0.0f);
        if (c.re == 0 && c.img == 0) return out;

        float cMagSqr = (c.re * c.re) + (c.img * c.img);
        out.re = ((re * c.re) + (img * c.img)) / cMagSqr;
        out.img = ((img * c.re) - (re * c.img)) / cMagSqr;

        return out;
    };

    ComplexNumber ComplexNumber::operator/(float b) const {
        ComplexNumber out(0.0f, 0.0f);
        if (b == 0) return out;

        out.re = re / b;
        out.img = img / b;
        return out;
    };

    ComplexNumber& ComplexNumber::operator/=(const ComplexNumber& c) {
        *this = (*this) / c;
        return *this;
    };

    ComplexNumber& ComplexNumber::operator/=(float b) {
        *this = (*this) / b;
        return *this;
    };

    ComplexNumber ComplexNumber::conjugate() const {
        ComplexNumber out(re, -img);
        return out;
    };
}
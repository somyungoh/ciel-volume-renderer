#pragma once

#include "vector.h"

namespace ciel {

class Matrix
{
public:
    Matrix()
    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m[i][j] = 0;
            }
        }
    }
    ~Matrix() = default;

    Matrix(const double value)
    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m[i][j] = value;
            }
        }
    }

    Matrix(const Matrix &v)
    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m[i][j] = v.m[i][j];
            }
        }
    }

    Matrix(const double v[3][3])
    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m[i][j] = v[i][j];
            }
        }
    }

    Matrix(const double m00,
           const double m01,
           const double m02,
           const double m10,
           const double m11,
           const double m12,
           const double m20,
           const double m21,
           const double m22)
    {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
    }

    Matrix(const Vector r0, const Vector r1, const Vector r2)
    {
        m[0][0] = r0[0];
        m[0][1] = r0[1];
        m[0][2] = r0[2];
        m[1][0] = r1[0];
        m[1][1] = r1[1];
        m[1][2] = r1[2];
        m[2][0] = r2[0];
        m[2][1] = r2[1];
        m[2][2] = r2[2];
    }

    const double &operator()(const int a, const int b) const { return m[a][b]; }

    const Matrix        operator+(const Matrix &v) const;
    const Matrix        operator-(const Matrix &v) const;
    friend const Matrix operator-(const Matrix &v); // unary minus;

    friend const Matrix operator*(const double w, const Matrix &v);
    const Matrix        operator*(const double v) const;
    const Matrix        operator/(const double v) const;

    const Matrix operator*(const Matrix &v) const;

    Matrix &operator=(const Matrix &v);
    Matrix &operator+=(const Matrix &v);
    Matrix &operator-=(const Matrix &v);
    Matrix &operator*=(const double v);
    Matrix &operator*=(const Matrix &m);
    Matrix &operator/=(const double v);

    // Special operators

    //! anticommutator m && n = mn + nm
    const Matrix operator&&(const Matrix &m) const;

    //! commutator  m || n = mn - nm
    const Matrix operator||(const Matrix &m) const;

    //  Comparisons

    // These two ( == and != ) compare element by element
    bool operator==(const Matrix &v) const;
    bool operator!=(const Matrix &v) const;

    // These compare the value of the determinant
    bool operator<(const Matrix &v) const;
    bool operator<=(const Matrix &v) const;
    bool operator>(const Matrix &v) const;
    bool operator>=(const Matrix &v) const;

    // Computations

    const Matrix inverse() const;
    const Matrix transpose() const;
    double       det() const;
    double       trace() const;
    const Matrix exp() const;

    double Get(const int a, const int b) const { return m[a][b]; }
    void Set(const int a, const int b, const double value) { m[a][b] = value; }

private:
    double m[3][3];

    double cofactor(const int i, const int j) const;
};

const Matrix operator-(const Matrix &m);
const Matrix operator*(const double w, const Matrix &v);

} // namespace ciel

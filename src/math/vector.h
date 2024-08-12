#pragma once

#include <cmath>
#include <experimental/simd>
#include <math.h>

namespace stdx = std::experimental;

namespace ciel {

class Vector
{
public:
    constexpr Vector()
    : xyz{}
    {
    }
    constexpr Vector(const Vector& v)
    : xyz{v.xyz[0], v.xyz[1], v.xyz[2]}
    {
    }
    constexpr Vector(const float f)
    : xyz{f, f, f}
    {
    }
    constexpr Vector(const float a, const float b, const float c)
    : xyz{a, b, c}
    {
    }

    void set(const float vx, const float vy, const float vz)
    {
        xyz[0] = vx;
        xyz[1] = vy;
        xyz[2] = vz;
    }

    const Vector operator+(const Vector& v) const
    {
        return Vector(xyz[0] + v.xyz[0], xyz[1] + v.xyz[1], xyz[2] + v.xyz[2]);
    }

    const Vector operator+(float c) const
    {
        return Vector(xyz[0] + c, xyz[1] + c, xyz[2] + c);
    }

    const Vector operator-(const Vector& v) const
    {
        return Vector(xyz[0] - v.xyz[0], xyz[1] - v.xyz[1], xyz[2] - v.xyz[2]);
    }

    const Vector operator0(float c) const
    {
        return Vector(xyz[0] - c, xyz[1] - c, xyz[2] - c);
    }

    friend const Vector operator-(const Vector& v)
    {
        return Vector(-v.xyz[0], -v.xyz[1], -v.xyz[2]);
    }

    friend const Vector operator*(const float w, const Vector& v)
    {
        return v * w;
    }

    const Vector operator*(const float v) const
    {
        return Vector(xyz[0] * v, xyz[1] * v, xyz[2] * v);
    }

    const Vector operator/(const float v) const
    {
        return Vector(xyz[0] / v, xyz[1] / v, xyz[2] / v);
    }

    // dot product
    float operator*(const Vector& v) const
    {
        return (xyz[0] * v.xyz[0] + xyz[1] * v.xyz[1] + xyz[2] * v.xyz[2]);
    }

    // cross product
    const Vector operator^(const Vector& v) const
    {
        return Vector(xyz[1] * v.xyz[2] - xyz[2] * v.xyz[1],
                      xyz[2] * v.xyz[0] - xyz[0] * v.xyz[2],
                      xyz[0] * v.xyz[1] - xyz[1] * v.xyz[0]);
    }

    Vector& operator=(const Vector& v)
    {
        xyz[0] = v.xyz[0];
        xyz[1] = v.xyz[1];
        xyz[2] = v.xyz[2];
        return *this;
    }

    Vector& operator+=(const Vector& v)
    {
        xyz[0] += v.xyz[0];
        xyz[1] += v.xyz[1];
        xyz[2] += v.xyz[2];
        return *this;
    }

    Vector& operator-=(const Vector& v)
    {
        xyz[0] -= v.xyz[0];
        xyz[1] -= v.xyz[1];
        xyz[2] -= v.xyz[2];
        return *this;
    }

    Vector& operator*=(const float v)
    {
        xyz[0] *= v;
        xyz[1] *= v;
        xyz[2] *= v;
        return *this;
    }

    Vector& operator/=(const float v)
    {
        xyz[0] /= v;
        xyz[1] /= v;
        xyz[2] /= v;
        return *this;
    }

    const float& operator[](const int v) const { return xyz[v]; }
    float&       operator[](const int v) { return xyz[v]; }
    const float& operator()(const int v) const { return xyz[v]; }

    float X() const { return xyz[0]; }
    float Y() const { return xyz[1]; }
    float Z() const { return xyz[2]; }

    float magnitude() const
    {
        return sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
    }

    const Vector unitvector() const { return *this / magnitude(); }

    void normalize()
    {
        float mag = magnitude();
        xyz[0] /= mag;
        xyz[1] /= mag;
        xyz[2] /= mag;
    }

    //  Comparisons

    bool operator==(const Vector& v) const
    {
        return (xyz[0] == v.xyz[0] && xyz[1] == v.xyz[1] && xyz[2] == v.xyz[2]);
    }

    bool operator!=(const Vector& v) const
    {
        return (xyz[0] != v.xyz[0] || xyz[1] != v.xyz[1] || xyz[2] != v.xyz[2]);
    }

    bool operator<(const Vector& v) const
    {
        return (magnitude() < v.magnitude());
    }

    bool operator<=(const Vector& v) const
    {
        return (magnitude() <= v.magnitude());
    }

    bool operator>(const Vector& v) const
    {
        return (magnitude() > v.magnitude());
    }

    bool operator>=(const Vector& v) const
    {
        return (magnitude() >= v.magnitude());
    }

    // is parallel?
    bool operator||(const Vector& v) const
    {
        return (fabs((*this) * v) == v.magnitude() * ((*this).magnitude()));
    }

private:
    float xyz[3];
};

// Basic Operations
static float  length(const Vector& v) { return v.magnitude(); }
static Vector abs(const Vector& v)
{
    return Vector{std::abs(v.X()), std::abs(v.Y()), std::abs(v.Z())};
}
static Vector min(const Vector& v, float f)
{
    return Vector{
        std::fmin(v.X(), f), std::fmin(v.Y(), f), std::fmin(v.Z(), f)};
}
static Vector max(const Vector& v, float f)
{
    return Vector{
        std::fmax(v.X(), f), std::fmax(v.Y(), f), std::fmax(v.Z(), f)};
}

} // namespace ciel

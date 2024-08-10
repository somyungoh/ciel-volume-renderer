#pragma once

namespace ciel {

class Color
{
public:
    constexpr Color()
    : xyzw{}
    {
    }

    constexpr Color(const Color &v)
    : xyzw{v.xyzw[0], v.xyzw[1], v.xyzw[2], v.xyzw[3]}
    {
    }

    constexpr Color(const float a, const float b, const float c, const float d)
    : xyzw{a, b, c, d}
    {
    }

    void set(const float vx, const float vy, const float vz, const float vw)
    {
        xyzw[0] = vx;
        xyzw[1] = vy;
        xyzw[2] = vz;
        xyzw[3] = vw;
    }

    const Color operator+(const Color &v) const
    {
        return Color(xyzw[0] + v.xyzw[0],
                     xyzw[1] + v.xyzw[1],
                     xyzw[2] + v.xyzw[2],
                     xyzw[3] + v.xyzw[3]);
    }

    const Color operator-(const Color &v) const
    {
        return Color(xyzw[0] - v.xyzw[0],
                     xyzw[1] - v.xyzw[1],
                     xyzw[2] - v.xyzw[2],
                     xyzw[3] - v.xyzw[3]);
    }

    friend const Color operator-(const Color &v)
    {
        return Color(-v.xyzw[0], -v.xyzw[1], -v.xyzw[2], -v.xyzw[3]);
    }

    friend const Color operator*(const float w, const Color &v)
    {
        return v * w;
    }

    const Color operator*(const float v) const
    {
        return Color(xyzw[0] * v, xyzw[1] * v, xyzw[2] * v, xyzw[3] * v);
    }

    const Color operator/(const float v) const
    {
        return Color(xyzw[0] / v, xyzw[1] / v, xyzw[2] / v, xyzw[3] / v);
    }

    //! component product
    const Color operator*(const Color &v) const
    {
        return Color(xyzw[0] * v.xyzw[0],
                     xyzw[1] * v.xyzw[1],
                     xyzw[2] * v.xyzw[2],
                     xyzw[3] * v.xyzw[3]);
    }

    Color &operator=(const Color &v)
    {
        xyzw[0] = v.xyzw[0];
        xyzw[1] = v.xyzw[1];
        xyzw[2] = v.xyzw[2];
        xyzw[3] = v.xyzw[3];
        return *this;
    }

    Color &operator+=(const Color &v)
    {
        xyzw[0] += v.xyzw[0];
        xyzw[1] += v.xyzw[1];
        xyzw[2] += v.xyzw[2];
        xyzw[3] += v.xyzw[3];
        return *this;
    }

    Color &operator-=(const Color &v)
    {
        xyzw[0] -= v.xyzw[0];
        xyzw[1] -= v.xyzw[1];
        xyzw[2] -= v.xyzw[2];
        xyzw[3] -= v.xyzw[3];
        return *this;
    }

    Color &operator*=(const float v)
    {
        xyzw[0] *= v;
        xyzw[1] *= v;
        xyzw[2] *= v;
        xyzw[3] *= v;
        return *this;
    }

    Color &operator/=(const float v)
    {
        xyzw[0] /= v;
        xyzw[1] /= v;
        xyzw[2] /= v;
        xyzw[3] /= v;
        return *this;
    }

    float  operator[](const int v) const { return xyzw[v]; }
    float &operator[](const int v) { return xyzw[v]; }
    float  operator()(const int v) const { return xyzw[v]; }

    float X() const { return xyzw[0]; }
    float Y() const { return xyzw[1]; }
    float Z() const { return xyzw[2]; }
    float W() const { return xyzw[3]; }

    //  Comparisons

    bool operator==(const Color &v) const
    {
        return (xyzw[0] == v.xyzw[0] && xyzw[1] == v.xyzw[1] &&
                xyzw[2] == v.xyzw[2] && xyzw[3] == v.xyzw[3]);
    }

    bool operator!=(const Color &v) const
    {
        return (xyzw[0] != v.xyzw[0] || xyzw[1] != v.xyzw[1] ||
                xyzw[2] != v.xyzw[2] || xyzw[3] != v.xyzw[3]);
    }

private:
    float xyzw[4];
};

} // namespace ciel

#pragma once

namespace ciel {

class Color
{
public:
    Color() { xyzw[0] = xyzw[1] = xyzw[2] = xyzw[3] = 0; }
    ~Color() = default;

    Color(const Color &v)
    {
        xyzw[0] = v.xyzw[0];
        xyzw[1] = v.xyzw[1];
        xyzw[2] = v.xyzw[2];
        xyzw[3] = v.xyzw[3];
    }

    Color(const double a, const double b, const double c, const double d)
    {
        xyzw[0] = a;
        xyzw[1] = b;
        xyzw[2] = c;
        xyzw[3] = d;
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

    friend const Color operator*(const double w, const Color &v)
    {
        return v * w;
    }

    const Color operator*(const double v) const
    {
        return Color(xyzw[0] * v, xyzw[1] * v, xyzw[2] * v, xyzw[3] * v);
    }

    const Color operator/(const double v) const
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

    Color &operator*=(const double v)
    {
        xyzw[0] *= v;
        xyzw[1] *= v;
        xyzw[2] *= v;
        xyzw[3] *= v;
        return *this;
    }

    Color &operator/=(const double v)
    {
        xyzw[0] /= v;
        xyzw[1] /= v;
        xyzw[2] /= v;
        xyzw[3] /= v;
        return *this;
    }

    const double &operator[](const int v) const { return xyzw[v]; }
    double       &operator[](const int v) { return xyzw[v]; }
    const double &operator()(const int v) const { return xyzw[v]; }

    const double X() const { return xyzw[0]; }
    const double Y() const { return xyzw[1]; }
    const double Z() const { return xyzw[2]; }
    const double W() const { return xyzw[3]; }

    //  Comparisons

    const bool operator==(const Color &v) const
    {
        return (xyzw[0] == v.xyzw[0] && xyzw[1] == v.xyzw[1] &&
                xyzw[2] == v.xyzw[2] && xyzw[3] == v.xyzw[3]);
    }

    const bool operator!=(const Color &v) const
    {
        return (xyzw[0] != v.xyzw[0] || xyzw[1] != v.xyzw[1] ||
                xyzw[2] != v.xyzw[2] || xyzw[3] != v.xyzw[3]);
    }

private:
    double xyzw[4];
};

} // namespace ciel

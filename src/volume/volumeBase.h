#pragma once

#include <memory> // shared_ptr

namespace ciel {

class Vector;
class Matrix;
class Color;

template<typename T>
struct DxDyType
{
    typedef int _DxDyType;
};

template<>
struct DxDyType<float>
{
    typedef Vector _DxDyType;
};

template<>
struct DxDyType<Vector>
{
    typedef Matrix _DxDyType;
};

template<typename T>
class VolumeBase
{
public:
    using volumeDataType = T;
    using volumeDxDyType = typename DxDyType<T>::_DxDyType;
    using Ptr = std::shared_ptr<VolumeBase<volumeDataType>>;
    using ConstPtr = std::shared_ptr<const VolumeBase<volumeDataType>>;

    virtual volumeDataType eval([[maybe_unused]] const Vector &p) const
    {
        volumeDataType base{};
        return base;
    }
    virtual volumeDxDyType dxdy([[maybe_unused]] const Vector &p) const
    {
        volumeDxDyType base{};
        return base;
    }

    static Ptr create()
    {
        return std::make_shared<VolumeBase<volumeDataType>>();
    };
    static ConstPtr createConst()
    {
        return std::make_shared<const VolumeBase<volumeDataType>>();
    };
};

// type definitions
using VolumeScalar = VolumeBase<float>;
using VolumeVector = VolumeBase<Vector>;
using VolumeColor = VolumeBase<Color>;
using VolumeMatrix = VolumeBase<Matrix>;

// typedef std::shared_ptr<VolumeBase<float>>  VolumeScalarPtr;
// typedef std::shared_ptr<VolumeBase<Color>>  VolumeColorPtr;
// typedef std::shared_ptr<VolumeBase<Vector>> VolumeVectorPtr;
// typedef std::shared_ptr<VolumeBase<Matrix>> VolumeMatrixPtr;

} // namespace ciel

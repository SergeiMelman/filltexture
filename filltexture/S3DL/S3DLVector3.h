#pragma once

#include "S3DLVector2.h"

template<typename T>
struct S3DLVector3T : public S3DLVector2T<T>
{
    union
    {
        T b;
        T z;
    };

    // constructor
    S3DLVector3T()
        : S3DLVector2T<T>()
        , z(0)
    {
    }

    S3DLVector3T(T _x, T _y, T _z)
        : S3DLVector2T<T>(_x, _y)
        , z(_z)
    {
    }

    S3DLVector3T(const S3DLVector3T<T> & v)
        : S3DLVector2T<T>(v.x, v.y)
        , z(v.z)
    {
    }

    // multiplication by scalar
    S3DLVector3T<T> operator*(const T & scalar) const
    {
        return {this->x * scalar, this->y * scalar, this->z * scalar};
    }

    // addition
    S3DLVector3T<T> operator+(const S3DLVector3T<T> & other) const
    {
        return {this->x + other.x, this->y + other.y, this->z + other.z};
    }
};

typedef S3DLVector3T<float> S3DLVector3;
typedef S3DLVector3T<unsigned char> S3DLRGB;
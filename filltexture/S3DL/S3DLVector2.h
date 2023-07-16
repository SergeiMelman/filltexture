#pragma once

template<typename T>
struct S3DLVector2T
{
    union
    {
        T r;
        T x;
    };
    union
    {
        T g;
        T y;
    };
    // constructor
    S3DLVector2T()
        : x(0)
        , y(0)
    {
    }
    S3DLVector2T(T _x, T _y)
        : x(_x)
        , y(_y)
    {
    }
    S3DLVector2T(const S3DLVector2T<T> & v)
        : x(v.x)
        , y(v.y)
    {
    }
    // copy operator
    S3DLVector2T<T> & operator=(const S3DLVector2T<T> & v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }
};

typedef S3DLVector2T<float> S3DLVector2;
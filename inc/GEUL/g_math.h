/*
* Simple 3d Math
* Copyright (C) 2024 by: apartfromtime
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _G_MATH_H_
#define _G_MATH_H_


//-----------------------------------------------------------------------------
//
// Math functions.
//
// Includes:
//
// Color
// Rectangle
// Vector2
// Vector3
// Vector4
// Plane
// Matrix4
// Viewport
//
//-----------------------------------------------------------------------------


#include <stdint.h>
#include <float.h>
#include <math.h>

#ifndef M_PI
#define M_PI        3.14159265f
#endif

#ifndef H_PI
#define H_PI        1.57079633f
#endif

#define FLOOR(a)            ((a)>0?(int32_t)(a):-(int32_t)(-a))
#define ROUND(a)            (fabs(ceil(a)-a) > 0.5 ? ceil(a) : floor(a))
#define CLAMP(x, min, max)  (x < min ? min : x > max ? max : x)
#define ABS(a)              (((a) < 0) ? -(a) : (a))
#define SIGN(x)             (x >= 0 ? 1 : -1)
#define MIN(a, b)           (a < b ? a : b)
#define MAX(a, b)           (a > b ? a : b)
#define DEG2RAD(x)          (x * (( float )(M_PI) / 180.0f))
#define RAD2DEG(x)          (x * (180.0f / ( float )(M_PI)))


//-----------------------------------------------------------------------------
//
// Type definitions
//
//-----------------------------------------------------------------------------


// BYTE1
typedef unsigned char byte1_t;

// BYTE2
typedef struct _byte2
{
    union
    {
        struct
        {
            uint16_t n;
        };

        struct
        {
            uint8_t n0;
            uint8_t n1;
        };
    };
} byte2_t;

// BYTE3
typedef struct _byte3
{
    union
    {
        struct
        {
            uint32_t n;
        };

        struct
        {
            uint8_t n0;
            uint8_t n1;
            uint8_t n2;
            uint8_t pad;
        };
    };
} byte3_t;

// BYTE4
typedef struct _byte4
{
    union
    {
        struct
        {
            uint32_t n;
        };

        struct
        {
            uint8_t n0;
            uint8_t n1;
            uint8_t n2;
            uint8_t n3;
        };
    };
} byte4_t;

// COLOR
typedef struct _color
{
    float r;
    float g;
    float b;
    float a;
} color_t;

// RECTANGLE
typedef struct _rect
{
    long min[2];
    long max[2];
} rect_t;

// VECTOR2
typedef struct _vector2
{
    float x;
    float y;
} vector2_t;

// VECTOR3
typedef struct _vector3
{
    float x;
    float y;
    float z;
    float pad;
} vector3_t;

// VECTOR4
typedef struct _vector4
{
    float x;
    float y;
    float z;
    float w;
} vector4_t;

// MATRIX4
typedef struct _matrix4_t
{
    float n[16];
} matrix4_t;

// PLANE
typedef struct _plane
{
    float a;
    float b;
    float c;
    float d;
} plane_t;

// VIEWPORT
typedef struct _viewport
{
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
    float minZ;
    float maxZ;
} viewport_t;


//-----------------------------------------------------------------------------
//
// Type declarations
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//
// COLOR
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// creates a color
//-----------------------------------------------------------------------------
inline color_t Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
{
    color_t c = {};

    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;

    return c;
}

//-----------------------------------------------------------------------------
// adds two color values together to create a new color value.
//-----------------------------------------------------------------------------
inline color_t AddColor(const color_t c0, const color_t c1)
{
    color_t c = Color();

    c.r = c0.r + c1.r;
    c.g = c0.g + c1.g;
    c.b = c0.b + c1.b;
    c.a = c0.a + c1.a;

    c.r = CLAMP(c.r, 0.0f, 1.0f);
    c.g = CLAMP(c.g, 0.0f, 1.0f);
    c.b = CLAMP(c.b, 0.0f, 1.0f);
    c.a = CLAMP(c.a, 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// adjusts the contrast value of a color.
//-----------------------------------------------------------------------------
inline color_t AdjustColorContrast(const color_t color, float contrast)
{
    color_t c = Color();

    c.r = CLAMP(0.5f + (contrast * (color.r - 0.5f)), 0.0f, 1.0f);
    c.g = CLAMP(0.5f + (contrast * (color.g - 0.5f)), 0.0f, 1.0f);
    c.b = CLAMP(0.5f + (contrast * (color.b - 0.5f)), 0.0f, 1.0f);
    c.a = color.a;

    return c;
}

//-----------------------------------------------------------------------------
// adjusts the saturation value of a color.
//-----------------------------------------------------------------------------
inline color_t AdjustColorSaturation(const color_t color, float saturation)
{
    color_t c = Color();

    // Approximate values for each component's contribution to luminance.
    // Based upon the NTSC standard described in ITU-R Recommendation BT.709.
    const float luminance = color.r * 0.2125f + color.g * 0.7154f + color.b * 0.0721f;

    c.r = CLAMP(luminance + (saturation * (color.r - luminance)), 0.0f, 1.0f);
    c.g = CLAMP(luminance + (saturation * (color.g - luminance)), 0.0f, 1.0f);
    c.b = CLAMP(luminance + (saturation * (color.b - luminance)), 0.0f, 1.0f);
    c.a = color.a;

    return c;
}

//-----------------------------------------------------------------------------
// uses linear interpolation to create a color value.
//-----------------------------------------------------------------------------
inline color_t InterpolateColor(const color_t c0, const color_t c1, float s)
{
    color_t c = Color();

    c.r = CLAMP(c0.r + s * (c1.r - c0.r), 0.0f, 1.0f);
    c.g = CLAMP(c0.g + s * (c1.g - c0.g), 0.0f, 1.0f);
    c.b = CLAMP(c0.b + s * (c1.b - c0.b), 0.0f, 1.0f);
    c.a = CLAMP(c0.a + s * (c1.a - c0.a), 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// blends two colors.
//-----------------------------------------------------------------------------
inline color_t ModulateColor(const color_t c0, const color_t c1)
{
    color_t c = Color();

    c.r = CLAMP(c0.r * c1.r, 0.0f, 1.0f);
    c.g = CLAMP(c0.g * c1.g, 0.0f, 1.0f);
    c.b = CLAMP(c0.b * c1.b, 0.0f, 1.0f);
    c.a = CLAMP(c0.a * c1.a, 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// creates the negative color value of a color value.
//-----------------------------------------------------------------------------
inline color_t NegateColor(const color_t c0)
{
    color_t c = Color();

    c.r = CLAMP(1.0f - c0.r, 0.0f, 1.0f);
    c.g = CLAMP(1.0f - c0.g, 0.0f, 1.0f);
    c.b = CLAMP(1.0f - c0.b, 0.0f, 1.0f);
    c.a = CLAMP(1.0f - c0.a, 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// scales a color value.
//-----------------------------------------------------------------------------
inline color_t ScaleColor(const color_t c0, float s)
{
    color_t c = Color();

    c.r = CLAMP(c0.r * s, 0.0f, 1.0f);
    c.g = CLAMP(c0.g * s, 0.0f, 1.0f);
    c.b = CLAMP(c0.b * s, 0.0f, 1.0f);
    c.a = CLAMP(c0.a * s, 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// subtracts two color values to create a new color value.
//-----------------------------------------------------------------------------
inline color_t SubtractColor(const color_t c0, const color_t c1)
{
    color_t c = Color();

    c.r = c0.r - c1.r;
    c.g = c0.g - c1.g;
    c.b = c0.b - c1.b;
    c.a = c0.a - c1.a;

    c.r = CLAMP(c.r, 0.0f, 1.0f);
    c.g = CLAMP(c.g, 0.0f, 1.0f);
    c.b = CLAMP(c.b, 0.0f, 1.0f);
    c.a = CLAMP(c.a, 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// returns color in rgba format.
//-----------------------------------------------------------------------------
inline byte4_t RGBAColor(const color_t& color) noexcept
{
    byte4_t c = {};
    
    c.n0 = (uint8_t)(CLAMP(color.r, 0.0f, 1.0f) * 255.0f);
    c.n1 = (uint8_t)(CLAMP(color.g, 0.0f, 1.0f) * 255.0f);
    c.n2 = (uint8_t)(CLAMP(color.b, 0.0f, 1.0f) * 255.0f);
    c.n3 = (uint8_t)(CLAMP(color.a, 0.0f, 1.0f) * 255.0f);

    return c;
}

//-----------------------------------------------------------------------------
// returns color in bgra format.
//-----------------------------------------------------------------------------
inline byte4_t BGRAColor(const color_t& color) noexcept
{
    byte4_t c = {};

    c.n0 = (uint8_t)(CLAMP(color.b, 0.0f, 1.0f) * 255.0f);
    c.n1 = (uint8_t)(CLAMP(color.g, 0.0f, 1.0f) * 255.0f);
    c.n2 = (uint8_t)(CLAMP(color.r, 0.0f, 1.0f) * 255.0f);
    c.n3 = (uint8_t)(CLAMP(color.a, 0.0f, 1.0f) * 255.0f);

    return c;
}


//-----------------------------------------------------------------------------
//
// RECTANGLE
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructs a rectangle
//-----------------------------------------------------------------------------
inline rect_t Rectangle(long x0 = 0, long y0 = 0, long x1 = 0, long y1 = 0)
{
    rect_t r = {};

    r.min[0] = x0;
    r.min[1] = y0;
    r.max[0] = x1;
    r.max[1] = y1;

    return r;
}

//-----------------------------------------------------------------------------
// tests to see if rectangles intersect
//-----------------------------------------------------------------------------
inline bool IntersectsRectangle(const rect_t& r0, const rect_t& r1)
{
    const long minX = r0.max[0] < r1.max[0] ? r0.max[0] : r1.max[0];
    const long minY = r0.max[1] < r1.max[1] ? r0.max[1] : r1.max[1];
    const long maxX = r0.min[0] > r1.min[1] ? r0.min[0] : r1.min[0];
    const long maxY = r0.min[1] > r1.min[1] ? r0.min[1] : r1.min[1];

    if (minX > maxX && minY > maxY)
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
// returns true if point is outside the specified rectangle
//-----------------------------------------------------------------------------
inline bool ContainsRectangle(const rect_t& rect, long x, long y)
{
    if ((rect.min[0] <= x && x < rect.max[0]) &&
        (rect.min[1] <= y && y < rect.max[1]))
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
// returns true if point is outside the specified rectangle
//-----------------------------------------------------------------------------
inline bool OutsideRectangle(const rect_t& rect, long x, long y)
{
    if (x < rect.min[0] || x > rect.max[0] ||
        y < rect.min[1] || y > rect.max[1])
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
// decreases the rectangle by a specified amount in x and y
//-----------------------------------------------------------------------------
inline void DeflateRectangle(rect_t& rect, long h, long v)
{
    rect.min[0] += h >> 1;
    rect.min[1] += v >> 1;
    rect.max[0] -= h >> 1;
    rect.max[1] -= v >> 1;
}

//-----------------------------------------------------------------------------
// increases the rectangle by a specified amount in x and y
//-----------------------------------------------------------------------------
inline void InflateRectangle(rect_t& rect, long h, long v)
{
    rect.min[0] -= h >> 1;
    rect.min[1] -= v >> 1;
    rect.max[0] += h >> 1;
    rect.max[1] += v >> 1;
}

//-----------------------------------------------------------------------------
// moves or offsets the rectangle by moving the upper-left position
//-----------------------------------------------------------------------------
inline void OffsetRectangle(rect_t& rect, long x, long y)
{
    rect.min[0] += x;
    rect.min[1] += y;
    rect.max[0] += x;
    rect.max[1] += y;
}


//-----------------------------------------------------------------------------
//
// VECTOR4
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructs a 4d vector
//-----------------------------------------------------------------------------
inline vector4_t Vector4(float x = 0, float y = 0, float z = 0,
    float w = 0)
{
    vector4_t v = {};

    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;

    return v;
}

//-----------------------------------------------------------------------------
// add two 4d vectors (a + b)
//-----------------------------------------------------------------------------
inline vector4_t AddVector4(vector4_t a, vector4_t b)
{
    vector4_t v = Vector4();

    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = a.z + b.z;
    v.w = a.w + b.w;

    return v;
}

//-----------------------------------------------------------------------------
// returns a point in barycentric coordinates, using the specified 4d vectors.
//-----------------------------------------------------------------------------
inline vector4_t BaryCentricVector4(const vector4_t a, const vector4_t b,
    const vector4_t c, float f, float g)
{
    vector4_t v = Vector4();

    v.x = a.x + f * (b.x - a.x) + g * (c.x - a.x);
    v.y = a.y + f * (b.y - a.y) + g * (c.y - a.y);
    v.z = a.z + f * (b.z - a.z) + g * (c.z - a.z);
    v.w = a.w + f * (b.w - a.w) + g * (c.w - a.w);

    return v;
}

//-----------------------------------------------------------------------------
// performs a Catmull-Rom interpolation, using the specified 4d vectors.
//-----------------------------------------------------------------------------
inline vector4_t CatmullRomVector4(const vector4_t a, const vector4_t b,
    const vector4_t c, const vector4_t d, float s)
{
    vector4_t v = Vector4();

    const float s2 = powf(s, 2);
    const float s3 = powf(s, 3);

    v.x = (((-s3 + (2 * s2) - s) * a.x) + (((3 * s3) - (5 * s2) + 2) * b.x) +
        (((-3 * s3) + (4 * s2) + s) * c.x) + ((s3 - s2) * d.x)) / 2.0f;
    v.y = (((-s3 + (2 * s2) - s) * a.y) + (((3 * s3) - (5 * s2) + 2) * b.y) +
        (((-3 * s3) + (4 * s2) + s) * c.y) + ((s3 - s2) * d.y)) / 2.0f;
    v.z = (((-s3 + (2 * s2) - s) * a.z) + (((3 * s3) - (5 * s2) + 2) * b.z) +
        (((-3 * s3) + (4 * s2) + s) * c.z) + ((s3 - s2) * d.z)) / 2.0f;
    v.w = (((-s3 + (2 * s2) - s) * a.w) + (((3 * s3) - (5 * s2) + 2) * b.w) +
        (((-3 * s3) + (4 * s2) + s) * c.w) + ((s3 - s2) * d.w)) / 2.0f;

    return v;
}

//-----------------------------------------------------------------------------
// determines the cross-product of two 4d vectors
//-----------------------------------------------------------------------------
inline vector4_t CrossVector4(vector4_t a, vector4_t b, vector4_t c)
{
    vector4_t v = Vector4();

    v.x = ((b.z * c.w - b.w * c.z) * a.y) - ((b.y * c.w - b.w * c.y) * a.z) +
        ((b.y * c.z - b.z * c.y) * a.w);
    v.y = ((b.w * c.z - b.z * c.w) * a.x) - ((b.w * c.x - b.x * c.w) * a.z) +
        ((b.z * c.x - b.x * c.z) * a.w);
    v.z = ((b.y * c.w - b.w * c.y) * a.x) - ((b.x * c.w - b.w * c.x) * a.y) +
        ((b.x * c.y - b.y * c.x) * a.w);
    v.w = ((b.z * c.y - b.y * c.z) * a.x) - ((b.z * c.x - b.x * c.z) * a.y) +
        ((b.y * c.x - b.x * c.y) * a.z);

    return v;
}

//-----------------------------------------------------------------------------
// dot product of two 4d vectors
//-----------------------------------------------------------------------------
inline float DotVector4(vector4_t a, vector4_t b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

//-----------------------------------------------------------------------------
// performs a Hermite spline interpolation, using the specified 4d vectors.
//-----------------------------------------------------------------------------
inline vector4_t HermiteVector4(const vector4_t a, const vector4_t t1,
    const vector4_t b, const vector4_t t2, float s)
{
    vector4_t v = Vector4();

    const float s2 = powf(s, 2);
    const float s3 = powf(s, 3);

    v.x = (((2 * s3) - (3 * s2) + 1) * a.x) + (((-2 * s3) + (3 * s2)) * b.x) +
        ((s3 - (2 * s2) + s) * t1.x) + ((s3 - s2) * t2.x);
    v.y = (((2 * s3) - (3 * s2) + 1) * a.y) + (((-2 * s3) + (3 * s2)) * b.y) +
        ((s3 - (2 * s2) + s) * t1.y) + ((s3 - s2) * t2.y);
    v.z = (((2 * s3) - (3 * s2) + 1) * a.z) + (((-2 * s3) + (3 * s2)) * b.z) +
        ((s3 - (2 * s2) + s) * t1.z) + ((s3 - s2) * t2.z);
    v.w = (((2 * s3) - (3 * s2) + 1) * a.w) + (((-2 * s3) + (3 * s2)) * b.w) +
        ((s3 - (2 * s2) + s) * t1.w) + ((s3 - s2) * t2.w);

    return v;
}

//-----------------------------------------------------------------------------
// length of a 4d vector
//-----------------------------------------------------------------------------
inline float LengthVector4(vector4_t a)
{
    return sqrtf(DotVector4(a, a));
}

//-----------------------------------------------------------------------------
// square of the length of a 4d vector
//-----------------------------------------------------------------------------
inline float LengthSquareVector4(vector4_t a)
{
    return DotVector4(a, a);
}

//-----------------------------------------------------------------------------
// performs a linear interpolation between two 4d vectors
//-----------------------------------------------------------------------------
inline vector4_t LerpVector4(vector4_t a, vector4_t b, float s)
{
    vector4_t v = Vector4();

    v.x = a.x + (s * (b.x - a.x));
    v.y = a.y + (s * (b.y - a.y));
    v.z = a.z + (s * (b.z - a.z));
    v.w = a.w + (s * (b.w - a.w));

    return v;
}

//-----------------------------------------------------------------------------
// 4d vector that is made up of the largest components of two 4d vectors
//-----------------------------------------------------------------------------
inline vector4_t MaximizeVector4(vector4_t a, vector4_t b)
{
    vector4_t v = Vector4();

    v.x = MAX(a.x, b.x);
    v.y = MAX(a.y, b.y);
    v.z = MAX(a.z, b.z);
    v.w = MAX(a.w, b.w);

    return v;
}

//-----------------------------------------------------------------------------
// 4d vector that is made up of the smallest components of two 4d vectors
//-----------------------------------------------------------------------------
inline vector4_t MinimizeVector4(vector4_t a, vector4_t b)
{
    vector4_t v = Vector4();

    v.x = MIN(a.x, b.x);
    v.y = MIN(a.y, b.y);
    v.z = MIN(a.z, b.z);
    v.w = MIN(a.w, b.w);

    return v;
}

//-----------------------------------------------------------------------------
// normalize a 4d vector
//-----------------------------------------------------------------------------
inline vector4_t NormalizeVector4(vector4_t a)
{
    vector4_t v = Vector4();

    if (v.x != 0.0f || v.y != 0.0f || v.z != 0.0f || v.w != 0.0f)
    {
        const float l = sqrtf(DotVector4(v, v));
        v.x = a.x * (1.0f / l);
        v.y = a.y * (1.0f / l);
        v.z = a.z * (1.0f / l);
        v.w = a.w * (1.0f / l);
    }

    return v;
}

//-----------------------------------------------------------------------------
// scales a 4d vector
//-----------------------------------------------------------------------------
inline vector4_t ScaleVector4(vector4_t a, float s)
{
    vector4_t v = Vector4();

    v.x = a.x * s;
    v.y = a.y * s;
    v.z = a.z * s;
    v.w = a.w * s;

    return v;
}

//-----------------------------------------------------------------------------
// subtract two 4d vectors (a - b)
//-----------------------------------------------------------------------------
inline vector4_t SubtractVector4(vector4_t a, vector4_t b)
{
    vector4_t v = Vector4();

    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;
    v.w = a.w + b.w;

    return v;
}

//-----------------------------------------------------------------------------
// transforms a 4d vector by a given matrix
//-----------------------------------------------------------------------------
inline vector4_t TransformVector4(vector4_t v, matrix4_t m)
{
    vector4_t a = Vector4();

    a.x = v.x * m.n[ 0] + v.y * m.n[ 4] + v.z * m.n[ 8] + v.w * m.n[12];
    a.y = v.x * m.n[ 1] + v.y * m.n[ 5] + v.z * m.n[ 9] + v.w * m.n[13];
    a.z = v.x * m.n[ 2] + v.y * m.n[ 6] + v.z * m.n[10] + v.w * m.n[14];
    a.w = v.x * m.n[ 3] + v.y * m.n[ 7] + v.z * m.n[11] + v.w * m.n[15];

    return a;
}


//-----------------------------------------------------------------------------
//
// VECTOR3
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructs a 3d vector
//-----------------------------------------------------------------------------
inline vector3_t Vector3(float x = 0, float y = 0, float z = 0)
{
    vector3_t v = {};

    v.x = x;
    v.y = y;
    v.z = z;

    return v;
}

//-----------------------------------------------------------------------------
// add two 3d vectors (a + b)
//-----------------------------------------------------------------------------
inline vector3_t AddVector3(vector3_t a, vector3_t b)
{
    vector3_t v = Vector3();

    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = a.z + b.z;

    return v;
}

//-----------------------------------------------------------------------------
// returns a point in barycentric coordinates, using the specified 3d vectors.
//-----------------------------------------------------------------------------
inline vector3_t BaryCentricVector3(const vector3_t a, const vector3_t b,
    const vector3_t c, float f, float g)
{
    vector3_t v = Vector3();

    v.x = a.x + f * (b.x - a.x) + g * (c.x - a.x);
    v.y = a.y + f * (b.y - a.y) + g * (c.y - a.y);
    v.z = a.z + f * (b.z - a.z) + g * (c.z - a.z);

    return v;
}

//-----------------------------------------------------------------------------
// performs a Catmull-Rom interpolation, using the specified 3d vectors.
//-----------------------------------------------------------------------------
inline vector3_t CatmullRomVector3(const vector3_t a, const vector3_t b,
    const vector3_t c, const vector3_t d, float s)
{
    vector3_t v = Vector3();

    const float s2 = powf(s, 2);
    const float s3 = powf(s, 3);

    v.x = (((-s3 + (2 * s2) - s) * a.x) + (((3 * s3) - (5 * s2) + 2) * b.x) +
        (((-3 * s3) + (4 * s2) + s) * c.x) + ((s3 - s2) * d.x)) / 2.0f;
    v.y = (((-s3 + (2 * s2) - s) * a.y) + (((3 * s3) - (5 * s2) + 2) * b.y) +
        (((-3 * s3) + (4 * s2) + s) * c.y) + ((s3 - s2) * d.y)) / 2.0f;
    v.z = (((-s3 + (2 * s2) - s) * a.z) + (((3 * s3) - (5 * s2) + 2) * b.z) +
        (((-3 * s3) + (4 * s2) + s) * c.z) + ((s3 - s2) * d.z)) / 2.0f;

    return v;
}

//-----------------------------------------------------------------------------
// determines the cross-product of two 3d vectors
//-----------------------------------------------------------------------------
inline vector3_t CrossVector3(vector3_t a, vector3_t b)
{
    vector3_t v = Vector3();

    v.x = a.y * b.z - a.z * b.y;
    v.y = a.z * b.x - a.x * b.z;
    v.z = a.x * b.y - a.y * b.x;

    return v;
}

//-----------------------------------------------------------------------------
// dot product of two 3d vectors
//-----------------------------------------------------------------------------
inline float DotVector3(vector3_t a, vector3_t b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

//-----------------------------------------------------------------------------
// performs a Hermite spline interpolation, using the specified 3d vectors.
//-----------------------------------------------------------------------------
inline vector3_t HermiteVector3(const vector3_t a, const vector3_t t1,
    const vector3_t b, const vector3_t t2, float s)
{
    vector3_t v = Vector3();

    const float s2 = powf(s, 2);
    const float s3 = powf(s, 3);

    v.x = (((2 * s3) - (3 * s2) + 1) * a.x) + (((-2 * s3) + (3 * s2)) * b.x) +
        ((s3 - (2 * s2) + s) * t1.x) + ((s3 - s2) * t2.x);
    v.y = (((2 * s3) - (3 * s2) + 1) * a.y) + (((-2 * s3) + (3 * s2)) * b.y) +
        ((s3 - (2 * s2) + s) * t1.y) + ((s3 - s2) * t2.y);
    v.z = (((2 * s3) - (3 * s2) + 1) * a.z) + (((-2 * s3) + (3 * s2)) * b.z) +
        ((s3 - (2 * s2) + s) * t1.z) + ((s3 - s2) * t2.z);

    return v;
}

//-----------------------------------------------------------------------------
// length of a 3d vector
//-----------------------------------------------------------------------------
inline float LengthVector3(vector3_t a)
{
    return sqrtf(DotVector3(a, a));
}

//-----------------------------------------------------------------------------
// square of the length of a 3d vector
//-----------------------------------------------------------------------------
inline float LengthSquareVector3(vector3_t a)
{
    return DotVector3(a, a);
}

//-----------------------------------------------------------------------------
// performs a linear interpolation between two 3d vectors
//-----------------------------------------------------------------------------
inline vector3_t InterpolateVector3(vector3_t a, vector3_t b, float s)
{
    vector3_t v = Vector3();

    v.x = a.x + (s * (b.x - a.x));
    v.y = a.y + (s * (b.y - a.y));
    v.z = a.z + (s * (b.z - a.z));

    return v;
}

//-----------------------------------------------------------------------------
// 3d vector that is made up of the largest components of two 3d vectors
//-----------------------------------------------------------------------------
inline vector3_t MaximizeVector3(vector3_t a, vector3_t b)
{
    vector3_t v = Vector3();

    v.x = MAX(a.x, b.x);
    v.y = MAX(a.y, b.y);
    v.z = MAX(a.z, b.z);

    return v;
}

//-----------------------------------------------------------------------------
// 3d vector that is made up of the smallest components of two 3d vectors
//-----------------------------------------------------------------------------
inline vector3_t MinimizeVector3(vector3_t a, vector3_t b)
{
    vector3_t v = Vector3();

    v.x = MIN(a.x, b.x);
    v.y = MIN(a.y, b.y);
    v.z = MIN(a.z, b.z);

    return v;
}

//-----------------------------------------------------------------------------
// normalize a 3d vector
//-----------------------------------------------------------------------------
inline vector3_t NormalizeVector3(vector3_t a)
{
    vector3_t v = Vector3();

    if (a.x != 0.0f || a.y != 0.0f || a.z != 0.0f)
    {
        const float l = sqrtf(DotVector3(a, a));
        v.x = a.x * (1.0f / l);
        v.y = a.y * (1.0f / l);
        v.z = a.z * (1.0f / l);
    }

    return v;
}

//-----------------------------------------------------------------------------
// scales a 3d vector
//-----------------------------------------------------------------------------
inline vector3_t ScaleVector3(vector3_t a, float s)
{
    vector3_t v = Vector3();

    v.x = a.x * s;
    v.y = a.y * s;
    v.z = a.z * s;

    return v;
}

//-----------------------------------------------------------------------------
// subtract two 3d vectors (a - b)
//-----------------------------------------------------------------------------
inline vector3_t SubtractVector3(vector3_t a, vector3_t b)
{
    vector3_t v = Vector3();

    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;

    return v;
}

//-----------------------------------------------------------------------------
// transforms a 3d vector by a given matrix
//-----------------------------------------------------------------------------
inline vector4_t TransformVector3(vector3_t v, matrix4_t m)
{
    vector4_t a = Vector4();

    a.x = v.x * m.n[ 0] + v.y * m.n[ 4] + v.z * m.n[ 8] + m.n[12];
    a.y = v.x * m.n[ 1] + v.y * m.n[ 5] + v.z * m.n[ 9] + m.n[13];
    a.z = v.x * m.n[ 2] + v.y * m.n[ 6] + v.z * m.n[10] + m.n[14];
    a.w = v.x * m.n[ 3] + v.y * m.n[ 7] + v.z * m.n[11] + m.n[15];

    return a;
}

//-----------------------------------------------------------------------------
// transforms a 3d vector by a given matrix
//-----------------------------------------------------------------------------
inline vector3_t TransformVector3Coord(vector3_t v, matrix4_t m)
{
    vector3_t a = Vector3();

    a.x = v.x * m.n[ 0] + v.y * m.n[ 4] + v.z * m.n[ 8] + m.n[12];
    a.y = v.x * m.n[ 1] + v.y * m.n[ 5] + v.z * m.n[ 9] + m.n[13];
    a.z = v.x * m.n[ 2] + v.y * m.n[ 6] + v.z * m.n[10] + m.n[14];

    return a;
}

//-----------------------------------------------------------------------------
// transforms the 3d vector normal by the given matrix.
//-----------------------------------------------------------------------------
inline vector3_t TransformVector3Normal(const vector3_t v, const matrix4_t m)
{
    vector3_t a = Vector3();

    a.x = v.x * m.n[ 0] + v.y * m.n[ 4] + v.z * m.n[ 8] + m.n[12];
    a.y = v.x * m.n[ 1] + v.y * m.n[ 5] + v.z * m.n[ 9] + m.n[13];
    a.z = v.x * m.n[ 2] + v.y * m.n[ 6] + v.z * m.n[10] + m.n[14];

    return a;	
}


//-----------------------------------------------------------------------------
//
// VECTOR2
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructs a 2d vector
//-----------------------------------------------------------------------------
inline vector2_t Vector2(float x = 0, float y = 0)
{
    vector2_t v = {};

    v.x = x;
    v.y = y;

    return v;
}

//-----------------------------------------------------------------------------
// add two 2d vectors (a + b)
//-----------------------------------------------------------------------------
inline vector2_t AddVector2(vector2_t a, vector2_t b)
{
    vector2_t v = Vector2();

    v.x = a.x + b.x;
    v.y = a.y + b.y;

    return v;
}

//-----------------------------------------------------------------------------
// returns a point in barycentric coordinates, using the specified 2D vectors.
//-----------------------------------------------------------------------------
inline vector2_t BaryCentricVector2(const vector2_t a, const vector2_t b,
    const vector2_t c, float f, float g)
{
    vector2_t v = Vector2();

    v.x = a.x + f * (b.x - a.x) + g * (c.x - a.x);
    v.y = a.y + f * (b.y - a.y) + g * (c.y - a.y);

    return v;
}

//-----------------------------------------------------------------------------
// performs a Catmull-Rom interpolation, using the specified 2D vectors.
//-----------------------------------------------------------------------------
inline vector2_t CatmullRomVector2(const vector2_t a, const vector2_t b,
    const vector2_t c, const vector2_t d, float s)
{
    vector2_t v = Vector2();

    const float s2 = powf(s, 2);
    const float s3 = powf(s, 3);

    v.x = (((-s3 + (2 * s2) - s) * a.x) + (((3 * s3) - (5 * s2) + 2) * b.x) +
        (((-3 * s3) + (4 * s2) + s) * c.x) + ((s3 - s2) * d.x)) / 2.0f;
    v.y = (((-s3 + (2 * s2) - s) * a.y) + (((3 * s3) - (5 * s2) + 2) * b.y) +
        (((-3 * s3) + (4 * s2) + s) * c.y) + ((s3 - s2) * d.y)) / 2.0f;

    return v;
}

//-----------------------------------------------------------------------------
// returns the z-component by taking the cross product of two 2D vectors.
// if the value of the z-component is positive, the vector V2 is
// counterclockwise from the vector V1. This information is useful for
// back-face culling.
//-----------------------------------------------------------------------------
inline float CounterClockwiseWindingVector2(const vector2_t a, const vector2_t b)
{
    return (a.x * b.y) - (a.y * b.x);
}

//-----------------------------------------------------------------------------
// dot product of two 2d vectors
//-----------------------------------------------------------------------------
inline float DotVector2(vector2_t a, vector2_t b)
{
    return (a.x * b.x) + (a.y * b.y);
}

//-----------------------------------------------------------------------------
// performs a Hermite spline interpolation, using the specified 2D vectors.
//-----------------------------------------------------------------------------
inline vector2_t HermiteVector2(const vector2_t a, const vector2_t t1,
    const vector2_t b, const vector2_t t2, float s)
{
    vector2_t v = Vector2();

    const float s2 = powf(s, 2);
    const float s3 = powf(s, 3);

    v.x = (((2 * s3) - (3 * s2) + 1) * a.x) + (((-2 * s3) + (3 * s2)) * b.x) +
        ((s3 - (2 * s2) + s) * t1.x) + ((s3 - s2) * t2.x);
    v.y = (((2 * s3) - (3 * s2) + 1) * a.y) + (((-2 * s3) + (3 * s2)) * b.y) +
        ((s3 - (2 * s2) + s) * t1.y) + ((s3 - s2) * t2.y);

    return v;
}

//-----------------------------------------------------------------------------
// length of a 2d vector
//-----------------------------------------------------------------------------
inline float LengthVector2(const vector2_t a)
{
    return sqrtf(DotVector2(a, a));
}

//-----------------------------------------------------------------------------
// square of the length of a 2d vector
//-----------------------------------------------------------------------------
inline float LengthSquaredVector2(const vector2_t a)
{
    return DotVector2(a, a);
}

//-----------------------------------------------------------------------------
// performs a linear interpolation between two 2d vectors
//-----------------------------------------------------------------------------
inline vector2_t InterpolateVector2(const vector2_t a, const vector2_t b, float s)
{
    vector2_t v = Vector2();

    v.x = a.x + (s * (b.x - a.x));
    v.y = a.y + (s * (b.y - a.y));

    return v;
}

//-----------------------------------------------------------------------------
// 2d vector that is made up of the largest components of two 2d vectors
//-----------------------------------------------------------------------------
inline vector2_t MaximizeVector2(const vector2_t a, const vector2_t b)
{
    vector2_t v = Vector2();

    v.x = MAX(a.x, b.x);
    v.y = MAX(a.y, b.y);

    return v;
}

//-----------------------------------------------------------------------------
// 2d vector that is made up of the smallest components of two 2d vectors
//-----------------------------------------------------------------------------
inline vector2_t MinimizeVector2(const vector2_t a, const vector2_t b)
{
    vector2_t v = Vector2();

    v.x = MIN(a.x, b.x);
    v.y = MIN(a.y, b.y);

    return v;
}

//-----------------------------------------------------------------------------
// normalize a 2d vector
//-----------------------------------------------------------------------------
inline vector2_t NormalizeVector2(const vector2_t a)
{
    vector2_t v = Vector2();

    if (a.x != 0.0f || a.y != 0.0f)
    {
        const float l = sqrtf(DotVector2(a, a));
        v.x = a.x * (1.0f / l);
        v.y = a.y * (1.0f / l);
    }

    return v;
}

//-----------------------------------------------------------------------------
// scales a 2d vector
//-----------------------------------------------------------------------------
inline vector2_t ScaleVector2(const vector2_t a, float s)
{
    vector2_t v = Vector2();

    v.x = a.x * s;
    v.y = a.y * s;

    return v;
}

//-----------------------------------------------------------------------------
// subtract two 2d vectors (a - b)
//-----------------------------------------------------------------------------
inline vector2_t SubtractVector2(const vector2_t a, const vector2_t b)
{
    vector2_t v = Vector2();

    v.x = a.x - b.x;
    v.y = a.y - b.y;

    return v;
}

//-----------------------------------------------------------------------------
// transforms a 2d vector by a given matrix
//-----------------------------------------------------------------------------
inline vector4_t TransformVector2(const vector2_t v, const matrix4_t m)
{
    vector4_t a = Vector4();

    a.x = v.x * m.n[ 0] + v.y * m.n[ 4];
    a.y = v.x * m.n[ 1] + v.y * m.n[ 5];
    a.z = 0.0f;
    a.w = v.x * m.n[ 3] + v.y * m.n[ 7];

    return a;
}

//-----------------------------------------------------------------------------
// transforms a 2d vector by a given matrix, projecting the result back into w = 1
//-----------------------------------------------------------------------------
inline vector2_t TransformVector2Coord(const vector2_t v, const matrix4_t m)
{
    vector2_t a = Vector2();

    a.x = v.x * m.n[ 0] + v.y * m.n[ 4] + m.n[12];
    a.y = v.x * m.n[ 1] + v.y * m.n[ 5] + m.n[13];

    return a;
}

//-----------------------------------------------------------------------------
// transforms the 2d vector normal by the given matrix.
//-----------------------------------------------------------------------------
inline vector2_t TransformVector2Normal(const vector2_t v, const matrix4_t m)
{
    vector2_t a = Vector2();

    a.x = v.x * m.n[ 0] + v.y * m.n[ 4];
    a.y = v.x * m.n[ 1] + v.y * m.n[ 5];

    return a;	
}


//-----------------------------------------------------------------------------
//
// PLANE
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructs a plane
//-----------------------------------------------------------------------------
inline plane_t Plane(float a = 0, float b = 0, float c = 0, float d = 0)
{
    plane_t p = {};

    p.a = a;
    p.b = b;
    p.c = c;
    p.d = d;

    return p;
}

//-----------------------------------------------------------------------------
// computes the dot product of a plane and a 4d vector
//-----------------------------------------------------------------------------
inline float DotPlane(const plane_t p, const vector4_t v)
{
    return DotVector4(Vector4(p.a, p.b, p.c, p.d), v);
}

//-----------------------------------------------------------------------------
// computes the dot product of a plane and a 3D vector. The w parameter of the
// vector is assumed to be 1.
//-----------------------------------------------------------------------------
inline float DotPlaneCoordinate(const plane_t p, const vector3_t position)
{
    return DotVector4(Vector4(p.a, p.b, p.c, p.d),
        Vector4(position.x, position.y, position.z, 1.0f));
}

//-----------------------------------------------------------------------------
// computes the dot product of a plane and a 3D vector. The w parameter of the
// vector is assumed to be 0.
//-----------------------------------------------------------------------------
inline float DotPlaneNormal(const plane_t p, const vector3_t normal)
{
    return DotVector4(Vector4(p.a, p.b, p.c, p.d),
        Vector4(normal.x, normal.y, normal.z, 0.0f));
}

//-----------------------------------------------------------------------------
// constructs a plane from a point and a normal.
//-----------------------------------------------------------------------------
inline plane_t FromPointNormalPlane(const vector3_t point, const vector3_t normal)
{
    plane_t p = Plane();

    p.a = normal.x;
    p.b = normal.y;
    p.c = normal.z;
    p.d = -DotVector3(normal, point);

    return p;
}

//-----------------------------------------------------------------------------
// constructs a plane from three points.
//-----------------------------------------------------------------------------
inline plane_t FromPointsPlane(const vector3_t v0, const vector3_t v1,
    const vector3_t v2)
{
    plane_t p = Plane();
    const vector3_t n = CrossVector3(SubtractVector3(v1, v0), SubtractVector3(v2, v0));
    p = FromPointNormalPlane(v0, n);

    return p;
}

//-----------------------------------------------------------------------------
// finds the intersection between a plane and a line.
//-----------------------------------------------------------------------------
inline bool
LineIntersectPlane(vector3_t& out, const plane_t plane, vector3_t p0,
    const vector3_t p1)
{
    vector3_t v = SubtractVector3(p1, p0);
    float d = LengthVector3(v);
    vector3_t pn = Vector3(plane.a, plane.b, plane.c);
    vector3_t r0 = p0;
    vector3_t rd = Vector3(v.x / d, v.y / d, v.z / d);
    float vd = DotVector3(pn, rd);

    if (vd == 0)
    {
        return false;
    }
    
    if (vd > 0)
    {
        pn = Vector3(-pn.x, -pn.y, -pn.z);
        vd = DotVector3(pn, rd);
    }

    float v0 = -DotVector3(pn, r0) + plane.d;
    float t = v0 / vd;

    if (t < 0)
    {
        return false;
    }
    
    out = AddVector3(r0, ScaleVector3(rd, t));

    return true;
}

//-----------------------------------------------------------------------------
// normalizes the plane coefficients so that the plane normal has unit length
//-----------------------------------------------------------------------------
inline plane_t NormalizePlane(const plane_t p)
{
    const vector3_t v = NormalizeVector3(Vector3(p.a, p.b, p.c));

    return Plane(v.x, v.y, v.z, p.d);
}

//-----------------------------------------------------------------------------
// scale the plane with the given scaling factor.
//-----------------------------------------------------------------------------
inline plane_t ScalePlane(const plane_t p, float s)
{
    return Plane(p.a * s, p.b * s, p.c * s, p.d * s);
}

//-----------------------------------------------------------------------------
// transforms a plane by a matrix. The input matrix is the inverse transpose of
// the actual transformation
//-----------------------------------------------------------------------------
inline plane_t TransformPlane(plane_t p, matrix4_t m)
{
    const vector4_t v = TransformVector4(Vector4(p.a, p.b, p.c, p.d), m);

    return Plane(v.x, v.y, v.z, v.w);
}


//-----------------------------------------------------------------------------
//
// MATRIX4
//
//-----------------------------------------------------------------------------


#define EPSILON 0.0001
#define FLOAT_EQ(x, v) (((v - EPSILON) < x) && (x < (v + EPSILON)))

//-----------------------------------------------------------------------------
// creates an identity matrix
//-----------------------------------------------------------------------------
inline matrix4_t Matrix4(
    float _11 = 1.0f, float _12 = 0.0f, float _13 = 0.0f, float _14 = 0.0f,
    float _21 = 0.0f, float _22 = 1.0f, float _23 = 0.0f, float _24 = 0.0f,
    float _31 = 0.0f, float _32 = 0.0f, float _33 = 1.0f, float _34 = 0.0f,
    float _41 = 0.0f, float _42 = 0.0f, float _43 = 0.0f, float _44 = 1.0f)
{
    matrix4_t m = {};

    m.n[ 0] = _11;
    m.n[ 1] = _12;
    m.n[ 2] = _13;
    m.n[ 3] = _14;
    m.n[ 4] = _21;
    m.n[ 5] = _22;
    m.n[ 6] = _23;
    m.n[ 7] = _24;
    m.n[ 8] = _31;
    m.n[ 9] = _32;
    m.n[10] = _33;
    m.n[11] = _34;
    m.n[12] = _41;
    m.n[13] = _42;
    m.n[14] = _43;
    m.n[15] = _44;

    return m;
}

//-----------------------------------------------------------------------------
// returns the determinant of the matrix
//-----------------------------------------------------------------------------
inline float DeterminantMatrix4(const matrix4_t m)
{
    const float a0 = (m.n[ 0] * m.n[ 5]) - (m.n[ 4] * m.n[ 1]);
    const float a1 = (m.n[ 0] * m.n[ 6]) - (m.n[ 4] * m.n[ 2]);
    const float a2 = (m.n[ 0] * m.n[ 7]) - (m.n[ 4] * m.n[ 3]);
    const float a3 = (m.n[ 1] * m.n[ 6]) - (m.n[ 5] * m.n[ 2]);
    const float a4 = (m.n[ 1] * m.n[ 7]) - (m.n[ 5] * m.n[ 3]);
    const float a5 = (m.n[ 2] * m.n[ 7]) - (m.n[ 6] * m.n[ 3]);
    const float b0 = (m.n[ 8] * m.n[13]) - (m.n[12] * m.n[ 9]);
    const float b1 = (m.n[ 8] * m.n[14]) - (m.n[12] * m.n[10]);
    const float b2 = (m.n[ 8] * m.n[15]) - (m.n[12] * m.n[11]);
    const float b3 = (m.n[ 9] * m.n[14]) - (m.n[13] * m.n[10]);
    const float b4 = (m.n[ 9] * m.n[15]) - (m.n[13] * m.n[11]);
    const float b5 = (m.n[10] * m.n[15]) - (m.n[14] * m.n[11]);

    const float d = (a0 * b5) - (a1 * b4) + (a2 * b3) + (a3 * b2) - (a4 * b1) +
        (a5 * b0);

    return d;
}

//-----------------------------------------------------------------------------
// calculates the inverse of a matrix
//-----------------------------------------------------------------------------
inline matrix4_t InverseMatrix4(const matrix4_t m)
{
    const float a0 = (m.n[ 0] * m.n[ 5]) - (m.n[ 4] * m.n[ 1]);
    const float a1 = (m.n[ 0] * m.n[ 6]) - (m.n[ 4] * m.n[ 2]);
    const float a2 = (m.n[ 0] * m.n[ 7]) - (m.n[ 4] * m.n[ 3]);
    const float a3 = (m.n[ 1] * m.n[ 6]) - (m.n[ 5] * m.n[ 2]);
    const float a4 = (m.n[ 1] * m.n[ 7]) - (m.n[ 5] * m.n[ 3]);
    const float a5 = (m.n[ 2] * m.n[ 7]) - (m.n[ 6] * m.n[ 3]);
    const float b0 = (m.n[ 8] * m.n[13]) - (m.n[12] * m.n[ 9]);
    const float b1 = (m.n[ 8] * m.n[14]) - (m.n[12] * m.n[10]);
    const float b2 = (m.n[ 8] * m.n[15]) - (m.n[12] * m.n[11]);
    const float b3 = (m.n[ 9] * m.n[14]) - (m.n[13] * m.n[10]);
    const float b4 = (m.n[ 9] * m.n[15]) - (m.n[13] * m.n[11]);
    const float b5 = (m.n[10] * m.n[15]) - (m.n[14] * m.n[11]);

    const float d = (a0 * b5) - (a1 * b4) + (a2 * b3) + (a3 * b2) - (a4 * b1) +
        (a5 * b0);

    if (d == 0.0f)
    {
        return Matrix4();
    }

    const float id = 1.0f / d;

    matrix4_t a = Matrix4();
    a.n[ 0] = ((m.n[ 5] * b5) - (m.n[ 6] * b4) + (m.n[ 7] * b3)) * id;
    a.n[ 4] = ((m.n[ 4] * b5) + (m.n[ 6] * b2) - (m.n[ 7] * b1)) * id;
    a.n[ 8] = ((m.n[ 4] * b4) - (m.n[ 5] * b2) + (m.n[ 7] * b0)) * id;
    a.n[12] = ((m.n[ 4] * b3) + (m.n[ 5] * b1) - (m.n[ 6] * b0)) * id;

    a.n[ 1] = ((m.n[ 1] * b5) + (m.n[ 2] * b4) - (m.n[ 3] * b3)) * id;
    a.n[ 5] = ((m.n[ 0] * b5) - (m.n[ 2] * b2) + (m.n[ 3] * b1)) * id;
    a.n[ 9] = ((m.n[ 0] * b4) + (m.n[ 1] * b2) - (m.n[ 3] * b0)) * id;
    a.n[13] = ((m.n[ 0] * b3) - (m.n[ 1] * b1) + (m.n[11] * b0)) * id;

    a.n[ 2] = ((m.n[13] * a5) - (m.n[14] * a4) + (m.n[15] * a3)) * id;
    a.n[ 6] = ((m.n[12] * a5) + (m.n[14] * a2) - (m.n[15] * a1)) * id;
    a.n[10] = ((m.n[12] * a4) - (m.n[13] * a2) + (m.n[15] * a0)) * id;
    a.n[14] = ((m.n[12] * a3) + (m.n[13] * a1) - (m.n[14] * a0)) * id;

    a.n[ 3] = ((m.n[ 9] * a5) + (m.n[10] * a4) - (m.n[11] * a3)) * id;
    a.n[ 7] = ((m.n[ 8] * a5) - (m.n[10] * a2) + (m.n[11] * a1)) * id;
    a.n[11] = ((m.n[ 8] * a4) + (m.n[ 9] * a2) - (m.n[11] * a0)) * id;
    a.n[15] = ((m.n[ 8] * a3) - (m.n[ 9] * a1) + (m.n[10] * a0)) * id;

    return a;
}

//-----------------------------------------------------------------------------
// determines the product of two matrices
//-----------------------------------------------------------------------------
inline matrix4_t MultiplyMatrix4(const matrix4_t a, const matrix4_t b)
{
    matrix4_t m = Matrix4();

    m.n[ 0] = a.n[ 0] * b.n[ 0] + a.n[ 1] * b.n[ 4] + a.n[ 2] * b.n[ 8] +
              a.n[ 3] * b.n[12];
    m.n[ 1] = a.n[ 0] * b.n[ 1] + a.n[ 1] * b.n[ 5] + a.n[ 2] * b.n[ 9] +
              a.n[ 3] * b.n[13];
    m.n[ 2] = a.n[ 0] * b.n[ 2] + a.n[ 1] * b.n[ 6] + a.n[ 2] * b.n[10] +
              a.n[ 3] * b.n[14];
    m.n[ 3] = a.n[ 0] * b.n[ 3] + a.n[ 1] * b.n[ 7] + a.n[ 2] * b.n[11] +
              a.n[ 3] * b.n[15];

    m.n[ 4] = a.n[ 4] * b.n[ 0] + a.n[ 5] * b.n[ 4] + a.n[ 6] * b.n[ 8] +
              a.n[ 7] * b.n[12];
    m.n[ 5] = a.n[ 4] * b.n[ 1] + a.n[ 5] * b.n[ 5] + a.n[ 6] * b.n[ 9] +
              a.n[ 7] * b.n[13];
    m.n[ 6] = a.n[ 4] * b.n[ 2] + a.n[ 5] * b.n[ 6] + a.n[ 6] * b.n[10] +
              a.n[ 7] * b.n[14];
    m.n[ 7] = a.n[ 4] * b.n[ 3] + a.n[ 5] * b.n[ 7] + a.n[ 6] * b.n[11] +
              a.n[ 7] * b.n[15];

    m.n[ 8] = a.n[ 8] * b.n[ 0] + a.n[ 9] * b.n[ 4] + a.n[10] * b.n[ 8] +
              a.n[11] * b.n[12];
    m.n[ 9] = a.n[ 8] * b.n[ 1] + a.n[ 9] * b.n[ 5] + a.n[10] * b.n[ 9] +
              a.n[11] * b.n[13];
    m.n[10] = a.n[ 8] * b.n[ 2] + a.n[ 9] * b.n[ 6] + a.n[10] * b.n[10] +
              a.n[11] * b.n[14];
    m.n[11] = a.n[ 8] * b.n[ 3] + a.n[ 9] * b.n[ 7] + a.n[10] * b.n[11] +
              a.n[11] * b.n[15];

    m.n[12] = a.n[12] * b.n[ 0] + a.n[13] * b.n[ 4] + a.n[14] * b.n[ 8] +
              a.n[15] * b.n[12];
    m.n[13] = a.n[12] * b.n[ 1] + a.n[13] * b.n[ 5] + a.n[14] * b.n[ 9] +
              a.n[15] * b.n[13];
    m.n[14] = a.n[12] * b.n[ 2] + a.n[13] * b.n[ 6] + a.n[14] * b.n[10] +
              a.n[15] * b.n[14];
    m.n[15] = a.n[12] * b.n[ 3] + a.n[13] * b.n[ 7] + a.n[14] * b.n[11] +
              a.n[15] * b.n[15];

    return m;
}

//-----------------------------------------------------------------------------
// builds a left-handed look-at matrix
//-----------------------------------------------------------------------------
inline matrix4_t LookAtLHMatrix4(const vector3_t eye, const vector3_t at,
	const vector3_t up)
{
    vector3_t xa = Vector3();
    vector3_t ya = Vector3();
    vector3_t za = Vector3();

    za = NormalizeVector3(SubtractVector3(at, eye));
    xa = NormalizeVector3(CrossVector3(up, za));
    ya = CrossVector3(za, xa);

    return Matrix4(
                    xa.x,                 ya.x,                 za.x,     0.0f,
                    xa.y,                 ya.y,                 za.y,     0.0f,
                    xa.z,                 ya.z,                 za.z,     0.0f,
        -DotVector3(xa, eye), -DotVector3(ya, eye), -DotVector3(za, eye), 1.0f
    );
}

//-----------------------------------------------------------------------------
// builds a right-handed look-at matrix
//-----------------------------------------------------------------------------
inline matrix4_t LookAtRHMatrix4(const vector3_t eye, const vector3_t at,
	const vector3_t up)
{
    vector3_t xa = Vector3();
    vector3_t ya = Vector3();
    vector3_t za = Vector3();

    za = NormalizeVector3(SubtractVector3(eye, at));
    xa = NormalizeVector3(CrossVector3(up, za));
    ya = CrossVector3(za, xa);

    return Matrix4(
                    xa.x,                 ya.x,                 za.x,     0.0f,
                    xa.y,                 ya.y,                 za.y,     0.0f,
                    xa.z,                 ya.z,                 za.z,     0.0f,
        -DotVector3(xa, eye), -DotVector3(ya, eye), -DotVector3(za, eye), 1.0f
    );
}

//-----------------------------------------------------------------------------
// builds a left-handed orthographic projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t OrthographicLHMatrix4(float w, float h, float zn, float zf)
{
    return Matrix4(
        2.0f / w, 0.0f,     0.0f,             0.0f,
        0.0f,     2.0f / h, 0.0f,             0.0f,
        0.0f,     0.0f,     1.0f / (zf - zn), 0.0f,
        0.0f,     0.0f,       zn / (zn - zf), 1.0f
    );
}

//-----------------------------------------------------------------------------
// builds a left-handed orthographic projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t OrthographicRHMatrix4(float w, float h, float zn, float zf)
{
    return Matrix4(
        2.0f / w, 0.0f,     0.0f,             0.0f,
        0.0f,     2.0f / h, 0.0f,             0.0f,
        0.0f,     0.0f,     1.0f / (zn - zf), 0.0f,
        0.0f,     0.0f,       zn / (zf - zn), 1.0f
    );
}

//-----------------------------------------------------------------------------
// builds a customized, left-handed orthographic projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t OrthographicOffCenterLHMatrix4(float l, float r, float t, float b,
    float zn, float zf)
{
    return Matrix4(
        2.0f / (r - l),     0.0f,            0.0f,             0.0f,
        0.0f,               2.0f / (t - b),  0.0f,             0.0f,
        0.0f,               0.0f,            1.0f / (zf - zn), 0.0f,
        (l + r) / (l - r), (t + b) / (b - t),  zn / (zn - zf), 1.0f
    );
}

//-----------------------------------------------------------------------------
// builds a customized, right-handed orthographic projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t OrthographicOffCenterRHMatrix4(float l, float r, float t, float b,
    float zn, float zf)
{
    return Matrix4(
        2.0f / (r - l),     0.0f,            0.0f,             0.0f,
        0.0f,               2.0f / (t - b),  0.0f,             0.0f,
        0.0f,               0.0f,            1.0f / (zn - zf), 0.0f,
        (l + r) / (l - r), (t + b) / (b - t),  zn / (zf - zn), 1.0f
    );
}

//-----------------------------------------------------------------------------
// builds a left-handed perspective projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveLHMatrix4(float w, float h, float zn, float zf)
{
    return Matrix4(
        2.0f / w, 0.0f,     0.0f,                 0.0f,
        0.0f,     2.0f / h, 0.0f,                 0.0f,
        0.0f,     0.0f,     zf / (zf - zn),       1.0f,
        0.0f,     0.0f,   (-zn * zf) / (zf - zn), 0.0f
    );
}

//-----------------------------------------------------------------------------
// builds a right-handed perspective projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveRHMatrix4(float w, float h, float zn, float zf)
{
    return Matrix4(
        2.0f / w, 0.0f,     0.0f,                 0.0f,
        0.0f,     2.0f / h, 0.0f,                 0.0f,
        0.0f,     0.0f,     zf / (zn - zf),       1.0f,
        0.0f,     0.0f,   (-zn * zf) / (zn - zf), 0.0f
    );
}

//-----------------------------------------------------------------------------
// builds a left-handed perspective projection matrix based on a field of view
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveFovLHMatrix4(float fovy, float aspect, float zn, float zf)
{
    const float yScale = tanf((H_PI - (fovy / 2.0f)));
    const float xScale = yScale / aspect;

    return Matrix4(
        xScale, 0.0f,   0.0f,                 0.0f,
        0.0f,   yScale, 0.0f,                 0.0f,
        0.0f,   0.0f,   zf / (zf - zn),       1.0f,
        0.0f,   0.0f, (-zn * zf) / (zf - zn), 0.0f
    );
}

//-----------------------------------------------------------------------------
// builds a right-handed perspective projection matrix based on a field of view
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveFovRHMatrix4(float fovy, float aspect, float zn, float zf)
{
    const float yScale = tanf((H_PI - (fovy / 2.0f)));
    const float xScale = yScale / aspect;

    return Matrix4(
        xScale, 0.0f,   0.0f,                 0.0f,
        0.0f,   yScale, 0.0f,                 0.0f,
        0.0f,   0.0f,   zf / (zn - zf),       1.0f,
        0.0f,   0.0f, (-zn * zf) / (zn - zf), 0.0f
    );
}

//-----------------------------------------------------------------------------
// builds a customized, left-handed perspective projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveOffCenterLHMatrix4(float l, float r, float t, float b,
    float zn, float zf)
{
    return Matrix4(
        (2.0f * zn) / (r - l), 0.0f,                 0.0f,                 0.0f,
        0.0f,                 (2.0f * zn) / (b - t), 0.0f,                 0.0f,
        (l + r) / (r - l),    (t + b) / (b - t),     zf / (zf - zn),       1.0f,
        0.0f,                  0.0f,               (-zn * zf) / (zf - zn), 0.0f
    );
}

//-----------------------------------------------------------------------------
// builds a customized, right-handed perspective projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveOffCenterMatrix4(float l, float r, float t, float b,
    float zn, float zf)
{
    return Matrix4(
        (2.0f * zn) / (r - l), 0.0f,                 0.0f,                 0.0f,
        0.0f,                 (2.0f * zn) / (b - t), 0.0f,                 0.0f,
        (l + r) / (r - l),    (t + b) / (b - t),     zf / (zn - zf),       1.0f,
        0.0f,                  0.0f,               (-zn * zf) / (zn - zf), 0.0f
    );
}

//-----------------------------------------------------------------------------
// builds a matrix that reflects the coordinate system about a plane.
//-----------------------------------------------------------------------------
inline matrix4_t ReflectMatrix4(const plane_t plane)
{
    const plane_t p = NormalizePlane(plane);

    const float ta = -2.0f * p.a;
    const float tb = -2.0f * p.b;
    const float tc = -2.0f * p.c;

    return Matrix4(ta * p.a + 1, tb * p.a,     tc * p.a,     0,
                   ta * p.b,     tb * p.b + 1, tc * p.b,     0,
                   ta * p.c,     tb * p.c,     tc * p.c + 1, 0,
                   ta * p.d,     tb * p.d,     tc * p.d,     1);
}

//-----------------------------------------------------------------------------
// builds a matrix that rotates around an arbitrary axis
//-----------------------------------------------------------------------------
inline matrix4_t RotationAxisAngleMatrix4(const vector3_t v, float angle)
{
    const float ct = sinf(H_PI - angle);
    const float st = sinf(angle);
    const float om = (1.0f - ct);

    return Matrix4(
        v.x * v.x * om + ct, v.x * v.y * om + v.z * st, v.x * v.z * om - v.y * st, 0.0f,
        v.x * v.y * om - v.z * st, v.y * v.y * om + ct, v.y * v.z * om + v.x * st, 0.0f,
        v.x * v.z * om + v.y * st, v.y * v.z * om - v.x * st, v.z * v.z * om + ct, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// builds a matrix that rotates around the x-axis
//-----------------------------------------------------------------------------
inline matrix4_t RotationXMatrix4(float angle)
{
    matrix4_t m = Matrix4();

    m.n[ 5] =  cosf(angle);
    m.n[ 6] =  sinf(angle);
    m.n[ 9] = -m.n[6];
    m.n[10] =  m.n[5];

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix that rotates around the y-axis
//-----------------------------------------------------------------------------
inline matrix4_t RotationYMatrix4(float angle)
{
    matrix4_t m = Matrix4();

    m.n[ 0] =  cosf(angle);
    m.n[ 2] = -sinf(angle);
    m.n[ 8] = -m.n[2];
    m.n[10] =  m.n[0];

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix that rotates around the z-axis
//-----------------------------------------------------------------------------
inline matrix4_t RotationZMatrix4(float angle)
{
    matrix4_t m = Matrix4();

    m.n[ 0] =  cosf(angle);
    m.n[ 1] =  sinf(angle);
    m.n[ 4] = -m.n[1];
    m.n[ 5] =  m.n[0];

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix with a specified yaw, pitch, and roll
//-----------------------------------------------------------------------------
inline matrix4_t RotationYawPitchRollMatrix4(float y, float p, float r)
{
	return MultiplyMatrix4(RotationZMatrix4(r),
        MultiplyMatrix4(RotationXMatrix4(p), RotationYMatrix4(y)));
}


//-----------------------------------------------------------------------------
// builds a matrix that scales along the x-axis, the y-axis, and the z-axis
//-----------------------------------------------------------------------------
inline matrix4_t ScalingMatrix4(float x, float y, float z)
{
    matrix4_t m = Matrix4();

    m.n[ 0] = x;
    m.n[ 5] = y;
    m.n[10] = z;

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix using the specified offsets
//-----------------------------------------------------------------------------
inline matrix4_t TranslationMatrix4(float x, float y, float z)
{
    matrix4_t m = Matrix4();

    m.n[12] = x;
    m.n[13] = y;
    m.n[14] = z;

    return m;
}

//-----------------------------------------------------------------------------
// builds the matrix transpose of a matrix
//-----------------------------------------------------------------------------
inline matrix4_t TransposeMatrix4(const matrix4_t m)
{
    matrix4_t a = Matrix4();

    a.n[ 0] = m.n[ 0];
    a.n[ 1] = m.n[ 4];
    a.n[ 2] = m.n[ 8];
    a.n[ 3] = m.n[12];
    a.n[ 4] = m.n[ 1];
    a.n[ 5] = m.n[ 5];
    a.n[ 6] = m.n[ 9];
    a.n[ 7] = m.n[13];
    a.n[ 8] = m.n[ 2];
    a.n[ 9] = m.n[ 6];
    a.n[10] = m.n[10];
    a.n[11] = m.n[14];
    a.n[12] = m.n[ 3];
    a.n[13] = m.n[ 7];
    a.n[14] = m.n[11];
    a.n[15] = m.n[15];

    return a;
}

//-----------------------------------------------------------------------------
// builds a 2D transformation matrix that represents transformations in the xy
// plane
//-----------------------------------------------------------------------------
inline matrix4_t Transformation2DMatrix4(const vector2_t scalingCenter,
    const vector2_t scale, const vector2_t rotationCenter, float angle,
    const vector2_t translation)
{
    matrix4_t r = MultiplyMatrix4(InverseMatrix4(
        TranslationMatrix4(rotationCenter.x, rotationCenter.y, 0.0f)),
        RotationZMatrix4(angle));
    r = MultiplyMatrix4(r, TranslationMatrix4(rotationCenter.x, rotationCenter.y, 0.0f));

    matrix4_t s = MultiplyMatrix4(InverseMatrix4(
        TranslationMatrix4(scalingCenter.x, scalingCenter.y, 0.0f)),
        ScalingMatrix4(scale.x, scale.y, 0.0f));
    s = MultiplyMatrix4(s, TranslationMatrix4(scalingCenter.x, scalingCenter.y, 0.0f));

    const matrix4_t t = TranslationMatrix4(translation.x, translation.y, 0.0f);

    return MultiplyMatrix4(MultiplyMatrix4(r, s), t);
}

//-----------------------------------------------------------------------------
// builds a 3D transformation matrix that represents transformations in the xy
// plane
//-----------------------------------------------------------------------------
inline matrix4_t Transformation3DMatrix4(const vector3_t scalingCenter,
    const vector3_t scale, const vector3_t rotationCenter, float angle,
    const vector3_t translation)
{
    matrix4_t r = MultiplyMatrix4(InverseMatrix4(
        TranslationMatrix4(rotationCenter.x, rotationCenter.y, rotationCenter.z)),
        RotationZMatrix4(angle));
    r = MultiplyMatrix4(r, TranslationMatrix4(rotationCenter.x, rotationCenter.y,
		rotationCenter.z));

    matrix4_t s = MultiplyMatrix4(InverseMatrix4(
        TranslationMatrix4(scalingCenter.x, scalingCenter.y, scalingCenter.z)),
        ScalingMatrix4(scale.x, scale.y, 0.0f));
    s = MultiplyMatrix4(s, TranslationMatrix4(scalingCenter.x, scalingCenter.y,
		scalingCenter.z));

    const matrix4_t t = TranslationMatrix4(translation.x, translation.y,
        translation.z);

    return MultiplyMatrix4(MultiplyMatrix4(r, s), t);
}


//-----------------------------------------------------------------------------
//
// VIEWPORT
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructs a viewport
//-----------------------------------------------------------------------------
inline viewport_t Viewport(uint32_t x = 0, uint32_t y = 0, uint32_t w = 0,
    uint32_t h = 0, float minZ = 0.0f, float maxZ = 0.0f)
{
    viewport_t v = {};

    v.x = x;
    v.y = y;
    v.w = w;
    v.h = h;
    v.minZ = minZ;
    v.maxZ = maxZ;

    return v;
}

//-----------------------------------------------------------------------------
// projects a 3d vector from object space into screen space.
//-----------------------------------------------------------------------------
inline vector3_t ProjectViewport(const vector3_t v, const viewport_t viewport,
    const matrix4_t projection, const matrix4_t view,
    const matrix4_t world)
{
    // multiply world, view, projection, viewport
    const matrix4_t t = MultiplyMatrix4(OrthographicOffCenterLHMatrix4((float)viewport.x,
        (float)(viewport.x + viewport.w), (float)viewport.y,
        (float)(viewport.y + viewport.h), viewport.minZ,
        viewport.maxZ), MultiplyMatrix4(projection, MultiplyMatrix4(view, world)));
    return TransformVector3Coord(v, t);
}

//-----------------------------------------------------------------------------
// projects a 3d vector from screen space into object space.
//-----------------------------------------------------------------------------
inline vector3_t UnprojectViewport(const vector3_t v, const viewport_t viewport,
    const matrix4_t projection, const matrix4_t view, const matrix4_t world)
{
    // multiply viewport, projection, view, world
    const matrix4_t t = MultiplyMatrix4(world,
        MultiplyMatrix4(view,
        MultiplyMatrix4(projection,
        OrthographicOffCenterLHMatrix4((float)viewport.x,
            (float)(viewport.x + viewport.w),
            (float)viewport.y,
            (float)(viewport.y + viewport.h),
            viewport.minZ, viewport.maxZ))));

    return TransformVector3Coord(v, t);
}


#endif // #ifndef _G_MATH_H_

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

#ifndef G_PI
#define G_PI        3.14159265f
#endif // #ifndef G_PI

#ifndef G_PIHALF
#define G_PIHALF    1.57079633f
#endif // #ifndef G_PIHALF

#define FLOOR(a)            ((a)>0?(int32_t)(a):-(int32_t)(-a))
#define ROUND(a)            (fabs(ceil(a)-a) <= 0.5 ? ceil(a) : floor(a))
#define CLAMP(x, min, max)  (x < min ? min : x > max ? max : x)
#define ABS(a)              (((a) < 0) ? -(a) : (a))
#define SIGN(x)             (x >= 0 ? 1 : -1)
#define MIN(a, b)           (a < b ? a : b)
#define MAX(a, b)           (a > b ? a : b)
#define DEG2RAD(x)          (x * (( float )(G_PI) / 180.0f))
#define RAD2DEG(x)          (x * (180.0f / ( float )(G_PI)))


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
        struct {
            uint8_t n0;
            uint8_t n1;
        };

        struct {
            uint16_t n;
        };
    };
} byte2_t;

// BYTE3
typedef struct _byte3
{
    union
    {
        struct {
            uint8_t n0;
            uint8_t n1;
            uint8_t n2;
            uint8_t pad;
        };
        struct {
            uint32_t n;
        };
    };
} byte3_t;

// BYTE4
typedef struct _byte4
{
    union
    {
        struct {
            uint8_t n0;
            uint8_t n1;
            uint8_t n2;
            uint8_t n3;
        };
        struct {
            uint32_t n;
        };
    };
} byte4_t;

// VECTOR2
typedef struct _vector2
{
    float x;
    float y;
    float GetIndex(byte1_t index) {
        switch (index)
        {
        case 0: { return x; }
        case 1: { return y; }
        }
        return 0.0f;
    }
} vector2_t;

// VECTOR3
typedef struct _vector3
{
    float x;
    float y;
    float z;
    float pad;
    float GetIndex(byte1_t index) {
        switch (index)
        {
        case 0: { return x; }
        case 1: { return y; }
        case 2: { return z; }
        }
        return 0.0f;
    }
} vector3_t;

// VECTOR4
typedef struct _vector4
{
    union {
        struct {
            float r;
            float g;
            float b;
            float a;
        };
        struct {
            float x;
            float y;
            float z;
            float w;
        };
    };

    float GetIndex(byte1_t index) {
        switch (index)
        {
        case 0: { return x; }
        case 1: { return y; }
        case 2: { return z; }
        case 3: { return w; }
        }
        return 0.0f;
    }
} vector4_t;

// MATRIX4
typedef struct _matrix4_t
{
    vector4_t n[4];
    vector4_t GetRow(byte1_t index) { return n[index]; }
    vector4_t GetCol(byte1_t index) {
        vector4_t v = { 0 };
        switch (index)
        {
        case 0:
        {
            v.x = n[0].x;
            v.y = n[1].x;
            v.z = n[2].x;
            v.w = n[3].x;
        } break;
        case 1:
        {
            v.x = n[0].y;
            v.y = n[1].y;
            v.z = n[2].y;
            v.w = n[3].y;
        } break;
        case 2:
        {
            v.x = n[0].z;
            v.y = n[1].z;
            v.z = n[2].z;
            v.w = n[3].z;
        } break;
        case 3:
        {
            v.x = n[0].w;
            v.y = n[1].w;
            v.z = n[2].w;
            v.w = n[3].w;
        } break;
        }
        return v;
    }
} matrix4_t;

// PLANE
typedef struct _plane
{
    float a;
    float b;
    float c;
    float d;
    float GetIndex(byte1_t index) {
        switch (index)
        {
        case 0: { return a; }
        case 1: { return b; }
        case 2: { return c; }
        case 3: { return d; }
        }
        return 0.0f;
    }
} plane_t;

// RECTANGLE
typedef struct _rect
{
    vector2_t min;
    vector2_t max;
    vector2_t GetMin() { return min; }
    vector2_t GetMax() { return max; }
} rect_t;

// VIEWPORT
typedef struct _viewport
{
    float x;
    float y;
    float w;
    float h;
    float n;
    float f;
    float pad0;
    float pad1;
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
inline vector4_t Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) noexcept
{
    vector4_t c = { 0 };

    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;

    return c;
}

//-----------------------------------------------------------------------------
// adds two color values together to create a new color value.
//-----------------------------------------------------------------------------
inline vector4_t AddColor(const vector4_t c0, const vector4_t c1) noexcept
{
    vector4_t c = { 0 };

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
inline vector4_t AdjustColorContrast(const vector4_t color, float contrast) noexcept
{
    vector4_t c = { 0 };

    c.r = CLAMP(0.5f + (contrast * (color.r - 0.5f)), 0.0f, 1.0f);
    c.g = CLAMP(0.5f + (contrast * (color.g - 0.5f)), 0.0f, 1.0f);
    c.b = CLAMP(0.5f + (contrast * (color.b - 0.5f)), 0.0f, 1.0f);
    c.a = color.a;

    return c;
}

//-----------------------------------------------------------------------------
// adjusts the saturation value of a color.
//-----------------------------------------------------------------------------
inline vector4_t AdjustColorSaturation(const vector4_t color, float saturation) noexcept
{
    vector4_t c = { 0 };

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
inline vector4_t InterpolateColor(const vector4_t c0, const vector4_t c1, float s) noexcept
{
    vector4_t c = { 0 };

    c.r = CLAMP(c0.r + s * (c1.r - c0.r), 0.0f, 1.0f);
    c.g = CLAMP(c0.g + s * (c1.g - c0.g), 0.0f, 1.0f);
    c.b = CLAMP(c0.b + s * (c1.b - c0.b), 0.0f, 1.0f);
    c.a = CLAMP(c0.a + s * (c1.a - c0.a), 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// blends two colors.
//-----------------------------------------------------------------------------
inline vector4_t ModulateColor(const vector4_t c0, const vector4_t c1) noexcept
{
    vector4_t c = { 0 };

    c.r = CLAMP(c0.r * c1.r, 0.0f, 1.0f);
    c.g = CLAMP(c0.g * c1.g, 0.0f, 1.0f);
    c.b = CLAMP(c0.b * c1.b, 0.0f, 1.0f);
    c.a = CLAMP(c0.a * c1.a, 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// creates the negative color value of a color value.
//-----------------------------------------------------------------------------
inline vector4_t NegateColor(const vector4_t c0) noexcept
{
    vector4_t c = { 0 };

    c.r = CLAMP(1.0f - c0.r, 0.0f, 1.0f);
    c.g = CLAMP(1.0f - c0.g, 0.0f, 1.0f);
    c.b = CLAMP(1.0f - c0.b, 0.0f, 1.0f);
    c.a = CLAMP(1.0f - c0.a, 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// scales a color value.
//-----------------------------------------------------------------------------
inline vector4_t ScaleColor(const vector4_t c0, float s) noexcept
{
    vector4_t c = { 0 };

    c.r = CLAMP(c0.r * s, 0.0f, 1.0f);
    c.g = CLAMP(c0.g * s, 0.0f, 1.0f);
    c.b = CLAMP(c0.b * s, 0.0f, 1.0f);
    c.a = CLAMP(c0.a * s, 0.0f, 1.0f);

    return c;
}

//-----------------------------------------------------------------------------
// subtracts two color values to create a new color value.
//-----------------------------------------------------------------------------
inline vector4_t SubtractColor(const vector4_t c0, const vector4_t c1)  noexcept
{
    vector4_t c = { 0 };

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
inline byte4_t RGBAColor(const vector4_t& color) noexcept
{
    byte4_t c = { 0 };

    c.n0 = (uint8_t)(CLAMP(color.r, 0.0f, 1.0f) * 255.0f);
    c.n1 = (uint8_t)(CLAMP(color.g, 0.0f, 1.0f) * 255.0f);
    c.n2 = (uint8_t)(CLAMP(color.b, 0.0f, 1.0f) * 255.0f);
    c.n3 = (uint8_t)(CLAMP(color.a, 0.0f, 1.0f) * 255.0f);

    return c;
}

//-----------------------------------------------------------------------------
// returns color in bgra format.
//-----------------------------------------------------------------------------
inline byte4_t BGRAColor(const vector4_t& color) noexcept
{
    byte4_t c = { 0 };

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
inline rect_t Rectangle(float x0 = 0.0f, float y0 = 0.0f, float x1 = 0.0f,
    float y1 = 0.0f)
{
    rect_t r = {};

    r.min.x = x0;
    r.min.y = y0;
    r.max.x = x1;
    r.max.y = y1;

    return r;
}

//-----------------------------------------------------------------------------
// tests to see if rectangles intersect
//-----------------------------------------------------------------------------
inline bool IntersectsRectangle(const rect_t& r0, const rect_t& r1)
{
    const float minX = r0.max.x < r1.max.x ? r0.max.x : r1.max.x;
    const float minY = r0.max.y < r1.max.y ? r0.max.y : r1.max.y;
    const float maxX = r0.min.x > r1.min.y ? r0.min.x : r1.min.x;
    const float maxY = r0.min.y > r1.min.y ? r0.min.y : r1.min.y;

    if (minX > maxX && minY > maxY)
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
// returns true if point is outside the specified rectangle
//-----------------------------------------------------------------------------
inline bool ContainsRectangle(const rect_t& rect, float x, float y)
{
    if ((rect.min.x <= x && x < rect.max.x) &&
        (rect.min.y <= y && y < rect.max.y))
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
    if (x < rect.min.x || x > rect.max.x ||
        y < rect.min.y || y > rect.max.y)
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
    rect.min.x += h >> 1;
    rect.min.y += v >> 1;
    rect.max.x -= h >> 1;
    rect.max.y -= v >> 1;
}

//-----------------------------------------------------------------------------
// increases the rectangle by a specified amount in x and y
//-----------------------------------------------------------------------------
inline void InflateRectangle(rect_t& rect, long h, long v)
{
    rect.min.x -= h >> 1;
    rect.min.y -= v >> 1;
    rect.max.x += h >> 1;
    rect.max.y += v >> 1;
}

//-----------------------------------------------------------------------------
// moves or offsets the rectangle by moving the upper-left position
//-----------------------------------------------------------------------------
inline void OffsetRectangle(rect_t& rect, long x, long y)
{
    rect.min.x += x;
    rect.min.y += y;
    rect.max.x += x;
    rect.max.y += y;
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

    if (a.x != 0.0f || a.y != 0.0f || a.z != 0.0f || a.w != 0.0f)
    {
        const float l = sqrtf(DotVector4(a, a));
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

    a.x = v.x * m.n[0].x + v.y * m.n[1].x + v.z * m.n[2].x + v.w * m.n[3].x;
    a.y = v.x * m.n[0].y + v.y * m.n[1].y + v.z * m.n[2].y + v.w * m.n[3].y;
    a.z = v.x * m.n[0].z + v.y * m.n[1].z + v.z * m.n[2].z + v.w * m.n[3].z;
    a.w = v.x * m.n[0].w + v.y * m.n[1].w + v.z * m.n[2].w + v.w * m.n[3].w;

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

    a.x = v.x * m.n[0].x + v.y * m.n[1].x + v.z * m.n[2].x + m.n[3].x;
    a.y = v.x * m.n[0].y + v.y * m.n[1].y + v.z * m.n[2].y + m.n[3].y;
    a.z = v.x * m.n[0].z + v.y * m.n[1].z + v.z * m.n[2].z + m.n[3].z;
    a.w = v.x * m.n[0].w + v.y * m.n[1].w + v.z * m.n[2].w + m.n[3].w;

    return a;
}

//-----------------------------------------------------------------------------
// transforms a 3d vector by a given matrix
//-----------------------------------------------------------------------------
inline vector3_t TransformVector3Coord(vector3_t v, matrix4_t m)
{
    vector3_t a = Vector3();

    a.x = v.x * m.n[0].x + v.y * m.n[1].x + v.z * m.n[2].x + m.n[3].x;
    a.y = v.x * m.n[0].y + v.y * m.n[1].y + v.z * m.n[2].y + m.n[3].y;
    a.z = v.x * m.n[0].z + v.y * m.n[1].z + v.z * m.n[2].z + m.n[3].z;

    return a;
}

//-----------------------------------------------------------------------------
// transforms the 3d vector normal by the given matrix.
//-----------------------------------------------------------------------------
inline vector3_t TransformVector3Normal(const vector3_t v, const matrix4_t m)
{
    vector3_t a = Vector3();

    a.x = v.x * m.n[0].x + v.y * m.n[1].x + v.z * m.n[2].x;
    a.y = v.x * m.n[0].y + v.y * m.n[1].y + v.z * m.n[2].y;
    a.z = v.x * m.n[0].z + v.y * m.n[1].z + v.z * m.n[2].z;

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

    a.x = v.x * m.n[0].x + v.y * m.n[1].x;
    a.y = v.x * m.n[0].y + v.y * m.n[1].y;
    a.z = 0.0f;
    a.w = v.x * m.n[0].w + v.y * m.n[1].w;

    return a;
}

//-----------------------------------------------------------------------------
// transforms a 2d vector by a given matrix, projecting the result back into w = 1
//-----------------------------------------------------------------------------
inline vector2_t TransformVector2Coord(const vector2_t v, const matrix4_t m)
{
    vector2_t a = Vector2();

    a.x = v.x * m.n[0].x + v.y * m.n[1].x + m.n[3].x;
    a.y = v.x * m.n[0].y + v.y * m.n[1].y + m.n[3].y;

    return a;
}

//-----------------------------------------------------------------------------
// transforms the 2d vector normal by the given matrix.
//-----------------------------------------------------------------------------
inline vector2_t TransformVector2Normal(const vector2_t v, const matrix4_t m)
{
    vector2_t a = Vector2();

    a.x = v.x * m.n[0].x + v.y * m.n[1].x;
    a.y = v.x * m.n[0].y + v.y * m.n[1].y;

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


//-----------------------------------------------------------------------------
// creates an identity matrix
//-----------------------------------------------------------------------------
inline matrix4_t Matrix4(
    float _00 = 1.0f, float _01 = 0.0f, float _02 = 0.0f, float _03 = 0.0f,
    float _10 = 0.0f, float _11 = 1.0f, float _12 = 0.0f, float _13 = 0.0f,
    float _20 = 0.0f, float _21 = 0.0f, float _22 = 1.0f, float _23 = 0.0f,
    float _30 = 0.0f, float _31 = 0.0f, float _32 = 0.0f, float _33 = 1.0f)
{
    matrix4_t m = { 0 };

    m.n[0].x = _00;
    m.n[0].y = _01;
    m.n[0].z = _02;
    m.n[0].w = _03;
    m.n[1].x = _10;
    m.n[1].y = _11;
    m.n[1].z = _12;
    m.n[1].w = _13;
    m.n[2].x = _20;
    m.n[2].y = _21;
    m.n[2].z = _22;
    m.n[2].w = _23;
    m.n[3].x = _30;
    m.n[3].y = _31;
    m.n[3].z = _32;
    m.n[3].w = _33;

    return m;
}

//-----------------------------------------------------------------------------
// returns the determinant of the matrix
//-----------------------------------------------------------------------------
inline float DeterminantMatrix4(const matrix4_t m)
{
    const float a0 = (m.n[0].x * m.n[1].y) - (m.n[1].x * m.n[0].y);
    const float a1 = (m.n[0].x * m.n[1].z) - (m.n[1].x * m.n[0].z);
    const float a2 = (m.n[0].x * m.n[1].w) - (m.n[1].x * m.n[0].w);
    const float a3 = (m.n[0].y * m.n[1].z) - (m.n[1].y * m.n[0].z);
    const float a4 = (m.n[0].y * m.n[1].w) - (m.n[1].y * m.n[0].w);
    const float a5 = (m.n[0].z * m.n[1].w) - (m.n[1].z * m.n[0].w);
    const float b0 = (m.n[2].x * m.n[3].y) - (m.n[3].x * m.n[2].y);
    const float b1 = (m.n[2].x * m.n[3].z) - (m.n[3].x * m.n[2].z);
    const float b2 = (m.n[2].x * m.n[3].w) - (m.n[3].x * m.n[2].w);
    const float b3 = (m.n[2].y * m.n[3].z) - (m.n[3].y * m.n[2].z);
    const float b4 = (m.n[2].y * m.n[3].w) - (m.n[3].y * m.n[2].w);
    const float b5 = (m.n[2].z * m.n[3].w) - (m.n[3].z * m.n[2].w);

    const float d = (a0 * b5) - (a1 * b4) + (a2 * b3) + (a3 * b2) - (a4 * b1) +
        (a5 * b0);

    return d;
}

//-----------------------------------------------------------------------------
// calculates the inverse of a matrix
//-----------------------------------------------------------------------------
inline matrix4_t InverseMatrix4(const matrix4_t m)
{
    const float a0 = (m.n[0].x * m.n[1].y) - (m.n[1].x * m.n[0].y);
    const float a1 = (m.n[0].x * m.n[1].z) - (m.n[1].x * m.n[0].z);
    const float a2 = (m.n[0].x * m.n[1].w) - (m.n[1].x * m.n[0].w);
    const float a3 = (m.n[0].y * m.n[1].z) - (m.n[1].y * m.n[0].z);
    const float a4 = (m.n[0].y * m.n[1].w) - (m.n[1].y * m.n[0].w);
    const float a5 = (m.n[0].z * m.n[1].w) - (m.n[1].z * m.n[0].w);
    const float b0 = (m.n[2].x * m.n[3].y) - (m.n[3].x * m.n[2].y);
    const float b1 = (m.n[2].x * m.n[3].z) - (m.n[3].x * m.n[2].z);
    const float b2 = (m.n[2].x * m.n[3].w) - (m.n[3].x * m.n[2].w);
    const float b3 = (m.n[2].y * m.n[3].z) - (m.n[3].y * m.n[2].z);
    const float b4 = (m.n[2].y * m.n[3].w) - (m.n[3].y * m.n[2].w);
    const float b5 = (m.n[2].z * m.n[3].w) - (m.n[3].z * m.n[2].w);

    const float d = (a0 * b5) - (a1 * b4) + (a2 * b3) + (a3 * b2) - (a4 * b1) +
        (a5 * b0);

    if (d == 0.0f)
    {
        return Matrix4();
    }

    const float id = 1.0f / d;

    matrix4_t a = Matrix4();
    a.n[0].x = ((m.n[1].y * b5) - (m.n[1].z * b4) + (m.n[1].w * b3)) * id;
    a.n[1].x = ((m.n[1].x * b5) + (m.n[1].z * b2) - (m.n[1].w * b1)) * id;
    a.n[2].x = ((m.n[1].x * b4) - (m.n[1].y * b2) + (m.n[1].w * b0)) * id;
    a.n[3].x = ((m.n[1].x * b3) + (m.n[1].y * b1) - (m.n[1].z * b0)) * id;

    a.n[0].y = ((m.n[0].y * b5) + (m.n[0].z * b4) - (m.n[0].w * b3)) * id;
    a.n[1].y = ((m.n[0].x * b5) - (m.n[0].z * b2) + (m.n[0].w * b1)) * id;
    a.n[2].y = ((m.n[0].x * b4) + (m.n[0].y * b2) - (m.n[0].w * b0)) * id;
    a.n[3].y = ((m.n[0].x * b3) - (m.n[0].y * b1) + (m.n[2].w * b0)) * id;

    a.n[0].z = ((m.n[3].y * a5) - (m.n[3].z * a4) + (m.n[3].w * a3)) * id;
    a.n[1].z = ((m.n[3].x * a5) + (m.n[3].z * a2) - (m.n[3].w * a1)) * id;
    a.n[2].z = ((m.n[3].x * a4) - (m.n[3].y * a2) + (m.n[3].w * a0)) * id;
    a.n[3].z = ((m.n[3].x * a3) + (m.n[3].y * a1) - (m.n[3].z * a0)) * id;

    a.n[0].w = ((m.n[2].y * a5) + (m.n[2].z * a4) - (m.n[2].w * a3)) * id;
    a.n[1].w = ((m.n[2].x * a5) - (m.n[2].z * a2) + (m.n[2].w * a1)) * id;
    a.n[2].w = ((m.n[2].x * a4) + (m.n[2].y * a2) - (m.n[2].w * a0)) * id;
    a.n[3].w = ((m.n[2].x * a3) - (m.n[2].y * a1) + (m.n[2].z * a0)) * id;

    return a;
}

//-----------------------------------------------------------------------------
// determines the product of two matrices
//-----------------------------------------------------------------------------
inline matrix4_t MultiplyMatrix4(const matrix4_t a, const matrix4_t b)
{
    matrix4_t m = Matrix4();

    m.n[0].x = a.n[0].x * b.n[0].x + a.n[0].y * b.n[1].x + a.n[0].z * b.n[2].x +
               a.n[0].w * b.n[3].x;
    m.n[0].y = a.n[0].x * b.n[0].y + a.n[0].y * b.n[1].y + a.n[0].z * b.n[2].y +
               a.n[0].w * b.n[3].y;
    m.n[0].z = a.n[0].x * b.n[0].z + a.n[0].y * b.n[1].z + a.n[0].z * b.n[2].z +
               a.n[0].w * b.n[3].z;
    m.n[0].w = a.n[0].x * b.n[0].w + a.n[0].y * b.n[1].w + a.n[0].z * b.n[2].w +
               a.n[0].w * b.n[3].w;

    m.n[1].x = a.n[1].x * b.n[0].x + a.n[1].y * b.n[1].x + a.n[1].z * b.n[2].x +
               a.n[1].w * b.n[3].x;
    m.n[1].y = a.n[1].x * b.n[0].y + a.n[1].y * b.n[1].y + a.n[1].z * b.n[2].y +
               a.n[1].w * b.n[3].y;
    m.n[1].z = a.n[1].x * b.n[0].z + a.n[1].y * b.n[1].z + a.n[1].z * b.n[2].z +
               a.n[1].w * b.n[3].z;
    m.n[1].w = a.n[1].x * b.n[0].w + a.n[1].y * b.n[1].w + a.n[1].z * b.n[2].w +
               a.n[1].w * b.n[3].w;

    m.n[2].x = a.n[2].x * b.n[0].x + a.n[2].y * b.n[1].x + a.n[2].z * b.n[2].x +
               a.n[2].w * b.n[3].x;
    m.n[2].y = a.n[2].x * b.n[0].y + a.n[2].y * b.n[1].y + a.n[2].z * b.n[2].y +
               a.n[2].w * b.n[3].y;
    m.n[2].z = a.n[2].x * b.n[0].z + a.n[2].y * b.n[1].z + a.n[2].z * b.n[2].z +
               a.n[2].w * b.n[3].z;
    m.n[2].w = a.n[2].x * b.n[0].w + a.n[2].y * b.n[1].w + a.n[2].z * b.n[2].w +
               a.n[2].w * b.n[3].w;

    m.n[3].x = a.n[3].x * b.n[0].x + a.n[3].y * b.n[1].x + a.n[3].z * b.n[2].x +
               a.n[3].w * b.n[3].x;
    m.n[3].y = a.n[3].x * b.n[0].y + a.n[3].y * b.n[1].y + a.n[3].z * b.n[2].y +
               a.n[3].w * b.n[3].y;
    m.n[3].z = a.n[3].x * b.n[0].z + a.n[3].y * b.n[1].z + a.n[3].z * b.n[2].z +
               a.n[3].w * b.n[3].z;
    m.n[3].w = a.n[3].x * b.n[0].w + a.n[3].y * b.n[1].w + a.n[3].z * b.n[2].w +
               a.n[3].w * b.n[3].w;

    return m;
}

//-----------------------------------------------------------------------------
// builds a left-handed look-at matrix
//-----------------------------------------------------------------------------
inline matrix4_t LookAtLHMatrix4(const vector3_t eye, const vector3_t at,
	const vector3_t up)
{
    matrix4_t m = Matrix4();
    vector3_t x = Vector3();
    vector3_t y = Vector3();
    vector3_t z = Vector3();

    z = NormalizeVector3(SubtractVector3(at, eye));
    x = NormalizeVector3(CrossVector3(up, z));
    y = CrossVector3(z, x);

    m.n[0].x = x.x;
    m.n[0].y = y.x;
    m.n[0].z = z.x;
    m.n[0].w = 0.0f;

    m.n[1].x = x.y;
    m.n[1].y = y.y;
    m.n[1].z = z.y;
    m.n[1].w = 0.0f;

    m.n[2].x = x.z;
    m.n[2].y = y.z;
    m.n[2].z = z.z;
    m.n[2].w = 0.0f;

    m.n[3].x = -DotVector3(x, eye);
    m.n[3].y = -DotVector3(y, eye);
    m.n[3].z = -DotVector3(z, eye);
    m.n[3].w = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a right-handed look-at matrix
//-----------------------------------------------------------------------------
inline matrix4_t LookAtRHMatrix4(const vector3_t eye, const vector3_t at,
	const vector3_t up)
{
    matrix4_t m = Matrix4();
    vector3_t x = Vector3();
    vector3_t y = Vector3();
    vector3_t z = Vector3();

    z = NormalizeVector3(SubtractVector3(eye, at));
    x = NormalizeVector3(CrossVector3(up, z));
    y = CrossVector3(z, x);

    m.n[0].x = x.x;
    m.n[0].y = y.x;
    m.n[0].z = z.x;
    m.n[0].w = 0.0f;

    m.n[1].x = x.y;
    m.n[1].y = y.y;
    m.n[1].z = z.y;
    m.n[1].w = 0.0f;

    m.n[2].x = x.z;
    m.n[2].y = y.z;
    m.n[2].z = z.z;
    m.n[2].w = 0.0f;

    m.n[3].x = -DotVector3(x, eye);
    m.n[3].y = -DotVector3(y, eye);
    m.n[3].z = -DotVector3(z, eye);
    m.n[3].w = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a left-handed orthographic projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t OrthographicLHMatrix4(float w, float h, float n, float f)
{
    matrix4_t m = Matrix4();

    m.n[0].x = 2.0f / w;
    m.n[1].y = 2.0f / h;
    m.n[2].z = 1.0f / (f - n);
    m.n[3].z = n / (n - f);
    m.n[3].w = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a left-handed orthographic projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t OrthographicRHMatrix4(float w, float h, float n, float f)
{
    matrix4_t m = Matrix4();

    m.n[0].x = 2.0f / w;
    m.n[1].y = 2.0f / h;
    m.n[2].z = 1.0f / (n - f);
    m.n[3].z = n / (f - n);
    m.n[3].w = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a customized, left-handed orthographic projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t OrthographicOffCenterLHMatrix4(float l, float r, float b, float t,
    float n, float f)
{
    matrix4_t m = Matrix4();

    m.n[0].x = 2.0f / (r - l);
    m.n[1].y = 2.0f / (t - b);
    m.n[2].z = 1.0f / (f - n);
    m.n[3].x = (l + r) / (l - r);
    m.n[3].y = (b + t) / (b - t);
    m.n[3].z = n / (n - f);
    m.n[3].w = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a customized, right-handed orthographic projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t OrthographicOffCenterRHMatrix4(float l, float r, float b, float t,
    float n, float f)
{
    matrix4_t m = Matrix4();

    m.n[0].x = 2.0f / (r - l);
    m.n[1].y = 2.0f / (t - b);
    m.n[2].z = 1.0f / (n - f);
    m.n[3].x = (l + r) / (l - r);
    m.n[3].y = (b + t) / (b - t);
    m.n[3].z = n / (n - f);
    m.n[3].w = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a left-handed perspective projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveLHMatrix4(float w, float h, float n, float f)
{
    matrix4_t m = Matrix4();

    m.n[0].x = 2.0f / w;
    m.n[1].y = 2.0f / h;
    m.n[2].z = f / (f - n);
    m.n[2].w = 1.0f;
    m.n[3].z = (n + f) / (n - f);
    m.n[3].w = 0.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a right-handed perspective projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveRHMatrix4(float w, float h, float n, float f)
{
    matrix4_t m = Matrix4();

    m.n[0].x = 2.0f / w;
    m.n[1].y = 2.0f / h;
    m.n[2].z = f / (n - f);
    m.n[2].w = -1.0f;
    m.n[3].z = (n + f) / (n - f);
    m.n[3].w = 0.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a left-handed perspective projection matrix based on a field of view
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveFovLHMatrix4(float fovy, float aspect, float n, float f)
{
    matrix4_t m = Matrix4();
    const float cotangent = tanf((G_PIHALF - (fovy / 2.0f)));

    m.n[0].x = cotangent / aspect;
    m.n[1].y = cotangent;
    m.n[2].z = f / (f - n);
    m.n[2].w = 1.0f;
    m.n[3].z = (n + f) / (n - f);
    m.n[3].w = 0.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a right-handed perspective projection matrix based on a field of view
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveFovRHMatrix4(float fovy, float aspect, float n, float f)
{
    matrix4_t m = Matrix4();
    const float cotangent = tanf((G_PIHALF - (fovy / 2.0f)));

    m.n[0].x = cotangent / aspect;
    m.n[1].y = cotangent;
    m.n[2].z = f / (n - f);
    m.n[2].w = -1.0f;
    m.n[3].z = (n + f) / (n - f);
    m.n[3].w = 0.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a customized, left-handed perspective projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveOffCenterLHMatrix4(float l, float r, float b, float t,
    float n, float f)
{
    matrix4_t m = Matrix4();

    m.n[0].x = (2.0f * n) / (r - l);
    m.n[1].y = (2.0f * n) / (t - b);
    m.n[2].x = (l + r) / (l - r);
    m.n[2].y = (b + t) / (b - t);
    m.n[2].z = f / (f - n);
    m.n[2].w = 1.0f;
    m.n[3].z = (n + f) / (n - f);
    m.n[3].w = 0.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a customized, right-handed perspective projection matrix
//-----------------------------------------------------------------------------
inline matrix4_t PerspectiveOffCenterRHMatrix4(float l, float r, float b, float t,
    float n, float f)
{
    matrix4_t m = Matrix4();

    m.n[0].x = (2.0f * n) / (r - l);
    m.n[1].y = (2.0f * n) / (t - b);
    m.n[2].x = (l + r) / (l - r);
    m.n[2].y = (b + t) / (b - t);
    m.n[2].z = f / (n - f);
    m.n[2].w = -1.0f;
    m.n[3].z = (n + f) / (n - f);
    m.n[3].w = 0.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix that reflects the coordinate system about a plane.
//-----------------------------------------------------------------------------
inline matrix4_t ReflectMatrix4(const plane_t plane)
{
    matrix4_t m = Matrix4();
    const plane_t p = NormalizePlane(plane);

    m.n[0].x = -2.0f * p.a * p.a + 1;
    m.n[0].y = -2.0f * p.b * p.a;
    m.n[0].z = -2.0f * p.c * p.a;

    m.n[1].x = -2.0f * p.a * p.b;
    m.n[1].y = -2.0f * p.b * p.b + 1;
    m.n[1].z = -2.0f * p.c * p.b;

    m.n[2].x = -2.0f * p.a * p.c;
    m.n[2].y = -2.0f * p.b * p.c;
    m.n[2].z = -2.0f * p.c * p.c + 1;
    m.n[2].w = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix that rotates around an arbitrary axis
//-----------------------------------------------------------------------------
inline matrix4_t RotationAxisAngleMatrix4(const vector3_t v, float angle)
{
    matrix4_t m = Matrix4();
    const float cosine = sinf(G_PIHALF - angle);
    const float sine = sinf(angle);
    const float oneMinusCosine = (1.0f - cosine);

    m.n[0].x = v.x * v.x * oneMinusCosine + cosine;
    m.n[0].y = v.x * v.y * oneMinusCosine + v.z * sine;
    m.n[0].z = v.x * v.z * oneMinusCosine - v.y * sine;

    m.n[1].x = v.x * v.y * oneMinusCosine - v.z * sine;
    m.n[1].y = v.y * v.y * oneMinusCosine + cosine;
    m.n[1].z = v.y * v.z * oneMinusCosine + v.x * sine;

    m.n[2].x = v.x * v.z * oneMinusCosine + v.y * sine;
    m.n[2].y = v.y * v.z * oneMinusCosine - v.x * sine;
    m.n[2].z = v.z * v.z * oneMinusCosine + cosine;

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix that rotates around the x-axis
//-----------------------------------------------------------------------------
inline matrix4_t RotationXMatrix4(float angle)
{
    matrix4_t m = Matrix4();

    m.n[1].y =  cosf(angle);
    m.n[1].z =  sinf(angle);
    m.n[2].y = -m.n[1].z;
    m.n[2].z =  m.n[1].y;

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix that rotates around the y-axis
//-----------------------------------------------------------------------------
inline matrix4_t RotationYMatrix4(float angle)
{
    matrix4_t m = Matrix4();

    m.n[0].x =  cosf(angle);
    m.n[0].z = -sinf(angle);
    m.n[2].x = -m.n[0].z;
    m.n[2].z =  m.n[0].x;

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix that rotates around the z-axis
//-----------------------------------------------------------------------------
inline matrix4_t RotationZMatrix4(float angle)
{
    matrix4_t m = Matrix4();

    m.n[0].x =  cosf(angle);
    m.n[0].y =  sinf(angle);
    m.n[1].x = -m.n[0].y;
    m.n[1].y =  m.n[0].x;

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

    m.n[0].x = x;
    m.n[1].y = y;
    m.n[2].z = z;

    return m;
}

//-----------------------------------------------------------------------------
// builds a matrix using the specified offsets
//-----------------------------------------------------------------------------
inline matrix4_t TranslationMatrix4(float x, float y, float z)
{
    matrix4_t m = Matrix4();

    m.n[3].x = x;
    m.n[3].y = y;
    m.n[3].z = z;

    return m;
}

//-----------------------------------------------------------------------------
// builds the matrix transpose of a matrix
//-----------------------------------------------------------------------------
inline matrix4_t TransposeMatrix4(const matrix4_t m)
{
    matrix4_t a = Matrix4();

    a.n[0].x = m.n[0].x;
    a.n[0].y = m.n[1].x;
    a.n[0].z = m.n[2].x;
    a.n[0].w = m.n[3].x;
    a.n[1].x = m.n[0].y;
    a.n[1].y = m.n[1].y;
    a.n[1].z = m.n[2].y;
    a.n[1].w = m.n[3].y;
    a.n[2].x = m.n[0].z;
    a.n[2].y = m.n[1].z;
    a.n[2].z = m.n[2].z;
    a.n[2].w = m.n[3].z;
    a.n[3].x = m.n[0].w;
    a.n[3].y = m.n[1].w;
    a.n[3].z = m.n[2].w;
    a.n[3].w = m.n[3].w;

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
// builds a 3D transformation matrix that represents transformations in the yz
// plane
//-----------------------------------------------------------------------------
inline matrix4_t TransformationX3DMatrix4(const vector3_t scalingCenter,
    const vector3_t scale, const vector3_t rotationCenter, float angle,
    const vector3_t translation)
{
    matrix4_t r = MultiplyMatrix4(InverseMatrix4(
        TranslationMatrix4(rotationCenter.x, rotationCenter.y, rotationCenter.z)),
        RotationXMatrix4(angle));
    r = MultiplyMatrix4(r, TranslationMatrix4(rotationCenter.x, rotationCenter.y,
        rotationCenter.z));

    matrix4_t s = MultiplyMatrix4(InverseMatrix4(
        TranslationMatrix4(scalingCenter.x, scalingCenter.y, scalingCenter.z)),
        ScalingMatrix4(scale.x, scale.y, scale.z));
    s = MultiplyMatrix4(s, TranslationMatrix4(scalingCenter.x, scalingCenter.y,
        scalingCenter.z));

    const matrix4_t t = TranslationMatrix4(translation.x, translation.y,
        translation.z);

    return MultiplyMatrix4(MultiplyMatrix4(r, s), t);
}

//-----------------------------------------------------------------------------
// builds a 3D transformation matrix that represents transformations in the xz
// plane
//-----------------------------------------------------------------------------
inline matrix4_t TransformationY3DMatrix4(const vector3_t scalingCenter,
    const vector3_t scale, const vector3_t rotationCenter, float angle,
    const vector3_t translation)
{
    matrix4_t r = MultiplyMatrix4(InverseMatrix4(
        TranslationMatrix4(rotationCenter.x, rotationCenter.y, rotationCenter.z)),
        RotationYMatrix4(angle));
    r = MultiplyMatrix4(r, TranslationMatrix4(rotationCenter.x, rotationCenter.y,
        rotationCenter.z));

    matrix4_t s = MultiplyMatrix4(InverseMatrix4(
        TranslationMatrix4(scalingCenter.x, scalingCenter.y, scalingCenter.z)),
        ScalingMatrix4(scale.x, scale.y, scale.z));
    s = MultiplyMatrix4(s, TranslationMatrix4(scalingCenter.x, scalingCenter.y,
        scalingCenter.z));

    const matrix4_t t = TranslationMatrix4(translation.x, translation.y,
        translation.z);

    return MultiplyMatrix4(MultiplyMatrix4(r, s), t);
}

//-----------------------------------------------------------------------------
// builds a 3D transformation matrix that represents transformations in the xy
// plane
//-----------------------------------------------------------------------------
inline matrix4_t TransformationZ3DMatrix4(const vector3_t scalingCenter,
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
        ScalingMatrix4(scale.x, scale.y, scale.z));
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
inline viewport_t Viewport3d(float x = 0.0f, float y = 0.0f, float w = 0.0f,
    float h = 0.0f, float n = 0.0f, float f = 0.0f)
{
    viewport_t v = {};

    v.x = x;
    v.y = y;
    v.w = w;
    v.h = h;
    v.n = n;
    v.f = f;

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
        (float)(viewport.y + viewport.h), viewport.n,
        viewport.f), MultiplyMatrix4(projection, MultiplyMatrix4(view, world)));
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
            viewport.n, viewport.f))));

    return TransformVector3Coord(v, t);
}


#endif // #ifndef _G_MATH_H_

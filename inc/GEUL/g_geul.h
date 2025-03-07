/*
* Simple 3d Image
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

#ifndef _G_GEUL_H_
#define _G_GEUL_H_

#include <stdint.h>

#define GEUL_MAJOR_VERSION      1
#define GEUL_MINOR_VERSION      0
#define GEUL_PATCH_VERSION      0

//-----------------------------------------------------------------------------
// Operating System and CPU.
//-----------------------------------------------------------------------------

#if defined (_WIN32)
#define GUEL_OS_WIN32    1
#else
#error Unsupported operating system
#endif // #if defined (_WIN32)

#if defined (GUEL_OS_WIN32)
#if defined (_M_IX86)
#define GUEL_CPU_X86     1
#endif // #if defined (_M_IX86)

#if defined (_M_X64)
#define GUEL_CPU_X64     1
#endif // #if defined (_M_X64)
#endif // #if defined (GUEL_OS_WIN32)

#if defined (GUEL_CPU_X86) || defined (GUEL_CPU_X64) || defined (GUEL_OS_WIN32)
#define GUEL_LITTLE_ENDIAN       1
#endif // #if defined (GUEL_CPU_X86) || defined (GUEL_CPU_X64) || defined (GUEL_OS_WIN32)

#if defined (GUEL_OS_WIN32)
#if defined (GEUL_DLL)
#define GEUL_DECLSPEC __declspec(dllexport)
#else
#define GEUL_DECLSPEC __declspec(dllimport)
#endif
#endif

//-----------------------------------------------------------------------------
// Endianess utility functions.
//-----------------------------------------------------------------------------

inline uint16_t
SwapU16(uint16_t v)
{
    uint16_t swapped;

    swapped = (v << 8 | v >> 8);

    return swapped;
}

inline int16_t
SwapI16(int16_t v)
{
    return (int16_t)SwapU16((uint16_t)v);
}

inline uint32_t
SwapU32(uint32_t v)
{
    uint32_t swapped;

    swapped = (v & 0xFF) << 24;
    swapped |= (v & 0xFF00) << 8;
    swapped |= (v >> 8) & 0xFF00;
    swapped |= (v >> 24);

    return swapped;
}

inline int32_t
SwapI32(int32_t v)
{
    return (int32_t)SwapU32((uint32_t)v);
}

#if defined (GUEL_LITTLE_ENDIAN)
#define LittleU16(x)    (x)
#define LittleU32(x)    (x)
#define LittleI16(x)    (x)
#define LittleI32(x)    (x)
#define BigU16(x)       (SwapU16(x))
#define BigU32(x)       (SwapU32(x))
#define BigI16(x)       (SwapI16(x))
#define BigI32(x)       (SwapI32(x))
#else
#define LittleU16(x)    (SwapU16(x))
#define LittleU32(x)    (SwapU32(x))
#define LittleI16(x)    (SwapI16(x))
#define LittleI32(x)    (SwapI32(x))
#define BigU16(x)       (x)
#define BigU32(x)       (x)
#define BigI16(x)       (x)
#define BigI32(x)       (x)
#endif

inline uint16_t*
WriteU16ToLE(void* dst, uint16_t val)
{
    uint16_t* p16 = (uint16_t*)dst;
    uint8_t* p = (uint8_t*)dst;

    val = LittleU16(val);

    p[0] = (val & 0xFF);
    p[1] = (val & 0xFF00) >> 8;

    return p16;
}

inline int16_t*
WriteI16ToLE(void* dst, int16_t val)
{
    return (int16_t*)WriteU16ToLE(dst, (uint16_t)val);
}

inline uint32_t*
WriteU32ToLE(void* dst, uint32_t val)
{
    uint32_t* p32 = (uint32_t*)dst;
    uint8_t* p = (uint8_t*)dst;

    val = LittleU32(val);

    p[0] = (val & 0xFF);
    p[1] = (val & 0xFF00) >> 8;
    p[2] = (val & 0xFF0000) >> 16;
    p[3] = (val & 0xFF000000) >> 24;

    return p32;
}

inline int32_t*
WriteI32ToLE(void* dst, int32_t val)
{
    return (int32_t*)WriteU32ToLE(dst, (uint32_t)val);
}

inline uint16_t*
WriteU16ToBE(void* dst, uint16_t val)
{
    uint16_t* p16 = (uint16_t*)dst;
    uint8_t* p = (uint8_t*)dst;

    val = BigU16(val);

    p[0] = (val & 0xFF);
    p[1] = (val & 0xFF00) >> 8;

    return p16;
}

inline int16_t*
WriteI16ToBE(void* dst, int16_t val)
{
    return (int16_t*)WriteU16ToBE(dst, (uint16_t)val);
}

inline uint32_t*
WriteU32ToBE(void* dst, uint32_t val)
{
    uint32_t* p32 = (uint32_t*)dst;
    uint8_t* p = (uint8_t*)dst;

    val = BigU32(val);

    p[0] = (val & 0xFF);
    p[1] = (val & 0xFF00) >> 8;
    p[2] = (val & 0xFF0000) >> 16;
    p[3] = (val & 0xFF000000) >> 24;

    return p32;
}

inline int32_t*
WriteI32ToBE(void* dst, int32_t val)
{
    return (int32_t*)WriteU32ToBE(dst, (uint32_t)val);
}

inline uint16_t
ReadU16FromLE(const void* src)
{
    uint16_t v = 0;
    uint8_t* p = (uint8_t*)src;

    v |= p[0];
    v |= ((uint16_t)p[1]) << 8;

    return LittleU16(v);
}

inline int16_t
ReadI16FromLE(const void* src)
{
    return (int16_t)ReadU16FromLE(src);
}

inline uint32_t
ReadU32FromLE(const void* src)
{
    uint32_t v = 0;
    uint8_t* p = (uint8_t*)src;

    v |= p[0];
    v |= ((uint32_t)p[1]) << 8;
    v |= ((uint32_t)p[2]) << 16;
    v |= ((uint32_t)p[3]) << 24;

    return LittleU32(v);
}

inline int32_t
ReadI32FromLE(const void* src)
{
    return (int32_t)ReadU32FromLE(src);
}

inline uint16_t
ReadU16FromBE(const void* src)
{
    uint16_t v = 0;
    uint8_t* p = (uint8_t*)src;

    v |= p[0];
    v |= ((uint16_t)p[1]) << 8;

    return BigU16(v);
}

inline int16_t
ReadI16FromBE(const void* src)
{
    return (int16_t)ReadU16FromBE(src);
}

inline uint32_t
ReadU32FromBE(const void* src)
{
    uint32_t v = 0;
    uint8_t* p = (uint8_t*)src;

    v |= p[0];
    v |= ((uint32_t)p[1]) << 8;
    v |= ((uint32_t)p[2]) << 16;
    v |= ((uint32_t)p[3]) << 24;

    return BigU32(v);
}

inline int32_t
ReadI32FromBE(const void* src)
{
    return (int32_t)ReadU32FromBE(src);
}

#include "g_math.h"
#include "g_tex.h"

#endif // #ifndef _G_GEUL_H_

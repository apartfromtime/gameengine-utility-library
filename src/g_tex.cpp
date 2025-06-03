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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "..\inc\GEUL\g_geul.h"

#pragma warning (disable : 4244)            // conversion from <type> to <type>

#define INLINE_OBJECT_NULL_CHK(ptr)                     \
    do {                                                \
        if (ptr == NULL) {                              \
            fprintf(stderr, "Not a valid object.\n");   \
            return false;                               \
        }                                               \
    } while (0)

#define INLINE_OBJECT_SIZE_CHK(os, chksize)             \
    do {                                                \
        if (os < chksize) {                             \
            fprintf(stderr, "Not a valid object.\n");   \
            return false;                               \
        }                                               \
    } while (0)

// returns bytes per pixel
static uint8_t
GetBytesForPixelFormat(uint32_t format)
{
    uint8_t bytes = 1;
    switch (format)
    {
    case GEUL_RGBA:
    case GEUL_BGRA:
    { bytes++; }
    case GEUL_RGB:
    case GEUL_BGR:
    { bytes++; }
    case GEUL_LUMINANCE_ALPHA:
    { bytes++; }
    }
    return bytes;
}

// returns width in bytes, depth must be bits per pixel
static uint32_t
WidthInBytes(uint32_t width, uint32_t depth)
{
    uint32_t bytes = width;
    switch (depth)
    {
    case 32: bytes += width;
    case 24: bytes += width;
    case 16: bytes += width; break;
    case  1: bytes >>= 1;
    case  2: bytes >>= 1;
    case  4: bytes >>= 1;
    };
    return bytes;
}

#ifndef _PNG_H
#define _PNG_H


//-----------------------------------------------------------------------------
//
//  Portable Network Graphics
//
//-----------------------------------------------------------------------------


#define ZLIB_WINAPI
#include <zlib.h>

enum
{
    PNG_FILTER_NONE = 0,
    PNG_FILTER_SUB = 1,
    PNG_FILTER_UP = 2,
    PNG_FILTER_AVERAGE = 3,
    PNG_FILTER_PAETH = 4,
    PNG_FILTER_ADAPTIVE = 5,
    PNG_FILTER_COUNT
};

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------

static const uint8_t PNG_IDENTIFIER[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A,
    0x1A, 0x0A };

static const uint8_t s_png_headersize = 13;
static const uint8_t s_png_signaturesize = 8;
static const uint8_t s_png_chunksize = 8;
static const uint8_t s_png_crcsize = 4;

//------------------------------------------------------------------------------
// Table of CRCs
//------------------------------------------------------------------------------
static const unsigned long s_crc_table[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

//------------------------------------------------------------------------------
// Update a running CRC with the bytes buf[0..len-1]--the CRC
// should be initialized to all 1's, and the transmitted value
// is the 1's complement of the final running CRC (see the
// crc() routine below)).
//------------------------------------------------------------------------------
static unsigned long
UpdateCrc(unsigned long crc, unsigned char* buf, int len)
{
    unsigned long c = crc;

    for (int n = 0; n < len; n++)
    {
        c = s_crc_table[(c ^ buf[n]) & 0xFF] ^ (c >> 8);
    }

    return c;
}

//------------------------------------------------------------------------------
// Return the CRC of the bytes buf[0..len-1].
//------------------------------------------------------------------------------
static unsigned long
Crc(unsigned char* buf, int len)
{
    return UpdateCrc(0xFFFFFFFFL, buf, len) ^ 0xFFFFFFFFL;
}

//------------------------------------------------------------------------------
// Critical and ancillary chunk definitions
//------------------------------------------------------------------------------
#define IHDR 0x52444849
#define gAMA 0x414d4167
#define PLTE 0x45544c50
#define tRNS 0x534e5274
#define IDAT 0x54414449
#define cHRM 0x4d524863
#define sRGB 0x42475273
#define iCCP 0x50434369
#define iTXt 0x74585469
#define tEXt 0x74584574
#define zTXt 0x7458547a
#define bKGD 0x44474b62
#define pHYs 0x73594870
#define sBIT 0x54494273
#define sPLT 0x544c5073
#define hIST 0x54534968
#define tIME 0x454d4974
#define IEND 0x444e4549

//------------------------------------------------------------------------------
// ShrinkPNG
//------------------------------------------------------------------------------
static void
ShrinkInterlacedPNG(uint8_t* pdst, uint32_t* pdstlen, uint32_t srcxsize,
    uint32_t srcysize, uint32_t srcdepth, uint8_t filtertype, uint8_t* psrc)
{
    uint8_t* buffer = pdst;
    uint8_t* pixels = psrc;
    float pixelsperbyte = (8.0f / (float)srcdepth);
    uint32_t bytes = (srcdepth + 7) >> 3;
    uint32_t pitch = WidthInBytes(srcxsize, srcdepth);
    uint32_t xskip = 1;
    uint32_t yskip = 1;
    uint32_t xorig = 0;
    uint32_t yorig = 0;
    uint32_t xsize = srcxsize;
    uint32_t ysize = srcysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t pass = 0;
    uint8_t mask[4] = { 0x01, 0x03, 0, 0x0F };
    uint16_t pix0 = 0;
    uint16_t pix1 = 0;
    uint16_t pae0 = 0;
    uint16_t raw1 = 0;
    uint16_t pri0 = 0;
    uint16_t pri1 = 0;
    uint16_t bpp = 0;
    uint16_t a = 0;
    uint16_t b = 0;
    uint16_t c = 0;
    uint8_t filter = 0;
    uint8_t bitX = 0;
    uint8_t bit0 = 0;

    const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
    const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
    const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
    const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };

    filtertype = (filtertype < PNG_FILTER_COUNT) ? ((srcdepth >= 8) ? filtertype : 0) : 0;
    do
    {
        xsize = (srcxsize - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
        ysize = (srcysize - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];
        xskip = i_xextent[pass];
        yskip = i_yextent[pass];
        xorig = i_xorigin[pass];
        yorig = i_yorigin[pass];

        y = 0;

        while (y < ysize)
        {
            filter = (filtertype >= PNG_FILTER_COUNT) ? 0 : filtertype;

            if (filtertype == PNG_FILTER_ADAPTIVE && srcdepth >= 8) {

                uint32_t sum[2] = { UINT32_MAX };

                for (int fltr = PNG_FILTER_NONE; fltr < PNG_FILTER_COUNT - 1; ++fltr)
                {
                    sum[1] = 0;
                    x = 0;

                    while (x < xsize)
                    {
                        int32_t x0 = (((x - 0) * xskip) + xorig);
                        int32_t x1 = (((x - 1) * xskip) + xorig);
                        int32_t y0 = (((y - 0) * yskip) + yorig);
                        int32_t y1 = (((y - 1) * yskip) + yorig);
                        int32_t xx = (uint32_t)(x0 / pixelsperbyte);
                        int32_t yy = y0 * pitch;
                        bpp = 0;

                        while (bpp < bytes)
                        {
                            pix1 = 0;
                            pri0 = 0;
                            pri1 = 0;
                            raw1 = 0;

                            pix0 = (pixels + yy + xx)[bpp];

                            if ((x1 >= 0) && (y1 >= 0)) {
                                pri1 = (pixels + (y1 * pitch) + (x1 * bytes))[bpp];
                            }
                            if ((y1 >= 0)) {
                                pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                            }
                            if ((x1 >= 0)) {
                                raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                            }

                            switch (fltr)
                            {
                            case 1:         // Sub
                            {
                                pix1 = raw1 & 0xFF;
                            } break;
                            case 2:         // Up
                            {
                                pix1 = pri0 & 0xFF;
                            } break;
                            case 3:         // Average
                            {
                                pix1 = FLOOR((raw1 + pri0) / 2) & 0xFF;
                            } break;
                            case 4:         // Paeth
                            {
                                a = ABS(pri0 - pri1);
                                b = ABS(raw1 - pri1);
                                c = ABS(raw1 + pri0 - (2 * pri1));

                                if (a <= b && a <= c) {
                                    pae0 = raw1 & 0xFF;
                                } else if (b <= c) {
                                    pae0 = pri0 & 0xFF;
                                } else {
                                    pae0 = pri1 & 0xFF;
                                }
                                pix1 = pae0 & 0xFF;
                            } break;
                            }

                            sum[1] += (pix0 - pix1) & 0xFF;
                            bpp++;
                        }
                        x++;
                    }

                    if (sum[1] < sum[0]) {
                        sum[0] = sum[1];
                        filter = fltr;
                    }
                }
            }

            *buffer++ = filter;
            x = 0;

            while (x < xsize)
            {
                int32_t x0 = (((x - 0) * xskip) + xorig);
                int32_t x1 = (((x - 1) * xskip) + xorig);
                int32_t y0 = (((y - 0) * yskip) + yorig);
                int32_t y1 = (((y - 1) * yskip) + yorig);
                int32_t xx = (uint32_t)(x0 / pixelsperbyte);
                int32_t yy = y0 * pitch;
                uint8_t mod = pixelsperbyte - 1;
                uint8_t curX = ( x & mod);
                uint8_t cur0 = (x0 & mod);
                bpp = 0;

                while (bpp < bytes)
                {
                    pix1 = 0;
                    pri0 = 0;
                    pri1 = 0;
                    raw1 = 0;

                    bitX = (mod - curX) * srcdepth;
                    bit0 = (mod - cur0) * srcdepth;

                    if (srcdepth <= 4) {
                        pix0 |= ((*(pixels+yy+xx)>>bit0)&mask[srcdepth-1])<<bitX;
                    } else {
                        pix0 = (pixels + yy + xx)[bpp];
                    }

                    if (srcdepth >= 8) {
                        if ((x1 >= 0) && (y1 >= 0)) {
                            pri1 = (pixels + (y1 * pitch) + (x1 * bytes))[bpp];
                        }
                        if ((y1 >= 0)) {
                            pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                        }
                        if ((x1 >= 0)) {
                            raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                        }

                        switch (filter)
                        {
                        case 1:         // Sub
                        {
                            pix1 = raw1 & 0xFF;
                        } break;
                        case 2:         // Up
                        {
                            pix1 = pri0 & 0xFF;
                        } break;
                        case 3:         // Average
                        {
                            pix1 = FLOOR((raw1 + pri0) / 2) & 0xFF;
                        } break;
                        case 4:         // Paeth
                        {
                            a = ABS(pri0 - pri1);
                            b = ABS(raw1 - pri1);
                            c = ABS(raw1 + pri0 - (2 * pri1));

                            if (a <= b && a <= c) {
                                pae0 = raw1 & 0xFF;
                            } else if (b <= c) {
                                pae0 = pri0 & 0xFF;
                            } else {
                                pae0 = pri1 & 0xFF;
                            }
                            pix1 = pae0 & 0xFF;
                        } break;
                        }
                    }

                    if (srcdepth >= 8 || (srcdepth <= 4 && curX == mod)) {
                        *buffer++ = (pix0 - pix1 & 0xFF); pix0 = 0;
                    }
                    bpp++;
                }
                x++;
            }
            y++;
        }
    } while (++pass < 7);

    if (pdstlen != NULL) { *pdstlen = buffer - pdst; }
}

//------------------------------------------------------------------------------
// ShrinkPNG
//------------------------------------------------------------------------------
static void
ShrinkPNG(uint8_t* pdst, uint32_t* pdstlen, uint32_t srcxsize, uint32_t srcysize,
    uint32_t srcdepth, uint8_t filtertype, uint8_t* psrc)
{
    uint8_t* buffer = pdst;
    uint8_t* pixels = psrc;
    float pixelsperbyte = (8.0f / (float)srcdepth);
    uint32_t bytes = (srcdepth + 7) >> 3;
    uint32_t pitch = WidthInBytes(srcxsize, srcdepth);
    uint32_t xsize = srcxsize;
    uint32_t ysize = srcysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint8_t mask[4] = { 0x01, 0x03, 0, 0x0F };
    uint16_t pix0 = 0;
    uint16_t pix1 = 0;
    uint16_t pae0 = 0;
    uint16_t raw1 = 0;
    uint16_t pri0 = 0;
    uint16_t pri1 = 0;
    uint16_t bpp = 0;
    uint16_t a = 0;
    uint16_t b = 0;
    uint16_t c = 0;
    uint8_t filter = 0;
    uint8_t bitX = 0;
    uint8_t bit0 = 0;

    filtertype = (filtertype < PNG_FILTER_COUNT) ? ((srcdepth >= 8) ? filtertype : 0) : 0;

    y = 0;

    while (y < ysize)
    {
        filter = (filtertype >= PNG_FILTER_COUNT) ? 0 : filtertype;

        if (filtertype == PNG_FILTER_ADAPTIVE && srcdepth >= 8) {

            uint32_t sum[2] = { UINT32_MAX };

            for (int fltr = PNG_FILTER_NONE; fltr < PNG_FILTER_COUNT - 1; ++fltr)
            {
                sum[1] = 0;
                x = 0;

                while (x < xsize)
                {
                    int32_t x0 = x;
                    int32_t y0 = y;
                    int32_t x1 = x - 1;
                    int32_t y1 = y - 1;
                    int32_t xx = (uint32_t)(x0 / pixelsperbyte);
                    int32_t yy = y0 * pitch;
                    bpp = 0;

                    while (bpp < bytes)
                    {
                        pix1 = 0;
                        pri0 = 0;
                        pri1 = 0;
                        raw1 = 0;

                        pix0 = (pixels + yy + xx)[bpp];

                        if ((x1 >= 0) && (y1 >= 0)) {
                            pri1 = (pixels + (y1 * pitch) + (x1 * bytes))[bpp];
                        }
                        if ((y1 >= 0)) {
                            pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                        }
                        if ((x1 >= 0)) {
                            raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                        }

                        switch (fltr)
                        {
                        case 1:         // Sub
                        {
                            pix1 = raw1 & 0xFF;
                        } break;
                        case 2:         // Up
                        {
                            pix1 = pri0 & 0xFF;
                        } break;
                        case 3:         // Average
                        {
                            pix1 = FLOOR((raw1 + pri0) / 2) & 0xFF;
                        } break;
                        case 4:         // Paeth
                        {
                            a = ABS(pri0 - pri1);
                            b = ABS(raw1 - pri1);
                            c = ABS(raw1 + pri0 - (2 * pri1));
                            if (a <= b && a <= c) {
                                pae0 = raw1 & 0xFF;
                            } else if (b <= c) {
                                pae0 = pri0 & 0xFF;
                            } else {
                                pae0 = pri1 & 0xFF;
                            }
                            pix1 = pae0 & 0xFF;
                        } break;
                        }

                        sum[1] += (pix0 - pix1) & 0xFF;
                        bpp++;
                    }
                    x++;
                }

                if (sum[1] < sum[0]) {
                    sum[0] = sum[1];
                    filter = fltr;
                }
            }
        }

        *buffer++ = filter;
        x = 0;

        while (x < xsize)
        {
            int32_t x0 = x;
            int32_t y0 = y;
            int32_t x1 = x - 1;
            int32_t y1 = y - 1;
            int32_t xx = (uint32_t)(x0 / pixelsperbyte);
            int32_t yy = y0 * pitch;
            uint8_t mod = pixelsperbyte - 1;
            uint8_t curX = ( x & mod);
            uint8_t cur0 = (x0 & mod);
            bpp = 0;

            while (bpp < bytes)
            {
                pix1 = 0;
                pri0 = 0;
                pri1 = 0;
                raw1 = 0;

                bitX = (mod - curX) * srcdepth;
                bit0 = (mod - cur0) * srcdepth;

                if (srcdepth <= 4) {
                    pix0 |= ((*(pixels+yy+xx)>>bit0)&mask[srcdepth-1])<<bitX;
                } else {
                    pix0 = (pixels + yy + xx)[bpp];
                }

                if (srcdepth >= 8) {
                    if ((x1 >= 0) && (y1 >= 0)) {
                        pri1 = (pixels + (y1 * pitch) + (x1 * bytes))[bpp];
                    }
                    if ((y1 >= 0)) {
                        pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                    }
                    if ((x1 >= 0)) {
                        raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                    }

                    switch (filter)
                    {
                    case 1:         // Sub
                    {
                        pix1 = raw1 & 0xFF;
                    } break;
                    case 2:         // Up
                    {
                        pix1 = pri0 & 0xFF;
                    } break;
                    case 3:         // Average
                    {
                        pix1 = FLOOR((raw1 + pri0) / 2) & 0xFF;
                    } break;
                    case 4:         // Paeth
                    {
                        a = ABS(pri0 - pri1);
                        b = ABS(raw1 - pri1);
                        c = ABS(raw1 + pri0 - (2 * pri1));
                        if (a <= b && a <= c) {
                            pae0 = raw1 & 0xFF;
                        } else if (b <= c) {
                            pae0 = pri0 & 0xFF;
                        } else {
                            pae0 = pri1 & 0xFF;
                        }
                        pix1 = pae0 & 0xFF;
                    } break;
                    }
                }

                if (srcdepth >= 8 || (srcdepth <= 4 && curX == mod)) {
                    *buffer++ = ((pix0 - pix1) & 0xFF); pix0 = 0;
                }
                bpp++;
            }
            x++;
        }
        y++;
    }

    if (pdstlen != NULL) { *pdstlen = buffer - pdst; }
}

//------------------------------------------------------------------------------
// SavePNG
//------------------------------------------------------------------------------
static bool
SaveToMemoryPNG(uint8_t** ppdst, uint32_t* ppdstsize, uint32_t codec,
    uint8_t* psrcbuf, uint32_t psrcsize, uint32_t srcxsize, uint32_t srcysize,
    uint8_t srcdepth, uint8_t srcsampledepth, palette_t* ppalette, rgba_t* pcolorkey)
{
    INLINE_OBJECT_NULL_CHK(ppdst);
    INLINE_OBJECT_NULL_CHK(ppdstsize);

    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, 0);

    if (srcdepth !=  1 && srcdepth !=  2 && srcdepth !=  4 && srcdepth !=  8 &&
        srcdepth != 16 && srcdepth != 24 && srcdepth != 32) {
        fprintf(stderr, "PNG, Unsupported depth: %d.\n", srcdepth);
        return false;
    }

    if (srcxsize == 0 || srcysize == 0) {
        fprintf(stderr, "PNG, Unsupported size: (%d, %d).\n", srcxsize, srcysize);
        return false;
    }

    // color-type interpretation of the image and number of bytes-per-pixel
    uint8_t colortype = 0;

    switch (srcdepth)
    {
        case 32:            // rgba
        {
            colortype = 6;
        } break;
        case 24:            // rgb
        {
            colortype = 2;
        } break;
        case 16:            // greyscale + alpha
        {
            colortype = 4;
        } break;
        case 8:
        case 4:
        case 2:
        case 1:
        {
            if (ppalette == NULL) {
                colortype = 0;          // greyscale
            } else {
                colortype = 3;          // palette
            }
        } break;
    }

    // src stuff
    uint8_t* srcptr = psrcbuf;
    uint32_t srcpitch = WidthInBytes(srcxsize, srcdepth);
    uint32_t palettesize = 0;
    uint32_t crc = 0;

    // palette
    if (srcdepth <= 8) {
        if (ppalette != NULL) {
            // the PLTE chunk contains from 1 to 256 palette entries, each
            // three-bytes
            palettesize = ppalette->size * 3;
        }
    }

    // byte encoded array
    int datasize = s_png_signaturesize +
        (s_png_chunksize + s_png_headersize + s_png_crcsize) +
        (s_png_chunksize + palettesize + s_png_crcsize) +
        (s_png_chunksize + (((srcysize + 1) * srcpitch + 1) + (srcysize + 1)) +
            s_png_crcsize) +
        (s_png_chunksize + s_png_crcsize);
    uint8_t* data = (uint8_t*)malloc(datasize);
    int byteswritten = 0;

    if (data == NULL) {
        fprintf(stderr, "PNG, Out of memory.\n");
        return false;
    }

    memset(data, 0, datasize);

    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;

    // fill in file header and info

    // identifier
    for (int i = 0; i < 8; ++i)
    {
        *dstbuf++ = PNG_IDENTIFIER[i];
        byteswritten++;
    }

    // chunk length and type
    uint32_t size = 0;
    uint32_t type = 0;

    // IHDR chunk
    size = s_png_headersize;
    type = BigU32(IHDR);

    WriteU32ToBE(dstbuf, size); dstbuf += 4;         // chunk size
    uint8_t* crcbuf = dstbuf;            // set pointer for crc calculation
    WriteU32ToBE(dstbuf, type); dstbuf += 4;         // chunk type
    byteswritten += 8;

    WriteU32ToBE(dstbuf, srcxsize); dstbuf += 4;
    WriteU32ToBE(dstbuf, srcysize); dstbuf += 4;

    // check for invalid color-type and depth
    if (colortype == 3 && srcdepth >= 16) {

        fprintf(stderr, "PNG, bad color-type (%d) and bit-depth (%d).\n",
            colortype, srcdepth);
        free(data);
        data = NULL;

        return false;
    }
    
    uint8_t compression = 0;
    uint8_t filter = 0;
    // 0 (no interlace) or 1 (Adam7 interlace)
    uint8_t interlace = (codec == GEUL_RLE) ? 1 : 0;

    *dstbuf++ = srcdepth <= 32 ? srcdepth <= 4 ? srcdepth : 8 : 16;
    *dstbuf++ = colortype;
    *dstbuf++ = compression;
    *dstbuf++ = filter;
    *dstbuf++ = interlace;
    byteswritten += 13;

    // CRC
    crc = Crc(crcbuf, (int)(dstbuf - crcbuf));
    WriteU32ToBE(dstbuf, crc); dstbuf += 4;
    byteswritten += 4;

    // palette
    if (palettesize) {

        // PLTE chunk
        size = palettesize;
        type = BigU32(PLTE);

        WriteU32ToBE(dstbuf, size); dstbuf += 4;
        crcbuf = dstbuf;
        WriteU32ToBE(dstbuf, type); dstbuf += 4;
        byteswritten += 8;

        for (uint32_t i = 0; i < ppalette->size; ++i)
        {
            *dstbuf++ = ppalette->data[i].r;
            *dstbuf++ = ppalette->data[i].g;
            *dstbuf++ = ppalette->data[i].b;
            byteswritten += 3;
        }

        // CRC
        crc = Crc(crcbuf, (int)(dstbuf - crcbuf));
        WriteU32ToBE(dstbuf, crc); dstbuf += 4;
        byteswritten += 4;
    }

    // transparency
    if (pcolorkey != NULL) {

        // tRNS chunk
        type = BigU32(tRNS);

        switch (colortype)
        {
        case 0:
        {
            // single gray level value
            size = 2;
            WriteU32ToBE(dstbuf, size); dstbuf += 4;
            crcbuf = dstbuf;
            WriteU32ToBE(dstbuf, type); dstbuf += 4;
            byteswritten += 8;

            WriteU16ToBE(dstbuf, pcolorkey->a); dstbuf += 2;
            byteswritten += 2;
        } break;
        case 2:
        {
            // single RGB color value
            size = 6;
            WriteU32ToBE(dstbuf, size); dstbuf += 4;
            crcbuf = dstbuf;
            WriteU32ToBE(dstbuf, type); dstbuf += 4;
            byteswritten += 8;

            WriteU16ToBE(dstbuf, pcolorkey->r); dstbuf += 2;
            WriteU16ToBE(dstbuf, pcolorkey->g); dstbuf += 2;
            WriteU16ToBE(dstbuf, pcolorkey->b); dstbuf += 2;
            byteswritten += 6;
        } break;
        case 3:
        {
            // series of one-byte alpha values, corresponding to entries in the PLTE chunk
            size = ppalette->size;
            WriteU32ToBE(dstbuf, size); dstbuf += 4;
            crcbuf = dstbuf;
            WriteU32ToBE(dstbuf, type); dstbuf += 4;
            byteswritten += 8;

            for (uint32_t i = 0; i < size; ++i)
            {
                *dstbuf++ = ppalette->data[i].a;
                byteswritten++;
            }
        } break;
        default:
        {
            fprintf(stderr, "PNG, bad color-type (%d) for tRNS.\n", colortype);

            free(data);
            data = NULL;

            return false;
        }
        }

        // CRC
        crc = Crc(crcbuf, (int)(dstbuf - crcbuf));
        WriteU32ToBE(dstbuf, crc); dstbuf += 4;
        byteswritten += 4;
    }

    // uncompressed data, bytes per line, per component, plus filter per row
    uint32_t idatlen = ((srcysize + 1) * (srcpitch + 1)) + (srcysize + 1);
    uint8_t* idatptr = (uint8_t*)malloc(((idatlen + 1) & ~1));
    uint8_t* idatbuf = idatptr;

    if (idatptr == NULL) {

        fprintf(stderr, "PNG, Out of memory.\n");
        free(data);
        data = NULL;

        return false;
    }

    memset(idatptr, 0, ((idatlen + 1) & ~1));

    // filter selection (adaptive filtering with five basic filter types)
    uint8_t filtermode = PNG_FILTER_ADAPTIVE;

    if (filtermode == PNG_FILTER_ADAPTIVE) {
        if (colortype == 3 || srcdepth <= 4) {
            filtermode = PNG_FILTER_NONE;
        }
    }

    // IDAT chunk
    // interlace and filter
    if (interlace) {
        ShrinkInterlacedPNG(idatbuf, &idatlen, srcxsize, srcysize, srcdepth,
            filtermode, srcptr);
    } else {
        ShrinkPNG(idatbuf, &idatlen, srcxsize, srcysize, srcdepth, filtermode,
            srcptr);
    }

    idatbuf = idatptr;

    uint32_t bytesencoded = 0;
    uint32_t bytesdecoded = 0;

    // ZLIB compression
    z_stream deflator = {};

    // deflate\inflate compression levels 0-10
    uint8_t compressionlevel = 6;

    int status = deflateInit(&deflator, MIN(compressionlevel, 9));

    if (status != Z_OK) {

        fprintf(stderr, "PNG, Deflate: failed with status %i.\n",
            status);

        free(idatptr);
        idatptr = NULL;

        free(data);
        data = NULL;

        return false;
    }

    // deflate
    uint32_t odatlen = idatlen;
    uint8_t* odatptr = (uint8_t*)malloc(((odatlen + 1) & ~1));
    uint8_t* odatbuf = odatptr;

    if (odatptr == NULL) {

        fprintf(stderr, "PNG, Out of memory.\n");

        free(idatptr);
        idatptr = NULL;

        free(data);
        data = NULL;

        return false;
    }

    memset(odatptr, 0, ((odatlen + 1) & ~1));

    unsigned int oabsrem = odatlen - deflator.total_out;
    unsigned int odatrem = 0;
    unsigned int iabsrem = idatlen - deflator.total_in;
    unsigned int idatrem = 0;

    idatrem = MIN(32768, idatlen);
    odatrem = MIN(32768, odatlen);

    do
    {
        deflator.next_in    = idatbuf;
        deflator.avail_in   = idatrem;
        deflator.next_out   = odatbuf;
        deflator.avail_out  = odatrem;

        status = deflate(&deflator, ((iabsrem > 0) ? Z_NO_FLUSH : Z_FINISH));

        // input
        iabsrem = idatlen - deflator.total_in;
        idatbuf = idatptr + deflator.total_in;
        idatrem = MIN(32768, iabsrem);
        bytesdecoded = deflator.total_in;

        // output
        oabsrem = odatlen - deflator.total_out;
        odatbuf = odatptr + deflator.total_out;
        odatrem = MIN(32768, oabsrem);
        bytesencoded = deflator.total_out;

    } while (status == Z_OK);

    deflateEnd(&deflator);

    // free uncompressed data
    free(idatptr);
    idatptr = NULL;

    // compression failed
    if (status != Z_STREAM_END) {

        fprintf(stderr, "PNG, Deflate: failed with status %i.\n", status);

        // free compressed data and return
        free(odatptr);
        odatptr = NULL;

        free(data);
        data = NULL;

        return false;
    } else {
        fprintf(stdout, "PNG, Deflate: completed successfully\n");

        // data
        oabsrem = bytesencoded;          // remaining bytes

        while (oabsrem > 0)
        {
            odatrem = MIN(32768, oabsrem);            // number of bytes
            oabsrem = oabsrem - odatrem;

            // IDAT chunk
            size = odatrem;
            type = BigU32(IDAT);

            WriteU32ToBE(dstbuf, size); dstbuf += 4;
            crcbuf = dstbuf;
            WriteU32ToBE(dstbuf, type); dstbuf += 4;
            byteswritten += 8;

            // IDAT data
            for (size_t i = 0; i < odatrem; ++i)
            {
                *dstbuf++ = *odatptr++;
                byteswritten++;
            }

            // CRC
            crc = Crc(crcbuf, (int)(dstbuf - crcbuf));
            WriteU32ToBE(dstbuf, crc); dstbuf += 4;
            byteswritten += 4;
        }
    }

    // IEND chunk
    size = 0;
    type = BigU32(IEND);

    WriteU32ToBE(dstbuf, size); dstbuf += 4;
    crcbuf = dstbuf;
    WriteU32ToBE(dstbuf, type); dstbuf += 4;
    byteswritten += 8;

    // CRC
    crc = Crc(crcbuf, (int)(dstbuf - crcbuf));
    WriteU32ToBE(dstbuf, crc); dstbuf += 4;
    byteswritten += 4;

    *ppdstsize = byteswritten;
    *ppdst = (uint8_t*)realloc(data, ((*ppdstsize + 1) & ~1));

    return true;
}

//-----------------------------------------------------------------------------
// GetInfoFromMemory
//-----------------------------------------------------------------------------
static bool
GetInfoFromMemoryPNG(uint8_t* srccolormap, uint32_t* srcxsize, uint32_t* srcysize,
    uint32_t* srcdepth, uint8_t* srcsampledepth, uint8_t* psrcbuf, uint32_t psrcsize)
{
    uint32_t chksize = s_png_signaturesize + s_png_headersize +
        (4 * (s_png_chunksize + s_png_crcsize));
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, chksize);

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    uint8_t* srcend = psrcbuf + srclen;

    // check png header
    for (int i = 0; i < 8; ++i)
    {
        if (*srcbuf++ != PNG_IDENTIFIER[i]) {
            return false;
        }
    }

    uint32_t xsize = 0;
    uint32_t ysize = 0;
    uint32_t depth = 0;
    uint32_t colortype = 0;
    uint32_t compression = 0;
    uint32_t filter = 0;
    uint32_t interlace = 0;
    uint32_t gamma = 0;
    uint32_t palnum = 0;
    uint8_t  colorkey[3] = { 0 };
    uint8_t  bytesperpixel = 0;
    uint8_t  sampledepth = 8;

    // A valid PNG image must contain an IHDR chunk, one or more IDAT chunks, and an
    // IEND chunk.
    bool endfound = false;
    bool first = true;          // image header chunk must precede all other chunks
    bool dattest = false;
    bool paltest = false;

    while (srcbuf < srcend && endfound == false)
    {
        uint32_t size = ReadU32FromBE(srcbuf); srcbuf += 4;
        uint32_t type = ReadU32FromBE(srcbuf); srcbuf += 4;

        switch (BigU32(type))
        {
            case IHDR:
            {
                // check for multiple IHDR
                if (first == false) {
                    return false;
                }

                first = false;

                // check chunk is valid length
                if (size != s_png_headersize) {
                    return false;
                }

                xsize = ReadU32FromBE(srcbuf); srcbuf += 4;
                ysize = ReadU32FromBE(srcbuf); srcbuf += 4;

                // zero is an invalid value
                if (xsize == 0 || ysize == 0) {
                    return false;
                }

                depth = *srcbuf++;

                if (depth != 1 && depth != 2 && depth != 4 && depth != 8) {
                    return false;
                }
                
                colortype = *srcbuf++;

                // check for invalid color-type and depth
                if (colortype == 3 && depth >= 16) {
                    return false;
                }

                // color-type interpretation of the image and number of bytes-per-pixel
                switch (colortype)
                {
                case 0:         // grayscale
                {
                    bytesperpixel = 1;
                } break;
                case 2:         // rgb
                {
                    bytesperpixel = 3;
                } break;
                case 3:         // palette
                {
                    bytesperpixel = 1;
                } break;
                case 4:         // grayscale + alpha
                {
                    bytesperpixel = 2;
                } break;
                case 6:         // rgba
                {
                    bytesperpixel = 4;
                } break;
                default:
                {
                    return false;
                }
                }

                compression = *srcbuf++;

                // compression method
                if (compression != 0) {
                    return false;
                }

                filter = *srcbuf++;

                // pre-processing method applied before compression
                if (filter != 0) {
                    return false;
                }

                interlace = *srcbuf++;
                
                // transmission order of image
                if (interlace > 1) {
                    return false;
                }

            } break;
            case gAMA:
            {
                if (first) {
                    return false;
                }

                // the gAMA chunk must precede the first IDAT chunk.
                if (dattest) {
                    return false;
                }

                // the gAMA chunk must precede the first PLTE chunk.
                if (paltest) {
                    return false;
                }

                srcbuf += size;
            } break;
            case PLTE:
            {
                if (first) {
                    return false;
                }

                // the PLTE chunk must precede the first IDAT chunk.
                if (dattest) {
                    return false;
                }

                // the PLTE chunk contains from 1 to 256 palette entries, each
                // three-bytes
                if (size > 768) {
                    return false;
                }

                // a chunk length not divisible by 3 is an error.
                palnum = size / 3;

                if ((palnum * 3) != size) {
                    return false;
                }
                
                if (paltest == false) {
                    paltest = true;
                }

                srcbuf += size;
            } break;
            case tRNS:
            {
                if (first) {
                    return false;
                }

                // the tRNS chunk must precede the first IDAT chunk.
                if (dattest) {
                    return false;
                }

                if (colortype == 3) {
                    // the tRNS chunk must follow the PLTE chunk, if any.
                    if (paltest == false) {
                        return false;
                    }
                } else {
                    // check for transparency in images with alpha
                    if (colortype == 4 || colortype == 6)
                    {
                        return false;
                    }
                }
                
                srcbuf += size;
            } break;
            case IDAT:
            {
                if (first) {
                    return false;
                }

                if (dattest == false) {
                    dattest = true;
                }

                srcbuf += size;
            } break;
            case cHRM:
            case sRGB:
            case iCCP:
            case iTXt:
            case tEXt:
            case zTXt:
            case bKGD:
            case pHYs:
            case sBIT:
            case sPLT:
            case hIST:
            case tIME:
            {
                if (first)
                {
                    return false;
                }

                srcbuf += size;
            } break;
            case IEND:
            {
                endfound = true;
            } break;
            default:
            {
                if (first)
                {
                    return false;
                }

                srcbuf += size;
            } break;
        }

        // skip CRC
        srcbuf += 4;
    }

    if (srccolormap != NULL) { *srccolormap = paltest; }
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = depth * bytesperpixel; }
    if (srcsampledepth != NULL) { *srcsampledepth = depth; }

    return true;
}

//------------------------------------------------------------------------------
// ExpandPNG
//------------------------------------------------------------------------------
static void
ExpandInterlacedPNG(uint8_t** ppdst, uint32_t dstxsize, uint32_t dstysize,
    uint32_t dstdepth, uint8_t* psrc)
{
    uint8_t* pixels = *ppdst;
    uint8_t* buffer = psrc;
    float pixelsperbyte = (8.0f / (float)dstdepth);
    uint32_t bytes = (dstdepth + 7) >> 3;
    uint32_t pitch = WidthInBytes(dstxsize, dstdepth);
    uint32_t xskip = 1;
    uint32_t yskip = 1;
    uint32_t xorig = 0;
    uint32_t yorig = 0;
    uint32_t xsize = dstxsize;
    uint32_t ysize = dstysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t pass = 0;
    uint8_t mask[4] = { 0x01, 0x03, 0, 0x0F };
    uint16_t pix0 = 0;
    uint16_t pix1 = 0;
    uint16_t pae0 = 0;
    uint16_t raw1 = 0;
    uint16_t pri0 = 0;
    uint16_t pri1 = 0;
    uint16_t bpp = 0;
    uint16_t a = 0;
    uint16_t b = 0;
    uint16_t c = 0;
    uint8_t filter = 0;
    uint8_t bitX = 0;
    uint8_t bit0 = 0;
    uint8_t bit1 = 0;

    const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
    const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
    const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
    const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };

    do
    {
        xsize = (dstxsize - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
        ysize = (dstysize - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];
        xskip = i_xextent[pass];
        yskip = i_yextent[pass];
        xorig = i_xorigin[pass];
        yorig = i_yorigin[pass];

        y = 0;

        while (y < ysize)
        {
            filter = *buffer++;
            x = 0;

            while (x < xsize)
            {
                int32_t x0 = (((x - 0) * xskip) + xorig);
                int32_t x1 = (((x - 1) * xskip) + xorig);
                int32_t y0 = (((y - 0) * yskip) + yorig);
                int32_t y1 = (((y - 1) * yskip) + yorig);
                int32_t xx = (uint32_t)(x0 / pixelsperbyte);
                int32_t yy = y0 * pitch;
                uint8_t mod = pixelsperbyte - 1;
                uint8_t curX = ( x & mod);
                uint8_t cur0 = (x0 & mod);
                uint8_t cur1 = (x1 & mod);
                bpp = 0;

                while (bpp < bytes)
                {
                    pix1 = 0;
                    pri0 = 0;
                    pri1 = 0;
                    raw1 = 0;

                    if (dstdepth >= 8 || (dstdepth <= 4 && curX == 0)) {
                        pix0 = *buffer++;
                    }

                    if (dstdepth <= 4) {
                        bitX = (mod - curX) * dstdepth;
                        bit0 = (mod - cur0) * dstdepth;
                        bit1 = (mod - cur1) * dstdepth;
                    }

                    switch (filter)
                    {
                    case 1:         // Sub
                    {
                        if (dstdepth <= 4) {
                            if ((x1 >= 0)) {
                                raw1 = (*(pixels + (y0 * pitch) +
                                    (uint32_t)(x1 / pixelsperbyte)) >> bit1) &
                                    mask[dstdepth - 1];
                            }
                        } else {
                            if ((x1 >= 0)) {
                                raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                            }
                        }
                        pix1 = raw1 & 0xFF;
                    } break;
                    case 2:         // Up
                    {
                        if (dstdepth <= 4) {
                            if ((y1 >= 0)) {
                                pri0 = (*(pixels + (y1 * pitch) +
                                    (uint32_t)(x0 / pixelsperbyte)) >> bit0) &
                                    mask[dstdepth - 1];
                            }
                        } else {
                            if ((y1 >= 0)) {
                                pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                            }
                        }
                        pix1 = pri0 & 0xFF;
                    } break;
                    case 3:         // Average
                    {
                        if (dstdepth <= 4) {
                            if ((x1 >= 0)) {
                                raw1 = (*(pixels + (y0 * pitch) +
                                    (uint32_t)(x1 / pixelsperbyte)) >> bit1) &
                                    mask[dstdepth - 1];
                            }
                            if ((y1 >= 0)) {
                                pri0 = (*(pixels + (y1 * pitch) +
                                    (uint32_t)(x0 / pixelsperbyte)) >> bit0) &
                                    mask[dstdepth - 1];
                            }
                        } else {
                            if ((x1 >= 0)) {
                                raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                            }
                            if ((y1 >= 0)) {
                                pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                            }
                        }
                        pix1 = FLOOR((raw1 + pri0) / 2) & 0xFF;
                    } break;
                    case 4:         // Paeth
                    {
                        if (dstdepth <= 4) {
                            if ((x1 >= 0) && (y1 >= 0)) {
                                pri1 = (*(pixels + (y1 * pitch) +
                                    (uint32_t)(x1 / pixelsperbyte)) >> bit1) &
                                    mask[dstdepth - 1];
                            }
                            if ((x1 >= 0)) {
                                raw1 = (*(pixels + (y0 * pitch) +
                                    (uint32_t)(x1 / pixelsperbyte)) >> bit1) &
                                    mask[dstdepth - 1];
                            }
                            if ((y1 >= 0)) {
                                pri0 = (*(pixels + (y1 * pitch) +
                                    (uint32_t)(x0 / pixelsperbyte)) >> bit0) &
                                    mask[dstdepth - 1];
                            }
                        } else {
                            if ((x1 >= 0) && (y1 >= 0)) {
                                pri1 = (pixels + (y1 * pitch) + (x1 * bytes))[bpp];
                            }
                            if ((x1 >= 0)) {
                                raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                            }
                            if ((y1 >= 0)) {
                                pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                            }
                        }

                        a = ABS(pri0 - pri1);
                        b = ABS(raw1 - pri1);
                        c = ABS(raw1 + pri0 - (2 * pri1));
                        if (a <= b && a <= c) {
                            pae0 = raw1 & 0xFF;
                        } else if (b <= c) {
                            pae0 = pri0 & 0xFF;
                        } else {
                            pae0 = pri1 & 0xFF;
                        }
                        pix1 = pae0 & 0xFF;
                    } break;
                    }

                    if (dstdepth <= 4) {
                        (pixels + yy + xx)[0] |= (((pix0 + pix1) >> bitX) &
                            mask[dstdepth - 1]) << bit0;
                    } else {
                        (pixels + yy + xx)[bpp] = ((pix0 + pix1) & 0xFF);
                    }
                    bpp++;
                }
                x++;
            }
            y++;
        }
    } while (++pass < 7);
}

//------------------------------------------------------------------------------
// ExpandPNG
//------------------------------------------------------------------------------
static void
ExpandPNG(uint8_t** ppdst, uint32_t dstxsize, uint32_t dstysize, uint32_t dstdepth,
    uint8_t* psrc)
{
    uint8_t* pixels = *ppdst;
    uint8_t* buffer = psrc;
    float pixelsperbyte = (8.0f / (float)dstdepth);
    uint32_t bytes = (dstdepth + 7) >> 3;
    uint32_t pitch = WidthInBytes(dstxsize, dstdepth);
    uint32_t xsize = dstxsize;
    uint32_t ysize = dstysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint8_t mask[4] = { 0x01, 0x03, 0, 0x0F };
    uint16_t pix0 = 0;
    uint16_t pix1 = 0;
    uint16_t pae0 = 0;
    uint16_t raw1 = 0;
    uint16_t pri0 = 0;
    uint16_t pri1 = 0;
    uint16_t bpp = 0;
    uint16_t a = 0;
    uint16_t b = 0;
    uint16_t c = 0;
    uint8_t filter = 0;
    uint8_t bitX = 0;
    uint8_t bit1 = 0;

    y = 0;

    while (y < ysize)
    {
        filter = *buffer++;
        x = 0;

        while (x < xsize)
        {
            int32_t x0 = x;
            int32_t y0 = y;
            int32_t x1 = x - 1;
            int32_t y1 = y - 1;
            int32_t xx = (uint32_t)(x0 / pixelsperbyte);
            int32_t yy = y0 * pitch;
            uint8_t mod = pixelsperbyte - 1;
            uint8_t curX = ( x & mod);
            uint8_t cur1 = (x1 & mod);
            bpp = 0;

            while (bpp < bytes)
            {
                pix1 = 0;
                pri0 = 0;
                pri1 = 0;
                raw1 = 0;

                if (dstdepth >= 8 || (dstdepth <= 4 && curX == 0)) {
                    pix0 = *buffer++;
                }

                if (dstdepth <= 4) {
                    bitX = (mod - curX) * dstdepth;
                    bit1 = (mod - cur1) * dstdepth;
                }

                switch (filter)
                {
                case 1:         // Sub
                {
                    if (dstdepth <= 4) {
                        if ((x1 >= 0)) {
                            raw1 = (*(pixels + (y0 * pitch) +
                                (uint32_t)(x1 / pixelsperbyte)) >> bit1) &
                                mask[dstdepth - 1];
                        }
                    } else {
                        if ((x1 >= 0)) {
                            raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                        }
                    }
                    pix1 = raw1 & 0xFF;
                } break;
                case 2:         // Up
                {
                    if (dstdepth <= 4) {
                        if ((y1 >= 0)) {
                            pri0 = (*(pixels + (y1 * pitch) +
                                (uint32_t)(x0 / pixelsperbyte)) >> bitX) &
                                mask[dstdepth - 1];
                        }
                    } else {
                        if ((y1 >= 0)) {
                            pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                        }
                    }
                    pix1 = pri0 & 0xFF;
                } break;
                case 3:         // Average
                {
                    if (dstdepth <= 4) {
                        if ((x1 >= 0)) {
                            raw1 = (*(pixels + (y0 * pitch) +
                                (uint32_t)(x1 / pixelsperbyte)) >> bit1) &
                                mask[dstdepth - 1];
                        }
                        if ((y1 >= 0)) {
                            pri0 = (*(pixels + (y1 * pitch) +
                                (uint32_t)(x0 / pixelsperbyte)) >> bitX) &
                                mask[dstdepth - 1];
                        }
                    } else {
                        if ((x1 >= 0)) {
                            raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                        }
                        if ((y1 >= 0)) {
                            pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                        }
                    }
                    pix1 = FLOOR((raw1 + pri0) / 2) & 0xFF;
                } break;
                case 4:         // Paeth
                {
                    if (dstdepth <= 4) {
                        if ((x1 >= 0) && (y1 >= 0)) {
                            pri1 = (*(pixels + (y1 * pitch) +
                                (uint32_t)(x1 / pixelsperbyte)) >> bit1) &
                                mask[dstdepth - 1];
                        }
                        if ((x1 >= 0)) {
                            raw1 = (*(pixels + (y0 * pitch) +
                                (uint32_t)(x1 / pixelsperbyte)) >> bit1) &
                                mask[dstdepth - 1];
                        }
                        if ((y1 >= 0)) {
                            pri0 = (*(pixels + (y1 * pitch) +
                                (uint32_t)(x0 / pixelsperbyte)) >> bitX) &
                                mask[dstdepth - 1];
                        }
                    } else {
                        if ((x1 >= 0) && (y1 >= 0)) {
                            pri1 = (pixels + (y1 * pitch) + (x1 * bytes))[bpp];
                        }
                        if ((x1 >= 0)) {
                            raw1 = (pixels + (y0 * pitch) + (x1 * bytes))[bpp];
                        }
                        if ((y1 >= 0)) {
                            pri0 = (pixels + (y1 * pitch) + (x0 * bytes))[bpp];
                        }
                    }
                    a = ABS(pri0 - pri1);
                    b = ABS(raw1 - pri1);
                    c = ABS(raw1 + pri0 - (2 * pri1));
                    if (a <= b && a <= c) {
                        pae0 = raw1 & 0xFF;
                    } else if (b <= c) {
                        pae0 = pri0 & 0xFF;
                    } else {
                        pae0 = pri1 & 0xFF;
                    }
                    pix1 = pae0 & 0xFF;
                } break;
                }

                if (dstdepth <= 4) {
                    (pixels + yy + xx)[0] |= (((pix0 + pix1) >> bitX) &
                        mask[dstdepth - 1]) << bitX;
                } else {
                    (pixels + yy + xx)[bpp] = ((pix0 + pix1) & 0xFF);
                }
                bpp++;
            }
            x++;
        }
        y++;
    }
}

//-----------------------------------------------------------------------------
// LoadPNG
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryPNG(uint8_t** ppdst, palette_t* ppalette, uint8_t* psrcbuf,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint32_t* srcdepth,
    uint8_t* srcsampledepth, rgba_t* pcolorkey)
{
    uint32_t chksize = s_png_signaturesize + s_png_headersize +
        (4 * (s_png_chunksize + s_png_crcsize));
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, chksize);

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    uint8_t* srcend = psrcbuf + srclen;

    // check png header
    for (int i = 0; i < 8; ++i)
    {
        if (*srcbuf++ != PNG_IDENTIFIER[i]) {
            fprintf(stderr, "PNG, Bad signature.\n");
            return false;
        }
    }

    // image
    uint32_t xsize = 0;
    uint32_t ysize = 0;
    uint32_t depth = 0;
    uint32_t bytes = 0;
    uint32_t pitch = 0;
    uint32_t gamma = 0;
    uint32_t palnum = 0;
    uint32_t idatasize = 0;
    uint32_t crc0 = 0;
    uint32_t crc1 = 0;
    uint8_t colortype = 0;
    uint8_t compression = 0;
    uint8_t filter = 0;
    uint8_t interlace = 0;
    uint8_t colorkey[3] = { 0 };

    // A valid PNG image must contain an IHDR chunk, one or more IDAT chunks, and an
    // IEND chunk.
    uint32_t size = ReadU32FromBE(srcbuf); srcbuf += 4;
    uint8_t* crcbuf = srcbuf;            // set pointer for crc calculation
    uint32_t type = ReadU32FromBE(srcbuf); srcbuf += 4;

    if (BigU32(type) != IHDR) {
        fprintf(stderr, "PNG, First not IHDR.\n");
        return false;
    }

    // check chunk is valid length
    if (size != s_png_headersize) {
        fprintf(stderr, "PNG, Bad IHDR len.\n");
        return false;
    }

    // zero is an invalid value
    xsize = ReadU32FromBE(srcbuf); srcbuf += 4;
    ysize = ReadU32FromBE(srcbuf); srcbuf += 4;
    if (xsize == 0 || ysize == 0) {
        fprintf(stderr, "PNG, 0-pixel image: x: %d, y: %d\n", xsize,
            ysize);
        return false;
    }

    depth = *srcbuf++;
    if (depth != 1 && depth != 2 && depth != 4 && depth != 8) {
        fprintf(stderr, "PNG, Unsupported bits: %d.\n", depth);
        return false;
    }

    // check for invalid color-type and depth
    colortype = *srcbuf++;
    if (colortype == 3 && depth >= 16) {
        fprintf(stderr, "PNG, Bad color-type (%d) and bit-depth (%d).\n",
            colortype, depth);
        return false;
    }

    // color-type interpretation of the image and number of bytes-per-pixel
    switch (colortype)
    {
    case 0:         // grayscale
    case 3:         // palette
    {
        bytes = 1;
    } break;
    case 2:         // rgb
    {
        bytes = 3;
    } break;
    case 4:         // grayscale + alpha
    {
        bytes = 2;
    } break;
    case 6:         // rgba
    {
        bytes = 4;
    } break;
    default:
    {
        fprintf(stderr, "PNG, Unsupported color type: %d.\n", colortype);
        return false;
    }
    }

    pitch = WidthInBytes(xsize, (depth * bytes));

    // compression method
    compression = *srcbuf++;
    if (compression != 0) {
        fprintf(stderr, "PNG, Unrecognised compression code: %d.\n", compression);
        return false;
    }

    // pre-processing method applied before compression
    filter = *srcbuf++;
    if (filter != 0) {
        fprintf(stderr, "PNG, Bad filter method: %d\n", filter);
        return false;
    }

    // transmission order of image
    interlace = *srcbuf++;
    if (interlace > 1) {
        fprintf(stderr, "PNG, Bad interlace method: %d\n", interlace);
        return false;
    }

    // CRC
    crc0 = ReadU32FromBE(srcbuf);
    crc1 = Crc(crcbuf, (int)(srcbuf - crcbuf));
    if (crc0 != crc1) {
        // crc mismatch
        fprintf(stderr, "PNG, CRC mismatch\n");
        return false;
    }
    srcbuf += 4;

    bool palparsed = false;
    bool datparsed = false;
    bool endparsed = false;

    while (srcbuf < srcend && endparsed == false)
    {
        size = ReadU32FromBE(srcbuf); srcbuf += 4;
        crcbuf = srcbuf;
        type = ReadU32FromBE(srcbuf); srcbuf += 4;

        switch (BigU32(type))
        {
            case IHDR:
            {
                // check for multiple IHDR
                fprintf(stderr, "PNG, Multiple IHDR.\n");
                return false;
            } break;
            case gAMA:
            {
                // the gAMA chunk must precede the first IDAT chunk.
                if (datparsed == true) {
                    fprintf(stderr, "PNG, gAMA after IDAT.\n");
                    return false;
                }

                // the gAMA chunk must precede the first IDAT chunk.
                if (palparsed) {
                    fprintf(stderr, "PNG, gAMA after PLTE.\n");
                    return false;
                }

                gamma = ReadU32FromBE(srcbuf); srcbuf += 4;
            } break;
            case PLTE:
            {
                // the PLTE chunk must precede the first IDAT chunk.
                if (datparsed == true) {
                    fprintf(stderr, "PNG, PLTE after IDAT.\n");
                    return false;
                }

                // the PLTE chunk contains from 1 to 256 palette entries, each
                // three-bytes
                // the number of entries is determined from the chunk length.
                if (size > 768) {
                    fprintf(stderr, "PNG, Invalid PLTE length: %d\n", size);
                    return false;
                }

                // a chunk length not divisible by 3 is an error.
                palnum = size / 3;

                if ((palnum * 3) != size) {
                    fprintf(stderr, "PNG, Invalid PLTE entries: %d\n", palnum);
                    return false;
                }

                if (palparsed == false) {
                    palparsed = true;
                }

                // the palette chunk must appear for color type 3, and can appear for
                // color types 2 and 6; it must not appear for color types 0 and 4.
                if (ppalette != NULL) {
                    for (uint32_t i = 0; i < palnum; ++i)
                    {
                        ppalette->data[i].r = *srcbuf++;
                        ppalette->data[i].g = *srcbuf++;
                        ppalette->data[i].b = *srcbuf++;
                        ppalette->data[i].a = 255;
                    }
                    ppalette->size = palnum;
                    ppalette->bits = 24;
                }

            } break;
            case tRNS:
            {
                // the tRNS chunk must precede the first IDAT chunk.
                if (datparsed == true) {
                    fprintf(stderr, "PNG, tRNS after IDAT.\n");
                    return false;
                }

                // palette
                if (colortype == 3) {
                    // the tRNS chunk must follow the PLTE chunk, if any.
                    if (palnum == 0) {
                        fprintf(stderr, "PNG, tRNS before PLTE.\n");
                        return false;
                    }

                    if (size > palnum) {
                        fprintf(stderr, "PNG, Bad tRNS len: %d\n", size);
                        return false;
                    }

                    if (ppalette != NULL) {
                        for (uint32_t i = 0; i < size; ++i)
                        {
                            ppalette->data[i].a = *srcbuf++;
                        }
                        ppalette->bits = 32;
                    }
                } else {
                    // check for transparency in images with alpha
                    // grayscale + alpha or rgba
                    if (colortype == 4 || colortype == 6) {
                        fprintf(stderr, "PNG, tRNS with alpha.\n");
                        return false;
                    }

                    for (uint32_t i = 0; i < bytes; ++i)
                    {
                        colorkey[i] = ReadU16FromBE(srcbuf) & 0xFF;
                        srcbuf += 2;
                    }

                    if (pcolorkey != NULL) {
                        pcolorkey->r = colorkey[0];
                        pcolorkey->g = colorkey[1];
                        pcolorkey->b = colorkey[2];
                        pcolorkey->a = 255;
                    }
                }
            } break;
            case IDAT:
            {
                if (datparsed == false) {
                    datparsed = true;
                }
                idatasize += size; srcbuf += size;
            } break;
            case cHRM:
            case sRGB:
            case iCCP:
            case iTXt:
            case tEXt:
            case zTXt:
            case bKGD:
            case pHYs:
            case sBIT:
            case sPLT:
            case hIST:
            case tIME:
            {
                srcbuf += size;
            } break;
            case IEND:
            {
                // end of PNG chunks
                endparsed = true;
            } break;
            default:
            {
                char fourcc[5] = { 0 };
                fourcc[0] = (uint8_t)((type >> 24) & 0xFF);
                fourcc[1] = (uint8_t)((type >> 16) & 0xFF);
                fourcc[2] = (uint8_t)((type >>  8) & 0xFF);
                fourcc[3] = (uint8_t)((type >>  0) & 0xFF);
                fourcc[4] = '\0';
                fprintf(stderr, "PNG, Chunk not supported: %s\n", fourcc);
                srcbuf += size;
            } break;
        }

        // CRC
        crc0 = ReadU32FromBE(srcbuf);
        crc1 = Crc(crcbuf, (int)(srcbuf - crcbuf));
        if (crc0 != crc1) {
            // crc mismatch
            fprintf(stderr, "PNG, CRC mismatch\n");
            return false;
        }
        srcbuf += 4;
    }

    // dst stuff
    INLINE_OBJECT_NULL_CHK(ppdst);

    // idat chunks
    uint8_t* idataptr = (uint8_t*)malloc(((idatasize + 1) & ~1));
    uint8_t* idatabuf = idataptr;

    if (idataptr == NULL) {
        fprintf(stderr, "PNG, Out of memory.\n");
        return false;
    }

    memset(idataptr, 0, ((idatasize + 1) & ~1));

    srcbuf = srcptr + 8;            // reset current to begining and skip signature

    while (srcbuf < srcend)
    {
        size = ReadU32FromBE(srcbuf); srcbuf += 4;
        type = ReadU32FromBE(srcbuf); srcbuf += 4;
        switch (BigU32(type))
        {
            case IDAT:
            {
                memcpy(idatabuf, srcbuf, size);
                idatabuf += size;
                srcbuf  += size;
            } break;
            case IEND:
            {
                srcbuf = srcend;
                continue;
            } break;
            default:
            {
                srcbuf += size;
            } break;
        }
        srcbuf += 4;
    }

    idatabuf = idataptr;            // reset current to begining

    uint32_t bytesencoded = 0;
    uint32_t bytesdecoded = 0;

    // ZLIB decompression
    z_stream inflator = { 0 };
    int status = inflateInit(&inflator);
    if (status != Z_OK) {

        fprintf(stderr, "PNG, Inflate: failed with status %i.\n", status);
        free(idataptr);
        idataptr = NULL;

        return false;
    }

    // inflate data, bytes per line, per component, plus filter per row
    uint32_t odatasize = ((ysize + 1) * (pitch + 1)) + (ysize + 1);
    uint8_t* odataptr = (uint8_t*)malloc(((odatasize + 1) & ~1));
    uint8_t* odatabuf = odataptr;

    if (odataptr == NULL) {

        fprintf(stderr, "PNG, Out of memory.\n");
        free(idataptr);
        idataptr = NULL;

        return false;
    }

    memset(odataptr, 0, ((odatasize + 1) & ~1));

    unsigned int oremaining = odatasize - inflator.total_out;
    unsigned int ochunk = 0;
    unsigned int iremaining = idatasize - inflator.total_in;
    unsigned int ichunk = 0;

    ichunk = MIN(32768, idatasize);
    ochunk = MIN(32768, odatasize);

    do
    {
        inflator.next_in    = idatabuf;
        inflator.avail_in   = ichunk;
        inflator.next_out   = odatabuf;
        inflator.avail_out  = ochunk;

        status = inflate(&inflator, ((iremaining > 0) ? Z_NO_FLUSH : Z_FINISH));

        // input
        iremaining = idatasize - inflator.total_in;
        idatabuf = idataptr + inflator.total_in;
        ichunk = MIN(32768, iremaining);
        bytesdecoded = inflator.total_in;

        // output
        oremaining = odatasize - inflator.total_out;
        odatabuf = odataptr + inflator.total_out;
        ochunk = MIN(32768, oremaining);
        bytesencoded = inflator.total_out;

    } while (status == Z_OK);

    inflateEnd(&inflator);

    // free compressed data
    free(idataptr);
    idataptr = NULL;

    // decompression failed
    if (status != Z_STREAM_END) {

        fprintf(stderr, "PNG, Inflate: failed with status %i.\n", status);
        free(odataptr);          // free un-compressed data and return
        odataptr = NULL;

        return false;
    }

    uint8_t* dataptr = (uint8_t*)malloc(((odatasize + 1) & ~1));

    if (dataptr == NULL) {

        fprintf(stderr, "PNG, Out of memory.\n");
        free(odataptr);
        odataptr = NULL;

        return false;
    }

    memcpy(dataptr, odataptr, odatasize);          // filtered image data
    uint8_t* databuf = dataptr;

    free(odataptr);
    odataptr = NULL;

    // deinterlace and filter
    uint32_t pixlen = xsize * ysize * bytes;
    uint8_t* pixels = (uint8_t*)malloc(pixlen);
    uint8_t* pixbuf = pixels;

    if (pixels == NULL) {
        
        fprintf(stderr, "PNG, Out of memory.\n");
        free(dataptr);
        dataptr = NULL;

        return false;
    }

    memset(pixels, 0, xsize * ysize * bytes);

    if (interlace) {
        ExpandInterlacedPNG(&pixels, xsize, ysize, depth * bytes,
            databuf);
    } else {
        ExpandPNG(&pixels, xsize, ysize, depth * bytes, databuf);
    }

    free(dataptr);
    dataptr = NULL;

    // gamma
    if (gamma != 0) {
        float fgamma = (gamma * (1.0f / 2.2f)) / 100000.0f;
        // grayscale
        if ((colortype == 0 || colortype == 4) && depth == 8) {
            for (unsigned int i = 0; i < xsize * ysize; ++i)
            {
                pixbuf[0] = powf((pixbuf[0] / 255.0f), fgamma) * 255.0f;
                pixbuf += bytes;
            }
        }
        // rgb
        if ((colortype == 2 || colortype == 6) && depth == 8) {
            for (unsigned int i = 0; i < xsize * ysize; ++i)
            {
                pixbuf[0] = powf((pixbuf[0] / 255.0f), fgamma) * 255.0f;
                pixbuf[1] = powf((pixbuf[1] / 255.0f), fgamma) * 255.0f;
                pixbuf[2] = powf((pixbuf[2] / 255.0f), fgamma) * 255.0f;
                pixbuf += bytes;
            }
        }
    }

    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = depth * bytes; }
    if (srcsampledepth != NULL) { *srcsampledepth = 8; }

    return true;
}

#endif // #ifndef _PNG_H_

uint32_t
ParseABS(uint8_t** pixels, uint32_t bytesperpixel, uint32_t maxcount)
{
    uint8_t* buffer = *pixels;
    uint32_t count = 1;
    uint32_t sample0 = 0;
    uint32_t sample1 = 0;

    memcpy(&sample1, buffer, bytesperpixel);
    while (count < maxcount) {
        sample0 = sample1;
        memcpy(&sample1, (buffer + bytesperpixel), bytesperpixel);
        if (sample0 == sample1) { break; }
        buffer += bytesperpixel;
        count++;
    }

    return count;
}

uint32_t
ParseRLE(uint8_t** pixels, uint32_t bytesperpixel, uint32_t maxcount)
{
    uint8_t* buffer = *pixels;
    uint32_t count = 1;
    uint32_t sample0 = 0;
    uint32_t sample1 = 0;

    memcpy(&sample1, buffer, bytesperpixel);
    while (count < maxcount) {
        sample0 = sample1;
        memcpy(&sample1, (buffer + bytesperpixel), bytesperpixel);
        if (sample0 != sample1) { break; }
        buffer += bytesperpixel;
        count++;
    }

    return count;
}

#ifndef _TGA_H_
#define _TGA_H_


//-----------------------------------------------------------------------------
//
//  Truevision TGA File Format
//
//-----------------------------------------------------------------------------


#define TGA_NO_IMAGE_DATA           0
#define TGA_MAPPED                  1
#define TGA_RGB                     2
#define TGA_BLACK_AND_WHITE         3
#define TGA_MAPPED_RLE              9
#define TGA_RGB_RLE                 10
#define TGA_BLACK_AND_WHITE_RLE     11

static const uint32_t s_tga_file_size = 18;

//------------------------------------------------------------------------------
// SaveTGA
//------------------------------------------------------------------------------
static bool
SaveToMemoryTGA(uint8_t** ppdst, uint32_t* ppdstsize, uint32_t codec,
    uint8_t* psrcbuf, uint32_t psrcsize, uint32_t srcxsize, uint32_t srcysize,
    uint8_t srcdepth,  palette_t* ppalette, bool invertX, bool invertY)
{
    INLINE_OBJECT_NULL_CHK(ppdst);
    INLINE_OBJECT_NULL_CHK(ppdstsize);

    //INLINE_OBJECT_NULL_CHK(psrc);
    //INLINE_OBJECT_SIZE_CHK(psrcsize, psrcsize, 0);

    if (srcdepth != 8 && srcdepth != 16 && srcdepth != 24 && srcdepth != 32) {
        fprintf(stderr, "TGA, Unsupported depth: %d.\n", srcdepth);
        return false;
    }

    // src stuff
    uint32_t bytesperpixel = srcdepth >> 3;
    uint32_t pitch = srcxsize * bytesperpixel;
    uint8_t* rowptr = psrcbuf;
    uint8_t* bufptr = psrcbuf;
    uint16_t colormap_index = 0;
    uint16_t colormap_length = 0;
    uint8_t  id_length = 0;
    uint8_t  colormap_type = 0;
    uint8_t  image_type = TGA_NO_IMAGE_DATA;
    uint8_t  colormap_size = 0;
    uint16_t x_origin = 0;
    uint16_t y_origin = 0;
    uint16_t width = srcxsize;
    uint16_t height = srcysize;
    uint8_t  pixel_size = srcdepth;
    uint8_t  image_descriptor = 0;
    
    // palette
    if (srcdepth == 8) {
        if (ppalette != NULL) {
            colormap_type = 1;
            colormap_length = ppalette->size;
            colormap_size = ppalette->bits;
        }
    }

    if (psrcbuf == NULL) {
        image_type = TGA_NO_IMAGE_DATA;
    } else {
        if (codec == GEUL_RLE) {
            switch(srcdepth)
            {
                case 32:
                case 24:
                case 16:
                {
                    image_type = TGA_RGB_RLE;
                } break;
                case 8:
                {
                    if (ppalette == NULL) {
                        image_type = TGA_BLACK_AND_WHITE_RLE;
                    } else {
                        image_type = TGA_MAPPED_RLE;
                    }
                } break;
            }
        } else {            // ENCODE_RGB
            switch (srcdepth)
            {
                case 32:
                case 24:
                case 16:
                {
                    image_type = TGA_RGB;
                } break;
                case 8:
                {
                    if (ppalette == NULL) {
                        image_type = TGA_BLACK_AND_WHITE;
                    } else {
                        image_type = TGA_MAPPED;
                    }
                } break;
            }
        }
    }

    // byte encoded array
    uint32_t datasize = s_tga_file_size + ((srcysize * pitch) * 2) + colormap_length;
    uint8_t* data = (uint8_t*)malloc(datasize);
    uint32_t bytesencoded = 0;

    if (data == NULL) {
        fprintf(stderr, "TGA, Out of memory.\n");
        return false;
    }

    memset(data, 0, datasize);

    uint8_t* dstbuf = data;
    
    // fill in file header
    x_origin = (invertX == true ? 0 : srcxsize);
    y_origin = (invertY == true ? 0 : srcysize);

    *dstbuf++ = id_length;
    *dstbuf++ = colormap_type;
    *dstbuf++ = image_type;
    WriteU16ToLE(dstbuf, colormap_index);   dstbuf += 2;
    WriteU16ToLE(dstbuf, colormap_length);  dstbuf += 2;
    *dstbuf++ = colormap_size;
    WriteU16ToLE(dstbuf, x_origin);         dstbuf += 2;
    WriteU16ToLE(dstbuf, y_origin);         dstbuf += 2;
    WriteU16ToLE(dstbuf, width);            dstbuf += 2;
    WriteU16ToLE(dstbuf, height);           dstbuf += 2;
    *dstbuf++ = pixel_size;
    *dstbuf++ = image_descriptor;

    dstbuf += id_length;            // skip the image descriptor 
    
    if (colormap_length != 0 && ppalette != NULL) {
        // fill in the palette
        if (colormap_size == 15 || colormap_size == 16) {
            uint16_t rgb16 = 0;
            uint16_t r = 0;
            uint16_t g = 0;
            uint16_t b = 0;
            for (uint16_t i = colormap_index; i < colormap_length; ++i)
            {
                r = (ppalette->data[i].r * 0x1F) / 0xFF;
                g = (ppalette->data[i].g * 0x1F) / 0xFF;
                b = (ppalette->data[i].b * 0x1F) / 0xFF;
                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);
                WriteU16ToLE(dstbuf, rgb16); dstbuf += 2;
            }
        } else if (colormap_size == 24) {
            for (uint16_t i = colormap_index; i < colormap_length; ++i)
            {
                *dstbuf++ = ppalette->data[i].b;
                *dstbuf++ = ppalette->data[i].g;
                *dstbuf++ = ppalette->data[i].r;
            }
        } else if (colormap_size == 32) {            
            for (uint16_t i = colormap_index; i < colormap_length; ++i)
            {
                *dstbuf++ = ppalette->data[i].b;
                *dstbuf++ = ppalette->data[i].g;
                *dstbuf++ = ppalette->data[i].r;
                *dstbuf++ = ppalette->data[i].a;
            }
        }
    }

    if (psrcbuf != NULL) {
        uint32_t x = 0;
        uint32_t y = 0;
        if (codec == GEUL_RLE) {          // run-length encoding
            uint32_t abscount = 0;
            uint32_t rlecount = 0;
            uint32_t runcount = 0;
            uint32_t maxcount = 0;
            uint32_t absbound = srcxsize - 0xFF;
            uint32_t rlebound = srcxsize - 0x80;
            uint32_t sample = 0;
            uint8_t* buffer = NULL;
            switch (image_type)
            {
            case TGA_RGB_RLE:
            case TGA_MAPPED_RLE:
            case TGA_BLACK_AND_WHITE_RLE:
            {
                while (y < srcysize) {
                    bufptr = rowptr;
                    x = 0;
                    while (x < srcxsize) {
                        memcpy(&sample, (bufptr + (x * bytesperpixel)), bytesperpixel);
                        buffer =  (bufptr + (x * bytesperpixel));
                        maxcount = 0xFF;
                        if (x > absbound) { maxcount = srcxsize - x; };
                        abscount = ParseABS(&buffer, bytesperpixel, maxcount);
                        maxcount = 0x80;
                        if (x > rlebound) { maxcount = srcxsize - x; };
                        rlecount = ParseRLE(&buffer, bytesperpixel, maxcount);
                        if (abscount >= rlecount) {
                            runcount = abscount;
                            *dstbuf++ = (0 << 7) | (runcount - 1);
                            bytesencoded++;
                            memcpy(dstbuf, (bufptr + (x * bytesperpixel) * runcount),
                                bytesperpixel);
                            dstbuf += bytesperpixel * runcount;
                            bytesencoded += bytesperpixel * runcount;
                        } else {
                            runcount = rlecount;
                            *dstbuf++ = (1 << 7) | (runcount - 1);
                            bytesencoded++;
                            memcpy(dstbuf, &sample, bytesperpixel);
                            dstbuf += bytesperpixel;
                            bytesencoded += bytesperpixel;
                        }
                        x += runcount;
                    }
                    rowptr += pitch;
                    y++;
                }
            } break;
            }
        } else {            // everything else
            memcpy(dstbuf, bufptr, srcysize * pitch);
            dstbuf += srcysize * pitch;
            bytesencoded += srcysize * pitch;
        }
    }

    *ppdstsize = s_tga_file_size + id_length + colormap_length *
        ((colormap_size==15 ? 16 : colormap_size) >> 3) + bytesencoded;
    *ppdst = (uint8_t*)realloc(data, ((*ppdstsize + 1) & ~1));
    
    return true;
}

//-----------------------------------------------------------------------------
// GetInfo
//-----------------------------------------------------------------------------
static bool
GetInfoFromMemoryTGA(uint8_t* srccolormap, uint32_t* srcxsize, uint32_t* srcysize,
    uint32_t* srcdepth, uint8_t* psrcbuf, uint32_t psrcsize)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, s_tga_file_size);

    uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;

    // file struct
    uint8_t  id_length          = *srcbuf++;
    uint8_t  colormap_type      = *srcbuf++;
    uint8_t  image_type         = *srcbuf++;
    uint16_t colormap_index     = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t colormap_length    = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint8_t  colormap_size      = *srcbuf++;
    uint16_t x_origin           = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t y_origin           = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t width              = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t height             = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint8_t  pixel_size         = *srcbuf++;
    uint8_t  image_descriptor   = *srcbuf++;

    if (colormap_type == 1) {
        if (image_type != TGA_MAPPED &&
            image_type != TGA_MAPPED_RLE) {
            return false;
        } else if (
            colormap_size !=  8 && colormap_size != 15 &&
            colormap_size != 16 && colormap_size != 24 &&
            colormap_size != 32
            ) {
            return false;
        }
    } else if (colormap_type == 0) {
        if (image_type != TGA_RGB &&
            image_type != TGA_RGB_RLE &&
            image_type != TGA_BLACK_AND_WHITE &&
            image_type != TGA_BLACK_AND_WHITE_RLE) {
            return false;
        }
    } else {
        return false;
    }

    if (srcxsize != NULL) { *srcxsize = width; }
    if (srcysize != NULL) { *srcysize = height; }
    if (srcdepth != NULL) { *srcdepth = pixel_size; }
    if (srccolormap != NULL) { *srccolormap = colormap_type; }

    return true;
}

//-----------------------------------------------------------------------------
// LoadTGA
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryTGA(uint8_t** ppdst, palette_t* ppalette, uint8_t* psrcbuf,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint32_t* srcdepth)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, s_tga_file_size);

    // src stuff
    uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;

    // file struct
    uint8_t  id_length           = *srcbuf++;
    uint8_t  colormap_type       = *srcbuf++;
    uint8_t  image_type          = *srcbuf++;
    uint16_t colormap_index      = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t colormap_length     = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint8_t  colormap_size       = *srcbuf++;
    uint16_t x_origin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t y_origin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t width               = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t height              = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint8_t  pixel_size          = *srcbuf++;
    uint8_t  image_descriptor    = *srcbuf++;

    if (pixel_size != 8 && pixel_size != 16 && pixel_size != 24 && pixel_size != 32) {
        fprintf(stderr, "TGA, Unsupported bits: %d.\n", pixel_size);
        return false;
    }

    if (colormap_type == 1) {
        if (image_type != TGA_MAPPED &&
            image_type != TGA_MAPPED_RLE) {
            fprintf(stderr, "TGA, Colour map type and image type mismatch.\n");
            return false;
        } else if (
            colormap_size !=  8 && colormap_size != 15 &&
            colormap_size != 16 && colormap_size != 24 &&
            colormap_size != 32
            ) {
            fprintf(stderr, "TGA, Colour map type and colour map size mismatch.\n");
            return false;
        }
    } else if (colormap_type == 0) {
        if (image_type != TGA_RGB &&
            image_type != TGA_RGB_RLE &&
            image_type != TGA_BLACK_AND_WHITE &&
            image_type != TGA_BLACK_AND_WHITE_RLE) {
            fprintf(stderr, "TGA, Colour map type and image type mismatch.\n");
            return false;
        }
    } else {
        fprintf(stderr, "TGA, Unsupported colour map type: %d.\n", colormap_type);
        return false;
    }

    if (colormap_type == 1 &&
       (image_type == TGA_MAPPED || image_type == TGA_MAPPED_RLE)) {
        if (pixel_size == 8) {
            uint32_t palnum = colormap_length < 256 ? colormap_length : 256;
            uint8_t* palptr = srcbuf + id_length;
            if (ppalette != NULL) {
                if (colormap_size == 15 || colormap_size == 16) {
                    uint16_t pixel = 0;
                    for (size_t i = 0; i < palnum; ++i)
                    {
                        pixel = 0;
                        pixel |= *palptr++;
                        pixel |= *palptr++ << 8;
                        
                        ppalette->data[i].b = (((pixel >>  0) & 0x1F) * 0xFF) / 0x1F;
                        ppalette->data[i].g = (((pixel >>  5) & 0x1F) * 0xFF) / 0x1F;
                        ppalette->data[i].r = (((pixel >> 10) & 0x1F) * 0xFF) / 0x1F;
                        ppalette->data[i].a = 255;
                    }
                    ppalette->size = palnum;
                    ppalette->bits = 24;
                } else if (colormap_size == 24) {
                    for (size_t i = 0; i < palnum; ++i)
                    {
                        ppalette->data[i].b = *palptr++;
                        ppalette->data[i].g = *palptr++;
                        ppalette->data[i].r = *palptr++;
                        ppalette->data[i].a = 255;
                    }
                    ppalette->size = palnum;
                    ppalette->bits = 24;
                } else if (colormap_size == 32) {
                    for (size_t i = 0; i < palnum; ++i)
                    {
                        ppalette->data[i].b = *palptr++;
                        ppalette->data[i].g = *palptr++;
                        ppalette->data[i].r = *palptr++;
                        ppalette->data[i].a = *palptr++;
                    }
                    ppalette->size = palnum;
                    ppalette->bits = 32;
                }
            }
        }
    }

    // skip the id and palette
    srcbuf = srcbuf + id_length + colormap_length *
        ((colormap_size==15 ? 16 : colormap_size) >> 3);

    // dst stuff
    uint32_t xsize = width;
    uint32_t ysize = height;
    uint32_t bytesperpixel = pixel_size >> 3;
    uint32_t pitch = xsize * bytesperpixel;
    uint32_t rlevalue = 0;
    uint32_t rlecount = 0;
    uint8_t rgba[4] = { 0 };

    INLINE_OBJECT_NULL_CHK(ppdst);

    uint8_t* pixels = (uint8_t*)malloc(xsize * ysize * bytesperpixel);
    uint8_t* rowptr = pixels;           // start of current dst row
    uint8_t* buffer = pixels;           // current dst row

    if (pixels == NULL) {
        fprintf(stderr, "TGA: Out of memory.\n");
        return false;
    }

    memset(pixels, 0, xsize * ysize * bytesperpixel);
    
    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = pixel_size; }

    switch (image_type)
    {
        case TGA_MAPPED_RLE:
        case TGA_RGB_RLE:
        case TGA_BLACK_AND_WHITE_RLE:
        {
            uint32_t y = 0;
            uint32_t x = 0;
            
            while (y < ysize)
            {
                buffer = rowptr;
                rlecount = 0;
                rlevalue = 0;
                x = 0;
                while (x < xsize)
                {
                    if (rlecount == 0) {            // get rle count
                        rlevalue = *srcbuf++;
                        rlecount = (rlevalue & 0x7F) + 1;
                        for (uint32_t i = 0; i < bytesperpixel; ++i)
                        {
                            rgba[i] = *srcbuf++;
                        }
                    } else if (!(rlevalue & 0x80)) {
                        // if this is not a repeating count read next pixel of
                        // component size
                        for (uint32_t i = 0; i < bytesperpixel; ++i)
                        {
                            rgba[i] = *srcbuf++;
                        }
                    }
                    memcpy(buffer, rgba, bytesperpixel);
                    buffer += bytesperpixel;
                    rlecount--;
                    x++;
                }
                rowptr += pitch;
                y++;
            }
        } break;
        case TGA_MAPPED:
        case TGA_RGB:
        case TGA_BLACK_AND_WHITE:
        {
            memcpy(buffer, srcbuf, ysize * pitch);
        } break;
    }

    return true;
}

#endif  // #ifndef _TGA_H_

#ifndef _BMP_H_
#define _BMP_H_


//-----------------------------------------------------------------------------
//
//  Windows Bitmap (VERSION 3.0)
//
//-----------------------------------------------------------------------------


#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#else
// constants for the biCompression field
#define BI_RGB          0L
#define BI_RLE8         1L
#define BI_RLE4         2L
#define BI_BITFIELDS    3L
#endif // #ifdef _WIN32

#define BMP_IDENTIFIER          0x4D42

static const uint32_t s_bmp_file_size = 14;
static const uint32_t s_bmp_v3_info_size = 40;

//------------------------------------------------------------------------------
// SaveBMP
//------------------------------------------------------------------------------
static bool
SaveToMemoryBMP(uint8_t** ppdst, uint32_t* ppdstsize, uint32_t codec,
    uint8_t* psrcbuf, uint32_t psrcsize, uint32_t srcxsize, uint32_t srcysize,
    uint32_t srcdepth, palette_t* ppalette, rgba_t* pcolorkey, bool invertY)
{
    INLINE_OBJECT_NULL_CHK(ppdst);
    INLINE_OBJECT_NULL_CHK(ppdstsize);

    //INLINE_OBJECT_NULL_CHK(psrc);
    //INLINE_OBJECT_SIZE_CHK(psrcsize, 0);

    if (srcdepth !=  1 && srcdepth !=  4 && srcdepth != 8 && srcdepth != 24 &&
        srcdepth != 32) {
        fprintf(stderr, "BMP, Unsupported depth: %d.\n", srcdepth);
        return false;
    }

    uint8_t dstdepth = srcdepth;

    // dst stuff
    uint32_t dstpitch = (WidthInBytes(srcxsize, dstdepth) + 3) & ~3;           // 4-byte boundary
    uint32_t dstpalettesize = 0;

    // palette
    if (srcdepth <= 8) {
        if (ppalette != NULL) {
            dstpalettesize = ppalette->size * 4;
        }
    }

    // src stuff
    uint32_t srcpitch = WidthInBytes(srcxsize, srcdepth);
    uint8_t* pixels = psrcbuf;
    uint8_t* rowptr = psrcbuf;
    uint8_t* rowbuf = psrcbuf;
    uint32_t padbytes = dstpitch - srcpitch;

    // byte encoded array
    int32_t datasize = s_bmp_file_size + s_bmp_v3_info_size + ((srcysize * dstpitch)) +
        dstpalettesize;
    uint8_t* data = (uint8_t*)malloc(datasize);

    if (data == NULL) {
        fprintf(stderr, "BMP, Out of memory.\n");
        return false;
    }

    memset(data, 0, datasize);

    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;

    // fill in file and info header
    uint16_t filetype = BMP_IDENTIFIER;
    uint32_t filesize = datasize;
    uint32_t offset = s_bmp_file_size + s_bmp_v3_info_size + dstpalettesize;

    uint32_t infosize = s_bmp_v3_info_size;
    int32_t  width = srcxsize;
    int32_t  height = (invertY == true) ? srcysize : -(int32_t)(srcysize);          // bottom-up dib
    uint16_t planes = 1;
    uint16_t bits = dstdepth;
    uint32_t compression = BI_RGB;

    if (codec == GEUL_RLE) {
        if (srcdepth == 8) {
            compression = BI_RLE8;
        }
        else if (srcdepth == 4) {
            compression = BI_RLE4;
        }
    }

    int32_t  xresolution = 0;
    int32_t  yresolution = 0;
    uint32_t num_colours = dstpalettesize >> 2;
    uint32_t num_colour_indexes = 0;
    
    // file struct
    WriteU16ToLE(dstbuf, filetype); dstbuf += 2;            // type
    WriteU32ToLE(dstbuf, filesize); dstbuf += 4;            // size
    WriteU16ToLE(dstbuf,        0); dstbuf += 2;            // reserved1
    WriteU16ToLE(dstbuf,        0); dstbuf += 2;            // reserved2
    WriteU32ToLE(dstbuf,   offset); dstbuf += 4;            // offset

    // info struct
    WriteU32ToLE(dstbuf, infosize);     dstbuf += 4;            // size
    WriteI32ToLE(dstbuf, width);        dstbuf += 4;            // width
    WriteI32ToLE(dstbuf, height);       dstbuf += 4;            // height
    WriteU16ToLE(dstbuf, planes);       dstbuf += 2;            // planes
    WriteU16ToLE(dstbuf, bits);         dstbuf += 2;            // bits
    WriteU32ToLE(dstbuf, compression);  dstbuf += 4;            // compression

    height = ABS(height);
    uint32_t imagesize = width * height * (dstdepth < 8 ? 1 : (dstdepth >> 3));

    WriteU32ToLE(dstbuf, imagesize);    dstbuf += 4;            // image size
    WriteI32ToLE(dstbuf, xresolution);  dstbuf += 4;            // xresolution
    WriteI32ToLE(dstbuf, yresolution);  dstbuf += 4;            // yresolution
    WriteU32ToLE(dstbuf, num_colours);  dstbuf += 4;            // num_colours
    WriteU32ToLE(dstbuf, num_colour_indexes); dstbuf += 4;      // num_colour_indexes

    // fill in the palette
    if (srcdepth <= 8) {
        if (ppalette != NULL) {
            for (uint32_t i = 0; i < dstpalettesize >> 2; ++i)
            {
                *dstbuf++ = ppalette->data[i].b;
                *dstbuf++ = ppalette->data[i].g;
                *dstbuf++ = ppalette->data[i].r;
                *dstbuf++ = ppalette->data[i].a;
            }
        }
    }

    uint32_t bytesencoded = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    if (psrcbuf != NULL) {
        if (compression != BI_RGB) {            // run-length encoding
            int32_t colorkey = -1;
            if (pcolorkey != NULL && ppalette != NULL) {
                for (uint32_t i = 0; i < dstpalettesize >> 2; ++i)
                {
                    if (ppalette->data[i].r == pcolorkey->r &&
                        ppalette->data[i].g == pcolorkey->g &&
                        ppalette->data[i].b == pcolorkey->b &&
                        ppalette->data[i].a == pcolorkey->a) {
                        colorkey = i;
                        break;
                    }
                }
            }
            uint8_t* buffer = NULL;
            uint32_t abscount = 0;
            uint32_t rlecount = 0;
            uint32_t runcount = 0;
            uint32_t maxcount = 0xFF;
            uint32_t maxbound = width - 0xFF;
            uint8_t sample = 0;
            switch (dstdepth)
            {
            case 4:         // 4-bit encoding
            {
                // expand to 4 to 8-bits
                pixels = NULL;
                pixels = (uint8_t*)malloc(srcxsize * srcysize);
                uint8_t* pixbuf = pixels;
                uint8_t* srcbuf = rowbuf;
                memset(pixels, 0, srcxsize * srcysize);
                while (y < srcysize) {
                    x = 0;
                    while (x < srcxsize) {
                        if ((x & 1) == 0) {
                            sample = *srcbuf++; *pixbuf++ = sample >> 4;
                        }
                        if ((x & 1) == 1) {
                            *pixbuf++ = sample & 0xF;
                        }
                        x++;
                    }
                    y++;
                }
                srcpitch = width;
                x = 0;
                y = 0;
                rowbuf = pixels;
                rowptr = pixels;
            };
            case 8:         // 8-bit encoding
            {
                while (y < (uint32_t)height) {
                    rowbuf = rowptr;
                    x = 0;
                    if (*rowbuf == colorkey) {
                        uint32_t count = 0;
                        while (count < width * (height - y)) {
                            if (*(rowbuf + count) != colorkey) {
                                break;
                            }
                            count++;
                        }
                        if (count >= (uint32_t)width) {
                            uint32_t dx = 0;
                            uint32_t dy = 0;
                            while (count >= (uint32_t)width)
                            {
                                count -= width;
                                dy++;
                            }
                            dx = count;
                            y += dy;
                            x += dx;
                            *dstbuf++ = 0x00;
                            *dstbuf++ = 0x02;
                            *dstbuf++ = dx;
                            *dstbuf++ = dy;
                            bytesencoded++;
                            bytesencoded++;
                            bytesencoded++;
                            bytesencoded++;
                            if (y < (uint32_t)height) {
                                rowptr = psrcbuf + (y * srcpitch);
                                rowbuf = rowptr + x;
                            }
                        }
                    }
                    while (x < (uint32_t)width) {
                        sample = *(rowbuf + x);
                        buffer =  (rowbuf + x);
                        maxcount = 0xFF;
                        if (x > maxbound) { maxcount = width - x; };
                        abscount = ParseABS(&buffer, 1, maxcount);
                        rlecount = ParseRLE(&buffer, 1, maxcount);
                        if (abscount >= 3 && abscount >= rlecount) {
                            for (uint32_t i = 0; i < abscount; i++) {
                                if (*(rowbuf + x + i) == colorkey) {
                                    abscount = i;
                                    break;
                                }
                            }
                        }
                        if (abscount >= 3 && abscount >= rlecount) {
                            runcount = abscount;
                            *dstbuf++ = 0x00;
                            *dstbuf++ = runcount;
                            bytesencoded++;
                            bytesencoded++;
                            uint32_t count = runcount;
                            if (dstdepth == 4) {
                                count = (runcount >> 1);
                            }
                            for (uint32_t i = 0; i < count; ++i)
                            {
                                sample = 0;
                                switch (dstdepth)
                                {
                                case 4:
                                {
                                    sample |= *(rowbuf + x + i + 0) << 4;
                                    sample |= *(rowbuf + x + i + 1);
                                } break;
                                case 8:
                                {
                                    sample = *(rowbuf + x + i);
                                } break;
                                }
                                *dstbuf++ = sample;
                                bytesencoded++;
                            }
                            // pad-byte
                            if ((runcount & 1)) {
                                *dstbuf++ = 0x00;
                                bytesencoded++;
                            }
                        } else {
                            runcount = rlecount;
                            if (sample == colorkey) {
                                *dstbuf++ = 0x00;
                                *dstbuf++ = 0x02;
                                bytesencoded++;
                                bytesencoded++;
                                sample = 0;
                            } else {
                                switch (dstdepth)
                                {
                                case 4:
                                {
                                    int d1 = sample & 0xF;
                                    int d0 = sample << 4;
                                    sample = d0 + d1;
                                } break;
                                }
                            }
                            *dstbuf++ = runcount;
                            *dstbuf++ = sample;
                            bytesencoded++;
                            bytesencoded++;
                        }
                        x += runcount;
                    }
                    *dstbuf++ = 0x00;
                    *dstbuf++ = 0x00;
                    bytesencoded++;
                    bytesencoded++;
                    rowptr += srcpitch;
                    y++;
                }
                // end of bitmap 
                *dstbuf++ = 0x00;
                *dstbuf++ = 0x01;
                bytesencoded++;
                bytesencoded++;
                if (dstdepth == 4) {
                    free(pixels);
                    pixels = NULL;
                }
            } break;
            }
        } else {            // everything else
            while (y < (uint32_t)height) {
                rowbuf = rowptr;
                memcpy(dstbuf, rowbuf, srcpitch);
                rowptr += srcpitch;
                dstbuf += srcpitch + padbytes;
                bytesencoded += srcpitch + padbytes;
                y++;
            }
        }
    } else {
        // end of bitmap 
        *dstbuf++ = 0x00;
        *dstbuf++ = 0x01;
        bytesencoded++;
        bytesencoded++;
    }

    datasize = s_bmp_file_size + s_bmp_v3_info_size + dstpalettesize + bytesencoded;

    // rewrite file struct size
    dstbuf = dstptr + 2;
    WriteU32ToLE(dstbuf, datasize); dstbuf += 4;

    *ppdstsize = datasize;
    *ppdst = (uint8_t*)realloc(data, (*ppdstsize + 1) & ~1);

    return true;
}

//------------------------------------------------------------------------------
// GetInfo
//------------------------------------------------------------------------------
static bool
GetInfoFromMemoryBMP(uint32_t* srcxsize, uint32_t* srcysize, uint32_t* srcdepth,
    uint8_t* psrcbuf, uint32_t psrcsize)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, s_bmp_file_size);

    uint8_t* srcbuf = psrcbuf;

    // file struct
    uint16_t type        = ReadU16FromLE(srcbuf); srcbuf += 2;
    srcbuf += 4;            // file size
    uint16_t reserved1   = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t reserved2   = ReadU16FromLE(srcbuf); srcbuf += 2;
    srcbuf += 4;            // offset
    // info struct
    srcbuf += 4;            // info size
    int32_t  xsize       = ReadI32FromLE(srcbuf); srcbuf += 4;
    int32_t  ysize       = ReadI32FromLE(srcbuf); srcbuf += 4;
    uint16_t planes      = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t bits        = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint32_t compression = ReadU32FromLE(srcbuf); srcbuf += 4;

    if (type != BMP_IDENTIFIER || reserved1 != 0 || reserved2 != 0 || planes != 1 ||
        compression > 2) {
        return false;
    }

    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ABS(ysize); }
    if (srcdepth != NULL) { *srcdepth = bits & 0xFF; }

    return true;
}

//------------------------------------------------------------------------------
// LoadBMP
//------------------------------------------------------------------------------
static bool
LoadFromMemoryBMP(uint8_t** ppdst, palette_t* ppalette, uint8_t* psrcbuf,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint32_t* srcdepth)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, s_bmp_file_size);

    uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;

    // file struct
    uint16_t filetype   = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint32_t filesize   = ReadU32FromLE(srcbuf); srcbuf += 4;
    uint16_t reserved1  = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t reserved2  = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint32_t offset     = ReadU32FromLE(srcbuf); srcbuf += 4;

    // info struct
    uint32_t infosize           = ReadU32FromLE(srcbuf); srcbuf += 4;
    int32_t  width              = ReadI32FromLE(srcbuf); srcbuf += 4;
    int32_t  height             = ReadI32FromLE(srcbuf); srcbuf += 4;
    uint16_t planes             = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t bits               = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint32_t compression        = ReadU32FromLE(srcbuf); srcbuf += 4;
    uint32_t imagesize          = ReadU32FromLE(srcbuf); srcbuf += 4;
    int32_t  xresolution        = ReadI32FromLE(srcbuf); srcbuf += 4;
    int32_t  yresolution        = ReadI32FromLE(srcbuf); srcbuf += 4;
    uint32_t num_colours        = ReadU32FromLE(srcbuf); srcbuf += 4;
    uint32_t num_colour_indexes = ReadU32FromLE(srcbuf); srcbuf += 4;

    if (filetype != BMP_IDENTIFIER) {
        fprintf(stderr, "BMP, version mismatch: %d.\n", filetype);
        return false;
    }

    if (reserved1 != 0 || reserved2 != 0) {
        fprintf(stderr, "BMP, reserved parameters non-zero: reserved1: %d, reserved2: \
            %d.\n", reserved1, reserved2);
        return false;
    }

    if (planes != 1 || compression > 2) {
        fprintf(stderr, "BMP, number of bit-planes must be 1: %d.\n", planes);
        return false;
    }

    if (compression > 2) {
        fprintf(stderr, "BMP, invalid compression format: %d.\n", compression);
        return false;
    }

    if (bits !=  1 && bits !=  4 && bits != 8 && bits != 24 && bits != 32) {
        fprintf(stderr, "BMP, unsupported bits: %d.\n", bits);
        return false;
    }

    if (bits == 1 || bits == 4 || bits == 8) {

        uint8_t* palptr = srcptr + s_bmp_file_size + s_bmp_v3_info_size;
        uint32_t palnum = num_colours;

        if (ppalette != 0) {

            for (uint32_t i = 0; i < palnum; ++i)
            {
                ppalette->data[i].b = *palptr++;
                ppalette->data[i].g = *palptr++;
                ppalette->data[i].r = *palptr++;
                ppalette->data[i].a = *palptr++;
            }

            ppalette->size = palnum;
            ppalette->bits = 32;

            uint32_t alpha = 0;

            for (size_t i = 0; i < ppalette->size; ++i)
            {
                alpha |= ppalette->data[i].a;
            }

            // if alpha is all 0s replace with 255s
            if (alpha == 0) {
                for (size_t i = 0; i < ppalette->size; ++i)
                {
                    ppalette->data[i].a = 255;
                }
            }
        }
    }

    srcbuf = srcptr + offset;            // ptr to data

    bool rle = ((compression == BI_RLE8) || (compression == BI_RLE4));          // BI_RLE4=2 && BI_RLE8=1
    int32_t xsize = width;
    int32_t ysize = height;
    int32_t pitch = WidthInBytes(xsize, bits);
    int32_t widthbytes = ((xsize * bits + 31) >> 5) * 4;           // 4-byte boundary
    int32_t padbytes = widthbytes - pitch;
    int32_t rlecount = 0;
    uint8_t sample = 0;

    INLINE_OBJECT_NULL_CHK(ppdst);

    uint8_t* pixels = (uint8_t*)malloc(ABS(ysize) * pitch);
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixels;

    if (pixels == NULL) {
        fprintf(stderr, "BMP, Out of memory.\n");
        return false;
    }

    memset(pixels, 0, ABS(ysize) * pitch);

    // bottom-up dib
    if (ysize >= 0) {
        pixptr = (pixels + ((ysize-1)) * pitch);
        pitch = -pitch;
    }

    ysize = ABS(ysize);

    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ABS(ysize); }
    if (srcdepth != NULL) { *srcdepth = (bits & 0xFF); }

    if (rle) {          // run-length encoding

        int32_t x = 0;
        int32_t y = 0;
        uint8_t bit = 0;

        while (y < ysize)
        {
            pixbuf = pixptr;
            bit = 4;
            x = 0;

            while (1)
            {    
                unsigned char data0 = *srcbuf++;
                unsigned char data1 = *srcbuf++;

                // encoded modes
                if (!data0) {
                    if (data1 >= 0x03 && data1 <= 0xFF) {           // absolute encoding

                        // rle count word aligned
                        int rlebytes = (data1 + 1) & ~1;
                        rlecount = rlebytes >> 1;
                        padbytes = rlecount % 2;

                        switch (bits)
                        {
                            case 4:
                            {
                                for (int i = 0; i < data1; ++i)
                                {
                                    if ((i & 1) == 0) {
                                        sample = *srcbuf++;
                                    }

                                    int d1 = sample & 0xF;
                                    int d0 = sample >> 4;

                                    if ((i & 1) == 0) {
                                        *pixbuf |= d0 << bit;
                                    }
                                    if ((i & 1) == 1) {
                                        *pixbuf |= d1 << bit;
                                    }

                                    bit ^= 4;
                                    x++;

                                    if ((x & 1) == 0) {
                                        pixbuf++;
                                    }
                                }

                                srcbuf += padbytes;
                            } break;
                            case 8:
                            {
                                memcpy(pixbuf, srcbuf, data1);
                                pixbuf += data1;
                                srcbuf += rlebytes;
                            } break;
                        }
                    } else if (data1 == 0x02) {           // delta encoding
                        int dx = *srcbuf++;
                        int dy = *srcbuf++;

                        dx += (int)(pixbuf - pixptr) * (8 / bits);
                        y += dy;

                        if (y >= ysize) break;

                        if (height >= 0) {
                            pixptr = pixels + ((ysize - (y + 1)) * ABS(pitch));
                        } else {
                            pixptr = pixels + (y * ABS(pitch));
                        }

                        if (bits == 4) {
                            dx = (dx >= 2) ? (dx >> 1) : dx;
                        }

                        pixbuf = pixptr + dx;
                    } else if (data1 == 0x01) {           // end of bitmap
                        return true;
                    } else {            // end of scanline
                        break;
                    }
                } else {            // run-length encoding
                    // rle count
                    rlecount = data0;
                    sample = data1;

                    switch (bits)
                    {
                    case 4:         // 4-bit
                    {
                        for (int i = 0; i < rlecount; ++i)
                        {
                            int d1 = sample & 0xF;
                            int d0 = sample >> 4;

                            if ((i & 1) == 0) {
                                *pixbuf |= d0 << bit;
                            }
                            if ((i & 1) == 1) {
                                *pixbuf |= d1 << bit;
                            }

                            bit ^= 4;
                            x++;

                            if ((x & 1) == 0) {
                                pixbuf++;
                            }
                        }
                    } break;
                    case 8:         // 8-bit
                    {
                        memset(pixbuf, data1, data0);
                        pixbuf += data0;
                    } break;
                    }
                }
            }
            pixptr += pitch;
            y++;
        }
    } else {          // everything else 
        int32_t y = 0;

        while (y < ysize)
        {
            pixbuf = pixptr;
            memcpy(pixbuf, srcbuf, ABS(pitch));
            srcbuf += ABS(pitch) + padbytes;
            pixptr += pitch;
            y++;
        }

        if (bits == 32) {

            uint32_t alpha = 0;
            pixbuf = pixels;

            for (int i = 0; i < xsize * ABS(ysize); ++i, pixbuf += 4)
            {
                alpha |= pixbuf[3];
            }

            // if alpha is all 0's replace with 255's
            if (alpha == 0) {
                pixbuf = pixels;
                for (int i = 0; i < xsize * ABS(ysize); ++i, pixbuf += 4)
                {
                    pixbuf[3] = 255;
                }
            }
        }
    }

    return true;
}

#endif // #ifndef _BMP_H_

#ifndef _PCX_H_
#define _PCX_H_


//-----------------------------------------------------------------------------
//
//  ZSoft PCX File Format
//
//-----------------------------------------------------------------------------


// default 16 color ega palette
static rgba_t ega_palette[16] = {
    {   0,   0,   0, 255 },
    {   0,   0, 170, 255 },
    {   0, 170,   0, 255 },
    {   0, 170, 170, 255 },
    { 170,   0,   0, 255 },
    { 170,   0, 170, 255 },
    { 170,  85,   0, 255 },
    { 170, 170, 170, 255 },
    {  85,  85,  85, 255 },
    {  85,  85, 255, 255 },
    {  85, 255,  85, 255 },
    {  85, 255, 255, 255 },
    { 255,  85,  85, 255 },
    { 255,  85, 255, 255 },
    { 255, 255,  85, 255 },
    { 255, 255, 255, 255 }
};

static const uint32_t s_pcx_v5_info_size = 128;

//-----------------------------------------------------------------------------
// SavePCX
//-----------------------------------------------------------------------------
static bool
SaveToMemoryPCX(uint8_t** ppdst, uint32_t* ppdstsize, uint32_t codec,
    uint8_t* psrcbuf, uint32_t psrcsize, uint32_t srcxsize, uint32_t srcysize,
    uint8_t srcdepth, palette_t* ppalette)
{
    INLINE_OBJECT_NULL_CHK(ppdst);
    INLINE_OBJECT_NULL_CHK(ppdstsize);

    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, 0);

    if (srcdepth !=  1 && srcdepth != 2 && srcdepth != 4 && srcdepth != 8 &&
        srcdepth != 24) {
        fprintf(stderr, "PCX, Unsupported src depth: %d.\n", srcdepth);
        return false;
    }

    uint32_t xsize = srcxsize;
    uint32_t ysize = srcysize;
    uint8_t palettetype = 0;

    if (srcdepth <= 8) {
        palettetype = 1;
    }

    // dst stuff
    uint32_t dstdepth = (srcdepth == 24) ? 8 : srcdepth;
    uint32_t dstpitch = (WidthInBytes(srcxsize, dstdepth) + 1) & ~1;           // must be an even number

    // src stuff
    uint32_t srcbytes = ((srcdepth == 24) ? 3 : 1);
    uint32_t srcpitch = WidthInBytes(srcxsize, srcdepth);

    if (srcdepth < 8) {
        xsize = srcpitch;
    }

    uint8_t* rowptr = psrcbuf;
    uint8_t* bufptr = psrcbuf;
    uint8_t* buffer = NULL;

    uint32_t dstpadbytes = dstpitch - (srcpitch / srcbytes);
    uint32_t dstpalettesize = 0;

    // palette
    if (srcdepth == 8 && ppalette == NULL) {
        fprintf(stderr, "PCX, Missing palette.\n");
        return false;
    }
    if (srcdepth == 8) {
        dstpalettesize = 768;
    }

    // byte encoded array
    uint32_t datasize = s_pcx_v5_info_size + (ysize * (srcbytes * dstpitch)) +
        dstpalettesize;
    uint8_t* data = (uint8_t*)malloc(datasize+datasize);

    if (data == NULL) {
        fprintf(stderr, "PCX, Out of memory.\n");
        return false;
    }

    memset(data, 0, datasize);

    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;

    // fill in file header
    *dstbuf++ = 0x0A;           // PCX Id Number (Always 0x0A)
    *dstbuf++ = 5;          // Version Number
    *dstbuf++ = 1;          // Encoding Format
    *dstbuf++ = (srcdepth == 24) ? 8 : srcdepth;          // Bits Per Pixel
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Left of image
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Top of image
    WriteU16ToLE(dstbuf, srcxsize-1);   dstbuf += 2;            // Right of image
    WriteU16ToLE(dstbuf, srcysize-1);   dstbuf += 2;            // Bottom of image
    WriteU16ToLE(dstbuf, 72);           dstbuf += 2;            // Horizontal resolution (h-dpi) 
    WriteU16ToLE(dstbuf, 72);           dstbuf += 2;            // Vertical resolution (v-dpi)
    uint8_t* egaptr = dstbuf;           // set pointer to 16-Color EGA Palette
    dstbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)
    *dstbuf++ = srcbytes;            // Number of Bit Planes
    WriteU16ToLE(dstbuf, dstpitch);     dstbuf += 2;            // Bytes per scan-line (must be an even number)    
    WriteU16ToLE(dstbuf, palettetype);  dstbuf += 2;            // Palette Type
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Horizontal Screen Size
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Vertical Screen Size
    dstbuf += 54;           // Reserved2 (Always 0)

    uint32_t bytesencoded = 0;
    uint32_t colorplane = 0;
    uint32_t rlecount = 0;
    uint32_t runcount = 0;
    uint32_t maxcount = 0x3F;
    uint32_t maxbound = xsize - 0x3F;
    uint32_t x = 0;
    uint32_t y = 0;
    uint8_t sample = 0;

    while (y < ysize) {
        colorplane = 0;
        do {
            bufptr = rowptr;
            x = 0;
            while (x < xsize) {
                sample = *(bufptr + ((x * srcbytes) + colorplane));
                buffer =  (bufptr + ((x * srcbytes) + colorplane));
                maxcount = 0x3F;
                if (x > maxbound) { maxcount = xsize - x; };
                rlecount = ParseRLE(&buffer, srcbytes, maxcount);
                runcount = rlecount;
                if (rlecount == 1) {
                    if ((0xC0 != (0xC0 & sample))) {
                        *dstbuf++ = sample;
                        bytesencoded++;
                    } else {
                        *dstbuf++ = runcount | 0xC0;
                        *dstbuf++ = sample;
                        bytesencoded++;
                        bytesencoded++;
                    }
                } else {
                    *dstbuf++ = runcount | 0xC0;
                    *dstbuf++ = sample;
                    bytesencoded++;
                    bytesencoded++;
                }
                x += runcount;
            }
            dstbuf += dstpadbytes;
            bytesencoded += dstpadbytes;
            colorplane++;
        } while (colorplane < srcbytes);
        rowptr += srcpitch;
        y++;
    }

    // palette
    if (srcbytes == 1) {

        uint32_t palnum = 0;
        rgba_t* palptr = NULL;

        switch (srcdepth)
        {
        case 1:
        case 2:
        case 4:
        {
            dstbuf = egaptr;            // 16-Color EGA Palette
            palnum = 16;
            palptr = (ppalette == NULL) ? ega_palette : ppalette->data;
        } break;
        case 8:
        {
            *dstbuf++ = 0x0C;           // 256-Color Palette Code
            bytesencoded++;
            palnum = 256;
            palptr = ppalette->data;
        } break;
        }

        if (ppalette != NULL) {
            for (uint32_t i = 0; i < palnum; ++i)
            {
                *dstbuf++ = palptr[i].r;
                *dstbuf++ = palptr[i].g;
                *dstbuf++ = palptr[i].b;
            }
            bytesencoded += dstpalettesize;
        }
    }

    *ppdstsize = s_pcx_v5_info_size + bytesencoded;
    *ppdst = (uint8_t*)realloc(data, *ppdstsize + 1 & ~1);

    return true;
}

//-----------------------------------------------------------------------------
// GetInfoPCX
//-----------------------------------------------------------------------------
static bool
GetInfoFromMemoryPCX(uint32_t* srcxsize, uint32_t* srcysize, uint32_t* srcdepth,
    uint8_t* psrcbuf, uint32_t psrcsize)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, s_pcx_v5_info_size);

    uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;

    uint8_t  identifier      = *srcbuf++;
    uint8_t  version         = *srcbuf++;
    uint8_t  encoding        = *srcbuf++;
    uint8_t  bitsPerPixel    = *srcbuf++;
    uint16_t xMin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t yMin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t xMax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t yMax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t horzRes         = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t vertRes         = ReadU16FromLE(srcbuf); srcbuf += 2;
    srcbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)
    uint8_t  numBitPlanes    = *srcbuf++;
    uint16_t bytesPerLine    = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t paletteType     = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t horzScreenSize  = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t vertScreenSize  = ReadU16FromLE(srcbuf); srcbuf += 2;
    srcbuf += 54;           // Reserved2 (Always 0)

    if (identifier != 0x0A) {
        return false;
    }

    if (encoding != 1) {
        return false;
    }

    if (version != 0 && version != 2 && version != 3 && version != 4 && version != 5) {
        return false;
    }

    if (bitsPerPixel != 1 && bitsPerPixel != 2 && bitsPerPixel != 4 &&
        bitsPerPixel != 8) {
        return false;
    }

    if (srcxsize != NULL) { *srcxsize = (xMax - xMin) + 1; }
    if (srcysize != NULL) { *srcysize = (yMax - yMin) + 1; }
    if (srcdepth != NULL) { *srcdepth = (bitsPerPixel * numBitPlanes); }

    return true;
}

//-----------------------------------------------------------------------------
// LoadPCX
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryPCX(uint8_t** ppdst, palette_t* ppalette, uint8_t* psrcbuf,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint32_t* srcdepth)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, s_pcx_v5_info_size);

    uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    uint8_t* srcend = psrcbuf + psrcsize;

    uint8_t  identifier      = *srcbuf++;
    uint8_t  version         = *srcbuf++;
    uint8_t  encoding        = *srcbuf++;
    uint8_t  bitsperpixel    = *srcbuf++;
    uint16_t xmin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t ymin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t xmax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t ymax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t horzres         = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t vertres         = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint8_t* egaptr = srcbuf;           // set pointer to 16-Color EGA Palette
    srcbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)
    uint8_t  numbitplanes    = *srcbuf++;
    uint16_t bytesperline    = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t palettetype     = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t horzscreensize  = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t vertscreensize  = ReadU16FromLE(srcbuf); srcbuf += 2;
    srcbuf += 54;           // Reserved2 (Always 0)

    if (identifier != 0x0A) {
        fprintf(stderr, "PCX, Invalid manufacturer code: %d.\n", identifier);
        return false;
    }

    if (encoding != 1) {
        fprintf(stderr, "PCX, Invalid encoding value: %d.\n", encoding);
        return false;
    }

    if (version != 0 && version != 2 && version != 3 && version != 4 && version != 5) {
        fprintf(stderr, "PCX, Version mismatch: %d.\n", version);
        return false;
    }

    if (bitsperpixel != 1 && bitsperpixel != 2 && bitsperpixel != 4 &&
        bitsperpixel != 8) {
        fprintf(stderr, "PCX, Unsupported depth: %d.\n", bitsperpixel);
        return false;
    }

    uint32_t xsize = (xmax - xmin) + 1;
    uint32_t ysize = (ymax - ymin) + 1;
    uint32_t widthinbytes = bytesperline;
    uint32_t pitch = WidthInBytes(xsize, bitsperpixel);
    uint32_t padbytes = widthinbytes - pitch;
    uint32_t totalbytes = numbitplanes * pitch;
    uint32_t rlecount = 0;
    uint32_t subtotal = 0;
    uint32_t colorplane = 0;

    INLINE_OBJECT_NULL_CHK(ppdst);

    uint8_t* pixels = (uint8_t*)malloc(ysize * pitch);
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixels;
    uint32_t y = 0;

    if (pixels == NULL) {
        fprintf(stderr, "PCX, Out of memory.\n");
        return false;
    }

    memset(pixels, 0, ysize * pitch);

    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = bitsperpixel * numbitplanes; }

    while (y < ysize) {
        pixbuf = pixptr;
        colorplane = 1;
        subtotal = 0;
        do {
            rlecount = 1;
            if ((0xC0 & *srcbuf) == 0xC0) {
                rlecount = (*srcbuf++ & 0x3F);
            }
            subtotal += rlecount;
            while (rlecount--) {
                *pixbuf = *srcbuf;
                pixbuf += numbitplanes;
            }
            if (subtotal % pitch == 0) {
                srcbuf += padbytes;
                pixbuf = pixptr + colorplane++;
            }
            srcbuf++;
        } while (subtotal < totalbytes);
        pixptr += pitch;
        y++;
    }

    // palette
    if (numbitplanes == 1) {
        uint32_t palnum = 0;
        uint8_t* palptr = NULL;
        switch (bitsperpixel)
        {
        case 1:
        case 2:
        case 4:
        {
            palnum = 16;
            palptr = egaptr;
        } break;
        case 8:
        {
            palnum = 256;
            palptr = srcend - 768;
        } break;
        }
        if (ppalette != NULL) {
            for (uint32_t i = 0; i < palnum; ++i)
            {
                ppalette->data[i].r = *palptr++;
                ppalette->data[i].g = *palptr++;
                ppalette->data[i].b = *palptr++;
                ppalette->data[i].a = 255;
            }
            ppalette->size = palnum;
            ppalette->bits = 24;
        }
    }

    return true;
}

#endif // #ifndef _PCX_H

//-----------------------------------------------------------------------------
// FastFill
//-----------------------------------------------------------------------------
static void
FastFill(image_t* pimage, palette_t* ppalette, rgba_t newcolor, rgba_t oldcolor)
{
    uint8_t* bufdst = NULL;
    uint32_t bytes = 1;
    uint32_t dc = 0;
    uint32_t sc = 0;
    uint32_t pc = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    if (pimage == NULL || pimage->pixels == NULL) {
        return;
    }

    switch (pimage->format)
    {
    case GEUL_RGBA:
    {
        dc = (newcolor.a << 24) + (newcolor.b << 16) + (newcolor.g << 8) + newcolor.r;
        sc = (oldcolor.a << 24) + (oldcolor.b << 16) + (oldcolor.g << 8) + oldcolor.r;
    } break;
    case GEUL_BGRA:
    {
        dc = (newcolor.a << 24) + (newcolor.r << 16) + (newcolor.g << 8) + newcolor.b;
        sc = (oldcolor.a << 24) + (oldcolor.r << 16) + (oldcolor.g << 8) + oldcolor.b;
    } break;
    case GEUL_RGB:
    {
        dc = (    0 << 24) + (newcolor.b << 16) + (newcolor.g << 8) + newcolor.r;
        sc = (    0 << 24) + (oldcolor.b << 16) + (oldcolor.g << 8) + oldcolor.r;
    } break;
    case GEUL_BGR:
    {
        dc = (    0 << 24) + (newcolor.r << 16) + (newcolor.g << 8) + newcolor.b;
        sc = (    0 << 24) + (oldcolor.r << 16) + (oldcolor.g << 8) + oldcolor.b;
    } break;
    case GEUL_LUMINANCE_ALPHA:
    {
        // FIXME: possible rounding error?
        dc = (newcolor.a << 8) + (uint8_t)(newcolor.r * 0.2990f +
            newcolor.g * 0.5870f + newcolor.b * 0.1140f);
        sc = (newcolor.a << 8) + (uint8_t)(oldcolor.r * 0.2990f +
            oldcolor.g * 0.5870f + oldcolor.b * 0.1140f);
    } break;
    case GEUL_LUMINANCE:
    {
        // FIXME: possible rounding error?
        dc = (    0 << 8) + (uint8_t)(newcolor.r * 0.2990f +
            newcolor.g * 0.5870f + newcolor.b * 0.1140f);
        sc = (    0 << 8) + (uint8_t)(oldcolor.r * 0.2990f +
            oldcolor.g * 0.5870f + oldcolor.b * 0.1140f);
    } break;
    case GEUL_COLOUR_INDEX:
    {
        int32_t dstindex = -1;
        int32_t srcindex = -1;

        if (ppalette == NULL) {
            return;
        }

        for (uint32_t i = 0; i < ppalette->size; ++i)
        {
            if (ppalette->data[i].r == newcolor.r &&
                ppalette->data[i].g == newcolor.g &&
                ppalette->data[i].b == newcolor.b &&
                ppalette->data[i].a == newcolor.a) {
                dstindex = i;
                break;
            }
        }

        for (uint32_t i = 0; i < ppalette->size; ++i)
        {
            if (ppalette->data[i].r == oldcolor.r &&
                ppalette->data[i].g == oldcolor.g &&
                ppalette->data[i].b == oldcolor.b &&
                ppalette->data[i].a == oldcolor.a) {
                srcindex = i;
                break;
            }
        }

        if (dstindex == -1 || srcindex == -1) {
            return;
        }

        dc = dstindex;
        sc = srcindex;
    } break;
    }

    bufdst = pimage->pixels;
    bytes = GetBytesForPixelFormat(pimage->format);

    while (y < pimage->height)
    {
        x = (pimage->width + 3) / 4;
        switch (pimage->width % 4)
        {
        case 0: do {
                memcpy(&pc, bufdst, bytes);
                if (pc == sc) memset(bufdst, dc, bytes);
                bufdst += bytes;
        case 3:
                memcpy(&pc, bufdst, bytes);
                if (pc == sc) memset(bufdst, dc, bytes);
                bufdst += bytes;
        case 2:
                memcpy(&pc, bufdst, bytes);
                if (pc == sc) memset(bufdst, dc, bytes);
                bufdst += bytes;
        case 1:
                memcpy(&pc, bufdst, bytes);
                if (pc == sc) memset(bufdst, dc, bytes);
                bufdst += bytes;
        } while (--x > 0);
        }
        y++;
    }
}

//-----------------------------------------------------------------------------
// SaveImageToMemory
//-----------------------------------------------------------------------------
bool
SaveImageToMemory(uint8_t** ppdst, uint32_t* ppdstsize, uint32_t fileformat,
    uint32_t codec, image_t* pimage, palette_t* ppalette)
{
    uint32_t depth = 8;
    uint32_t pixelsize = 0;
    uint32_t format = 0;
    bool result = false;

    if (pimage != NULL) {
        // supported output formats
        switch (fileformat)
        {
        case GEUL_PNG:
        {
            if (pimage->format == GEUL_RGBA) {
                format = GEUL_RGBA;
            } else if (pimage->format == GEUL_RGB) {
                format = GEUL_RGB;
            } else if (pimage->format == GEUL_LUMINANCE_ALPHA) {
                format = GEUL_LUMINANCE_ALPHA;
            } else if (pimage->format == GEUL_LUMINANCE) {
                format = GEUL_LUMINANCE;
            } else if (pimage->format == GEUL_COLOUR_INDEX) {
                format = GEUL_COLOUR_INDEX;
            } else {
                fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        save src data to dst format.\n");
                return false;
            }
        } break;
        case GEUL_BMP:
        {
            if (pimage->format == GEUL_BGRA) {
                format = GEUL_BGRA;
            } else if (pimage->format == GEUL_BGR) {
                format = GEUL_BGR;
            } else if (pimage->format == GEUL_COLOUR_INDEX) {
                format = GEUL_COLOUR_INDEX;
            } else {
                fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        save src data to dst format.\n");
                return false;
            }
        } break;
        case GEUL_PCX:
        {
            if (pimage->format == GEUL_RGB) {
                format = GEUL_RGB;
            } else if (pimage->format == GEUL_COLOUR_INDEX) {
                format = GEUL_COLOUR_INDEX;
            } else {
                fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        save src data to dst format.\n");
                return false;
            }
        } break;
        case GEUL_TGA:
        {
            if (pimage->format == GEUL_BGRA) {
                format = GEUL_BGRA;
            } else if (pimage->format == GEUL_BGR) {
                format = GEUL_BGR;
            } else if (pimage->format == GEUL_LUMINANCE) {
                format = GEUL_LUMINANCE;
            } else if (pimage->format == GEUL_COLOUR_INDEX) {
                format = GEUL_COLOUR_INDEX;
            } else {
                fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        save src data to dst format.\n");
                return false;
            }
        } break;
        default:
        {
            fprintf(stderr, "SaveImage, Unsupported image format.\n");
            return false;
        }
        }
        depth *= GetBytesForPixelFormat(format);
        pixelsize = pimage->width * pimage->height *
            GetBytesForPixelFormat(pimage->format);
        if (pimage->format != GEUL_COLOUR_INDEX) {
            ppalette = NULL;
        }
        switch (fileformat)
        {
        case GEUL_PNG:
        {
            result = SaveToMemoryPNG(ppdst, ppdstsize, codec, pimage->pixels,
                pixelsize, pimage->width, pimage->height, depth, 8,
                ppalette, NULL);
        } break;
        case GEUL_BMP:
        {
            result = SaveToMemoryBMP(ppdst, ppdstsize, codec, pimage->pixels,
                pixelsize, pimage->width, pimage->height, depth,
                ppalette, NULL, false);
        } break;
        case GEUL_PCX:
        {
            result = SaveToMemoryPCX(ppdst, ppdstsize, codec, pimage->pixels,
                pixelsize, pimage->width, pimage->height, depth,
                ppalette);
        } break;
        case GEUL_TGA:
        {
            result = SaveToMemoryTGA(ppdst, ppdstsize, codec, pimage->pixels,
                pixelsize, pimage->width, pimage->height, depth,
                ppalette, false,
                false);
        } break;
        }
        if (pimage->pixels != NULL) {
            free(pimage->pixels);
            pimage->pixels = NULL;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
// SaveImageToFile
//-----------------------------------------------------------------------------
bool
SaveImageToFile(const char* pdstfile, uint32_t dstformat, uint32_t dstcodec,
    image_t* psrcimage, palette_t* psrcpalette)
{
    bool result = false;

    if (pdstfile == NULL || strlen(pdstfile) == 0) {
        fprintf(stderr, "SaveImage, Not a valid filename.\n");
        return result;
    }

    // open file
    FILE* hFile = NULL;
    
    fopen_s(&hFile, pdstfile, "wb");

    uint32_t dstsize = 0;
    uint8_t* rawdst = NULL;
    void* dstbuf = rawdst;

    if (hFile != NULL) {
        // save image
        result = SaveImageToMemory(&rawdst, &dstsize, dstformat, dstcodec,
            psrcimage, psrcpalette);
    } else {
        fprintf(stderr, "SaveImage, Couldn't open file %s.\n", pdstfile);
    }

    size_t fileSize = dstsize;

    // file write
    size_t bytesWritten = 0;

    if (hFile != NULL && dstbuf != NULL && fileSize != 0) {
        bytesWritten = fwrite(dstbuf, sizeof(uint8_t), fileSize, hFile);
        fclose(hFile);
        hFile = NULL;
    }

    if (dstbuf != NULL) {
        free(dstbuf);
        dstbuf = NULL;
    }

    rawdst = NULL;

    return result;
}

//-----------------------------------------------------------------------------
// GetImageInfoFromMemory
//-----------------------------------------------------------------------------
bool
GetImageInfoFromMemory(image_t* psrcinfo, uint8_t* psrcbuf, uint32_t psrcsize)
{
    uint32_t depth = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t format = 0;
    uint32_t type = GEUL_UNSIGNED_BYTE;
    uint8_t colormap = 0;
    bool result = false;

    INLINE_OBJECT_NULL_CHK(psrcinfo);

    if ((result = GetInfoFromMemoryPNG(&colormap, &width, &height, &depth, NULL,
        psrcbuf, psrcsize)) == true) {
        if (depth == 32) { format = GEUL_RGBA; }
        else if (depth == 24) { format = GEUL_RGB; }
        else if (depth == 16) { format = GEUL_LUMINANCE_ALPHA; }
        else if (depth <=  8 && colormap == 0) { format = GEUL_LUMINANCE; }
        else if (depth <=  8 && colormap == 1) { format = GEUL_COLOUR_INDEX;
            type = GEUL_BITMAP; }
    } else if ((result = GetInfoFromMemoryBMP(&width, &height, &depth, psrcbuf,
        psrcsize)) == true) {
        if (depth == 32) { format = GEUL_BGRA; }
        else if (depth == 24) { format = GEUL_BGR; }
        else if (depth <=  8 && colormap == 1) { format = GEUL_COLOUR_INDEX;
            type = GEUL_BITMAP; }
    } else if ((result = GetInfoFromMemoryPCX(&width, &height, &depth, psrcbuf,
        psrcsize)) == true) {
        if (depth <= 8) { format = GEUL_COLOUR_INDEX; type = GEUL_BITMAP; }
        else { format = GEUL_RGB; }
    } else if ((result = GetInfoFromMemoryTGA(&colormap, &width, &height, &depth,
        psrcbuf, psrcsize)) == true) {
        if (depth == 32) { format = GEUL_BGRA; }
        else if (depth == 24) { format = GEUL_BGR; }
        else if (depth == 16) { format = GEUL_BGR; }
        else if (depth ==  8 && colormap == 0) { format = GEUL_LUMINANCE; }
        else if (depth ==  8 && colormap == 1) { format = GEUL_COLOUR_INDEX;
            type = GEUL_BITMAP; }
    } else { fprintf(stderr, "GetImageInfo, Unsupported image format\n"); }

    if (result == true) {
        psrcinfo->depth = depth;
        psrcinfo->width = width;
        psrcinfo->height = height;
        psrcinfo->format = format;
        psrcinfo->type = type;
    }

    return result;
}

//-----------------------------------------------------------------------------
// GetImageInfoFromFile
//-----------------------------------------------------------------------------
bool
GetImageInfoFromFile(image_t* psrcinfo, const char* psrcfile)
{
    bool result = false;

    if (psrcfile == NULL || strlen(psrcfile) == 0) {
        fprintf(stderr, "GetImageInfo, Not a valid filename.\n");
        return result;
    }

    // open file
    FILE* hFile = NULL;

    fopen_s(&hFile, psrcfile, "rb");
    
    long end = 0;
    long pos = 0;

    // file size
    if (hFile != NULL) {
        pos = ftell(hFile);
        fseek(hFile,  0L, SEEK_END);
        end = ftell(hFile);
        fseek(hFile, pos, SEEK_SET);
    }
    
    size_t fileSize = (end - pos);

    // allocate memory
    uint8_t* rawsrc = NULL;
    void* srcbuf = rawsrc;

    if (fileSize != 0) {
        srcbuf = malloc(((fileSize + 1) & ~1));
        if (srcbuf == NULL) {
            fprintf(stderr, "GetImageInfo, Out of memory.\n");
            return result;
        }
        memset(srcbuf, 0, ((fileSize + 1) & ~1));
    }

    rawsrc = (uint8_t*)srcbuf;

    // file read
    size_t bytesRead = 0;
    if (hFile != NULL && fileSize != 0) {
        bytesRead = fread(srcbuf, sizeof(uint8_t), fileSize, hFile);
        fclose(hFile);
        hFile = NULL;
    }

    if (fileSize == bytesRead && bytesRead != 0) {
        // image info
        result = GetImageInfoFromMemory(psrcinfo, rawsrc, (uint32_t)bytesRead);
        free(srcbuf);
        srcbuf = NULL;
        rawsrc = NULL;
    }

    return result;
}

//-----------------------------------------------------------------------------
// LoadImageFromMemory
//-----------------------------------------------------------------------------
bool
LoadImageFromMemory(image_t* pdstimage, palette_t* pdstpalette, rgba_t colorkey,
    uint8_t* psrcbuf, uint32_t srcsize)
{
    palette_t srcpalette = { 0 };
    uint8_t* pixels = NULL;
    uint32_t depth = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t format = 0;
    uint32_t type = GEUL_UNSIGNED_BYTE;
    uint32_t fileformat = 0;
    bool result = false;

    if (pdstimage != NULL) {
        rgba_t pngcolorkey = { 0, 0, 0, 0 };
        if ((result = LoadFromMemoryPNG(&pixels, &srcpalette, psrcbuf, srcsize,
            &width, &height, &depth, NULL, &pngcolorkey)) == true) {
            if (depth == 32) { format = GEUL_RGBA; }
            else if (depth == 24) { format = GEUL_RGB; }
            else if (depth == 16) { format = GEUL_LUMINANCE_ALPHA; }
            else if (depth <= 8 && srcpalette.size == 0) { format = GEUL_LUMINANCE; }
            else if (depth <= 8 && srcpalette.size != 0) { format = GEUL_COLOUR_INDEX;
                type = GEUL_BITMAP; }
            if (pngcolorkey.b != 0 && pngcolorkey.g != 0 && pngcolorkey.r != 0 &&
                pngcolorkey.a != 0) { colorkey = pngcolorkey; }
            fileformat = GEUL_PNG;
        } else if ((result = LoadFromMemoryBMP(&pixels, &srcpalette, psrcbuf, srcsize,
            &width, &height, &depth)) == true) {
            if (depth == 32) { format = GEUL_BGRA; }
            else if (depth == 24) { format = GEUL_BGR; }
            else if (depth <= 8 && srcpalette.size != 0) { format = GEUL_COLOUR_INDEX;
            type = GEUL_BITMAP; }
            fileformat = GEUL_BMP;
        } else if ((result = LoadFromMemoryPCX(&pixels, &srcpalette, psrcbuf, srcsize,
            &width, &height, &depth)) == true) {
            if (depth <= 8 && srcpalette.size != 0) { format = GEUL_COLOUR_INDEX;
            type = GEUL_BITMAP; }
            else { format = GEUL_RGB; }
            fileformat = GEUL_PCX;
        } else if ((result = LoadFromMemoryTGA(&pixels, &srcpalette, psrcbuf, srcsize,
            &width, &height, &depth)) == true) {
            if (depth == 32) { format = GEUL_BGRA; }
            else if (depth == 24) { format = GEUL_BGR; }
            else if (depth == 16) { format = GEUL_BGRA; }
            else if (depth == 8 && srcpalette.size == 0) { format = GEUL_LUMINANCE; }
            else if (depth == 8 && srcpalette.size != 0) { format = GEUL_COLOUR_INDEX;
                type = GEUL_BITMAP; }
            fileformat = GEUL_TGA;
        } else { fprintf(stderr, "LoadImage, Unsupported image format\n"); }

        // unpack types
        if (fileformat == GEUL_TGA && depth == 16 && result == true) {
            uint8_t* buffer = (uint8_t*)malloc(width * height * 3);
            uint8_t* pixbuf = buffer;
            uint8_t* srcbuf = pixels;
            uint32_t srcpitch = width * 2;
            uint32_t x = 0;
            uint32_t y = 0;
            uint32_t bshift = 0;
            uint32_t gshift = 5;
            uint32_t rshift = 10;
            uint32_t ashift = 15;
            uint32_t bmask = 0x001F;
            uint32_t gmask = 0x03E0;
            uint32_t rmask = 0x7C00;
            //uint32_t amask = 0x8000;
            float color = 255.0f / 31.0f;
            //float alpha = 255.0f;
            uint16_t pixel = 0;
            memset(buffer, 0, width * height * 3);
            while (y++ < height) {
                x = 0;
                while (x < width) {
                    memcpy(&pixel, (srcbuf + (x * 2)), 2);
                    *pixbuf++ = ((pixel & bmask) >> bshift) * color;
                    *pixbuf++ = ((pixel & gmask) >> gshift) * color;
                    *pixbuf++ = ((pixel & rmask) >> rshift) * color;
                    //*pixbuf++ = ((pixel & amask) >> ashift) * alpha;
                    x++;
                }
                srcbuf += srcpitch;
            }
            free(pixels);
            pixels = buffer;
            depth = 24;
            format = GEUL_BGR;
        }

        if ((fileformat == GEUL_PNG || fileformat == GEUL_BMP ||
             fileformat == GEUL_PCX) && depth <= 4 && result == true) {
            uint8_t* buffer = (uint8_t*)malloc(width * height);
            uint8_t* pixbuf = buffer;
            uint8_t* srcbuf = pixels;
            uint32_t x = 0;
            uint32_t y = 0;
            uint8_t pixelsperbyte =  8 / depth;
            uint8_t maskbits[4] = { 0x01, 0x03, 0, 0x0F };
            uint8_t scalebit[4] = { 0xFF, 0x55, 0, 0x11 };
            uint8_t mask = maskbits[depth-1];
            uint8_t resetbit = 8 - depth;
            uint8_t bit = resetbit;

            memset(buffer, 0, width * height);

            while (y < height) {
                x = (width / pixelsperbyte);
                do {
                    switch (pixelsperbyte)
                    {
                    case 8: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                    case 7: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                    case 6: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                    case 5: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                    case 4: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                    case 3: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                    case 2: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                    case 1: *pixbuf++ = (*srcbuf >> bit) & mask; bit = resetbit;
                        srcbuf++;
                    }
                } while (--x > 0);
                switch (width & (pixelsperbyte - 1))
                {
                case 7: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                case 6: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                case 5: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                case 4: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                case 3: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                case 2: *pixbuf++ = (*srcbuf >> bit) & mask; bit -= depth;
                case 1: *pixbuf++ = (*srcbuf >> bit) & mask; bit = resetbit;
                    srcbuf++;
                }
                y++;
            }
            pixbuf = buffer;
            if (format == GEUL_LUMINANCE) {
                for (unsigned int i = 0; i < width * height; ++i)
                {
                    *pixbuf++ *= scalebit[depth-1];
                }
            }
            free(pixels);
            pixels = buffer;
            depth = 8;
        }

        // palette
        if (pdstpalette != NULL && format == GEUL_COLOUR_INDEX && result == true) {
            for (uint32_t i = 0; i < srcpalette.size; ++i)
            {
                pdstpalette->data[i] = srcpalette.data[i];
            }
            pdstpalette->size = srcpalette.size;
            pdstpalette->bits = srcpalette.bits;
        }

        // transparency
        if (result == true) {
            if (colorkey.b != 0 || colorkey.g != 0 || colorkey.r != 0 ||
                colorkey.a != 0) {
                const rgba_t transparent_black = { 0, 0, 0, 0 };
                FastFill(pdstimage, &srcpalette, transparent_black, colorkey);
            }
        }

        // dst stuff
        if (result == true) {
            pdstimage->pixels = pixels;
            pdstimage->depth = depth;
            pdstimage->width = width;
            pdstimage->height = height;
            pdstimage->format = format;
            pdstimage->type = type;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
// LoadImageFromFile
//-----------------------------------------------------------------------------
bool
LoadImageFromFile(image_t* pdstimage, palette_t* pdstpalette, rgba_t colorkey,
    const char* psrcfile)
{
    bool result = false;

    if (psrcfile == NULL || strlen(psrcfile) == 0) {
        return result;
    }

    // open file
    FILE* hFile = NULL;
    
    fopen_s(&hFile, psrcfile, "rb");
    
    long end = 0;
    long pos = 0;

    // file size
    if (hFile != NULL) {
        pos = ftell(hFile);
        fseek(hFile,  0L, SEEK_END);
        end = ftell(hFile);
        fseek(hFile, pos, SEEK_SET);
    } else {
        fprintf(stderr, "LoadImage, Couldn't open file %s.\n", psrcfile);
    }
    
    size_t fileSize = (end - pos);

    // allocate memory
    uint8_t* rawsrc = NULL;
    void* srcbuf = rawsrc;

    if (fileSize != 0) {
        srcbuf = malloc(((fileSize + 1) & ~1));
        if (srcbuf == NULL) {
            fprintf(stderr, "LoadImage, Out of memory.\n");
            return result;
        }
        memset(srcbuf, 0, ((fileSize + 1) & ~1));
    }

    rawsrc = (uint8_t*)srcbuf;

    // file read
    size_t bytesRead = 0;
    if (hFile != NULL && fileSize != 0) {
        bytesRead = fread(srcbuf, sizeof(uint8_t), fileSize, hFile);
        fclose(hFile);
        hFile = NULL;
    }

    if (fileSize == bytesRead && bytesRead != 0) {
        // load image
        result = LoadImageFromMemory(pdstimage, pdstpalette, colorkey, rawsrc,
            (uint32_t)bytesRead);
        free(srcbuf);
        srcbuf = NULL;
        rawsrc = NULL;
    }

    return result;
}

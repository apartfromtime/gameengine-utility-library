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

#define GEUL_CAST_TO_U16(x) (x & 0xFFFF)
#define GEUL_CAST_TO_U8(x) (x & 0xFF)
#define GEUL_FLT_TO_U8(x) ((uint8_t)((uint32_t)(x)))

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

static const uint8_t s_png_id[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A,
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
// ShrinkIndexInterlaced
//------------------------------------------------------------------------------
static void
ShrinkIndexInterlacedPNG(uint8_t* pdst, uint32_t* pdstlen, uint32_t width,
    uint32_t height, uint8_t depth, uint8_t filtertype, uint8_t* psrc)
{
    filtertype = 0;         // quiet compiler warning
    uint8_t* buffer = pdst;
    uint8_t* pixels = psrc;
    //uint32_t bytes = (depth + 7) >> 3;
    uint32_t pitch = WidthInBytes(width, depth);
    uint32_t xskip = 1;
    uint32_t yskip = 1;
    uint32_t xorig = 0;
    uint32_t yorig = 0;
    uint32_t w = width;
    uint32_t h = height;
    uint32_t x = 0;
    uint32_t y = 0;
    int32_t x0 = 0;
    uint32_t pass = 0;
    uint8_t maskbits[4] = { 0x01, 0x03, 0, 0x0F };
    uint8_t mask = maskbits[depth-1];
    uint16_t pix0 = 0;
    uint16_t pix1 = 0;
    uint8_t filter = 0;
    uint8_t bit0 = 0;
    uint8_t bit1 = 0;
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixptr;
    uint8_t ppb = (depth == 1 ? 3 : depth == 2 ? 2 : 1);
    uint8_t bpp = (depth == 1 ? 0 : depth == 2 ? 1 : 2);
    uint8_t mod = (8 / depth) - 1;
    uint8_t cur0 = 0;
    uint8_t cur1 = 0;
    const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
    const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
    const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
    const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };
    do {
        w = (width  - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
        h = (height - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];
        xskip = i_xextent[pass];
        yskip = i_yextent[pass];
        xorig = i_xorigin[pass];
        yorig = i_yorigin[pass];
        pix1 = 0;
        y = 0;
        while (y < h) {
            *buffer++ = filter;
            pixptr = pixels + (((y * yskip) + yorig) * pitch);
            pixbuf = pixptr;
            x = 0;
            while (x < w) {
                x0 = (((x - 0) * xskip) + xorig);
                cur0 = ( x & mod);
                cur1 = (x0 & mod);
                bit0 = (mod - cur0) << bpp;
                bit1 = (mod - cur1) << bpp;
                pix0 |= ((*(pixbuf + (x0 >> ppb)) >> bit1) & mask) << bit0;
                if (cur0 == mod) { *buffer++ = (pix0 - pix1 & 0xFF); pix0 = 0; }
                x++;
            }
            y++;
        }
    } while (++pass < 7);
    if (pdstlen != NULL) { *pdstlen = (uint32_t)(buffer - pdst); }
}

//------------------------------------------------------------------------------
// ShrinkInterlaced
//------------------------------------------------------------------------------
static void
ShrinkInterlacedPNG(uint8_t* pdst, uint32_t* pdstlen, uint32_t width,
    uint32_t height, uint8_t depth, uint8_t filtertype, uint8_t* psrc)
{
    uint8_t* buffer = pdst;
    uint8_t* pixels = psrc;
    //float pixelsperbyte = (8.0f / (float)depth);
    uint32_t bytes = (depth + 7) >> 3;
    uint32_t pitch = WidthInBytes(width, depth);
    uint32_t xskip = 1;
    uint32_t yskip = 1;
    uint32_t xorig = 0;
    uint32_t yorig = 0;
    uint32_t w = width;
    uint32_t h = height;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t pass = 0;
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
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixptr;
    uint8_t latch = 0;
    uint8_t* filter0 = &latch;
    uint8_t* filter1 = &latch;
    uint8_t* filter2 = &latch;
    uint8_t* pfilter1 = NULL;
    uint8_t increment = 1;
    uint8_t xinc = 0;
    uint8_t yinc = 0;
    const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
    const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
    const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
    const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };
    filtertype = (filtertype < PNG_FILTER_COUNT) ? filtertype : 0;
    do {
        w = (width  - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
        h = (height - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];
        xskip = i_xextent[pass];
        yskip = i_yextent[pass];
        xorig = i_xorigin[pass];
        yorig = i_yorigin[pass];
        filter1 = &latch;
        yinc = 0;
        y = 0;
        while (y < h) {
            pixptr = pixels + ((((y - 0) * yskip) + yorig) * pitch);
            pfilter1 = filter1;
            filter = (filtertype >= PNG_FILTER_COUNT) ? 0 : filtertype;
            if (filtertype == PNG_FILTER_ADAPTIVE) {
                uint32_t sum[2] = { UINT32_MAX };
                for (uint8_t f = PNG_FILTER_NONE; f < PNG_FILTER_COUNT - 1; ++f) {
                    pixbuf = pixptr;
                    sum[1] = 0;
                    x = 0;
                    switch (f)
                    {
                    case 0:
                    {
                        pix1 = 0;
                        pixbuf += xorig * bytes;
                        while (x++ < w) {
                            bpp = 0;
                            while (bpp++ < bytes) { sum[1] += (*pixbuf++ - pix1) & 0xFF; }
                            pixbuf += (xskip - 1) * bytes;
                        }
                    } break;
                    case 1:
                    {
                        filter0 = &latch;
                        xinc = 0;
                        pixbuf += xorig * bytes;
                        while (x++ < w) {
                            bpp = 0;
                            while (bpp++ < bytes) {
                                sum[1] += (*pixbuf++ - *filter0) & 0xFF;
                                filter0 += xinc;
                            }
                            xinc = increment;
                            filter0 = pixbuf - bytes;
                            pixbuf += (xskip - 1) * bytes;
                        }
                    } break;
                    case 2:
                    {
                        pixbuf += xorig * bytes;
                        while (x++ < w) {
                            bpp = 0;
                            while (bpp++ < bytes) {
                                sum[1] += (*pixbuf++ - *filter1) & 0xFF;
                                filter1 += yinc;
                            }
                            filter1 += yinc * (xskip - 1) * bytes;
                            pixbuf += (xskip - 1) * bytes;
                        }
                    } break;
                    case 3:
                    {
                        filter0 = &latch;
                        xinc = 0;
                        pixbuf += xorig * bytes;
                        while (x++ < w) {
                            bpp = 0;
                            while (bpp++ < bytes) {
                                pix1 = ((*filter0 + *filter1) >> 1) & 0xFF;
                                sum[1] += (*pixbuf++ - pix1) & 0xFF;
                                filter0 += xinc;
                                filter1 += yinc;
                            }
                            xinc = increment;
                            filter0 = pixbuf - bytes;
                            filter1 += yinc * (xskip - 1) * bytes;
                            pixbuf += (xskip - 1) * bytes;
                        }
                    } break;
                    case 4:
                    {
                        filter0 = &latch;
                        filter2 = &latch;
                        xinc = 0;
                        pixbuf += xorig * bytes;
                        while (x++ < w) {
                            bpp = 0;
                            while (bpp++ < bytes) {
                                pix0 = *pixbuf++;
                                raw1 = *filter0;
                                pri0 = *filter1;
                                pri1 = *filter2;
                                a = ABS(pri0 - pri1);
                                b = ABS(raw1 - pri1);
                                c = ABS(raw1 + pri0 - (2 * pri1));
                                if (a <= b && a <= c) {
                                    pae0 = raw1 & 0xFF;
                                }
                                else if (b <= c) {
                                    pae0 = pri0 & 0xFF;
                                }
                                else {
                                    pae0 = pri1 & 0xFF;
                                }
                                pix1 = pae0 & 0xFF;
                                sum[1] += ((pix0 - pix1) & 0xFF);
                                filter0 += xinc;
                                filter1 += yinc;
                                filter2 += xinc;
                            }
                            xinc = increment;
                            filter0 = pixbuf - bytes;
                            filter2 = filter1 - bytes;
                            filter1 += yinc * (xskip - 1) * bytes;
                            pixbuf += (xskip - 1) * bytes;
                        }
                    } break;
                    }
                    if (sum[1] < sum[0]) {
                        sum[0] = sum[1];
                        filter = f;
                    }
                }
            }
            *buffer++ = filter;
            filter1 = pfilter1;
            pixbuf = pixptr;
            x = 0;
            switch (filter)
            {
            case 0:
            {
                pix1 = 0;
                pixbuf += xorig * bytes;
                while (x++ < w) {
                    bpp = 0;
                    while (bpp++ < bytes) { *buffer++ = (*pixbuf++ - pix1) & 0xFF; }
                    pixbuf += (xskip - 1) * bytes;
                }
            } break;
            case 1:
            {
                filter0 = &latch;
                xinc = 0;
                pixbuf += xorig * bytes;
                while (x++ < w) {
                    bpp = 0;
                    while (bpp++ < bytes) {
                        *buffer++ = (*pixbuf++ - *filter0) & 0xFF;
                        filter0 += xinc;
                    }
                    xinc = increment;
                    filter0 = pixbuf - bytes;
                    pixbuf += (xskip - 1) * bytes;
                }
            } break;
            case 2:
            {
                pixbuf += xorig * bytes;
                while (x++ < w) {
                    bpp = 0;
                    while (bpp++ < bytes) {
                        *buffer++ = (*pixbuf++ - *filter1) & 0xFF;
                        filter1 += yinc;
                    }
                    filter1 += yinc * (xskip - 1) * bytes;
                    pixbuf += (xskip - 1) * bytes;
                }
            } break;
            case 3:
            {
                filter0 = &latch;
                xinc = 0;
                pixbuf += xorig * bytes;
                while (x++ < w) {
                    bpp = 0;
                    while (bpp++ < bytes) {
                        pix1 = ((*filter0 + *filter1) >> 1) & 0xFF;
                        *buffer++ = (*pixbuf++ - pix1) & 0xFF;
                        filter0 += xinc;
                        filter1 += yinc;
                    }
                    xinc = increment;
                    filter0 = pixbuf - bytes;
                    filter1 += yinc * (xskip - 1) * bytes;
                    pixbuf += (xskip - 1) * bytes;
                }
            } break;
            case 4:
            {
                filter0 = &latch;
                filter2 = &latch;
                xinc = 0;
                pixbuf += xorig * bytes;
                while (x++ < w) {
                    bpp = 0;
                    while (bpp++ < bytes) {
                        pix0 = *pixbuf++;
                        raw1 = *filter0;
                        pri0 = *filter1;
                        pri1 = *filter2;
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
                        *buffer++ = ((pix0 - pix1) & 0xFF);
                        filter0 += xinc;
                        filter1 += yinc;
                        filter2 += xinc;
                    }
                    xinc = increment;
                    filter0 = pixbuf - bytes;
                    filter2 = filter1 - bytes;
                    filter1 += yinc * (xskip - 1) * bytes;
                    pixbuf += (xskip - 1) * bytes;
                }
            } break;
            }
            filter1 = pixptr + xorig * bytes;
            yinc = increment;
            y++;
        }
    } while (++pass < 7);
    if (pdstlen != NULL) { *pdstlen = (uint32_t)(buffer - pdst); }
}

//------------------------------------------------------------------------------
// ShrinkIndex
//------------------------------------------------------------------------------
static void
ShrinkIndexPNG(uint8_t* pdst, uint32_t* pdstlen, uint32_t width, uint32_t height,
    uint8_t depth, uint8_t filtertype, uint8_t* psrc)
{
    depth = 0;          // qiet compiler warning
    filtertype = 0;         // quiet compiler warning
    uint8_t* buffer = pdst;
    uint8_t* pixptr = psrc;
    uint8_t* pixbuf = pixptr;
    uint32_t y = 0;
    uint8_t filter = 0;
    y = 0;
    while (y++ < height) {
        *buffer++ = filter;
        pixbuf = pixptr;
        memcpy(buffer, pixbuf, width);
        buffer += width;
        pixptr += width;
    }
    if (pdstlen != NULL) { *pdstlen = (uint32_t)(buffer - pdst); }
}

//------------------------------------------------------------------------------
// Shrink
//------------------------------------------------------------------------------
static void
ShrinkPNG(uint8_t* pdst, uint32_t* pdstlen, uint32_t width, uint32_t height,
    uint8_t depth, uint8_t filtertype, uint8_t* psrc)
{
    uint8_t* buffer = pdst;
    uint8_t* pixels = psrc;
    uint32_t bytes = (depth + 7) >> 3;
    uint32_t pitch = WidthInBytes(width, depth);
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t pae0 = 0;
    uint16_t raw0 = 0;
    uint16_t raw1 = 0;
    uint16_t pri0 = 0;
    uint16_t pri1 = 0;
    uint16_t bpp = 0;
    uint16_t a = 0;
    uint16_t b = 0;
    uint16_t c = 0;
    uint8_t filter = 0;
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixptr;
    uint8_t latch = 0;
    uint8_t* filter0 = &latch;
    uint8_t* filter1 = &latch;
    uint8_t* filter2 = &latch;
    uint8_t* pfilter1 = NULL;
    uint8_t increment = 1;
    uint8_t xinc = 0;
    uint8_t yinc = 0;
    filtertype = (filtertype < PNG_FILTER_COUNT) ? filtertype : 0;
    y = 0;
    while (y < height) {
        pfilter1 = filter1;
        filter = (filtertype >= PNG_FILTER_COUNT) ? 0 : filtertype;
        if (filtertype == PNG_FILTER_ADAPTIVE) {
            uint32_t sum[2] = { UINT32_MAX };
            for (uint8_t f = PNG_FILTER_NONE; f < PNG_FILTER_COUNT - 1; ++f) {
                sum[1] = 0;
                pixbuf = pixptr;
                x = 0;
                switch (f)
                {
                case 0:
                {
                    while (x++ < width * bytes) { sum[1] += *pixbuf++ & 0xFF; }
                } break;
                case 1:
                {
                    filter0 = &latch;
                    xinc = 0;
                    while (x++ < width) {
                        bpp = 0;
                        while (bpp++ < bytes) {
                            sum[1] += ((*pixbuf++ - *filter0) & 0xFF);
                            filter0 += xinc;
                        }
                        xinc = increment;
                        filter0 = pixbuf - bytes;
                    }
                } break;
                case 2:
                {
                    while (x++ < width) {
                        bpp = 0;
                        while (bpp++ < bytes) {
                            sum[1] += ((*pixbuf++ - *filter1) & 0xFF);
                            filter1 += yinc;
                        }
                    }
                } break;
                case 3:
                {
                    filter0 = &latch;
                    xinc = 0;
                    while (x++ < width) {
                        bpp = 0;
                        while (bpp++ < bytes) {
                            raw0 = ((*filter0 + *filter1) >> 1) & 0xFF;
                            sum[1] += ((*pixbuf++ - raw0) & 0xFF);
                            filter0 += xinc;
                            filter1 += yinc;
                        }
                        xinc = increment;
                        filter0 = pixbuf - bytes;
                    }
                } break;
                case 4:
                {
                    filter0 = &latch;
                    filter2 = &latch;
                    xinc = 0;
                    while (x++ < width) {
                        bpp = 0;
                        while (bpp++ < bytes) {
                            raw1 = *filter0;
                            pri0 = *filter1;
                            pri1 = *filter2;
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
                            raw0 = pae0 & 0xFF;
                            sum[1] += ((*pixbuf++ - raw0) & 0xFF);
                            filter0 += xinc;
                            filter1 += yinc;
                            filter2 += xinc;
                        }
                        xinc = increment;
                        filter0 = pixbuf - bytes;
                        filter2 = filter1 - bytes;
                    }
                } break;
                }
                if (sum[1] < sum[0]) {
                    sum[0] = sum[1];
                    filter = f;
                }
            }
        }
        *buffer++ = filter;
        filter1 = pfilter1;
        pixbuf = pixptr;
        x = 0;
        switch (filter)
        {
        case 0:
        {
            while (x++ < width * bytes) { *buffer++ = *pixbuf++ & 0xFF; }
        } break;
        case 1:
        {
            filter0 = &latch;
            xinc = 0;
            while (x++ < width) {
                bpp = 0;
                while (bpp++ < bytes) {
                    *buffer++ = ((*pixbuf++ - *filter0) & 0xFF);
                    filter0 += xinc;
                }
                xinc = increment;
                filter0 = pixbuf - bytes;
            }
        } break;
        case 2:
        {
            while (x++ < width) {
                bpp = 0;
                while (bpp++ < bytes) {
                    *buffer++ = ((*pixbuf++ - *filter1) & 0xFF);
                    filter1 += yinc;
                }
            }
        } break;
        case 3:
        {
            filter0 = &latch;
            xinc = 0;
            while (x++ < width) {
                bpp = 0;
                while (bpp++ < bytes) {
                    raw0 = ((*filter0 + *filter1) >> 1) & 0xFF;
                    *buffer++ = ((*pixbuf++ - raw0) & 0xFF);
                    filter0 += xinc;
                    filter1 += yinc;
                }
                xinc = increment;
                filter0 = pixbuf - bytes;
            }
        } break;
        case 4:
        {
            filter0 = &latch;
            filter2 = &latch;
            xinc = 0;
            while (x++ < width) {
                bpp = 0;
                while (bpp++ < bytes) {
                    raw1 = *filter0;
                    pri0 = *filter1;
                    pri1 = *filter2;
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
                    raw0 = pae0 & 0xFF;
                    *buffer++ = ((*pixbuf++ - raw0) & 0xFF);
                    filter0 += xinc;
                    filter1 += yinc;
                    filter2 += xinc;
                }
                xinc = increment;
                filter0 = pixbuf - bytes;
                filter2 = filter1 - bytes;
            }
        } break;
        }
        filter1 = pixptr;
        yinc = increment;
        pixptr += pitch;
        y++;
    }
    if (pdstlen != NULL) { *pdstlen = (uint32_t)(buffer - pdst); }
}

//------------------------------------------------------------------------------
// SaveFromMemory
//------------------------------------------------------------------------------
static bool
SaveToMemoryPNG(uint8_t** ppdst, uint32_t* ppdstsize, uint32_t codec,
    uint8_t* psrcbuf, uint32_t psrcsize, uint32_t width, uint32_t height,
    uint8_t depth, uint8_t sampledepth, palette_t* ppalette, uint8_t* pcolorkey)
{
    INLINE_OBJECT_NULL_CHK(ppdst);
    INLINE_OBJECT_NULL_CHK(ppdstsize);

    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, 0);

    if (depth !=  1 && depth !=  2 && depth !=  4 && depth !=  8 &&
        depth != 16 && depth != 24 && depth != 32) {
        fprintf(stderr, "PNG, Unsupported depth: %d.\n", depth);
        return false;
    }

    if (width == 0 || height == 0) {
        fprintf(stderr, "PNG, Unsupported size: (%d, %d).\n", width, height);
        return false;
    }

    // color-type interpretation of the image and number of bytes-per-pixel
    uint8_t colortype = 0;
    uint32_t palettesize = 0;

    switch (depth)
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
            // the PLTE chunk contains from 1 to 256 palette entries, each three-bytes
            palettesize = ppalette->size * 3;
        }
    } break;
    }

    // check for invalid color-type and sampledepth
    if (colortype == 3 && sampledepth >= 16) {
        fprintf(stderr, "PNG, bad color-type (%d) and bit-depth (%d).\n", colortype,
            sampledepth);
        return false;
    }

    // src stuff
    uint8_t* srcptr = psrcbuf;
    uint32_t srcpitch = WidthInBytes(width, depth);
    uint32_t crc = 0;

    // byte encoded array
    int bytearraysize = s_png_signaturesize +
        (s_png_chunksize + s_png_headersize + s_png_crcsize) +
        (s_png_chunksize + palettesize + s_png_crcsize) +
        (s_png_chunksize + (((height + 1) * srcpitch + 1) + (height + 1)) +
            s_png_crcsize) +
        (s_png_chunksize + s_png_crcsize);
    uint8_t* bytearray = (uint8_t*)malloc(bytearraysize);
    if (bytearray == NULL) {
        fprintf(stderr, "PNG, Out of memory.\n");
        return false;
    }
    memset(bytearray, 0, bytearraysize);
    //uint8_t* dstptr = bytearray;
    uint8_t* dstbuf = bytearray;
    int byteswritten = 0;

    // fill in file header and info

    // identifier
    for (int i = 0; i < 8; ++i) {
        *dstbuf++ = s_png_id[i];
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
    WriteU32ToBE(dstbuf, width); dstbuf += 4;
    WriteU32ToBE(dstbuf, height); dstbuf += 4;
    uint8_t compression = 0;
    uint8_t filter = 0;
    // 0 (no interlace) or 1 (Adam7 interlace)
    uint8_t interlace = (codec == GEUL_RLE) ? 1 : 0;
    *dstbuf++ = depth <= 32 ? depth <= 4 ? depth : 8 : 16;
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
    if (colortype == 3 && palettesize != 0) {
        // PLTE chunk
        size = palettesize;
        type = BigU32(PLTE);
        WriteU32ToBE(dstbuf, size); dstbuf += 4;
        crcbuf = dstbuf;
        WriteU32ToBE(dstbuf, type); dstbuf += 4;
        byteswritten += 8;
        for (uint32_t i = 0; i < ppalette->size; ++i)
        {
            *dstbuf++ = ppalette->entry[i*4+0];
            *dstbuf++ = ppalette->entry[i*4+1];
            *dstbuf++ = ppalette->entry[i*4+2];
            byteswritten += 3;
        }
        // CRC
        crc = Crc(crcbuf, (int)(dstbuf - crcbuf));
        WriteU32ToBE(dstbuf, crc); dstbuf += 4;
        byteswritten += 4;
    }

    // transparency
    if (pcolorkey != NULL && (colortype == 0 || colortype == 2 || colortype == 3)) {

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
            WriteU16ToBE(dstbuf, pcolorkey[3]); dstbuf += 2;
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
            WriteU16ToBE(dstbuf, pcolorkey[0]); dstbuf += 2;
            WriteU16ToBE(dstbuf, pcolorkey[1]); dstbuf += 2;
            WriteU16ToBE(dstbuf, pcolorkey[2]); dstbuf += 2;
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
                *dstbuf++ = ppalette->entry[i*4+3];
                byteswritten++;
            }
        } break;
        }
        // CRC
        crc = Crc(crcbuf, (int)(dstbuf - crcbuf));
        WriteU32ToBE(dstbuf, crc); dstbuf += 4;
        byteswritten += 4;
    }

    // uncompressed data, bytes per line, per component, plus filter per row
    uint32_t idatlen = ((height + 1) * (srcpitch + 1)) + (height + 1);
    uint8_t* idatptr = (uint8_t*)malloc(((idatlen + 1) & ~1));
    if (idatptr == NULL) {

        fprintf(stderr, "PNG, Out of memory.\n");
        free(bytearray);
        bytearray = NULL;

        return false;
    }
    memset(idatptr, 0, ((idatlen + 1) & ~1));
    uint8_t* idatbuf = idatptr;

    // filter selection (adaptive filtering with five basic filter types)
    uint8_t filtermode = PNG_FILTER_ADAPTIVE;
    if (filtermode == PNG_FILTER_ADAPTIVE) {
        if (colortype == 3 || depth <= 4) {
            filtermode = PNG_FILTER_NONE;
        }
    }

    // IDAT chunk
    // interlace and filter
    if (interlace) {
        if (depth <= 4) {
            ShrinkIndexInterlacedPNG(idatbuf, &idatlen, width, height, depth,
                filtermode, srcptr);
        } else {
            ShrinkInterlacedPNG(idatbuf, &idatlen, width, height, depth,
                filtermode, srcptr);
        }
    } else {
        if (depth <= 4) {
            ShrinkIndexPNG(idatbuf, &idatlen, width, height, depth, filtermode,
                srcptr);
        } else {
            ShrinkPNG(idatbuf, &idatlen, width, height, depth, filtermode,
                srcptr);
        }
    }

    idatbuf = idatptr;

    uint32_t bytesencoded = 0;
    uint32_t bytesdecoded = 0;

    // ZLIB compression
    z_stream deflator = { 0 };

    // deflate\inflate compression levels 0-10
    uint8_t compressionlevel = 6;

    int status = deflateInit(&deflator, MIN(compressionlevel, 9));

    if (status != Z_OK) {

        fprintf(stderr, "PNG, Deflate: failed with status %i.\n",
            status);

        free(idatptr);
        idatptr = NULL;

        free(bytearray);
        bytearray = NULL;

        return false;
    }

    // deflate
    uint32_t odatlen = idatlen;
    uint8_t* odatptr = (uint8_t*)malloc(((odatlen + 1) & ~1));
    if (odatptr == NULL) {

        fprintf(stderr, "PNG, Out of memory.\n");

        free(idatptr);
        idatptr = NULL;

        free(bytearray);
        bytearray = NULL;

        return false;
    }
    memset(odatptr, 0, ((odatlen + 1) & ~1));
    uint8_t* odatbuf = odatptr;

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

        free(bytearray);
        bytearray = NULL;

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
    *ppdst = (uint8_t*)realloc(bytearray, ((*ppdstsize + 1) & ~1));

    return true;
}

//-----------------------------------------------------------------------------
// GetInfoFromMemory
//-----------------------------------------------------------------------------
static bool
GetInfoFromMemoryPNG(uint8_t* pcolormap, uint32_t* pwidth, uint32_t* pheight,
    uint32_t* pdepth, uint8_t* psampledepth, uint8_t* psrcbuf, uint32_t psrcsize)
{
    uint32_t chksize = s_png_signaturesize + s_png_headersize +
        (4 * (s_png_chunksize + s_png_crcsize));
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, chksize);
    uint32_t srclen = psrcsize;
    //uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    uint8_t* srcend = psrcbuf + srclen;
    for (int i = 0; i < 8; ++i) {
        if (*srcbuf++ != s_png_id[i]) {
            return false;
        }
    }
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t depth = 0;
    uint32_t bytes = 0;
    uint32_t colortype = 0;
    uint32_t compression = 0;
    uint32_t filter = 0;
    uint32_t interlace = 0;
    uint32_t palnum = 0;
    uint32_t crc0 = 0;
    uint32_t crc1 = 0;
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
    width = ReadU32FromBE(srcbuf); srcbuf += 4;
    height = ReadU32FromBE(srcbuf); srcbuf += 4;
    if (width == 0 || height == 0) {
        fprintf(stderr, "PNG, 0-pixel image: x: %d, y: %d\n", width, height);
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
        fprintf(stderr, "PNG, Bad color-type (%d) and bit-depth (%d).\n", colortype,
            depth);
        return false;
    }
    // color-type interpretation of the image and number of bytes-per-pixel
    switch (colortype)
    {
    case 6: bytes++;            // rgba
    case 2: bytes++;            // rgb
    case 4: bytes++;            // grayscale + alpha
    case 3:                     // palette
    case 0: bytes++;            // grayscale
        break;
    default:
    {
        fprintf(stderr, "PNG, Unsupported color type: %d.\n", colortype);
        return false;
    }
    }
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
    while (srcbuf < srcend && endparsed == false) {
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
                if (datparsed) {
                    return false;
                }
                // the gAMA chunk must precede the first PLTE chunk.
                if (palparsed) {
                    return false;
                }
                srcbuf += size;
            } break;
            case PLTE:
            {
                // the PLTE chunk must precede the first IDAT chunk.
                if (datparsed) {
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
                if (palparsed == false) {
                    palparsed = true;
                }
                srcbuf += size;
            } break;
            case tRNS:
            {
                // the tRNS chunk must precede the first IDAT chunk.
                if (datparsed) {
                    return false;
                }
                if (colortype == 3) {
                    // the tRNS chunk must follow the PLTE chunk, if any.
                    if (palparsed == false) {
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
                if (datparsed == false) {
                    datparsed = true;
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
                srcbuf += size;
            } break;
            case IEND:
            {
                endparsed = true;
            } break;
            default:
            {
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
    if (pcolormap != NULL) { *pcolormap = palparsed; }
    if (pwidth != NULL) { *pwidth = width; }
    if (pheight != NULL) { *pheight = height; }
    if (pdepth != NULL) { *pdepth = depth * bytes; }
    if (psampledepth != NULL) { *psampledepth = 8; }

    return true;
}

//------------------------------------------------------------------------------
// ExpandIndexInterlaced
//------------------------------------------------------------------------------
static void
ExpandIndexInterlacedPNG(uint8_t** ppdst, uint32_t width, uint32_t height,
    uint8_t depth, uint8_t* psrc)
{
    uint8_t* pixels = *ppdst;
    uint8_t* buffer = psrc;
    uint32_t pitch = WidthInBytes(width, depth);
    uint32_t xskip = 1;
    uint32_t yskip = 1;
    uint32_t xorig = 0;
    uint32_t yorig = 0;
    uint32_t w = width;
    uint32_t h = height;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t pass = 0;
    uint8_t maskbits[4] = { 0x01, 0x03, 0, 0x0F };
    uint8_t mask = maskbits[depth-1];
    uint16_t pix0 = 0;
    uint16_t pix1 = 0;
    uint16_t pae0 = 0;
    uint16_t raw1 = 0;
    uint16_t pri0 = 0;
    uint16_t pri1 = 0;
    uint16_t a = 0;
    uint16_t b = 0;
    uint16_t c = 0;
    uint8_t filter = 0;
    uint8_t bit0 = 0;
    uint8_t bit1 = 0;
    uint8_t bit2 = 0;
    uint8_t cur0 = 0;
    uint8_t cur1 = 0;
    uint8_t cur2 = 0;
    int32_t x0 = 0;
    int32_t x1 = 0;
    uint8_t ppb = (depth == 1 ? 3 : depth == 2 ? 2 : 1);
    uint8_t bpp = (depth == 1 ? 0 : depth == 2 ? 1 : 2);
    uint8_t mod = (8 / depth) - 1;
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixptr;
    uint8_t latch = 0;
    uint8_t* filter0 = &latch;
    uint8_t* filter1 = &latch;
    uint8_t* filter2 = &latch;

    const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
    const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
    const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
    const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };

    do {
        w = (width  - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
        h = (height - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];
        xskip = i_xextent[pass];
        yskip = i_yextent[pass];
        xorig = i_xorigin[pass];
        yorig = i_yorigin[pass];
        y = 0;
        while (y < h) {
            filter = *buffer++;
            pixptr = pixels + (((y * yskip) + yorig) * pitch);
            pixbuf = pixptr;
            x = 0;
            switch (filter)
            {
            case 0:
            {
                pix1 = 0;
                while (x < w) {
                    x0 = ((x - 0) * xskip) + xorig;
                    cur0 = ( x & mod);
                    cur1 = (x0 & mod);
                    bit0 = (mod - cur0) << bpp;
                    bit1 = (mod - cur1) << bpp;
                    pix0 = (cur0 == 0) ? *buffer++ : pix0;
                    *(pixbuf + (x0 >> ppb)) |=
                        (((pix0 + pix1) >> bit0) & mask) << bit1;
                    x++;
                }
            } break;
            case 1:
            {
                filter0 = &latch;
                while (x < w) {
                    x0 = ((x - 0) * xskip) + xorig;
                    x1 = ((x - 1) * xskip) + xorig;
                    cur0 = ( x & mod);
                    cur2 = (x1 & mod);
                    bit0 = (mod - cur0) << bpp;
                    bit2 = (mod - cur2) << bpp;
                    pix0 = (cur0 == 0) ? *buffer++ : pix0;
                    raw1 = (*(filter0 + (x1 >> ppb)) >> bit2) & mask;
                    pix1 = raw1 & 0xFF;
                    filter0 = pixbuf;
                    *(pixbuf + (x0 >> ppb)) |=
                        (((pix0 + pix1) >> bit0) & mask) << bit0;
                    x++;
                }
            } break;
            case 2:
            {
                while (x < w) {
                    x0 = ((x - 0) * xskip) + xorig;
                    x1 = ((x - 1) * xskip) + xorig;
                    cur0 = ( x & mod);
                    cur1 = (x0 & mod);
                    bit0 = (mod - cur0) << bpp;
                    bit1 = (mod - cur1) << bpp;
                    pix0 = (cur0 == 0) ? *buffer++ : pix0;
                    pri0 = (*(filter1 + (x0 >> ppb)) >> bit1) & mask;
                    pix1 = pri0 & 0xFF;
                    *(pixbuf + (x0 >> ppb)) |=
                        (((pix0 + pix1) >> bit0) & mask) << bit0;
                    x++;
                }
            } break;
            case 3:
            {
                filter0 = &latch;
                while (x < w) {
                    x0 = ((x - 0) * xskip) + xorig;
                    x1 = ((x - 1) * xskip) + xorig;
                    cur0 = ( x & mod);
                    cur1 = (x0 & mod);
                    cur2 = (x1 & mod);
                    bit0 = (mod - cur0) << bpp;
                    bit1 = (mod - cur1) << bpp;
                    bit2 = (mod - cur2) << bpp;
                    pix0 = (cur0 == 0) ? *buffer++ : pix0;
                    raw1 = (*(filter0 + (x1 >> ppb)) >> bit2) & mask;
                    pri0 = (*(filter1 + (x0 >> ppb)) >> bit1) & mask;
                    pix1 = ((raw1 + pri0) >> 1) & 0xFF;
                    filter0 = pixbuf;
                    *(pixbuf + (x0 >> ppb)) |=
                        (((pix0 + pix1) >> bit0) & mask) << bit0;
                    x++;
                }
            } break;
            case 4:
            {
                filter0 = &latch;
                filter2 = &latch;
                while (x < w) {
                    x0 = ((x - 0) * xskip) + xorig;
                    x1 = ((x - 1) * xskip) + xorig;
                    cur0 = ( x & mod);
                    cur1 = (x0 & mod);
                    cur2 = (x1 & mod);
                    bit0 = (mod - cur0) << bpp;
                    bit1 = (mod - cur1) << bpp;
                    bit2 = (mod - cur2) << bpp;
                    pix0 = (cur0 == 0) ? *buffer++ : pix0;
                    raw1 = (*(filter0 + (x1 >> ppb)) >> bit2) & mask;
                    pri0 = (*(filter1 + (x0 >> ppb)) >> bit1) & mask;
                    pri1 = (*(filter2 + (x1 >> ppb)) >> bit2) & mask;
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
                    filter0 = pixbuf;
                    filter2 = filter1;
                    *(pixbuf + (x0 >> ppb)) |=
                        (((pix0 + pix1) >> bit0) & mask) << bit0;
                    x++;
                }
            } break;
            }
            filter1 = pixptr;
            y++;
        }
    } while (++pass < 7);
}

//------------------------------------------------------------------------------
// ExpandInterlaced
//------------------------------------------------------------------------------
static void
ExpandInterlacedPNG(uint8_t** ppdst, uint32_t width, uint32_t height,
    uint8_t depth, uint8_t* psrc)
{
    uint8_t* pixels = *ppdst;
    uint8_t* buffer = psrc;
    //float pixelsperbyte = (8.0f / (float)depth);
    uint32_t bytes = (depth + 7) >> 3;
    uint32_t pitch = WidthInBytes(width, depth);
    uint32_t xskip = 1;
    uint32_t yskip = 1;
    uint32_t xorig = 0;
    uint32_t yorig = 0;
    uint32_t w = width;
    uint32_t h = height;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t pass = 0;
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
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixptr;
    uint8_t latch = 0;
    uint8_t* filter0 = &latch;
    uint8_t* filter1 = &latch;
    uint8_t* filter2 = &latch;
    uint8_t increment = 1;
    uint8_t xinc = 0;
    uint8_t yinc = 0;

    const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
    const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
    const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
    const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };

    do {
        w = (width - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
        h = (height - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];
        xskip = i_xextent[pass];
        yskip = i_yextent[pass];
        xorig = i_xorigin[pass];
        yorig = i_yorigin[pass];
        y = 0;
        while (y < h)
        {
            filter = *buffer++;
            pixptr = pixels + (((y * yskip) + yorig) * pitch);
            pixbuf = pixptr;
            x = 0;
            switch (filter)
            {
            case 0:
            {
                pixbuf += xorig * bytes;
                while (x < w) {
                    bpp = 0;
                    while (bpp < bytes) { *pixbuf++ = *buffer++ & 0xFF; bpp++; }
                    pixbuf += (xskip - 1) * bytes;
                    x++;
                }
            } break;
            case 1:
            {
                filter0 = &latch;
                xinc = 0;
                pixbuf += xorig * bytes;
                while (x < w) {
                    bpp = 0;
                    while (bpp < bytes) {
                        *pixbuf++ = (*buffer++ + *filter0) & 0xFF;
                        filter0 += xinc;
                        bpp++;
                    }
                    xinc = increment;
                    filter0 = pixbuf - bytes;
                    pixbuf += (xskip - 1) * bytes;
                    x++;
                }
            } break;
            case 2:
            {
                pixbuf += xorig * bytes;
                while (x < w) {
                    bpp = 0;
                    while (bpp < bytes) {
                        *pixbuf++ = (*buffer++ + *filter1) & 0xFF;
                        filter1 += yinc;
                        bpp++;
                    }
                    filter1 += (xskip - 1) * bytes;
                    pixbuf += (xskip - 1) * bytes;
                    x++;
                }
            } break;
            case 3:
            {
                filter0 = &latch;
                xinc = 0;
                pixbuf += xorig * bytes;
                while (x < w) {
                    bpp = 0;
                    while (bpp < bytes) {
                        pix1 = ((*filter0 + *filter1) >> 1) & 0xFF;
                        *pixbuf++ = (*buffer++ + pix1) & 0xFF;
                        filter0 += xinc;
                        filter1 += yinc;
                        bpp++;
                    }
                    xinc = increment;
                    filter0 = pixbuf - bytes;
                    filter1 += (xskip - 1) * bytes;
                    pixbuf += (xskip - 1) * bytes;
                    x++;
                }
            } break;
            case 4:
            {
                filter0 = &latch;
                filter2 = &latch;
                xinc = 0;
                pixbuf += xorig * bytes;
                while (x < w) {
                    bpp = 0;
                    while (bpp < bytes) {
                        pix0 = *buffer++;
                        raw1 = *filter0;
                        pri0 = *filter1;
                        pri1 = *filter2;
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
                        *pixbuf++ = ((pix0 + pix1) & 0xFF);
                        filter0 += xinc;
                        filter1 += yinc;
                        filter2 += xinc;
                        bpp++;
                    }
                    xinc = increment;
                    filter0 = pixbuf - bytes;
                    filter2 = filter1 - bytes;
                    filter1 += (xskip - 1) * bytes;
                    pixbuf += (xskip - 1) * bytes;
                    x++;
                }
            } break;
            }
            filter1 = pixptr + xorig * bytes;
            yinc = increment;
            y++;
        }
    } while (++pass < 7);
}

//------------------------------------------------------------------------------
// ExpandIndex
//------------------------------------------------------------------------------
static void
ExpandIndexPNG(uint8_t** ppdst, uint32_t width, uint32_t height, uint8_t depth,
    uint8_t* psrc)
{
    uint8_t* pixels = *ppdst;
    uint8_t* buffer = psrc;
    //uint32_t bytes = (depth + 7) >> 3;
    uint32_t pitch = WidthInBytes(width, depth);
    uint32_t x = 0;
    uint32_t y = 0;
    uint8_t maskbits[4] = { 0x01, 0x03, 0, 0x0F };
    uint8_t mask = maskbits[depth-1];
    uint16_t pix0 = 0;
    uint16_t pix1 = 0;
    uint16_t pae0 = 0;
    uint16_t raw1 = 0;
    uint16_t pri0 = 0;
    uint16_t pri1 = 0;
    uint16_t a = 0;
    uint16_t b = 0;
    uint16_t c = 0;
    uint8_t filter = 0;
    uint8_t bit0 = 0;
    uint8_t bit1 = 0;
    uint8_t cur0 = 0;
    uint8_t cur1 = 0;
    int32_t xx = 0;
    uint8_t pixelsperbyte = 8 / depth;
    uint8_t bpp = (depth == 1 ? 0 : depth == 2 ? 1 : 2);
    uint8_t mod = pixelsperbyte - 1;
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixptr;
    uint8_t latch = 0;
    uint8_t* filter0 = &latch;
    uint8_t* filter1 = &latch;
    uint8_t* filter2 = &latch;
    uint8_t increment = 1;
    //uint8_t xinc = 0;
    uint8_t yinc = 0;

    y = 0;
    while (y++ < height)
    {
        filter = *buffer++;
        pixbuf = pixptr;
        x = 0;
        switch (filter)
        {
        case 0:
        {
            while (x < width) {
                pix0 = *buffer++;
                xx = 0;
                while (xx < pixelsperbyte) {
                    bit0 = (mod - (xx & mod)) << bpp;
                    *pixbuf |= ((pix0 >> bit0) & mask) << bit0;
                    xx++;
                }
                pixbuf++;
                x += xx;
            }
        } break;
        case 1:
        {
            filter0 = &latch;
            while (x < width) {
                pix0 = *buffer++;
                xx = 0;
                while (xx < pixelsperbyte) {
                    cur0 = ((xx - 0) & mod);
                    cur1 = ((xx - 1) & mod);
                    bit0 = (mod - cur0) << bpp;
                    bit1 = (mod - cur1) << bpp;
                    raw1 = (*filter0 >> bit1) & mask;
                    pix1 = raw1 & 0xFF;
                    *pixbuf |= (((pix0 + pix1) >> bit0) & mask) << bit0;
                    xx++;
                }
                filter0 = pixbuf;
                pixbuf++;
                x += xx;
            }
        } break;
        case 2:
        {
            while (x < width) {
                pix0 = *buffer++;
                xx = 0;
                while (xx < pixelsperbyte) {
                    cur0 = (xx & mod);
                    bit0 = (mod - cur0) << bpp;
                    pri0 = (*filter1 >> bit0) & mask;
                    pix1 = pri0 & 0xFF;
                    *pixbuf |= (((pix0 + pix1) >> bit0) & mask) << bit0;
                    xx++;
                }
                filter1 += yinc;
                pixbuf++;
                x += xx;
            }
        } break;
        case 3:
        {
            filter0 = &latch;
            while (x < width) {
                pix0 = *buffer++;
                xx = 0;
                while (xx < pixelsperbyte) {
                    cur0 = ((xx - 0) & mod);
                    cur1 = ((xx - 1) & mod);
                    bit0 = (mod - cur0) << bpp;
                    bit1 = (mod - cur1) << bpp;
                    raw1 = (*filter0 >> bit1) & mask;
                    pri0 = (*filter1 >> bit0) & mask;
                    pix1 = ((raw1 + pri0) >> 1) & 0xFF;
                    *pixbuf |= (((pix0 + pix1) >> bit0) & mask) << bit0;
                    xx++;
                }
                filter0 = pixbuf;
                filter1 += yinc;
                pixbuf++;
                x += xx;
            }
        } break;
        case 4:
        {
            filter0 = &latch;
            filter2 = &latch;
            while (x < width) {
                pix0 = *buffer++;
                xx = 0;
                while (xx < pixelsperbyte) {
                    cur0 = ((xx - 0) & mod);
                    cur1 = ((xx - 1) & mod);
                    bit0 = (mod - cur0) << bpp;
                    bit1 = (mod - cur1) << bpp;
                    raw1 = (*filter0 >> bit1) & mask;
                    pri0 = (*filter1 >> bit0) & mask;
                    pri1 = (*filter2 >> bit1) & mask;
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
                    *pixbuf |= (((pix0 + pix1) >> bit0) & mask) << bit0;
                    xx++;
                }
                filter0 = pixbuf;
                filter2 = filter1;
                filter1 += yinc;
                pixbuf++;
                x += xx;
            }
        } break;
        }
        filter1 = pixptr;
        yinc = increment;
        pixptr += pitch;
    }
}

//------------------------------------------------------------------------------
// Expand
//------------------------------------------------------------------------------
static void
ExpandPNG(uint8_t** ppdst, uint32_t width, uint32_t height, uint32_t depth,
    uint8_t* psrc)
{
    uint8_t* pixels = *ppdst;
    uint8_t* buffer = psrc;
    //float pixelsperbyte = (8.0f / (float)depth);
    uint32_t bytes = (depth + 7) >> 3;
    uint32_t pitch = WidthInBytes(width, depth);
    uint32_t x = 0;
    uint32_t y = 0;
    //uint16_t pix0 = 0;
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
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixptr;
    uint8_t latch = 0;
    uint8_t* filter0 = &latch;
    uint8_t* filter1 = &latch;
    uint8_t* filter2 = &latch;
    uint8_t increment = 1;
    uint8_t xinc = 0;
    uint8_t yinc = 0;

    y = 0;
    while (y++ < height)
    {
        filter = *buffer++;
        pixbuf = pixptr;
        x = 0;
        switch (filter)
        {
        case 0:
        {
            while (x++ < width * bytes) { *pixbuf++ = *buffer++; }
        } break;
        case 1:
        {
            filter0 = &latch;
            xinc = 0;
            while (x++ < width) {
                bpp = 0;
                while (bpp++ < bytes) {
                    *pixbuf++ = (*buffer++ + *filter0) & 0xFF;
                    filter0 += xinc;
                }
                xinc = increment;
                filter0 = pixbuf - bytes;
            }
        } break;
        case 2:
        {
            while (x++ < width) {
                bpp = 0;
                while (bpp++ < bytes) {
                    *pixbuf++ = (*buffer++ + *filter1) & 0xFF;
                    filter1 += yinc;
                }
            }
        } break;
        case 3:
        {
            filter0 = &latch;
            xinc = 0;
            while (x++ < width) {
                bpp = 0;
                while (bpp++ < bytes) {
                    pix1 = ((*filter0 + *filter1) >> 1) & 0xFF;
                    *pixbuf++ = (*buffer++ + pix1) & 0xFF;
                    filter0 += xinc;
                    filter1 += yinc;
                }
                xinc = increment;
                filter0 = pixbuf - bytes;
            }
        } break;
        case 4:
        {
            filter0 = &latch;
            filter2 = &latch;
            xinc = 0;
            while (x++ < width) {
                bpp = 0;
                while (bpp++ < bytes) {
                    raw1 = *filter0;
                    pri0 = *filter1;
                    pri1 = *filter2;
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
                    *pixbuf++ = (*buffer++ + pix1) & 0xFF;
                    filter0 += xinc;
                    filter1 += yinc;
                    filter2 += xinc;
                }
                xinc = increment;
                filter0 = pixbuf - bytes;
                filter2 = filter1 - bytes;
            }
        } break;
        }
        filter1 = pixptr;
        yinc = increment;
        pixptr += pitch;
    }
}

//-----------------------------------------------------------------------------
// LoadFromMemory
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryPNG(uint8_t** ppdst, palette_t* ppalette, uint8_t* psrcbuf,
    uint32_t psrcsize, uint32_t* pwidth, uint32_t* pheight, uint8_t* pdepth,
    uint8_t* psampledepth, uint8_t* pcolorkey)
{
    uint32_t chksize = s_png_signaturesize + s_png_headersize +
        (4 * (s_png_chunksize + s_png_crcsize));
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, chksize);
    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    uint8_t* srcend = psrcbuf + srclen;
    for (int i = 0; i < 8; ++i) {
        if (*srcbuf++ != s_png_id[i]) {
            fprintf(stderr, "PNG, Bad signature.\n");
            return false;
        }
    }
    uint32_t width = 0;
    uint32_t height = 0;
    uint8_t depth = 0;
    uint8_t bytes = 0;
    uint32_t pitch = 0;
    uint32_t gamma = 0;
    uint16_t palnum = 0;
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
    width = ReadU32FromBE(srcbuf); srcbuf += 4;
    height = ReadU32FromBE(srcbuf); srcbuf += 4;
    if (width == 0 || height == 0) {
        fprintf(stderr, "PNG, 0-pixel image: x: %d, y: %d\n", width,
            height);
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
    case 6: bytes++;            // rgba
    case 2: bytes++;            // rgb
    case 4: bytes++;            // grayscale + alpha
    case 3:                     // palette
    case 0: bytes++;            // grayscale
        break;
    default:
    {
        fprintf(stderr, "PNG, Unsupported color type: %d.\n", colortype);
        return false;
    }
    }
    pitch = WidthInBytes(width, (depth * bytes));
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
    while (srcbuf < srcend && endparsed == false) {
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
                palnum = GEUL_CAST_TO_U16(size) / 3;
                uint32_t num = palnum * 3;
                if (num != size) {
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
                        ppalette->entry[i*4+0] = *srcbuf++;
                        ppalette->entry[i*4+1] = *srcbuf++;
                        ppalette->entry[i*4+2] = *srcbuf++;
                        ppalette->entry[i*4+3] = 255;
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
                            ppalette->entry[i*4+3] = *srcbuf++;
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
                        pcolorkey[0] = colorkey[0];
                        pcolorkey[1] = colorkey[1];
                        pcolorkey[2] = colorkey[2];
                        pcolorkey[3] = 255;
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
    if (idataptr == NULL) {
        fprintf(stderr, "PNG, Out of memory.\n");
        return false;
    }
    memset(idataptr, 0, ((idatasize + 1) & ~1));
    uint8_t* idatabuf = idataptr;

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
                srcbuf   += size;
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
    uint32_t odatasize = ((height + 1) * (pitch + 1)) + (height + 1);
    uint8_t* odataptr = (uint8_t*)malloc(((odatasize + 1) & ~1));
    if (odataptr == NULL) {

        fprintf(stderr, "PNG, Out of memory.\n");
        free(idataptr);
        idataptr = NULL;

        return false;
    }
    memset(odataptr, 0, ((odatasize + 1) & ~1));
    uint8_t* odatabuf = odataptr;

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
    uint32_t pixlen = width * height * bytes;
    uint8_t* pixels = (uint8_t*)malloc(pixlen);
    if (pixels == NULL) {
        
        fprintf(stderr, "PNG, Out of memory.\n");
        free(dataptr);
        dataptr = NULL;

        return false;
    }
    memset(pixels, 0, width * height * bytes);
    uint8_t* pixbuf = pixels;

    if (interlace) {
        if (depth <= 4) {
            ExpandIndexInterlacedPNG(&pixels, width, height, depth * bytes, databuf);
        } else {
            ExpandInterlacedPNG(&pixels, width, height, depth * bytes,
                databuf);
        }
    } else {
        if (depth <= 4) {
            ExpandIndexPNG(&pixels, width, height, depth * bytes, databuf);
        } else {
            ExpandPNG(&pixels, width, height, depth * bytes, databuf);
        }
    }

    free(dataptr);
    dataptr = NULL;

    // gamma
    const float ik = 1.0f / 255.0f;
    if (gamma > 100000) {
        float y = (gamma * (1.0f / 2.2f)) / 100000.0f;
        if ((colortype == 0 || colortype == 4) && depth == 8) {         // grayscale
            for (unsigned int i = 0; i < width * height; ++i) {
                pixbuf[0] = GEUL_FLT_TO_U8(ROUND(powf((pixbuf[0]*ik),y)*255.0f));
                pixbuf += bytes;
            }
        }
        if ((colortype == 2 || colortype == 6) && depth == 8) {         // rgb
            for (unsigned int i = 0; i < width * height; ++i) {
                pixbuf[0] = GEUL_FLT_TO_U8(ROUND(powf((pixbuf[0]*ik),y)*255.0f));
                pixbuf[1] = GEUL_FLT_TO_U8(ROUND(powf((pixbuf[1]*ik),y)*255.0f));
                pixbuf[2] = GEUL_FLT_TO_U8(ROUND(powf((pixbuf[2]*ik),y)*255.0f));
                pixbuf += bytes;
            }
        }
    }

    *ppdst = pixels;
    if (pwidth != NULL) { *pwidth = width; }
    if (pheight != NULL) { *pheight = height; }
    if (pdepth != NULL) { *pdepth = depth * bytes; }
    if (psampledepth != NULL) { *psampledepth = 8; }

    return true;
}

#endif // #ifndef _PNG_H_

uint8_t
ParseABS(uint8_t** pixels, uint32_t bytesperpixel, uint8_t maxcount)
{
    uint8_t* buffer = *pixels;
    uint8_t count = 1;
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

uint8_t
ParseRLE(uint8_t** pixels, uint32_t bytesperpixel, uint8_t maxcount)
{
    uint8_t* buffer = *pixels;
    uint8_t count = 1;
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
// Save
//------------------------------------------------------------------------------
static bool
SaveToMemoryTGA(uint8_t** ppdst, uint32_t* ppdstsize, uint32_t codec,
    uint8_t* psrcbuf, uint32_t srcsize, uint16_t width, uint16_t height,
    uint8_t depth, palette_t* ppalette, bool invertX, bool invertY)
{
    INLINE_OBJECT_NULL_CHK(ppdst);
    INLINE_OBJECT_NULL_CHK(ppdstsize);
    //INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(srcsize, 0);
    if (depth != 8 && depth != 16 && depth != 24 && depth != 32) {
        fprintf(stderr, "TGA, Unsupported depth: %d.\n", depth);
        return false;
    }
    // src stuff
    uint32_t bytesperpixel = depth >> 3;
    uint32_t pitch = width * bytesperpixel;
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
    uint16_t iwidth = width;
    uint16_t iheight = height;
    uint8_t  pixel_size = depth;
    uint8_t  image_descriptor = 0;
    // palette
    if (depth == 8) {
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
            switch(depth)
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
            switch (depth)
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
    uint32_t bytearraysize = s_tga_file_size + ((height * pitch) * 2) +
        colormap_length;
    uint8_t* bytearray = (uint8_t*)malloc(bytearraysize);
    if (bytearray == NULL) {
        fprintf(stderr, "TGA, Out of memory.\n");
        return false;
    }
    memset(bytearray, 0, bytearraysize);
    uint8_t* dstbuf = bytearray;
    uint32_t bytesencoded = 0;

    // fill in file header
    x_origin = (invertX == true ? 0 : width);
    y_origin = (invertY == true ? 0 : height);
    *dstbuf++ = id_length;
    *dstbuf++ = colormap_type;
    *dstbuf++ = image_type;
    WriteU16ToLE(dstbuf, colormap_index);   dstbuf += 2;
    WriteU16ToLE(dstbuf, colormap_length);  dstbuf += 2;
    *dstbuf++ = colormap_size;
    WriteU16ToLE(dstbuf, x_origin);         dstbuf += 2;
    WriteU16ToLE(dstbuf, y_origin);         dstbuf += 2;
    WriteU16ToLE(dstbuf, iwidth);           dstbuf += 2;
    WriteU16ToLE(dstbuf, iheight);          dstbuf += 2;
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
            for (uint16_t i = colormap_index; i < colormap_length; ++i) {
                r = (ppalette->entry[i*4+0] * 0x1F) / 0xFF;
                g = (ppalette->entry[i*4+1] * 0x1F) / 0xFF;
                b = (ppalette->entry[i*4+2] * 0x1F) / 0xFF;
                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);
                WriteU16ToLE(dstbuf, rgb16); dstbuf += 2;
            }
        } else if (colormap_size == 24) {
            for (uint16_t i = colormap_index; i < colormap_length; ++i) {
                *dstbuf++ = ppalette->entry[i*4+2];
                *dstbuf++ = ppalette->entry[i*4+1];
                *dstbuf++ = ppalette->entry[i*4+0];
            }
        } else if (colormap_size == 32) {            
            for (uint16_t i = colormap_index; i < colormap_length; ++i) {
                *dstbuf++ = ppalette->entry[i*4+2];
                *dstbuf++ = ppalette->entry[i*4+1];
                *dstbuf++ = ppalette->entry[i*4+0];
                *dstbuf++ = ppalette->entry[i*4+3];
            }
        }
    }
    if (psrcbuf != NULL) {
        uint32_t x = 0;
        uint32_t y = 0;
        if (codec == GEUL_RLE) {          // run-length encoding
            uint8_t abscount = 0;
            uint8_t rlecount = 0;
            uint8_t runcount = 0;
            uint8_t maxcount = 0;
            uint32_t absbound = width - 0xFF;
            uint32_t rlebound = width - 0x80;
            uint32_t sample = 0;
            uint8_t* buffer = NULL;
            switch (image_type)
            {
            case TGA_RGB_RLE:
            case TGA_MAPPED_RLE:
            case TGA_BLACK_AND_WHITE_RLE:
            {
                while (y < height) {
                    bufptr = rowptr;
                    x = 0;
                    while (x < width) {
                        memcpy(&sample, (bufptr + (x * bytesperpixel)), bytesperpixel);
                        buffer =  (bufptr + (x * bytesperpixel));
                        maxcount = 0xFF;
                        if (x > absbound) { maxcount = GEUL_CAST_TO_U8(width - x); };
                        abscount = ParseABS(&buffer, bytesperpixel, maxcount);
                        maxcount = 0x80;
                        if (x > rlebound) { maxcount = GEUL_CAST_TO_U8(width - x); };
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
            memcpy(dstbuf, bufptr, height * pitch);
            dstbuf += height * pitch;
            bytesencoded += height * pitch;
        }
    }
    *ppdstsize = s_tga_file_size + id_length + colormap_length *
        ((colormap_size==15 ? 16 : colormap_size) >> 3) + bytesencoded;
    *ppdst = (uint8_t*)realloc(bytearray, ((*ppdstsize + 1) & ~1));
    return true;
}

//-----------------------------------------------------------------------------
// GetInfo
//-----------------------------------------------------------------------------
static bool
GetInfoFromMemoryTGA(uint8_t* pcolormap, uint32_t* pwidth, uint32_t* pheight,
    uint32_t* pdepth, uint8_t* psrcbuf, uint32_t psrcsize)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(psrcsize, s_tga_file_size);
    //uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    // file struct
    //uint8_t  id_length          = *srcbuf++;
    srcbuf++;
    uint8_t  colormap_type      = *srcbuf++;
    uint8_t  image_type         = *srcbuf++;
    //uint16_t colormap_index     = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t colormap_length    = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    uint8_t  colormap_size      = *srcbuf++;
    //uint16_t x_origin           = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t y_origin           = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    uint16_t width              = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t height             = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint8_t  pixel_size         = *srcbuf++;
    //uint8_t  image_descriptor   = *srcbuf++;
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
    if (pwidth != NULL) { *pwidth = width; }
    if (pheight != NULL) { *pheight = height; }
    if (pdepth != NULL) { *pdepth = pixel_size; }
    if (pcolormap != NULL) { *pcolormap = colormap_type; }
    return true;
}

//-----------------------------------------------------------------------------
// Load
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryTGA(uint8_t** ppdst, palette_t* ppalette, uint8_t* psrcbuf,
    uint32_t srcsize, uint32_t* pwidth, uint32_t* pheight, uint8_t* pdepth)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(srcsize, s_tga_file_size);
    // src stuff
    //uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    // file struct
    uint8_t  id_length           = *srcbuf++;
    uint8_t  colormap_type       = *srcbuf++;
    uint8_t  image_type          = *srcbuf++;
    //uint16_t colormap_index      = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    uint16_t colormap_length     = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint8_t  colormap_size       = *srcbuf++;
    //uint16_t x_origin            = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t y_origin            = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    uint16_t width               = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t height              = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint8_t  pixel_size          = *srcbuf++;
    //uint8_t  image_descriptor    = *srcbuf++;
    srcbuf++;
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
            uint16_t palnum = colormap_length < 256 ? colormap_length : 256;
            uint8_t* palptr = srcbuf + id_length;
            if (ppalette != NULL) {
                if (colormap_size == 15 || colormap_size == 16) {
                    uint16_t pixel = 0;
                    for (size_t i = 0; i < palnum; ++i) {
                        pixel = 0;
                        pixel |= *palptr++;
                        pixel |= *palptr++ << 8;
                        
                        ppalette->entry[i*4+2] = (((pixel >>  0) & 0x1F) * 0xFF) / 0x1F;
                        ppalette->entry[i*4+1] = (((pixel >>  5) & 0x1F) * 0xFF) / 0x1F;
                        ppalette->entry[i*4+0] = (((pixel >> 10) & 0x1F) * 0xFF) / 0x1F;
                        ppalette->entry[i*4+3] = 255;
                    }
                    ppalette->size = palnum;
                    ppalette->bits = 24;
                } else if (colormap_size == 24) {
                    for (size_t i = 0; i < palnum; ++i) {
                        ppalette->entry[i*4+2] = *palptr++;
                        ppalette->entry[i*4+1] = *palptr++;
                        ppalette->entry[i*4+0] = *palptr++;
                        ppalette->entry[i*4+3] = 255;
                    }
                    ppalette->size = palnum;
                    ppalette->bits = 24;
                } else if (colormap_size == 32) {
                    for (size_t i = 0; i < palnum; ++i) {
                        ppalette->entry[i*4+2] = *palptr++;
                        ppalette->entry[i*4+1] = *palptr++;
                        ppalette->entry[i*4+0] = *palptr++;
                        ppalette->entry[i*4+3] = *palptr++;
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
    if (pixels == NULL) {
        fprintf(stderr, "TGA: Out of memory.\n");
        return false;
    }
    memset(pixels, 0, xsize * ysize * bytesperpixel);
    uint8_t* rowptr = pixels;           // start of current dst row
    uint8_t* buffer = pixels;           // current dst row
    *ppdst = pixels;
    if (pwidth != NULL) { *pwidth = xsize; }
    if (pheight != NULL) { *pheight = ysize; }
    if (pdepth != NULL) { *pdepth = pixel_size; }
    switch (image_type)
    {
        case TGA_MAPPED_RLE:
        case TGA_RGB_RLE:
        case TGA_BLACK_AND_WHITE_RLE:
        {
            uint32_t y = 0;
            uint32_t x = 0;
            while (y < ysize) {
                buffer = rowptr;
                rlecount = 0;
                rlevalue = 0;
                x = 0;
                while (x < xsize) {
                    if (rlecount == 0) {            // get rle count
                        rlevalue = *srcbuf++;
                        rlecount = (rlevalue & 0x7F) + 1;
                        for (uint32_t i = 0; i < bytesperpixel; ++i) {
                            rgba[i] = *srcbuf++;
                        }
                    } else if (!(rlevalue & 0x80)) {
                        // if this is not a repeating count read next pixel of
                        // component size
                        for (uint32_t i = 0; i < bytesperpixel; ++i) {
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
// Save
//------------------------------------------------------------------------------
static bool
SaveToMemoryBMP(uint8_t** ppdst, uint32_t* ppdstsize, uint32_t codec,
    uint8_t* psrcbuf, uint32_t srcsize, uint32_t srcxsize, uint32_t srcysize,
    uint8_t srcdepth, palette_t* ppalette, uint8_t* pcolorkey, bool invertY)
{
    INLINE_OBJECT_NULL_CHK(ppdst);
    INLINE_OBJECT_NULL_CHK(ppdstsize);
    //INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(srcsize, 0);
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
    int32_t bytearraysize = s_bmp_file_size + s_bmp_v3_info_size +
        ((srcysize * dstpitch)) + dstpalettesize;
    uint8_t* bytearray = (uint8_t*)malloc(bytearraysize);
    if (bytearray == NULL) {
        fprintf(stderr, "BMP, Out of memory.\n");
        return false;
    }
    memset(bytearray, 0, bytearraysize);
    uint8_t* dstptr = bytearray;
    uint8_t* dstbuf = bytearray;
    // fill in file and info header
    uint16_t filetype = BMP_IDENTIFIER;
    uint32_t filesize = bytearraysize;
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
        } else if (srcdepth == 4) {
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
            for (uint32_t i = 0; i < dstpalettesize >> 2; ++i) {
                *dstbuf++ = ppalette->entry[i*4+2];
                *dstbuf++ = ppalette->entry[i*4+1];
                *dstbuf++ = ppalette->entry[i*4+0];
                *dstbuf++ = ppalette->entry[i*4+3];
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
                for (uint32_t i = 0; i < dstpalettesize >> 2; ++i) {
                    if (ppalette->entry[i*4+0] == pcolorkey[0] &&
                        ppalette->entry[i*4+1] == pcolorkey[1] &&
                        ppalette->entry[i*4+2] == pcolorkey[2] &&
                        ppalette->entry[i*4+3] == pcolorkey[3]) {
                        colorkey = i;
                        break;
                    }
                }
            }
            uint8_t* buffer = NULL;
            uint8_t abscount = 0;
            uint8_t rlecount = 0;
            uint8_t runcount = 0;
            uint8_t maxcount = 0xFF;
            uint32_t maxbound = width - 0xFF;
            uint8_t sample = 0;
            switch (dstdepth)
            {
            case 4:         // 4-bit encoding
            {
                // expand to 4 to 8-bits
                pixels = NULL;
                pixels = (uint8_t*)malloc(srcxsize * srcysize);
                if (pixels == NULL) {
                    fprintf(stderr, "BMP, Out of memory.\n");
                    free(bytearray);
                    return false;
                }
                memset(pixels, 0, srcxsize * srcysize);
                uint8_t* pixbuf = pixels;
                uint8_t* srcbuf = rowbuf;
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
                            uint8_t dx = 0;
                            uint8_t dy = 0;
                            while (count >= (uint32_t)width)
                            {
                                count -= width;
                                dy++;
                            }
                            dx = GEUL_CAST_TO_U8(count);
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
                        if (x > maxbound) { maxcount = GEUL_CAST_TO_U8(width - x); };
                        abscount = ParseABS(&buffer, 1, maxcount);
                        rlecount = ParseRLE(&buffer, 1, maxcount);
                        if (abscount >= 3 && abscount >= rlecount) {
                            for (uint8_t i = 0; i < abscount; i++) {
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
                            for (uint32_t i = 0; i < count; ++i) {
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
                                    uint8_t d1 = sample & 0xF;
                                    uint8_t d0 = sample << 4;
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
    bytearraysize = s_bmp_file_size + s_bmp_v3_info_size + dstpalettesize +
        bytesencoded;
    // rewrite file struct size
    dstbuf = dstptr + 2;
    WriteU32ToLE(dstbuf, bytearraysize); dstbuf += 4;
    *ppdstsize = bytearraysize;
    *ppdst = (uint8_t*)realloc(bytearray, (*ppdstsize + 1) & ~1);
    return true;
}

//------------------------------------------------------------------------------
// GetInfo
//------------------------------------------------------------------------------
static bool
GetInfoFromMemoryBMP(uint32_t* pwidth, uint32_t* pheight, uint32_t* pdepth,
    uint8_t* psrcbuf, uint32_t srcsize)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(srcsize, s_bmp_file_size);
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
    if (pwidth != NULL) { *pwidth = xsize; }
    if (pheight != NULL) { *pheight = ABS(ysize); }
    if (pdepth != NULL) { *pdepth = bits & 0xFF; }
    return true;
}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------
static bool
LoadFromMemoryBMP(uint8_t** ppdst, palette_t* ppalette, uint8_t* psrcbuf,
    uint32_t srcsize, uint32_t* pwidth, uint32_t* pheight, uint8_t* pdepth)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(srcsize, s_bmp_file_size);
    uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    // file struct
    uint16_t filetype   = ReadU16FromLE(srcbuf); srcbuf += 2;
    //uint32_t filesize   = ReadU32FromLE(srcbuf);
    srcbuf += 4;
    uint16_t reserved1  = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t reserved2  = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint32_t offset     = ReadU32FromLE(srcbuf); srcbuf += 4;
    // info struct
    //uint32_t infosize           = ReadU32FromLE(srcbuf);
    srcbuf += 4;
    int32_t  width              = ReadI32FromLE(srcbuf); srcbuf += 4;
    int32_t  height             = ReadI32FromLE(srcbuf); srcbuf += 4;
    uint16_t planes             = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t bits               = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint32_t compression        = ReadU32FromLE(srcbuf); srcbuf += 4;
    //uint32_t imagesize          = ReadU32FromLE(srcbuf);
    srcbuf += 4;
    //int32_t  xresolution        = ReadI32FromLE(srcbuf);
    srcbuf += 4;
    //int32_t  yresolution        = ReadI32FromLE(srcbuf);
    srcbuf += 4;
    uint32_t num_colours        = ReadU32FromLE(srcbuf); srcbuf += 4;
    //uint32_t num_colour_indexes = ReadU32FromLE(srcbuf);
    srcbuf += 4;
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
        uint16_t palnum = GEUL_CAST_TO_U16(num_colours);
        if (ppalette != 0) {
            for (uint32_t i = 0; i < palnum; ++i) {
                ppalette->entry[i*4+2] = *palptr++;
                ppalette->entry[i*4+1] = *palptr++;
                ppalette->entry[i*4+0] = *palptr++;
                ppalette->entry[i*4+3] = *palptr++;
            }
            ppalette->size = palnum;
            ppalette->bits = 32;
            uint32_t alpha = 0;
            for (size_t i = 0; i < ppalette->size; ++i) {
                alpha |= ppalette->entry[i*4+3];
            }
            // if alpha is all 0s replace with 255s
            if (alpha == 0) {
                for (size_t i = 0; i < ppalette->size; ++i)
                {
                    ppalette->entry[i*4+3] = 255;
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
    if (pixels == NULL) {
        fprintf(stderr, "BMP, Out of memory.\n");
        return false;
    }
    memset(pixels, 0, ABS(ysize) * pitch);
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixels;
    // bottom-up dib
    if (ysize >= 0) {
        pixptr = (pixels + ((ysize-1)) * pitch);
        pitch = -pitch;
    }
    ysize = ABS(ysize);
    *ppdst = pixels;
    if (pwidth != NULL) { *pwidth = xsize; }
    if (pheight != NULL) { *pheight = ABS(ysize); }
    if (pdepth != NULL) { *pdepth = (bits & 0xFF); }
    if (rle) {          // run-length encoding
        int32_t x = 0;
        int32_t y = 0;
        uint8_t bit = 0;
        while (y < ysize) {
            pixbuf = pixptr;
            bit = 4;
            x = 0;
            while (1) {
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
                                for (int i = 0; i < data1; ++i) {
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
                        for (int i = 0; i < rlecount; ++i) {
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
        while (y < ysize) {
            pixbuf = pixptr;
            memcpy(pixbuf, srcbuf, ABS(pitch));
            srcbuf += ABS(pitch) + padbytes;
            pixptr += pitch;
            y++;
        }
        if (bits == 32) {
            uint32_t alpha = 0;
            pixbuf = pixels;
            for (int i = 0; i < xsize * ABS(ysize); ++i, pixbuf += 4) {
                alpha |= pixbuf[3];
            }
            // if alpha is all 0's replace with 255's
            if (alpha == 0) {
                pixbuf = pixels;
                for (int i = 0; i < xsize * ABS(ysize); ++i, pixbuf += 4) {
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
static uint8_t ega_palette[64] = {
      0,   0,   0, 255,
      0,   0, 170, 255,
      0, 170,   0, 255,
      0, 170, 170, 255,
    170,   0,   0, 255,
    170,   0, 170, 255,
    170,  85,   0, 255,
    170, 170, 170, 255,
     85,  85,  85, 255,
     85,  85, 255, 255,
     85, 255,  85, 255,
     85, 255, 255, 255,
    255,  85,  85, 255,
    255,  85, 255, 255,
    255, 255,  85, 255,
    255, 255, 255, 255
};

static const uint32_t s_pcx_v5_info_size = 128;

//-----------------------------------------------------------------------------
// Save
//-----------------------------------------------------------------------------
static bool
SaveToMemoryPCX(uint8_t** ppdst, uint32_t* pdstsize, uint8_t* psrcbuf,
    uint32_t srcsize, uint16_t width, uint16_t height, uint8_t depth,
    palette_t* ppalette)
{
    INLINE_OBJECT_NULL_CHK(ppdst);
    INLINE_OBJECT_NULL_CHK(pdstsize);
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(srcsize, 0);
    if (depth !=  1 && depth != 2 && depth != 4 && depth != 8 && depth != 24) {
        fprintf(stderr, "PCX, Unsupported src depth: %d.\n", depth);
        return false;
    }
    uint32_t srcwidth = width;
    uint32_t srcheight = height;
    uint8_t palettetype = 0;
    if (depth <= 8) {
        palettetype = 1;
    }
    // dst stuff
    uint32_t dstdepth = (depth == 24) ? 8 : depth;
    uint16_t dstpitch = (WidthInBytes(width, dstdepth) + 1) & ~1;           // must be an even number
    // src stuff
    uint8_t srcbytes = ((depth == 24) ? 3 : 1);
    uint32_t srcpitch = WidthInBytes(width, depth);
    if (depth < 8) {
        srcwidth = srcpitch;
    }
    uint8_t* rowptr = psrcbuf;
    uint8_t* bufptr = psrcbuf;
    uint8_t* buffer = NULL;
    uint32_t dstpadbytes = dstpitch - (srcpitch / srcbytes);
    uint32_t dstpalettesize = 0;
    // palette
    if (depth == 8 && ppalette == NULL) {
        fprintf(stderr, "PCX, Missing palette.\n");
        return false;
    }
    if (depth == 8) {
        dstpalettesize = 768;
    }
    // byte encoded array
    uint32_t bytearraysize = s_pcx_v5_info_size + (srcheight * (srcbytes * dstpitch)) +
        dstpalettesize;
    uint8_t* bytearray = (uint8_t*)malloc(bytearraysize+bytearraysize);
    if (bytearray == NULL) {
        fprintf(stderr, "PCX, Out of memory.\n");
        return false;
    }
    memset(bytearray, 0, bytearraysize);
    //uint8_t* dstptr = bytearray;
    uint8_t* dstbuf = bytearray;
    // fill in file header
    *dstbuf++ = 0x0A;           // PCX Id Number (Always 0x0A)
    *dstbuf++ = 5;          // Version Number
    *dstbuf++ = 1;          // Encoding Format
    *dstbuf++ = (depth == 24) ? 8 : depth;          // Bits Per Pixel
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Left of image
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Top of image
    WriteU16ToLE(dstbuf, width-1);      dstbuf += 2;            // Right of image
    WriteU16ToLE(dstbuf, height-1);     dstbuf += 2;            // Bottom of image
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
    uint8_t rlecount = 0;
    uint8_t runcount = 0;
    uint8_t maxcount = 0x3F;
    uint32_t maxbound = srcwidth - 0x3F;
    uint32_t x = 0;
    uint32_t y = 0;
    uint8_t sample = 0;
    while (y < srcheight) {
        colorplane = 0;
        do {
            bufptr = rowptr;
            x = 0;
            while (x < srcwidth) {
                sample = *(bufptr + ((x * srcbytes) + colorplane));
                buffer =  (bufptr + ((x * srcbytes) + colorplane));
                maxcount = 0x3F;
                if (x > maxbound) { maxcount = GEUL_CAST_TO_U8(srcwidth - x); };
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
        uint8_t* palptr = NULL;
        switch (depth)
        {
        case 1:
        case 2:
        case 4:
        {
            dstbuf = egaptr;            // 16-Color EGA Palette
            palnum = 16;
            palptr = (ppalette == NULL) ? ega_palette : ppalette->entry;
        } break;
        case 8:
        {
            *dstbuf++ = 0x0C;           // 256-Color Palette Code
            bytesencoded++;
            palnum = 256;
            palptr = ppalette->entry;
        } break;
        }
        if (ppalette != NULL) {
            for (uint32_t i = 0; i < palnum; ++i) {
                *dstbuf++ = palptr[i*4+0];
                *dstbuf++ = palptr[i*4+1];
                *dstbuf++ = palptr[i*4+2];
            }
            bytesencoded += dstpalettesize;
        }
    }
    *pdstsize = s_pcx_v5_info_size + bytesencoded;
    *ppdst = (uint8_t*)realloc(bytearray, *pdstsize + 1 & ~1);
    return true;
}

//-----------------------------------------------------------------------------
// GetInfo
//-----------------------------------------------------------------------------
static bool
GetInfoFromMemoryPCX(uint32_t* pwidth, uint32_t* pheight, uint32_t* pdepth,
    uint8_t* psrcbuf, uint32_t srcsize)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(srcsize, s_pcx_v5_info_size);
    //uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    uint8_t  identifier      = *srcbuf++;
    uint8_t  version         = *srcbuf++;
    uint8_t  encoding        = *srcbuf++;
    uint8_t  bitsPerPixel    = *srcbuf++;
    uint16_t xMin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t yMin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t xMax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t yMax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    //uint16_t horzRes         = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t vertRes         = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    srcbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)
    uint8_t  numBitPlanes    = *srcbuf++;
    //uint16_t bytesPerLine    = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t paletteType     = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t horzScreenSize  = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t vertScreenSize  = ReadU16FromLE(srcbuf);
    srcbuf += 2;
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
    if (pwidth != NULL) { *pwidth = (xMax - xMin) + 1; }
    if (pheight != NULL) { *pheight = (yMax - yMin) + 1; }
    if (pdepth != NULL) { *pdepth = (bitsPerPixel * numBitPlanes); }
    return true;
}

//-----------------------------------------------------------------------------
// Load
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryPCX(uint8_t** ppdst, palette_t* ppalette, uint8_t* psrcbuf,
    uint32_t srcsize, uint32_t* pwidth, uint32_t* pheight, uint8_t* pdepth)
{
    INLINE_OBJECT_NULL_CHK(psrcbuf);
    INLINE_OBJECT_SIZE_CHK(srcsize, s_pcx_v5_info_size);
    //uint8_t* srcptr = psrcbuf;
    uint8_t* srcbuf = psrcbuf;
    uint8_t* srcend = psrcbuf + srcsize;
    uint8_t  identifier      = *srcbuf++;
    uint8_t  version         = *srcbuf++;
    uint8_t  encoding        = *srcbuf++;
    uint8_t  bitsperpixel    = *srcbuf++;
    uint16_t xmin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t ymin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t xmax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t ymax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    //uint16_t horzres         = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t vertres         = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    uint8_t* egaptr = srcbuf;           // set pointer to 16-Color EGA Palette
    srcbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)
    uint8_t  numbitplanes    = *srcbuf++;
    uint16_t bytesperline    = ReadU16FromLE(srcbuf); srcbuf += 2;
    //uint16_t palettetype     = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t horzscreensize  = ReadU16FromLE(srcbuf);
    srcbuf += 2;
    //uint16_t vertscreensize  = ReadU16FromLE(srcbuf);
    srcbuf += 2;
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
    if (pixels == NULL) {
        fprintf(stderr, "PCX, Out of memory.\n");
        return false;
    }
    memset(pixels, 0, ysize * pitch);
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixels;
    uint32_t y = 0;
    *ppdst = pixels;
    if (pwidth != NULL) { *pwidth = xsize; }
    if (pheight != NULL) { *pheight = ysize; }
    if (pdepth != NULL) { *pdepth = bitsperpixel * numbitplanes; }
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
        uint16_t palnum = 0;
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
            for (uint32_t i = 0; i < palnum; ++i) {
                ppalette->entry[i*4+0] = *palptr++;
                ppalette->entry[i*4+1] = *palptr++;
                ppalette->entry[i*4+2] = *palptr++;
                ppalette->entry[i*4+3] = 255;
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
FastFill(image_t* pimage, uint8_t* params0, uint8_t* params1)
{
    if (pimage == NULL || pimage->pixels == NULL) {
        return;
    }
    if (params0 == NULL || params1 == NULL) {
        return;
    }
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t pixel0 = 0;
    uint32_t pixel1 = 0;
    uint32_t pixel2 = 0;
    uint32_t bytes = GetBytesForPixelFormat(pimage->format);
    for (size_t i = 0; i < bytes; i++) {
        pixel1 |= params0[i] << (i << 3);
        pixel2 |= params1[i] << (i << 3);
    }
    uint8_t* buffer = pimage->pixels;
    while (y < pimage->height) {
        x = (pimage->width + 3) / 4;
        switch (pimage->width % 4)
        {
        case 0: do {
            memcpy(&pixel0, buffer, bytes);
            if (pixel0 == pixel1) memset(buffer, pixel2, bytes);
            buffer += bytes;
        case 3:
            memcpy(&pixel0, buffer, bytes);
            if (pixel0 == pixel1) memset(buffer, pixel2, bytes);
            buffer += bytes;
        case 2:
            memcpy(&pixel0, buffer, bytes);
            if (pixel0 == pixel1) memset(buffer, pixel2, bytes);
            buffer += bytes;
        case 1:
            memcpy(&pixel0, buffer, bytes);
            if (pixel0 == pixel1) memset(buffer, pixel2, bytes);
            buffer += bytes;
        } while (--x > 0);
        }
        y++;
    }
}

//-----------------------------------------------------------------------------
// SaveImageToMemory
//-----------------------------------------------------------------------------
bool
SaveImageToMemory(uint8_t** ppdst, uint32_t* pdstsize, uint32_t fileformat,
    uint32_t codec, image_t* pimage, palette_t* ppalette)
{
    uint8_t depth = 8;
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
            result = SaveToMemoryPNG(ppdst, pdstsize, codec, pimage->pixels,
                pixelsize, pimage->width, pimage->height, depth, 8,
                ppalette, NULL);
        } break;
        case GEUL_BMP:
        {
            result = SaveToMemoryBMP(ppdst, pdstsize, codec, pimage->pixels,
                pixelsize, pimage->width, pimage->height, depth,
                ppalette, NULL, false);
        } break;
        case GEUL_PCX:
        {
            if (pimage->width < UINT16_MAX && pimage->height < UINT16_MAX) {
                uint16_t w = GEUL_CAST_TO_U16(pimage->width);
                uint16_t h = GEUL_CAST_TO_U16(pimage->height);
                result = SaveToMemoryPCX(ppdst, pdstsize, pimage->pixels,
                    pixelsize, w, h, depth, ppalette);
            } else {
                fprintf(stderr, "SaveImage, Image width or height exceeds maximum.\n");
            }
        } break;
        case GEUL_TGA:
        {
            if (pimage->width < UINT16_MAX && pimage->height < UINT16_MAX) {
                uint16_t w = GEUL_CAST_TO_U16(pimage->width);
                uint16_t h = GEUL_CAST_TO_U16(pimage->height);
                result = SaveToMemoryTGA(ppdst, pdstsize, codec, pimage->pixels,
                    pixelsize, w, h, depth, ppalette, false, false);
            } else {
                fprintf(stderr, "SaveImage, Image width or height exceeds maximum.\n");
            }
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
SaveImageToFile(const char* pdstfile, uint32_t fileformat, uint32_t codec,
    image_t* pimage, palette_t* ppalette)
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
        result = SaveImageToMemory(&rawdst, &dstsize, fileformat, codec,
            pimage, ppalette);
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
LoadImageFromMemory(image_t* pimage, palette_t* ppalette, uint8_t* pcolorkey,
    uint8_t* psrcbuf, uint32_t srcsize)
{
    palette_t palette = { 0 };
    uint8_t* pixels = NULL;
    uint8_t depth = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t format = 0;
    uint32_t type = GEUL_UNSIGNED_BYTE;
    uint32_t fileformat = 0;
    uint8_t colorkey[4] = { 0 };
    bool result = false;
    if (pimage != NULL) {
        if ((result = LoadFromMemoryPNG(&pixels, &palette, psrcbuf, srcsize,
            &width, &height, &depth, NULL, colorkey)) == true) {
            if (depth == 32) { format = GEUL_RGBA; }
            else if (depth == 24) { format = GEUL_RGB; }
            else if (depth == 16) { format = GEUL_LUMINANCE_ALPHA; }
            else if (depth <= 8 && palette.size == 0) { format = GEUL_LUMINANCE; }
            else if (depth <= 8 && palette.size != 0) { format = GEUL_COLOUR_INDEX;
                type = GEUL_BITMAP; }
            if (colorkey[0] != 0 && colorkey[1] != 0 && colorkey[2] != 0 &&
                colorkey[3] != 0) { pcolorkey = &colorkey[0]; }
            fileformat = GEUL_PNG;
        } else if ((result = LoadFromMemoryBMP(&pixels, &palette, psrcbuf, srcsize,
            &width, &height, &depth)) == true) {
            if (depth == 32) { format = GEUL_BGRA; }
            else if (depth == 24) { format = GEUL_BGR; }
            else if (depth <= 8 && palette.size != 0) { format = GEUL_COLOUR_INDEX;
            type = GEUL_BITMAP; }
            fileformat = GEUL_BMP;
        } else if ((result = LoadFromMemoryPCX(&pixels, &palette, psrcbuf, srcsize,
            &width, &height, &depth)) == true) {
            if (depth <= 8 && palette.size != 0) { format = GEUL_COLOUR_INDEX;
            type = GEUL_BITMAP; }
            else { format = GEUL_RGB; }
            fileformat = GEUL_PCX;
        } else if ((result = LoadFromMemoryTGA(&pixels, &palette, psrcbuf, srcsize,
            &width, &height, &depth)) == true) {
            if (depth == 32) { format = GEUL_BGRA; }
            else if (depth == 24) { format = GEUL_BGR; }
            else if (depth == 16) { format = GEUL_BGRA; }
            else if (depth == 8 && palette.size == 0) { format = GEUL_LUMINANCE; }
            else if (depth == 8 && palette.size != 0) { format = GEUL_COLOUR_INDEX;
                type = GEUL_BITMAP; }
            fileformat = GEUL_TGA;
        } else { fprintf(stderr, "LoadImage, Unsupported image format.\n"); }
        // unpack types
        if (fileformat == GEUL_TGA && depth == 16 && result == true) {
            uint8_t* buffer = (uint8_t*)malloc(width * height * 3);
            do {
                if (buffer == NULL) {
                    fprintf(stderr, "LoadImage, Out of memory.\n");
                    break;
                }
                memset(buffer, 0, width * height * 3);
                uint8_t* pixbuf = buffer;
                uint8_t* srcbuf = pixels;
                uint32_t srcpitch = width * 2;
                uint32_t x = 0;
                uint32_t y = 0;
                uint32_t bshift = 0;
                uint32_t gshift = 5;
                uint32_t rshift = 10;
                uint32_t bmask = 0x001F;
                uint32_t gmask = 0x03E0;
                uint32_t rmask = 0x7C00;
                float k5 = 255.0f / 31.0f;
                uint16_t pixel = 0;
                while (y++ < height) {
                    x = 0;
                    while (x < width) {
                        memcpy(&pixel, (srcbuf + (x * 2)), 2);
                        *pixbuf++ = GEUL_FLT_TO_U8(((pixel&bmask)>>bshift)*k5);
                        *pixbuf++ = GEUL_FLT_TO_U8(((pixel&gmask)>>gshift)*k5);
                        *pixbuf++ = GEUL_FLT_TO_U8(((pixel&rmask)>>rshift)*k5);
                        x++;
                    }
                    srcbuf += srcpitch;
                }
                free(pixels);
                pixels = buffer;
                depth = 24;
                format = GEUL_BGR;
            } while (0);
        }
        if ((fileformat == GEUL_PNG || fileformat == GEUL_BMP ||
             fileformat == GEUL_PCX) && depth <= 4 && result == true) {
            uint8_t* buffer = (uint8_t*)malloc(width * height);
            do {
                if (buffer == NULL) {
                    fprintf(stderr, "LoadImage, Out of memory.\n");
                    break;
                }
                memset(buffer, 0, width * height);
                uint8_t* pixbuf = buffer;
                uint8_t* srcbuf = pixels;
                uint32_t x = 0;
                uint32_t y = 0;
                uint8_t pixelsperbyte = 8 / depth;
                uint8_t maskbits[4] = { 0x01, 0x03, 0, 0x0F };
                uint8_t scalebit[4] = { 0xFF, 0x55, 0, 0x11 };
                uint8_t mask = maskbits[depth-1];
                uint8_t resetbit = 8 - depth;
                uint8_t bit = resetbit;
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
                    for (unsigned int i = 0; i < width * height; ++i) {
                        *pixbuf++ *= scalebit[depth - 1];
                    }
                }
                free(pixels);
                pixels = buffer;
                depth = 8;
            } while (0);
        }
        // palette
        if (ppalette != NULL && format == GEUL_COLOUR_INDEX && result == true) {
            for (uint32_t i = 0; i < palette.size; ++i) {
                ppalette->entry[i] = palette.entry[i];
            }
            ppalette->size = palette.size;
            ppalette->bits = palette.bits;
        }
        // dst stuff
        if (result == true) {
            pimage->pixels = pixels;
            pimage->depth = depth;
            pimage->width = width;
            pimage->height = height;
            pimage->format = format;
            pimage->type = type;
        }
        // transparency
        if (result == true) {
            if (pcolorkey != NULL) {
                uint8_t transparency[4] = { 0 };
                if (format == GEUL_COLOUR_INDEX && type == GEUL_BITMAP &&
                    ppalette != NULL) {
                    bool index0 = false;
                    bool index1 = false;
                    uint8_t i0 = 0;
                    uint8_t i1 = 0;
                    for (uint8_t i = 0; i < ppalette->size; ++i) {
                        if (ppalette->entry[i*4+0] == pcolorkey[0] &&
                            ppalette->entry[i*4+1] == pcolorkey[1] &&
                            ppalette->entry[i*4+2] == pcolorkey[2] &&
                            ppalette->entry[i*4+3] == pcolorkey[3]) {
                            index0 = true; i0 = i;
                            break;
                        }
                    }
                    for (uint8_t i = 0; i < ppalette->size; ++i) {
                        if (ppalette->entry[i*4+0] == transparency[0] &&
                            ppalette->entry[i*4+1] == transparency[1] &&
                            ppalette->entry[i*4+2] == transparency[2] &&
                            ppalette->entry[i*4+3] == transparency[3]) {
                            index1 = true; i1 = i;
                            break;
                        }
                    }
                    if (index0 == true && index1 == true) {
                        FastFill(pimage, &i0, &i1);
                    }
                } else {
                    FastFill(pimage, pcolorkey, transparency);
                }
            }
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
// LoadImageFromFile
//-----------------------------------------------------------------------------
bool
LoadImageFromFile(image_t* pimage, palette_t* ppalette, uint8_t* pcolorkey,
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
        result = LoadImageFromMemory(pimage, ppalette, pcolorkey, rawsrc,
            (uint32_t)bytesRead);
        free(srcbuf);
        srcbuf = NULL;
        rawsrc = NULL;
    }
    return result;
}

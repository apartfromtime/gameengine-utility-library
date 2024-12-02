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
#pragma warning (disable : 4996)            // deprecation warning

static const uint32_t s_rgba_size = 4;

//-----------------------------------------------------------------------------
// Sampling
//-----------------------------------------------------------------------------

static unsigned char s_bitmask[8] = { 0, 0, 0, 0, 0x0F, 0, 0x03, 0x01 };

//-----------------------------------------------------------------------------
// Shrink 8-bits per index to N-pixels per byte
//-----------------------------------------------------------------------------
void ShrinkIndex8ToNbits(uint8_t* pdst, uint8_t* psrc, uint32_t srcskip,
    uint8_t count, uint8_t bitstart)
{
    uint8_t srcofs = 0;
    uint8_t dstofs = bitstart;
    uint8_t dstdec = 8-bitstart;
    uint8_t dstbit = s_bitmask[bitstart];

    *pdst = 0;

    switch (count)
    {
    case 8: *pdst |= (*(psrc + srcofs) & dstbit) << dstofs; dstofs -= dstdec; srcofs += srcskip;
    case 7: *pdst |= (*(psrc + srcofs) & dstbit) << dstofs; dstofs -= dstdec; srcofs += srcskip;
    case 6: *pdst |= (*(psrc + srcofs) & dstbit) << dstofs; dstofs -= dstdec; srcofs += srcskip;
    case 5: *pdst |= (*(psrc + srcofs) & dstbit) << dstofs; dstofs -= dstdec; srcofs += srcskip;
    case 4: *pdst |= (*(psrc + srcofs) & dstbit) << dstofs; dstofs -= dstdec; srcofs += srcskip;
    case 3: *pdst |= (*(psrc + srcofs) & dstbit) << dstofs; dstofs -= dstdec; srcofs += srcskip;
    case 2: *pdst |= (*(psrc + srcofs) & dstbit) << dstofs; dstofs -= dstdec; srcofs += srcskip;
    case 1: *pdst |= (*(psrc + srcofs) & dstbit) << dstofs;
    break;
    }
}

//-----------------------------------------------------------------------------
// Expand N-pixels per byte to 8-bits per index
//-----------------------------------------------------------------------------
void ExpandNbitsToIndex8(uint8_t* pdst, uint32_t dstskip, uint8_t sample,
    uint8_t count, uint8_t bitstart)
{
    uint8_t dstofs = 0;
    uint8_t srcofs = bitstart;
    uint8_t srcdec = 8-bitstart;
    uint8_t srcbit = s_bitmask[bitstart];

    switch (count)
    {
    case 8: *(pdst) = (sample >> srcofs) & srcbit; srcofs -= srcdec; dstofs += dstskip;
    case 7: *(pdst + dstofs) = (sample >> srcofs) & srcbit; srcofs -= srcdec; dstofs += dstskip;
    case 6: *(pdst + dstofs) = (sample >> srcofs) & srcbit; srcofs -= srcdec; dstofs += dstskip;
    case 5: *(pdst + dstofs) = (sample >> srcofs) & srcbit; srcofs -= srcdec; dstofs += dstskip;
    case 4: *(pdst + dstofs) = (sample >> srcofs) & srcbit; srcofs -= srcdec; dstofs += dstskip;
    case 3: *(pdst + dstofs) = (sample >> srcofs) & srcbit; srcofs -= srcdec; dstofs += dstskip;
    case 2: *(pdst + dstofs) = (sample >> srcofs) & srcbit; srcofs -= srcdec; dstofs += dstskip;
    case 1: *(pdst + dstofs) = (sample >> srcofs) & srcbit;
    break;
    }
}

#define PIXELS_PER_BYTE(depth) ((8.0f / (float)(depth)))

#ifndef _PNG_H
#define _PNG_H


//-----------------------------------------------------------------------------
//
//  Portable Network Graphics
//
//  PNG decoder\encoder with adaptive filtering and interlacing
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

// PNG IDENTIFIER: 137 80 78 71 13 10 26 10
static const uint8_t PNG_IDENTIFIER[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A,
    0x1A, 0x0A };

static const uint8_t s_png_headersize = 13;
static const uint8_t s_png_signaturesize = 8;
static const uint8_t s_png_chunksize = 8;
static const uint8_t s_png_crcsize = 4;

//------------------------------------------------------------------------------
// Table of CRCs
//------------------------------------------------------------------------------
static unsigned long g_crc_table[256] = {
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
unsigned long UpdateCrc(unsigned long crc, unsigned char* buf, int len)
{
    unsigned long c = crc;

    for (int n = 0; n < len; n++)
    {
        c = g_crc_table[(c ^ buf[n]) & 0xFF] ^ (c >> 8);
    }

    return c;
}

//------------------------------------------------------------------------------
// Return the CRC of the bytes buf[0..len-1].
//------------------------------------------------------------------------------
unsigned long Crc(unsigned char* buf, int len)
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
ShrinkPNG(uint8_t* pdst, uint32_t* pdstlen, uint8_t filtertype, uint32_t srcxskip,
    uint32_t srcyskip, uint32_t srcxsize, uint32_t srcysize, uint32_t srcdepth,
    uint8_t* psrc)
{
    uint32_t bytesencoded = 0;
    uint32_t xsize = 0;
    uint32_t ysize = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t i = 0;
    uint8_t* raw0buf = NULL;           // Raw(x)
    uint8_t* raw1buf = NULL;           // Raw(x-bpp)
    uint8_t* pri0buf = NULL;           // Pri(x)
    uint8_t* pri1buf = NULL;           // Pri(x-bpp)
    uint8_t* dstbuf = pdst;
    uint8_t* srcbuf = psrc;
    uint8_t* bufptr = NULL;
    uint8_t* buffer = NULL;
    uint16_t sample = 0;
    uint8_t bytesperpixel = 0;
    uint8_t ppb = 0;
    uint8_t bit = 0;

    if (srcxsize != 0 && srcysize != 0)
    {
        xsize = srcxsize;
        ysize = srcysize;

        if (srcdepth < 8)
        {
            if (srcdepth == 1)
            {
                ppb = 8;
                bit = 7;
            }
            else if (srcdepth == 2)
            {
                ppb = 4;
                bit = 6;
            }
            else if (srcdepth == 4)
            {
                ppb = 2;
                bit = 4;
            }

            bytesperpixel = 1;
            i = srcxskip;
        }
        else
        {
            bytesperpixel = (srcdepth >> 3);
            i = srcxskip * (srcdepth >> 3);
        }

        bufptr = (uint8_t*)malloc((xsize + 8) * bytesperpixel);
        buffer = bufptr;

        if (bufptr == NULL)
        {
            fprintf(stderr, "PNG, Out of memory\n");
            return;
        }

        // filtering - None = 0; Sub = 1; Up = 2; Average = 3; Paeth = 4
        uint8_t filtermode = (filtertype >= PNG_FILTER_COUNT) ? 0 : filtertype;

        while (y < ysize)
        {
            raw0buf = srcbuf;
            raw1buf = srcbuf;
            pri0buf = srcbuf;
            pri1buf = srcbuf;
            x = 0;

            uint16_t pae0 = 0;          // Paeth(x)
            uint16_t raw0 = 0;          // Raw(x)
            uint16_t raw1 = 0;          // Raw(x-bpp)
            uint16_t pri0 = 0;          // Prior(x)
            uint16_t pri1 = 0;          // Prior(x-bpp)
            uint16_t bpp = 0;
            int16_t p0 = 0;
            int16_t pa = 0;
            int16_t pb = 0;
            int16_t pc = 0;
            uint8_t enc = filtermode;

            if (filtermode == PNG_FILTER_ADAPTIVE)
            {
                uint32_t sum[2] = { UINT32_MAX };
                enc = 0;

                for (int f = 0; f < PNG_FILTER_COUNT - 1; ++f)
                {
                    raw0buf = srcbuf;
                    raw1buf = srcbuf;
                    pri0buf = srcbuf;
                    pri1buf = srcbuf;
                    x = 0;

                    if (y != 0) { pri1buf = raw1buf - srcyskip; }
                    if (y != 0) { pri0buf = raw0buf - srcyskip; }

                    sum[1] = 0;

                    while (x < xsize)
                    {
                        raw0 = 0;
                        raw1 = 0;
                        pri0 = 0;
                        pri1 = 0;
                        bpp = 0;

                        while (bpp < bytesperpixel)
                        {
                            if (y != 0 && x != 0)
                            {
                                pri1 = pri1buf[bpp];
                            }

                            if (y != 0)
                            {
                                pri0 = pri0buf[bpp];
                            }

                            if (x != 0)
                            {
                                raw1 = raw1buf[bpp];
                            }

                            raw0 = raw0buf[bpp];

                            switch (f)
                            {
                            case 0:         // None
                            {
                                sample = raw0 & 0xFF;
                            } break;
                            case 1:         // Sub
                            {
                                sample = (raw0 - raw1) & 0xFF;
                            } break;
                            case 2:         // Up
                            {
                                sample = (raw0 - pri0) & 0xFF;
                            } break;
                            case 3:         // Average
                            {
                                sample = (raw0 - FLOOR((raw1 + pri0) / 2)) & 0xFF;
                            } break;
                            case 4:         // Paeth
                            {
                                // paeth predictor
                                p0 = raw1 + pri0 - pri1;
                                pa = ABS(p0 - raw1);
                                pb = ABS(p0 - pri0);
                                pc = ABS(p0 - pri1);

                                if (pa <= pb && pa <= pc)
                                {
                                    pae0 = (raw0 - raw1) & 0xFF;
                                }
                                else if (pb <= pc)
                                {
                                    pae0 = (raw0 - pri0) & 0xFF;
                                }
                                else
                                {
                                    pae0 = (raw0 - pri1) & 0xFF;
                                }

                                sample = pae0;
                            } break;
                            }

                            sum[1] += sample;
                            bpp++;
                        }

                        if (y != 0 && x != 0) { pri1buf += i; }
                        if (x != 0) { raw1buf += i; }
                        pri0buf += i;
                        raw0buf += i;

                        x++;
                    }

                    if (sum[1] < sum[0])
                    {
                        sum[0] = sum[1];
                        enc = f;
                    }
                }
            }

            raw0buf = srcbuf;
            raw1buf = srcbuf;
            pri0buf = srcbuf;
            pri1buf = srcbuf;
            x = 0;

            if (y != 0) { pri1buf = raw1buf - srcyskip; }
            if (y != 0) { pri0buf = raw0buf - srcyskip; }

            memset(bufptr, 0, (xsize + 8) * bytesperpixel);
            buffer = bufptr;

            *dstbuf++ = enc;           // filter-type byte
            bytesencoded++;

            while (x < xsize)
            {
                raw0 = 0;
                raw1 = 0;
                pri0 = 0;
                pri1 = 0;
                bpp = 0;

                while (bpp < bytesperpixel)
                {
                    if (y != 0 && x != 0)
                    {
                        pri1 = pri1buf[bpp];
                    }

                    if (y != 0)
                    {
                        pri0 = pri0buf[bpp];
                    }

                    if (x != 0)
                    {
                        raw1 = raw1buf[bpp];
                    }

                    raw0 = raw0buf[bpp];

                    switch (enc)
                    {
                    case 0:         // None
                    {
                        sample = raw0 & 0xFF;
                    } break;
                    case 1:         // Sub
                    {
                        sample = (raw0 - raw1) & 0xFF;
                    } break;
                    case 2:         // Up
                    {
                        sample = (raw0 - pri0) & 0xFF;
                    } break;
                    case 3:         // Average
                    {
                        sample = (raw0 - FLOOR((raw1 + pri0) / 2)) & 0xFF;
                    } break;
                    case 4:         // Paeth
                    {
                        // paeth predictor
                        p0 = raw1 + pri0 - pri1;
                        pa = ABS(p0 - raw1);
                        pb = ABS(p0 - pri0);
                        pc = ABS(p0 - pri1);

                        if (pa <= pb && pa <= pc)
                        {
                            pae0 = (raw0 - raw1) & 0xFF;
                        }
                        else if (pb <= pc)
                        {
                            pae0 = (raw0 - pri0) & 0xFF;
                        }
                        else
                        {
                            pae0 = (raw0 - pri1) & 0xFF;
                        }

                        sample = pae0;
                    } break;
                    }

                    *buffer++ = (sample & 0xFF);
                    bpp++;
                }

                if (y != 0 && x != 0) { pri1buf += i; }
                if (x != 0) { raw1buf += i; }
                pri0buf += i;
                raw0buf += i;

                x++;
            }

            buffer = bufptr;
            bpp = 0;
            x = 0;

            if (srcdepth < 8)
            {
                int32_t rem = xsize;
                int32_t count = ppb;

                while (rem > 0)
                {
                    if (rem < count) { count = rem; }
                    ShrinkIndex8ToNbits(dstbuf, buffer, 1, count, bit);
                    buffer += count;
                    dstbuf++;
                    bytesencoded++;
                    rem -= count;
                }
            }
            else
            {
                while (x < xsize)
                {
                    bpp = 0;
                    while (bpp < bytesperpixel)
                    {
                        *dstbuf++ = *buffer++;
                        bytesencoded++;
                        bpp++;
                    }
                    x++;
                }
            }
            y++;

            if (y != ysize)
            {
                srcbuf += srcyskip;
            }
        }
    }

    free(bufptr);
    bufptr = NULL;

    if (pdstlen != NULL) { *pdstlen = bytesencoded; }
}

//------------------------------------------------------------------------------
// ShrinkInterlacedPNG
//------------------------------------------------------------------------------
static void
ShrinkInterlacedPNG(uint8_t* dstptr, uint32_t* pdstlen, uint8_t filtermode,
    uint32_t srcxsize, uint32_t srcysize, uint32_t srcdepth, uint8_t* srcptr)
{
    uint32_t srcbytesperpixel = (srcdepth < 8) ? 1 : (srcdepth >> 3);
    uint32_t srcpitch = srcxsize * srcbytesperpixel;
    uint32_t bytesencoded = 0;
    uint32_t dstlen = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint8_t* srcbuf = NULL;
    uint8_t* dstbuf = NULL;

    const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
    const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
    const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
    const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };

    for (int pass = 0; pass < 7; ++pass)
    {
        x = (srcxsize - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
        y = (srcysize - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];

        srcbuf = srcptr + (i_yorigin[pass] * srcpitch) +
            (i_xorigin[pass] * srcbytesperpixel);
        dstbuf = dstptr + dstlen;

        ShrinkPNG(dstbuf, &bytesencoded, filtermode, i_xextent[pass],
            i_yextent[pass] * srcpitch, x, y, srcdepth, srcbuf);

        dstlen += bytesencoded;
    }

    if (pdstlen != NULL) { *pdstlen = dstlen; }
}

//------------------------------------------------------------------------------
// SavePNG
//------------------------------------------------------------------------------
static bool
SaveToMemoryPNG(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec,
    uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize, uint8_t srcdepth,
    palette_t* psrcpalette, rgba_t* pcolorkey)
{
    if (ppdst == NULL || ppdstsize == NULL)
    {
        fprintf(stderr, "PNG, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL)
    {
        fprintf(stderr, "PNG, Invalid src data.\n");
        return false;
    }

    if (srcdepth !=  1 && srcdepth !=  2 && srcdepth !=  4 && srcdepth !=  8 &&
        srcdepth != 16 && srcdepth != 24 && srcdepth != 32)
    {
        fprintf(stderr, "PNG, Unsupported depth: %d.\n", srcdepth);
        return false;
    }

    uint32_t dstxsize = srcxsize;
    uint32_t dstysize = srcysize;
    uint8_t dstdepth = srcdepth < 8 ? srcdepth : 8;

    // color-type interpretation of the image and number of
    // bytes-per-pixel
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
            if (psrcpalette == NULL)
            {
                colortype = 0;          // greyscale
            }
            else
            {
                colortype = 3;          // palette
            }
        } break;
    }

    // src stuff
    int srcbytesperpixel = ((srcdepth == 32) ? 4 :
        (srcdepth == 24) ? 3 :
        (srcdepth == 16) ? 2 : 1);
    uint8_t* rawptr = psrc;
    uint8_t* rawbuf = psrc;

    // dst stuff
    float dstpixelsperbyte = PIXELS_PER_BYTE(srcdepth);
    uint32_t dstpitch = (uint32_t)(ceilf((float)(dstxsize) / dstpixelsperbyte));
    uint32_t bytesperpixel = (uint32_t)(1.0f / dstpixelsperbyte);
    uint32_t palettesize = 0;
    uint32_t crc = 0;

    // palette
    if (srcdepth <= 8)
    {
        if (psrcpalette != NULL)
        {
            // the PLTE chunk contains from 1 to 256 palette entries, each
            // three-bytes
            palettesize = psrcpalette->size * 3;
        }
    }

    // 8-byte signature + IHDR + (PLTE) + IDAT + IEND

    int byteswritten = 0;
    // big array for true-color images
    int datasize = s_png_signaturesize + (s_png_chunksize + s_png_headersize + s_png_crcsize) +
        (s_png_chunksize + palettesize + s_png_crcsize) +
        (s_png_chunksize + ((dstysize * dstpitch) * 2) + s_png_crcsize) +
        (s_png_chunksize + s_png_crcsize);
    uint8_t* data = (uint8_t*)malloc(datasize);

    if (data == NULL)
    {
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

    WriteU32ToBE(dstbuf, dstxsize); dstbuf += 4;
    WriteU32ToBE(dstbuf, dstysize); dstbuf += 4;

    // check for invalid color-type and depth
    if (colortype == 3 && dstdepth >= 16)
    {
        fprintf(stderr, "PNG, bad color-type (%d) and bit-depth (%d).\n",
            colortype, dstdepth);

        free(data);
        data = NULL;

        return false;
    }
    
    uint8_t compression = 0;
    uint8_t filter = 0;
    // 0 (no interlace) or 1 (Adam7 interlace)
    uint8_t interlace = (codec == ENCODE_INTERLACE) ? 1 : 0;

    interlace = (interlace < 2) ? interlace : 0;

    *dstbuf++ = dstdepth;
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
    if (palettesize)
    {
        // PLTE chunk
        size = palettesize;
        type = BigU32(PLTE);

        WriteU32ToBE(dstbuf, size); dstbuf += 4;
        crcbuf = dstbuf;
        WriteU32ToBE(dstbuf, type); dstbuf += 4;
        byteswritten += 8;

        for (unsigned int i = 0; i < psrcpalette->size; ++i)
        {
            *dstbuf++ = psrcpalette->data[i].r;
            *dstbuf++ = psrcpalette->data[i].g;
            *dstbuf++ = psrcpalette->data[i].b;
            byteswritten += 3;
        }

        // CRC
        crc = Crc(crcbuf, (int)(dstbuf - crcbuf));
        WriteU32ToBE(dstbuf, crc); dstbuf += 4;
        byteswritten += 4;
    }

    // transparency
    if (pcolorkey != NULL)
    {
        // tRNS chunk
        type = BigU32(tRNS);

        switch (colortype)
        {
        case 0:
        {
            // single gray level value, stored in the format:
            // Gray:  2 bytes, range 0 .. (2^bitdepth)-1
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
            // single RGB color value, stored in the format:
            // Red:   2 bytes, range 0 .. (2^bitdepth)-1
            // Green: 2 bytes, range 0 .. (2^bitdepth)-1
            // Blue:  2 bytes, range 0 .. (2^bitdepth)-1
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
            // series of one-byte alpha values, corresponding to entries in the PLTE chunk:
            // Alpha for palette index 0:  1 byte
            // Alpha for palette index 1:  1 byte
            // ...etc...
            size = psrcpalette->size;
            WriteU32ToBE(dstbuf, size); dstbuf += 4;
            crcbuf = dstbuf;
            WriteU32ToBE(dstbuf, type); dstbuf += 4;
            byteswritten += 8;

            for (uint32_t i = 0; i < size; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].a;
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
    uint32_t idatlen = (dstysize * (dstpitch + 1)) + ((interlace > 0) ? dstysize : 0);
    uint8_t* idatptr = (uint8_t*)malloc(((idatlen + 1) & ~1));
    uint8_t* idatbuf = idatptr;

    if (idatptr == NULL)
    {
        fprintf(stderr, "PNG, Out of memory.\n");

        free(data);
        data = NULL;

        return false;
    }

    memset(idatptr, 0, ((idatlen + 1) & ~1));

    // filter selection (adaptive filtering with five basic filter types)
    int filtermode = PNG_FILTER_ADAPTIVE;

    if (filtermode == PNG_FILTER_ADAPTIVE)
    {
        if (colortype == 3 || dstdepth < 8)
        {
            filtermode = PNG_FILTER_NONE;
        }
    }

    // IDAT chunk
    if (codec == ENCODE_INTERLACE)           // interlacing
    {
        ShrinkInterlacedPNG(idatbuf, &idatlen, filtermode, dstxsize, dstysize,
            srcdepth, rawbuf);
    }
    else            // no-interlacing
    {
        ShrinkPNG(idatbuf, &idatlen, filtermode, 1, dstpitch, dstxsize, dstysize,
            srcdepth, rawbuf);
    }

    idatbuf = idatptr;

    uint32_t bytesencoded = 0;
    uint32_t bytesdecoded = 0;

    // ZLIB compression
    z_stream deflator = {};

    // deflate\inflate compression levels 0-10
    uint8_t compressionlevel = 6;

    int status = deflateInit(&deflator, MIN(compressionlevel, 9));

    if (status != Z_OK)
    {
        fprintf(stderr, "PNG, Deflate: failed with status %i.\n",
            status);

        free(idatptr);
        idatptr = NULL;

        free(data);
        data = NULL;

        return false;
    }

    // deflate data
    uint32_t odatlen = idatlen;           // deflate size
    uint8_t* odatptr = (uint8_t*)malloc(((odatlen + 1) & ~1));
    uint8_t* odatbuf = odatptr;

    if (odatptr == NULL)
    {
        fprintf(stderr, "PNG, Out of memory.\n");

        free(idatptr);
        idatptr = NULL;

        free(data);
        data = NULL;

        return false;
    }

    memset(odatptr, 0, ((odatlen + 1) & ~1));

    unsigned int oabsrem = odatlen - deflator.total_out;        // absolute remaining output
    unsigned int odatrem = 0;         // relative remaining output
    unsigned int iabsrem = idatlen - deflator.total_in;         // absolute remaining input
    unsigned int idatrem = 0;         // relative remaining input

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

        if (status == Z_STREAM_END)
        {
            fprintf(stdout, "PNG, Deflate: completed successfully\n");
            break;
        }

        if (odatrem == 0)
        {
            fprintf(stderr, "PNG, Deflate: out of output data\n");
            break;
        }
    } while (1);

    deflateEnd(&deflator);

    // free compressed data
    free(idatptr);
    idatptr = NULL;

    // compression failed
    if (status != Z_STREAM_END)
    {
        fprintf(stderr, "PNG, Deflate: failed with status %i.\n",
            status);

        // free un-compressed data and return
        free(odatptr);
        odatptr = NULL;
    }
    else
    {
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
    uint8_t* srcdepth, uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < 8)
    {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

    // check png header
    for (int i = 0; i < 8; ++i)
    {
        if (*srcbuf++ != PNG_IDENTIFIER[i])
        {
            fprintf(stderr, "PNG, Bad signature.\n");
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
    uint8_t colorkey[3] = {};
    uint8_t bytesperpixel = 0;

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
                if (first == false)
                {
                    fprintf(stderr, "PNG, Multiple IHDR.\n");
                    return false;
                }

                first = false;

                // check chunk is valid length
                if (size != s_png_headersize)
                {
                    fprintf(stderr, "PNG, Bad IHDR len.\n");
                    return false;
                }

                xsize = ReadU32FromBE(srcbuf); srcbuf += 4;
                ysize = ReadU32FromBE(srcbuf); srcbuf += 4;

                // zero is an invalid value
                if (xsize == 0 || ysize == 0)
                {
                    fprintf(stderr, "PNG, 0-pixel image: x: %d, y: %d\n", xsize,
                        ysize);
                    return false;
                }

                depth = *srcbuf++;

                if (depth !=  1 && depth != 2 && depth != 4 && depth != 8)
                {
                    fprintf(stderr, "PNG, Unsupported bits: %d.\n", depth);
                    return false;
                }
                
                colortype = *srcbuf++;

                // check for invalid color-type and depth
                if (colortype == 3 && depth >= 16)
                {
                    fprintf(stderr, "PNG, Bad color-type (%d) and bit-depth (%d).\n",
                        colortype, depth);
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
                    fprintf(stderr, "PNG, Unsupported color type: %d.\n", colortype);
                    return false;
                }
                }

                compression = *srcbuf++;

                // compression method
                if (compression != 0)
                {
                    fprintf(stderr, "PNG, Unrecognised compression code: %d.\n",
                        compression);
                    return false;
                }

                filter = *srcbuf++;

                // pre-processing method applied before compression
                if (filter != 0)
                {
                    fprintf(stderr, "PNG, Bad filter method: %d\n", filter);
                    return false;
                }

                interlace = *srcbuf++;
                
                // transmission order of image
                if (interlace > 1)
                {
                    fprintf(stderr, "PNG, Bad interlace method: %d\n", interlace);
                    return false;
                }

            } break;
            case gAMA:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                // the gAMA chunk must precede the first IDAT chunk.
                if (dattest)
                {
                    fprintf(stderr, "PNG, gAMA after IDAT.\n");
                    return false;
                }

                // the gAMA chunk must precede the first PLTE chunk.
                if (paltest)
                {
                    fprintf(stderr, "PNG, gAMA after PLTE.\n");
                    return false;
                }

                srcbuf += size;
            } break;
            case PLTE:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                // the PLTE chunk must precede the first IDAT chunk.
                if (dattest)
                {
                    fprintf(stderr, "PNG, PLTE after IDAT.\n");
                    return false;
                }

                // the PLTE chunk contains from 1 to 256 palette entries, each
                // three-bytes
                // the number of entries is determined from the chunk length.
                if (size > 768)
                {
                    fprintf(stderr, "PNG, Invalid PLTE length: %d\n", size);
                    return false;
                }

                // a chunk length not divisible by 3 is an error.
                palnum = size / 3;

                if ((palnum * 3) != size)
                {    
                    fprintf(stderr, "PNG, Invalid PLTE entries: %d\n", palnum);
                    return false;
                }
                
                if (paltest == false)
                {
                    paltest = true;
                }

                srcbuf += size;
            } break;
            case tRNS:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                // the tRNS chunk must precede the first IDAT chunk.
                if (dattest)
                {
                    fprintf(stderr, "PNG, tRNS after IDAT.\n");
                    return false;
                }

                if (colortype == 3)
                {
                    // the tRNS chunk must follow the PLTE chunk, if any.
                    if (paltest == false)
                    {
                        fprintf(stderr, "PNG, tRNS before PLTE.\n");
                        return false;
                    }
                }
                else 
                {
                    // check for transparency in images with alpha
                    if (colortype == 4 || colortype == 6)
                    {
                        fprintf(stderr, "PNG, tRNS with alpha.\n");
                        return false;
                    }
                }
                
                srcbuf += size;
            } break;
            case IDAT:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                if (dattest == false)
                {
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
                    fprintf(stderr, "PNG, First not IHDR.\n");
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
                    fprintf(stderr, "PNG, First not IHDR.\n");

                    return false;
                }

                char fourcc[4] = {};
                fourcc[0] = (uint8_t)((type >> 24) & 0xFF);
                fourcc[1] = (uint8_t)((type >> 16) & 0xFF);
                fourcc[2] = (uint8_t)((type >>  8) & 0xFF);
                fourcc[3] = (uint8_t)((type >>  0) & 0xFF);
                
                fprintf(stderr, "PNG, Unsupported chunk: %s\n", fourcc);

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

    return true;
}

//------------------------------------------------------------------------------
// ExpandPNG
//------------------------------------------------------------------------------
static void
ExpandPNG(uint8_t* pdst, uint32_t ppdstsize, uint32_t dstxskip, uint32_t dstyskip,
    uint32_t dstxsize, uint32_t dstysize, uint32_t dstdepth,
    uint32_t dstbytesperpixel, uint8_t** ppsrc)
{
    uint32_t depth = dstdepth * dstbytesperpixel;

    // filtering
    uint32_t width = 0;
    uint32_t xsize = 0;
    uint32_t ysize = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t i = 0;
    uint32_t ppb = 0;
    uint32_t bit = 0;
    uint8_t filtermode = 0;
    uint8_t bytesperpixel = 0;

    // pixels
    uint16_t sample = 0;
    uint8_t* raw0buf = 0;            // Raw(x)
    uint8_t* raw1buf = 0;            // Raw(x-bpp)
    uint8_t* pri0buf = 0;            // Pri(x)
    uint8_t* pri1buf = 0;            // Pri(x-bpp)
    uint8_t* dstbuf = pdst;
    uint8_t* srcbuf = *ppsrc;

    // empty image
    if (dstxsize != 0 && dstysize != 0)
    {
        ysize = dstysize;

        if (depth < 8)
        {
            if (depth == 1)
            {
                ppb = 8;
                bit = 7;
            }
            else if (depth == 2)
            {
                ppb = 4;
                bit = 6;
            }
            else if (depth == 4)
            {
                ppb = 2;
                bit = 4;
            }

            bytesperpixel = 1;
            width = ((dstxsize * depth) + 7) >> 3;         // width in bytes
            i = dstxskip * ppb;
        }
        else
        {
            bytesperpixel = (depth >> 3);
            width = dstxsize;
            i = dstxskip * (depth >> 3);
        }

        while (y < ysize)
        {
            filtermode = *(*ppsrc)++;
            raw0buf = dstbuf;
            raw1buf = dstbuf;
            pri0buf = dstbuf;
            pri1buf = dstbuf;
            x = 0;

            uint16_t pae0 = 0;          // Paeth(x)
            uint16_t raw0 = 0;          // Raw(x)
            uint16_t raw1 = 0;          // Raw(x-bpp)
            uint16_t pri0 = 0;          // Prior(x)
            uint16_t pri1 = 0;          // Prior(x-bpp)
            uint16_t bpp = 0;
            int16_t p0 = 0;
            int16_t pa = 0;
            int16_t pb = 0;
            int16_t pc = 0;

            if (y != 0) { pri1buf = raw1buf - dstyskip; }
            if (y != 0) { pri0buf = raw0buf - dstyskip; }

            while (x < width)
            {
                raw0 = 0;
                pri1 = 0;
                raw1 = 0;
                pri0 = 0;
                bpp = 0;

                while (bpp < bytesperpixel)
                {
                    if (y != 0 && x != 0)
                    {
                        pri1 = pri1buf[bpp];
                    }

                    if (y != 0)
                    {
                        pri0 = pri0buf[bpp];
                    }

                    if (x != 0)
                    {
                        raw1 = raw1buf[bpp];
                    }

                    raw0 = *(*ppsrc)++;

                    switch (filtermode)
                    {
                    case 0:         // None
                    {
                        sample = raw0 & 0xFF;
                    } break;
                    case 1:         // Sub
                    {
                        sample = (raw0 + raw1) & 0xFF;
                    } break;
                    case 2:         // Up
                    {
                        sample = (raw0 + pri0) & 0xFF;
                    } break;
                    case 3:         // Average
                    {
                        sample = (raw0 + FLOOR((raw1 + pri0) / 2)) & 0xFF;
                    } break;
                    case 4:         // Paeth
                    {
                        // paeth predictor
                        p0 = raw1 + pri0 - pri1;
                        pa = ABS(p0 - raw1);
                        pb = ABS(p0 - pri0);
                        pc = ABS(p0 - pri1);

                        if (pa <= pb && pa <= pc)
                        {
                            pae0 = (raw0 + raw1) & 0xFF;
                        }
                        else if (pb <= pc)
                        {
                            pae0 = (raw0 + pri0) & 0xFF;
                        }
                        else
                        {
                            pae0 = (raw0 + pri1) & 0xFF;
                        }

                        sample = pae0;
                    } break;
                    }

                    if (depth < 8)
                    {
                        ExpandNbitsToIndex8(raw0buf, dstxskip, (sample & 0xFF), ppb, bit);
                    }
                    else
                    {
                        raw0buf[bpp] = (sample & 0xFF);
                    }

                    bpp++;
                }

                if (y != 0 && x != 0) { pri1buf += i; }
                if (x != 0) { raw1buf += i; }
                pri0buf += i;
                raw0buf += i;

                x++;
            }
            y++;

            if (y < ysize)
            {
                dstbuf += dstyskip;            // set the next span
            }
        }
    }
}

//------------------------------------------------------------------------------
// ExpandInterlacedPNG
//------------------------------------------------------------------------------
static void
ExpandInterlacedPNG(uint8_t* dstptr, uint32_t dstlen, uint32_t dstxsize,
    uint32_t dstysize, uint32_t dstpitch, uint32_t dstdepth,
    uint32_t dstbytesperpixel, uint8_t** ppsrc)
{
    const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
    const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
    const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
    const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };

    for (int pass = 0; pass < 7; ++pass)
    {
        // calculations from stb_image
        uint32_t w = (dstxsize - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
        uint32_t h = (dstysize - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];

        uint8_t* rawbuf = dstptr + (i_yorigin[pass] * dstpitch) +
            (i_xorigin[pass] * dstbytesperpixel);
        uint32_t rawlen = dstlen - (i_yorigin[pass] * dstpitch) +
            (i_xorigin[pass] * dstbytesperpixel);

        ExpandPNG(rawbuf, rawlen, i_xextent[pass], i_yextent[pass] * dstpitch, w,
            h, dstdepth, dstbytesperpixel, ppsrc);
    }
}

//-----------------------------------------------------------------------------
// LoadPNG
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryPNG(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth,
    rgba_t* pcolorkey)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < 8)
    {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

    // check png header
    for (int i = 0; i < 8; ++i)
    {
        if (*srcbuf++ != PNG_IDENTIFIER[i])
        {
            return false;
        }
    }

    // image
    pixel_t pixeltype = PIXELTYPE_UNKNOWN;
    uint32_t xsize = 0;
    uint32_t ysize = 0;
    uint32_t depth = 0;
    uint32_t bytesperpixel = 0;
    uint32_t colortype = 0;
    uint32_t compression = 0;
    uint32_t filter = 0;
    uint32_t interlace = 0;
    uint32_t gamma = 0;
    uint32_t palnum = 0;
    uint32_t idatlen = 0;
    uint8_t colorkey[3] = {};

    // A valid PNG image must contain an IHDR chunk, one or more IDAT chunks, and an
    // IEND chunk.
    bool endfound = false;
    bool first = true;
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
                if (first == false)
                {
                    fprintf(stderr, "PNG, Multiple IHDR.\n");
                    return false;
                }

                first = false;

                // check chunk is valid length
                if (size != s_png_headersize)
                {
                    fprintf(stderr, "PNG, Bad IHDR len.\n");
                    return false;
                }

                xsize = ReadU32FromBE(srcbuf); srcbuf += 4;
                ysize = ReadU32FromBE(srcbuf); srcbuf += 4;

                // zero is an invalid value
                if (xsize == 0 || ysize == 0)
                {
                    fprintf(stderr, "PNG, 0-pixel image: x: %d, y: %d\n", xsize,
                        ysize);
                    return false;
                }

                depth = *srcbuf++;

                if (depth !=  1 && depth != 2 && depth != 4 && depth != 8)
                {
                    fprintf(stderr, "PNG, Unsupported bits: %d.\n", depth);
                    return false;
                }
                
                colortype = *srcbuf++;

                // check for invalid color-type and depth
                if (colortype == 3 && depth >= 16)
                {
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
                        bytesperpixel = 1;
                    } break;
                    case 2:         // rgb
                    {
                        bytesperpixel = 3;
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
                        fprintf(stderr, "PNG, Unsupported color type: %d.\n",
                            colortype);
                        return false;
                    }
                }

                compression = *srcbuf++;

                // compression method
                if (compression == 0)
                {
                    compression = ENCODE_RGB;
                }
                else
                {
                    fprintf(stderr, "PNG, Unrecognised compression code: %d.\n",
                        compression);
                    return false;
                }

                filter = *srcbuf++;

                // pre-processing method applied before compression
                if (filter != 0)
                {
                    fprintf(stderr, "PNG, Bad filter method: %d\n", filter);
                    return false;
                }

                interlace = *srcbuf++;
                
                // transmission order of image
                if (interlace > 1)
                {
                    fprintf(stderr, "PNG, Bad interlace method: %d\n", interlace);
                    return false;
                }

            } break;
            case gAMA:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                // the gAMA chunk must precede the first IDAT chunk.
                if (dattest)
                {
                    fprintf(stderr, "PNG, gAMA after IDAT.\n");
                    return false;
                }

                // the gAMA chunk must precede the first IDAT chunk.
                if (paltest)
                {
                    fprintf(stderr, "PNG, gAMA after PLTE.\n");
                    return false;
                }

                gamma = ReadU32FromBE(srcbuf); srcbuf += 4;
            } break;
            case PLTE:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                // the PLTE chunk must precede the first IDAT chunk.
                if (dattest)
                {
                    fprintf(stderr, "PNG, PLTE after IDAT.\n");
                    return false;
                }

                // the PLTE chunk contains from 1 to 256 palette entries, each
                // three-bytes
                // the number of entries is determined from the chunk length.
                if (size > 768)
                {
                    fprintf(stderr, "PNG, Invalid PLTE length: %d\n", size);
                    return false;
                }

                // a chunk length not divisible by 3 is an error.
                palnum = size / 3;

                if ((palnum * 3) != size)
                {    
                    fprintf(stderr, "PNG, Invalid PLTE entries: %d\n", palnum);
                    return false;
                }

                if (paltest == false)
                {
                    paltest = true;
                }

                // the palette chunk must appear for color type 3, and can appear for color types
                // 2 and 6; it must not appear for color types 0 and 4.
                if (pdstpalette != NULL)
                {
                    for (uint32_t i = 0; i < palnum; ++i)
                    {
                        pdstpalette->data[i].r = *srcbuf++;
                        pdstpalette->data[i].g = *srcbuf++;
                        pdstpalette->data[i].b = *srcbuf++;
                        pdstpalette->data[i].a = 255;
                    }

                    pdstpalette->size = palnum;
                    pdstpalette->bits = 24;
                }

            } break;
            case tRNS:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                // the tRNS chunk must precede the first IDAT chunk.
                if (dattest)
                {
                    fprintf(stderr, "PNG, tRNS after IDAT.\n");
                    return false;
                }

                // palette
                if (colortype == 3)
                {
                    // the tRNS chunk must follow the PLTE chunk, if any.
                    if (palnum == 0)
                    {
                        fprintf(stderr, "PNG, tRNS before PLTE.\n");
                        return false;
                    }

                    if (size > palnum)
                    {
                        fprintf(stderr, "PNG, Bad tRNS len: %d\n", size);
                        return false;
                    }

                    if (pdstpalette != NULL)
                    {
                        for (uint32_t i = 0; i < size; ++i)
                        {
                            pdstpalette->data[i].a = *srcbuf++;
                        }

                        pdstpalette->bits = 32;
                    }
                }
                else 
                {
                    // check for transparency in images with alpha
                    // grayscale + alpha or rgba
                    if (colortype == 4 || colortype == 6)
                    {
                        fprintf(stderr, "PNG, tRNS with alpha.\n");
                        return false;
                    }

                    for (uint32_t i = 0; i < bytesperpixel; ++i)
                    {
                        colorkey[i] = (uint8_t)(ReadU16FromBE(srcbuf) & 255);
                        srcbuf += 2;
                    }

                    if (pcolorkey != NULL)
                    {
                        pcolorkey->r = colorkey[0];
                        pcolorkey->g = colorkey[1];
                        pcolorkey->b = colorkey[2];
                        pcolorkey->a = 255;
                    }
                }
            } break;
            case IDAT:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                if (dattest == false)
                {
                    dattest = true;
                }

                idatlen += size; srcbuf += size;
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
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                srcbuf += size;
            } break;
            case IEND:
            {
                // end of PNG chunks
                endfound = true;
            } break;
            default:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                char fourcc[5] = {};
                fourcc[0] = (uint8_t)((type >> 24) & 0xFF);
                fourcc[1] = (uint8_t)((type >> 16) & 0xFF);
                fourcc[2] = (uint8_t)((type >>  8) & 0xFF);
                fourcc[3] = (uint8_t)((type >>  0) & 0xFF);
                fourcc[4] = '\0';
                
                fprintf(stderr, "PNG, Chunk not supported: %s\n", fourcc);

                srcbuf += size;
            } break;
        }

        // skip CRC
        uint32_t crc = ReadU32FromBE(srcbuf); srcbuf += 4;
    }

    // dst stuff
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
        }
    }

    float dstpixelsperbyte = PIXELS_PER_BYTE(depth * bytesperpixel);
    int32_t dstpitch = (int32_t)(ceilf((float)(xsize) / dstpixelsperbyte) + 1) & ~1;

    // idat chunks
    uint8_t* idatptr = (uint8_t*)malloc(((idatlen + 1) & ~1));
    uint8_t* idatbuf = idatptr;

    if (idatptr == NULL)
    {
        fprintf(stderr, "PNG, Out of memory\n");
        return false;
    }

    memset(idatptr, 0, ((idatlen + 1) & ~1));

    srcbuf = srcptr + 8;            // reset current to begining and skip signature
    endfound = false;

    while (srcbuf < srcend && endfound == false)
    {
        // chunk length and type
        uint32_t size = ReadU32FromBE(srcbuf); srcbuf += 4;
        uint32_t type = ReadU32FromBE(srcbuf); srcbuf += 4;

        switch (BigU32(type))
        {
            case IDAT:
            {
                memcpy(idatbuf, srcbuf, size);
                idatbuf += size;
                srcbuf  += size;
            } break;
            case IEND:
            {
                endfound = true;
            } break;
            default:
            {
                srcbuf += size;
            } break;
        }

        // skip CRC
        srcbuf += 4;
    }

    idatbuf = idatptr;            // reset current to begining

    uint32_t bytesencoded = 0;
    uint32_t bytesdecoded = 0;

    // ZLIB decompression
    z_stream inflator = {};
    int status = inflateInit(&inflator);

    if (status != Z_OK)
    {
        fprintf(stderr, "PNG, Inflate: failed with status %i.\n", status);

        free(idatptr);
        idatptr = NULL;

        return false;
    }

    // inflate data, bytes per line, per component, plus filter per row
    uint32_t odatlen = (ysize * (dstpitch + 1)) + ((interlace > 0) ? ysize : 0);
    uint8_t* odatptr = (uint8_t*)malloc(((odatlen + 1) & ~1));
    uint8_t* odatbuf = odatptr;

    if (odatptr == NULL)
    {
        fprintf(stderr, "PNG, Out of memory\n");

        free(idatptr);
        idatptr = NULL;

        return false;
    }

    memset(odatptr, 0, ((odatlen + 1) & ~1));

    unsigned int oabsrem = odatlen - inflator.total_out;         // absolute remaining output
    unsigned int odatrem = 0;         // relative remaining output
    unsigned int iabsrem = idatlen - inflator.total_in;         // absolute remaining input
    unsigned int idatrem = 0;         // relative remaining input

    idatrem = MIN(32768, idatlen);
    odatrem = MIN(32768, odatlen);

    do
    {
        inflator.next_in    = idatbuf;
        inflator.avail_in   = idatrem;
        inflator.next_out   = odatbuf;
        inflator.avail_out  = odatrem;

        status = inflate(&inflator, ((iabsrem > 0) ? Z_NO_FLUSH : Z_FINISH));

        // input
        iabsrem = idatlen - inflator.total_in;
        idatbuf = idatptr + inflator.total_in;
        idatrem = MIN(32768, iabsrem);
        bytesdecoded = inflator.total_in;

        // output
        oabsrem = odatlen - inflator.total_out;
        odatbuf = odatptr + inflator.total_out;
        odatrem = MIN(32768, oabsrem);
        bytesencoded = inflator.total_out;

        if (status == Z_STREAM_END)
        {
            fprintf(stdout, "PNG, Inflate: completed successfully\n");
            break;
        }

        if (odatrem == 0)
        {
            fprintf(stderr, "PNG, Inflate: out of output data\n");
            break;
        }
    } while (1);

    inflateEnd(&inflator);

    // free compressed data
    free(idatptr);
    idatptr = NULL;

    // decompression failed
    if (status != Z_STREAM_END)
    {
        fprintf(stderr, "PNG, Inflate: failed with status %i.\n", status);

        // free un-compressed data and return
        free(odatptr);
        odatptr = NULL;

        return false;
    }

    // pixels
    uint32_t pixlen = xsize * ysize * bytesperpixel;
    uint8_t* pixels = (uint8_t*)malloc(pixlen);
    uint8_t* rawptr = pixels;
    uint8_t* rawbuf = pixels;
    uint32_t pitch = xsize * bytesperpixel;

    if (pixels == NULL)
    {
        fprintf(stderr, "PNG, Out of memory\n");

        free(odatptr);
        odatptr = NULL;

        return false;
    }

    memset(pixels, 0, pixlen);

    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL)
    {
        // color-type interpretation of the image and number of
        // bytes-per-pixel
        switch (colortype)
        {
            case 0:         // grayscale
            {
                *srcdepth = depth;
            } break;
            case 2:         // rgb
            {
                *srcdepth = 24;
            } break;
            case 3:         // palette
            {
                *srcdepth = depth;
            } break;
            case 4:         // grayscale + alpha
            {
                *srcdepth = 16;
            } break;
            case 6:         // rgba
            {
                *srcdepth = 32;
            } break;
        }
    }

    uint32_t datlen = (ysize * (dstpitch + 1)) + ((interlace > 0) ? ysize : 0);
    uint8_t* datptr = (uint8_t*)malloc(((datlen + 1) & ~1));

    if (datptr == NULL)
    {
        fprintf(stderr, "PNG, Out of memory\n");

        free(pixels);
        pixels = NULL;

        free(odatptr);
        odatptr = NULL;

        return false;
    }

    memcpy(datptr, odatptr, datlen);          // filtered image data
    uint8_t* datbuf = datptr;

    free(odatptr);
    odatptr = NULL;

    // interlace and filter
    if (interlace == 1)
    {
        ExpandInterlacedPNG(rawptr, pixlen, xsize, ysize, pitch, depth,
            bytesperpixel, &datbuf);
    }
    else
    {
        ExpandPNG(rawbuf, pixlen, 1, pitch, xsize, ysize, depth, bytesperpixel,
            &datbuf);
    }

    free(datptr);
    datptr = NULL;

    uint8_t scale = 0x01;

    switch (depth)
    {
        case 1:
        {
            scale = 0xFF;
        } break;
        case 2:
        {
            scale = 0x55;
        } break;
        case 4:
        {
            scale = 0x11;
        } break;
    }

    uint8_t* ppix = pixels;

    // grayscale
    if (colortype == 0 && (depth == 1 || depth == 2 || depth == 4))
    {
        for (unsigned int i = 0; i < xsize * ysize; ++i)
        {
            *ppix++ *= scale;
        }
    }

    // grayscale + alpha
    if (colortype == 4 && (depth == 1 || depth == 2 || depth == 4))
    {
        for (unsigned int i = 0; i < xsize * ysize; ++i)
        {
            *ppix++ *= scale;
            *ppix++ *= scale;
        }
    }

    return true;
}

#endif // #ifndef _PNG_H_


#ifndef _TGA_H_
#define _TGA_H_


//-----------------------------------------------------------------------------
//
//  Truevision TGA File Format
//
//  8, 16, 24 and 32-bit color mapped and rle parsing
//
//-----------------------------------------------------------------------------


#define TGA_NO_IMAGE_DATA           0
#define TGA_MAPPED                  1
#define TGA_RGB                     2
#define TGA_BLACK_AND_WHITE         3
#define TGA_MAPPED_RLE              9
#define TGA_RGB_RLE                 10
#define TGA_BLACK_AND_WHITE_RLE     11

typedef struct _tga_file
{
    uint8_t  id_length;
    uint8_t  colormap_type;
    uint8_t  image_type;
    uint16_t colormap_index;
    uint16_t colormap_length;
    uint8_t  colormap_size;
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t  pixel_size;
    uint8_t  image_descriptor;
} tga_file_t;

static const uint32_t s_tga_file_size = 18;

//------------------------------------------------------------------------------
// SaveTGA
//------------------------------------------------------------------------------
static bool
SaveToMemoryTGA(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec, uint8_t* psrc,
    uint32_t srcxsize, uint32_t srcysize, uint8_t srcdepth, palette_t* psrcpalette,
    bool invertX, bool invertY)
{
    if (ppdst == NULL || ppdstsize == 0)
    {
        fprintf(stderr, "TGA, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL)
    {
        fprintf(stderr, "TGA, Invalid src data.\n");
        // return false;
    }

    if (srcdepth != 8 && srcdepth != 16 && srcdepth != 24 && srcdepth != 32)
    {
        fprintf(stderr, "TGA, Unsupported depth: %d.\n", srcdepth);
        return false;
    }

    uint32_t xextent = srcxsize;
    uint32_t yextent = srcysize;

    // dst stuff
    uint8_t dstdepth = srcdepth;
    uint32_t dstbytesperpixel = dstdepth >> 3;
    uint32_t dstpitch = xextent * dstbytesperpixel;

    // src stuff
    uint32_t srcbytesperpixel = srcdepth >> 3;
    int32_t srcpitch = srcxsize * srcbytesperpixel;
    uint8_t* rawptr = psrc;
    uint8_t* rawbuf = psrc;

    uint8_t id_length = 0;
    uint8_t colormap_type = (psrcpalette != NULL) ? 1 : 0;
    uint8_t image_type = TGA_NO_IMAGE_DATA;
    uint16_t colormap_index = 0;
    uint16_t colormap_length = 0;
    uint8_t colormap_size = 0;
    uint8_t pixel_size = dstdepth;
    uint8_t image_descriptor = 0;
    
    // palette
    if (srcdepth == 8)
    {
        if (psrcpalette != NULL)
        {
            colormap_length = psrcpalette->size;
            colormap_size = psrcpalette->bits;
        }
    }

    if (psrc == NULL)
    {
        image_type = TGA_NO_IMAGE_DATA;
    }
    else
    {
        if (codec == ENCODE_RLE)
        {
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
                    if (psrcpalette == NULL)
                    {
                        image_type = TGA_BLACK_AND_WHITE_RLE;
                    }
                    else
                    {
                        image_type = TGA_MAPPED_RLE;
                    }
                } break;
            }
        }
        else            // ENCODE_RGB
        {
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
                    if (psrcpalette == NULL)
                    {
                        image_type = TGA_BLACK_AND_WHITE;
                    }
                    else
                    {
                        image_type = TGA_MAPPED;
                    }
                } break;
            }
        }
    }

    uint32_t bytesencoded = 0;
    // big array for true-color images
    uint32_t datasize = s_tga_file_size + ((yextent * dstpitch) * 2) + colormap_length;
    uint8_t* data = (uint8_t*)malloc(datasize);

    if (data == NULL)
    {
        fprintf(stderr, "TGA, Out of memory\n");
        return false;
    }

    memset(data, 0, datasize);

    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;
    
    // fill in file header
    tga_file_t tgafile = {};

    tgafile.id_length = id_length;
    tgafile.colormap_type = colormap_type;
    tgafile.image_type = image_type;
    tgafile.colormap_index = colormap_index;
    tgafile.colormap_length = colormap_length;
    tgafile.colormap_size = colormap_size;
    tgafile.x_origin = (invertX == true ? 0 : xextent);
    tgafile.y_origin = (invertY == true ? 0 : yextent);
    tgafile.width = xextent;
    tgafile.height = yextent;
    tgafile.pixel_size = pixel_size;
    tgafile.image_descriptor = image_descriptor;

    *dstbuf++ = tgafile.id_length;
    *dstbuf++ = tgafile.colormap_type;
    *dstbuf++ = tgafile.image_type;
    WriteU16ToLE(dstbuf, tgafile.colormap_index);   dstbuf += 2;
    WriteU16ToLE(dstbuf, tgafile.colormap_length);  dstbuf += 2;
    *dstbuf++ = tgafile.colormap_size;
    WriteU16ToLE(dstbuf, tgafile.x_origin);         dstbuf += 2;
    WriteU16ToLE(dstbuf, tgafile.y_origin);         dstbuf += 2;
    WriteU16ToLE(dstbuf, tgafile.width);            dstbuf += 2;
    WriteU16ToLE(dstbuf, tgafile.height);           dstbuf += 2;
    *dstbuf++ = tgafile.pixel_size;
    *dstbuf++ = tgafile.image_descriptor;

    dstbuf += id_length;            // skip the image descriptor 
    
    if (colormap_size != 0 && psrcpalette != NULL)
    {
        // fill in the palette
        if (colormap_size == 15 || colormap_size == 16)
        {
            uint16_t rgb16 = 0;
            uint16_t r = 0;
            uint16_t g = 0;
            uint16_t b = 0;

            for (uint16_t i = colormap_index; i < colormap_length; ++i)
            {
                r = (psrcpalette->data[i].r * 0x1F) / 0xFF;
                g = (psrcpalette->data[i].g * 0x1F) / 0xFF;
                b = (psrcpalette->data[i].b * 0x1F) / 0xFF;
                
                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                WriteU16ToLE(dstbuf, rgb16); dstbuf += 2;
            }
        }
        else if (colormap_size == 24)
        {            
            for (uint16_t i = colormap_index; i < colormap_length; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].b;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].r;
            }
        }
        else if (colormap_size == 32)
        {            
            for (uint16_t i = colormap_index; i < colormap_length; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].b;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].r;
                *dstbuf++ = psrcpalette->data[i].a;
            }
        }
    }

    if (psrc != NULL)
    {
        uint32_t x = 0;
        uint32_t y = 0;

        if (codec == ENCODE_RLE)            // run-length encoding
        {
            uint32_t maxrle = 0x80;
            uint32_t maxabs = 0xFF;
            uint32_t abscount = 0;
            uint32_t rlecount = 0;
            uint32_t rlevalue = 0;
            int32_t absrem = 0;
            int32_t rlerem = 0;
            uint8_t sample0 = 0;
            uint8_t sample1 = 0;
            uint8_t sample2 = 0;
            uint8_t sample3 = 0;
            uint8_t sample4 = 0;
            uint8_t sample5 = 0;
            uint8_t sample6 = 0;
            uint8_t sample7 = 0;
            uint8_t b = 0;
            uint8_t g = 0;
            uint8_t r = 0;
            uint8_t a = 0;

            switch (image_type)
            {
            case TGA_RGB_RLE:
            {
                if (srcdepth == 32)
                {
                    while (y++ < yextent)
                    {
                        rawbuf = rawptr;
                        x = 0;

                        while (x < xextent)
                        {
                            sample0 = 0;
                            sample1 = 0;
                            sample2 = 0;
                            sample3 = 0;
                            sample4 = 0;
                            sample5 = 0;
                            sample6 = 0;
                            sample7 = 0;

                            b = *(rawbuf + ((x * 4) + 0));
                            g = *(rawbuf + ((x * 4) + 1));
                            r = *(rawbuf + ((x * 4) + 2));
                            a = *(rawbuf + ((x * 4) + 3));

                            abscount = 1;
                            absrem = xextent - (x + abscount);

                            sample4 = b;
                            sample5 = g;
                            sample6 = r;
                            sample7 = a;

                            while (absrem > 0 && abscount < maxabs)
                            {
                                sample0 = sample4;
                                sample1 = sample5;
                                sample2 = sample6;
                                sample3 = sample7;

                                sample4 = *(rawbuf + (((x + abscount) * 4) + 0));
                                sample5 = *(rawbuf + (((x + abscount) * 4) + 1));
                                sample6 = *(rawbuf + (((x + abscount) * 4) + 2));
                                sample7 = *(rawbuf + (((x + abscount) * 4) + 3));

                                if (sample0 == sample4 &&
                                    sample1 == sample5 &&
                                    sample2 == sample6 &&
                                    sample3 == sample7)
                                {
                                    break;
                                }

                                abscount++;
                                absrem--;
                            }

                            rlecount = 1;
                            rlerem = xextent - (x + rlecount);

                            sample4 = b;
                            sample5 = g;
                            sample6 = r;
                            sample7 = a;

                            while (rlerem > 0 && rlecount < maxrle)
                            {
                                sample0 = sample4;
                                sample1 = sample5;
                                sample2 = sample6;
                                sample3 = sample7;

                                sample4 = *(rawbuf + (((x + rlecount) * 4) + 0));
                                sample5 = *(rawbuf + (((x + rlecount) * 4) + 1));
                                sample6 = *(rawbuf + (((x + rlecount) * 4) + 2));
                                sample7 = *(rawbuf + (((x + rlecount) * 4) + 3));

                                if (sample0 != sample4 ||
                                    sample1 != sample5 ||
                                    sample2 != sample6 ||
                                    sample3 != sample7)
                                {
                                    break;
                                }

                                rlecount++;
                                rlerem--;
                            }

                            if (abscount >= rlecount)
                            {
                                rlevalue = abscount;

                                *dstbuf++ = (0 << 7) | (rlevalue - 1);
                                bytesencoded++;

                                for (uint32_t i = 0; i < rlevalue; ++i)
                                {
                                    *dstbuf++ = *(rawbuf + ((x * 4) + i) + 0);
                                    *dstbuf++ = *(rawbuf + ((x * 4) + i) + 1);
                                    *dstbuf++ = *(rawbuf + ((x * 4) + i) + 2);
                                    *dstbuf++ = *(rawbuf + ((x * 4) + i) + 3);
                                    bytesencoded++;
                                    bytesencoded++;
                                    bytesencoded++;
                                    bytesencoded++;
                                }
                            }
                            else
                            {
                                rlevalue = rlecount;

                                *dstbuf++ = (1 << 7) | (rlevalue - 1);
                                bytesencoded++;

                                *dstbuf++ = sample0;
                                *dstbuf++ = sample1;
                                *dstbuf++ = sample2;
                                *dstbuf++ = sample3;
                                bytesencoded++;
                                bytesencoded++;
                                bytesencoded++;
                                bytesencoded++;
                            }

                            x += rlevalue;
                        }

                        if (y != yextent)
                        {
                            rawptr += srcpitch;
                        }
                    }
                }
                else if (srcdepth == 24)
                {
                    while (y++ < yextent)
                    {
                        rawbuf = rawptr;
                        x = 0;

                        while (x < xextent)
                        {
                            sample0 = 0;
                            sample1 = 0;
                            sample2 = 0;
                            sample3 = 0;
                            sample4 = 0;
                            sample5 = 0;

                            b = *(rawbuf + ((x * 3) + 0));
                            g = *(rawbuf + ((x * 3) + 1));
                            r = *(rawbuf + ((x * 3) + 2));

                            abscount = 1;
                            absrem = xextent - (x + abscount);
                            sample3 = b;
                            sample4 = g;
                            sample5 = r;

                            while (absrem > 0 && abscount < maxabs)
                            {
                                sample0 = sample3;
                                sample1 = sample4;
                                sample2 = sample5;

                                sample3 = *(rawbuf + (((x + abscount) * 3) + 0));
                                sample4 = *(rawbuf + (((x + abscount) * 3) + 1));
                                sample5 = *(rawbuf + (((x + abscount) * 3) + 2));

                                if (sample0 == sample3 &&
                                    sample1 == sample4 &&
                                    sample2 == sample5)
                                {
                                    break;
                                }

                                abscount++;
                                absrem--;
                            }

                            rlecount = 1;
                            rlerem = xextent - (x + rlecount);
                            sample3 = b;
                            sample4 = g;
                            sample5 = r;

                            while (rlerem > 0 && rlecount < maxrle)
                            {
                                sample0 = sample3;
                                sample1 = sample4;
                                sample2 = sample5;

                                sample3 = *(rawbuf + (((x + rlecount) * 3) + 0));
                                sample4 = *(rawbuf + (((x + rlecount) * 3) + 1));
                                sample5 = *(rawbuf + (((x + rlecount) * 3) + 2));

                                if (sample0 != sample3 ||
                                    sample1 != sample4 ||
                                    sample2 != sample5)
                                {
                                    break;
                                }

                                rlecount++;
                                rlerem--;
                            }

                            if (abscount >= rlecount)
                            {
                                rlevalue = abscount;

                                *dstbuf++ = (0 << 7) | (rlevalue - 1);
                                bytesencoded++;

                                for (uint32_t i = 0; i < rlevalue; ++i)
                                {
                                    *dstbuf++ = *(rawbuf + ((x * 3) + i) + 0);
                                    *dstbuf++ = *(rawbuf + ((x * 3) + i) + 1);
                                    *dstbuf++ = *(rawbuf + ((x * 3) + i) + 2);
                                    bytesencoded++;
                                    bytesencoded++;
                                    bytesencoded++;
                                }
                            }
                            else
                            {
                                rlevalue = rlecount;

                                *dstbuf++ = (1 << 7) | (rlevalue - 1);
                                bytesencoded++;

                                *dstbuf++ = sample0;
                                *dstbuf++ = sample1;
                                *dstbuf++ = sample2;

                                bytesencoded++;
                                bytesencoded++;
                                bytesencoded++;
                            }

                            x += rlevalue;
                        }

                        if (y != yextent)
                        {
                            rawptr += srcpitch;
                        }
                    }
                }
                else if (srcdepth == 16)
                {
                    while (y++ < yextent)
                    {
                        rawbuf = rawptr;
                        x = 0;

                        while (x < xextent)
                        {
                            sample0 = 0;
                            sample1 = 0;
                            sample2 = 0;

                            sample2 |= *(rawbuf + ((x * 2) + 0));
                            sample2 |= *(rawbuf + ((x * 2) + 1)) << 8;

                            abscount = 1;
                            absrem = xextent - (x + abscount);
                            sample1 = sample2;

                            while (absrem > 0 && abscount < maxabs)
                            {
                                sample0 = sample1;
                                sample1 = 0;

                                sample1 |= *(rawbuf + ((x + abscount) * 2) + 0);
                                sample1 |= *(rawbuf + ((x + abscount) * 2) + 1);

                                if (sample0 == sample1)
                                {
                                    break;
                                }

                                abscount++;
                                absrem--;
                            }

                            rlecount = 1;
                            rlerem = xextent - (x + rlecount);
                            sample1 = sample2;

                            while (rlerem > 0 && rlecount < maxrle)
                            {
                                sample0 = sample1;
                                sample1 = 0;

                                sample1 |= *(rawbuf + ((x + rlecount) * 2) + 0);
                                sample1 |= *(rawbuf + ((x + rlecount) * 2) + 1);

                                if (sample0 != sample1)
                                {
                                    break;
                                }

                                rlecount++;
                                rlerem--;
                            }

                            if (abscount >= rlecount)
                            {
                                rlevalue = abscount;

                                *dstbuf++ = (0 << 7) | (rlevalue - 1);
                                bytesencoded++;

                                for (uint32_t i = 0; i < rlecount; ++i)
                                {
                                    *dstbuf++ = *(rawbuf + ((x * 2) + i) + 0);
                                    *dstbuf++ = *(rawbuf + ((x * 2) + i) + 1);
                                    bytesencoded++;
                                    bytesencoded++;
                                }
                            }
                            else
                            {
                                rlevalue = rlecount;

                                *dstbuf++ = (1 << 7) | (rlevalue - 1);
                                bytesencoded++;

                                *dstbuf++ = (sample0 & 0xFF00);
                                *dstbuf++ = (sample0 & 0x00FF);
                                bytesencoded++;
                                bytesencoded++;
                                bytesencoded++;
                            }

                            x += rlevalue;
                        }

                        if (y != yextent)
                        {
                            rawptr += srcpitch;
                        }
                    }
                }
            } break;
            case TGA_MAPPED_RLE:
            case TGA_BLACK_AND_WHITE_RLE:
            {
                while (y++ < yextent)
                {
                    rawbuf = rawptr;
                    x = 0;

                    while (x < xextent)
                    {
                        sample0 = 0;
                        sample1 = 0;
                        sample2 = *(rawbuf + x);

                        abscount = 1;
                        absrem = xextent - (x + abscount);
                        sample1 = sample2;

                        while (absrem > 0 && abscount < maxabs)
                        {
                            sample0 = sample1;
                            sample1 = *(rawbuf + x + abscount);

                            if (sample0 == sample1)
                            {
                                break;
                            }

                            abscount++;
                            absrem--;
                        }

                        rlecount = 1;
                        rlerem = xextent - (x + rlecount);
                        sample1 = sample2;

                        while (rlerem > 0 && rlecount < maxrle)
                        {
                            sample0 = sample1;
                            sample1 = sample1 = *(rawbuf + x + rlecount);

                            if (sample0 != sample1)
                            {
                                break;
                            }

                            rlecount++;
                            rlerem--;
                        }

                        if (abscount >= rlecount)
                        {
                            rlevalue = abscount;

                            *dstbuf++ = (0 << 7) | (rlevalue - 1);
                            bytesencoded++;

                            for (uint32_t i = 0; i < rlecount; ++i)
                            {
                                *dstbuf++ = *(rawbuf + x + i);
                                bytesencoded++;
                            }
                        }
                        else
                        {
                            rlevalue = rlecount;

                            *dstbuf++ = (1 << 7) | (rlevalue - 1);
                            *dstbuf++ = sample0;
                            bytesencoded++;
                            bytesencoded++;
                        }

                        x += rlevalue;
                    }

                    if (y != yextent)
                    {
                        rawptr += srcpitch;
                    }
                }
            } break;
            }
        }
        else            // everything else
        {
            memcpy(dstbuf, rawbuf, yextent * srcpitch);
            dstbuf += yextent * srcpitch;
            bytesencoded += yextent * srcpitch;
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
    uint8_t* srcdepth, uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < s_tga_file_size)
    {
        return false;
    }

    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;

    tga_file_t tgafile = {};

    // file struct
    tgafile.id_length           = *srcbuf++;            // id length
    tgafile.colormap_type       = *srcbuf++;            // colormap_type
    tgafile.image_type          = *srcbuf++;            // image_type
    tgafile.colormap_index      = ReadU16FromLE(srcbuf); srcbuf += 2;           // colormap_index
    tgafile.colormap_length     = ReadU16FromLE(srcbuf); srcbuf += 2;           // colormap_length
    tgafile.colormap_size       = *srcbuf++;          // colormap_size
    tgafile.x_origin            = ReadU16FromLE(srcbuf); srcbuf += 2;           // x_origin
    tgafile.y_origin            = ReadU16FromLE(srcbuf); srcbuf += 2;           // y_origin
    tgafile.width               = ReadU16FromLE(srcbuf); srcbuf += 2;           // width
    tgafile.height              = ReadU16FromLE(srcbuf); srcbuf += 2;           // height
    tgafile.pixel_size          = *srcbuf++;            // pixel_size
    tgafile.image_descriptor    = *srcbuf++;            // image_descriptor

    if (tgafile.colormap_type == 1)
    {
        if (tgafile.image_type != TGA_MAPPED &&
            tgafile.image_type != TGA_MAPPED_RLE)
        {
            return false;
        }
        else if (tgafile.colormap_size !=  8 &&
                 tgafile.colormap_size != 15 &&
                 tgafile.colormap_size != 16 &&
                 tgafile.colormap_size != 24 &&
                 tgafile.colormap_size != 32)
        {
            return false;
        }
    }
    else if (tgafile.colormap_type == 0)
    {
        if (tgafile.image_type != TGA_RGB &&
            tgafile.image_type != TGA_RGB_RLE &&
            tgafile.image_type != TGA_BLACK_AND_WHITE &&
            tgafile.image_type != TGA_BLACK_AND_WHITE_RLE)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    uint32_t xsize = tgafile.width;            // image width
    uint32_t ysize = tgafile.height;           // image height
    uint8_t depth = tgafile.pixel_size;
    uint8_t colormap = tgafile.colormap_type;

    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = depth; }
    if (srccolormap != NULL) { *srccolormap = colormap; }

    return true;
}

//-----------------------------------------------------------------------------
// LoadTGA
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryTGA(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < s_tga_file_size)
    {
        fprintf(stderr, "TGA, Not a valid bitmap.\n");
        return false;
    }

    // src stuff
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;

    tga_file_t tgafile = {};

    // file struct
    tgafile.id_length           = *srcbuf++;            // id length
    tgafile.colormap_type       = *srcbuf++;            // colormap_type
    tgafile.image_type          = *srcbuf++;            // image_type
    tgafile.colormap_index      = ReadU16FromLE(srcbuf); srcbuf += 2;           // colormap_index
    tgafile.colormap_length     = ReadU16FromLE(srcbuf); srcbuf += 2;           // colormap_length
    tgafile.colormap_size       = *srcbuf++;          // colormap_size
    tgafile.x_origin            = ReadU16FromLE(srcbuf); srcbuf += 2;           // x_origin
    tgafile.y_origin            = ReadU16FromLE(srcbuf); srcbuf += 2;           // y_origin
    tgafile.width               = ReadU16FromLE(srcbuf); srcbuf += 2;           // width
    tgafile.height              = ReadU16FromLE(srcbuf); srcbuf += 2;           // height
    tgafile.pixel_size          = *srcbuf++;            // pixel_size
    tgafile.image_descriptor    = *srcbuf++;            // image_descriptor

    if (tgafile.pixel_size !=  8 && tgafile.pixel_size != 16 &&
        tgafile.pixel_size != 24 && tgafile.pixel_size != 32)
    {
        fprintf(stderr, "TGA, Unsupported bits: %d.\n", tgafile.pixel_size);
        return false;
    }

    if (tgafile.colormap_type == 1)
    {
        if (tgafile.image_type != TGA_MAPPED &&
            tgafile.image_type != TGA_MAPPED_RLE)
        {
            fprintf(stderr, "TGA, Colour map type and image type mismatch.\n");
            return false;
        }
        else if (tgafile.colormap_size !=  8 &&
                 tgafile.colormap_size != 15 &&
                 tgafile.colormap_size != 16 &&
                 tgafile.colormap_size != 24 &&
                 tgafile.colormap_size != 32)
        {
            fprintf(stderr, "TGA, Colour map type and colour map size mismatch.\n");
            return false;
        }
    }
    else if (tgafile.colormap_type == 0)
    {
        if (tgafile.image_type != TGA_RGB &&
            tgafile.image_type != TGA_RGB_RLE &&
            tgafile.image_type != TGA_BLACK_AND_WHITE &&
            tgafile.image_type != TGA_BLACK_AND_WHITE_RLE)
        {
            fprintf(stderr, "TGA, Colour map type and image type mismatch.\n");
            return false;
        }
    }
    else
    {
        fprintf(stderr, "TGA, Unsupported colour map type: %d.\n", tgafile.colormap_type);
        return false;
    }

    if (tgafile.colormap_type == 1 &&
       (tgafile.image_type == TGA_MAPPED || tgafile.image_type == TGA_MAPPED_RLE))
    {
        if (tgafile.pixel_size == 8)
        {
            uint8_t* palptr = srcbuf + tgafile.id_length;
            uint32_t palnum = tgafile.colormap_length < 256 ?
                tgafile.colormap_length : 256;
            
            if (pdstpalette != NULL)
            {
                if (tgafile.colormap_size == 15 || tgafile.colormap_size == 16)
                {
                    uint16_t pixel = 0;

                    for (size_t i = 0; i < palnum; ++i)
                    {
                        pixel = 0;
                        pixel |= *palptr++;
                        pixel |= *palptr++ << 8;
                        
                        pdstpalette->data[i].b = (((pixel >>  0) & 0x1F) * 0xFF) / 0x1F;
                        pdstpalette->data[i].g = (((pixel >>  5) & 0x1F) * 0xFF) / 0x1F;
                        pdstpalette->data[i].r = (((pixel >> 10) & 0x1F) * 0xFF) / 0x1F;
                        pdstpalette->data[i].a = 255;
                    }
                    
                    pdstpalette->size = palnum;
                    pdstpalette->bits = 24;
                }
                else if (tgafile.colormap_size == 24)
                {
                    for (size_t i = 0; i < palnum; ++i)
                    {
                        pdstpalette->data[i].b = *palptr++;
                        pdstpalette->data[i].g = *palptr++;
                        pdstpalette->data[i].r = *palptr++;
                        pdstpalette->data[i].a = 255;
                    }
                    
                    pdstpalette->size = palnum;
                    pdstpalette->bits = 24;
                }
                else if (tgafile.colormap_size == 32)
                {
                    for (size_t i = 0; i < palnum; ++i)
                    {
                        pdstpalette->data[i].b = *palptr++;
                        pdstpalette->data[i].g = *palptr++;
                        pdstpalette->data[i].r = *palptr++;
                        pdstpalette->data[i].a = *palptr++;
                    }
                    
                    pdstpalette->size = palnum;
                    pdstpalette->bits = 32;
                }
            }
        }
    }

    // skip the id and palette
    srcbuf = srcbuf + tgafile.id_length + tgafile.colormap_length *
        ((tgafile.colormap_size==15 ? 16 : tgafile.colormap_size) >> 3);

    // dst stuff
    uint32_t xsize = tgafile.width;
    uint32_t ysize = tgafile.height;
    uint32_t bytesperpixel = tgafile.pixel_size >> 3;
    uint32_t pitch = xsize * bytesperpixel;
    uint32_t rlevalue = 0;
    uint32_t rlecount = 0;
    uint8_t rgba[4] = {};

    uint8_t* pixels = (uint8_t*)malloc(xsize * ysize * bytesperpixel);
    uint8_t* rawptr = pixels;           // start of current dst row
    uint8_t* rawbuf = pixels;           // current dst row

    if (pixels == NULL)
    {
        fprintf(stderr, "TGA: Out of memory\n");
        return false;
    }

    memset(pixels, 0, xsize * ysize * bytesperpixel);
    
    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = tgafile.pixel_size; }

    switch (tgafile.image_type)
    {
        case TGA_MAPPED_RLE:
        case TGA_RGB_RLE:
        case TGA_BLACK_AND_WHITE_RLE:
        {
            uint32_t y = 0;
            uint32_t x = 0;
            
            while (y < ysize)
            {
                rawbuf = rawptr;
                rlecount = 0;
                rlevalue = 0;
                x = 0;

                while (x < xsize)
                {
                    if (rlecount == 0)          // get rle count
                    {
                        rlevalue = *srcbuf++;
                        rlecount = (rlevalue & 0x7F) + 1;

                        for (uint32_t i = 0; i < bytesperpixel; ++i)
                        {
                            rgba[i] = *srcbuf++;
                        }
                    }
                    else if (!(rlevalue & 0x80))
                    {
                        // if this is not a repeating count read next pixel of
                        // component size
                        for (uint32_t i = 0; i < bytesperpixel; ++i)
                        {
                            rgba[i] = *srcbuf++;
                        }
                    }
                    x++;

                    memcpy(rawbuf, rgba, bytesperpixel);
                    rawbuf += bytesperpixel;
                    rlecount--;
                }
                y++;

                if (y != ysize)
                {
                    rawptr += pitch;
                }
            }
        } break;
        case TGA_MAPPED:
        case TGA_RGB:
        case TGA_BLACK_AND_WHITE:
        {
            memcpy(rawbuf, srcbuf, ysize * pitch);
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
#define BI_BITFIELDS    3L          // introduced in version 4.0
#endif // #ifdef _WIN32

#define BMP_IDENTIFIER          0x4D42

typedef struct _bmp_file
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} bmp_file_t;

typedef struct _bmp_v3_info
{
    uint32_t size;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t  xresolution;
    int32_t  yresolution;
    uint32_t num_colours;
    uint32_t num_colour_indexes;
} bmp_v3_info_t;

static const uint32_t s_bmp_file_size = 14;
static const uint32_t s_bmp_v3_info_size = 40;

//------------------------------------------------------------------------------
// SaveBMP
// 
// FIXME: For run-length encoding depths 4 & 8 pixels should be in unpacked index
// format.
//------------------------------------------------------------------------------
static bool
SaveToMemoryBMP(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec,
    uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize, uint32_t srcdepth,
    palette_t* psrcpalette, rgba_t* pcolorkey, bool invertY)
{
    if (ppdst == NULL || ppdstsize == NULL)
    {
        fprintf(stderr, "BMP, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL)
    {
        fprintf(stderr, "BMP, Invalid src data.\n");
        return false;
    }

    if (srcdepth !=  1 && srcdepth !=  4 && srcdepth != 8 && srcdepth != 24 &&
        srcdepth != 32)
    {
        fprintf(stderr, "BMP, Unsupported depth: %d.\n", srcdepth);
        return false;
    }

    uint32_t compression = BI_RGB;

    if (codec == ENCODE_RLE)
    {
        if (srcdepth == 8)
        {
            compression = BI_RLE8;
        }
        else if (srcdepth == 4)
        {
            compression = BI_RLE4;
        }
    }

    uint32_t xextent = srcxsize;
    uint32_t yextent = srcysize;
    uint8_t dstdepth = srcdepth;

    // dst stuff
    float dstpixelsperbyte = PIXELS_PER_BYTE(dstdepth);
    uint32_t dstpitch = (uint32_t)(ceilf((float)(xextent) / dstpixelsperbyte) + 3) & ~3;           // bytes per scanline
    uint32_t dstpalettesize = 0;

    // palette
    if (srcdepth <= 8)
    {
        if (psrcpalette != NULL)
        {
            if (dstdepth != 1 && dstdepth != 2 && dstdepth != 4)
            {
                dstpalettesize = psrcpalette->size * 4;
            }
        }
    }

    // src stuff
    float srcpixelsperbyte = PIXELS_PER_BYTE(srcdepth);
    uint32_t srcpitch = (uint32_t)(ceilf((float)(xextent) / srcpixelsperbyte));
    uint8_t* rawptr = psrc;
    uint8_t* rawbuf = psrc;

    // FIXME: override srcpitch calculation
    if (compression == BI_RLE4)
    {
        srcpitch = xextent;
    }

    uint32_t padbytes = dstpitch - srcpitch;

    // big array for true-color images
    int32_t datasize = s_bmp_file_size + s_bmp_v3_info_size + ((yextent * dstpitch)) +
        dstpalettesize;
    uint8_t* data = (uint8_t*)malloc(datasize);

    if (data == NULL)
    {
        fprintf(stderr, "BMP, Out of memory\n");
        return false;
    }

    memset(data, 0, datasize * sizeof(uint8_t));

    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;

    // fill in file and info header
    bmp_file_t bmpfile = {};
    bmp_v3_info_t bmpinfo = {};

    bmpfile.type = 0x4D42;
    bmpfile.size = datasize;
    bmpfile.offset = s_bmp_file_size + s_bmp_v3_info_size + dstpalettesize;

    bmpinfo.size = s_bmp_v3_info_size;
    bmpinfo.width = xextent;
    bmpinfo.height = (invertY == true) ? yextent : -(int32_t)(yextent);          // bottom-up dib
    bmpinfo.planes = 1;
    bmpinfo.bits = dstdepth;
    bmpinfo.compression = compression;
    bmpinfo.imagesize = xextent * yextent * (dstdepth < 8 ? 1 : (dstdepth >> 3));
    bmpinfo.xresolution = 0;
    bmpinfo.yresolution = 0;
    bmpinfo.num_colours = dstpalettesize >> 2;
    bmpinfo.num_colour_indexes = 0;
    
    // file struct
    WriteU16ToLE(dstbuf, bmpfile.type);         dstbuf += 2;            // type
    WriteU32ToLE(dstbuf, bmpfile.size);         dstbuf += 4;            // size
    WriteU16ToLE(dstbuf, bmpfile.reserved1);    dstbuf += 2;            // reserved1
    WriteU16ToLE(dstbuf, bmpfile.reserved2);    dstbuf += 2;            // reserved2
    WriteU32ToLE(dstbuf, bmpfile.offset);       dstbuf += 4;            // offset

    // info struct
    WriteU32ToLE(dstbuf, bmpinfo.size);         dstbuf += 4;            // size
    WriteI32ToLE(dstbuf, bmpinfo.width);        dstbuf += 4;            // width
    WriteI32ToLE(dstbuf, bmpinfo.height);       dstbuf += 4;            // height
    WriteU16ToLE(dstbuf, bmpinfo.planes);       dstbuf += 2;            // planes
    WriteU16ToLE(dstbuf, bmpinfo.bits);         dstbuf += 2;            // bits
    WriteU32ToLE(dstbuf, bmpinfo.compression);  dstbuf += 4;            // compression
    WriteU32ToLE(dstbuf, bmpinfo.imagesize);    dstbuf += 4;            // image size
    WriteI32ToLE(dstbuf, bmpinfo.xresolution);  dstbuf += 4;            // xresolution
    WriteI32ToLE(dstbuf, bmpinfo.yresolution);  dstbuf += 4;            // yresolution
    WriteU32ToLE(dstbuf, bmpinfo.num_colours);  dstbuf += 4;            // num_colours
    WriteU32ToLE(dstbuf, bmpinfo.num_colour_indexes); dstbuf += 4;      // num_colour_indexes

    // fill in the palette
    if (srcdepth <= 8)
    {
        if (psrcpalette != NULL)
        {
            for (uint32_t i = 0; i < dstpalettesize >> 2; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].b;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].r;
                *dstbuf++ = psrcpalette->data[i].a;
            }
        }
    }

    uint32_t bytesencoded = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    if (psrc != NULL)
    {
        if (compression != BI_RGB)            // run-length encoding
        {
            int32_t colorkey = -1;

            if (pcolorkey != NULL && psrcpalette != NULL)
            {
                for (uint32_t i = 0; i < dstpalettesize >> 2; ++i)
                {
                    if (psrcpalette->data[i].r == pcolorkey->r &&
                        psrcpalette->data[i].g == pcolorkey->g &&
                        psrcpalette->data[i].b == pcolorkey->b &&
                        psrcpalette->data[i].a == pcolorkey->a)
                    {
                        colorkey = i;
                        break;
                    }
                }
            }

            uint32_t abscount = 0;
            uint32_t rlecount = 0;
            uint32_t rlevalue = 0;
            uint32_t maxrle = 0xFF;
            uint32_t maxabs = 0xFF;
            int32_t absrem = 0;
            int32_t rlerem = 0;
            uint8_t sample0 = 0;
            uint8_t sample1 = 0;
            uint8_t sample2 = 0;

            switch (dstdepth)
            {
            case 4:         // 4-bit encoding
            {
                while (y++ < yextent)
                {
                    rawbuf = rawptr;
                    x = 0;

                    while (x < xextent)
                    {
                        rlevalue = 0;
                        sample0 = 0;
                        sample1 = 0;
                        sample2 = *(rawbuf + x);

                        abscount = 1;
                        absrem = xextent - (x + abscount);
                        sample1 = sample2;

                        while (absrem > 0 && abscount < maxabs)
                        {
                            sample0 = sample1;
                            sample1 = *(rawbuf + x + abscount);

                            if (sample0 == sample1)
                            {
                                break;
                            }

                            abscount++;
                            absrem--;
                            if (abscount + 1 < maxabs) { abscount++; absrem--; }
                        }

                        rlecount = 1;
                        rlerem = xextent - (x + rlecount);
                        sample1 = sample2;

                        while (rlerem > 0 && rlecount < maxrle)
                        {
                            sample0 = sample1;
                            sample1 = *(rawbuf + x + rlecount);

                            if (sample0 != sample1)
                            {
                                break;
                            }

                            rlecount++;
                            rlerem--;
                            if (rlecount + 1 < maxabs) { rlecount++; rlerem--; }
                        }

                        if (abscount >= 3 && abscount >= rlecount)
                        {
                            rlevalue = abscount;

                            *dstbuf++ = 0x00;
                            *dstbuf++ = rlevalue;
                            bytesencoded++;
                            bytesencoded++;

                            for (uint32_t i = 0; i < (rlevalue >> 1); ++i)
                            {
                                sample2 = 0;
                                sample2 |= *(rawbuf + x + i + 0) << 4;
                                sample2 |= *(rawbuf + x + i + 1);
                                *dstbuf++ = sample2;
                                bytesencoded++;
                            }

                            // pad-byte
                            if ((rlecount & 1))
                            {
                                *dstbuf++ = 0x00;
                                bytesencoded++;
                            }
                        }
                        else
                        {
                            rlevalue = rlecount;

                            if (rlevalue == 1)
                            {
                                sample0 |= sample0 >> 4;
                            }

                            if (sample0 == colorkey)
                            {
                                *dstbuf++ = 0x00;
                                *dstbuf++ = 0x02;
                                bytesencoded++;
                                bytesencoded++;

                                sample0 = 0;
                            }

                            *dstbuf++ = rlevalue;
                            *dstbuf++ = sample0;
                            bytesencoded++;
                            bytesencoded++;
                        }

                        x += rlevalue;
                    }

                    *dstbuf++ = 0x00;
                    *dstbuf++ = 0x00;
                    bytesencoded++;
                    bytesencoded++;

                    if (y != yextent)
                    {
                        rawptr += srcpitch;
                    }
                }

                // end of bitmap 
                *dstbuf++ = 0x00;
                *dstbuf++ = 0x01;
                bytesencoded++;
                bytesencoded++;

            } break;
            case 8:         // 8-bit encoding
            {
                while (y++ < yextent)
                {
                    rawbuf = rawptr;
                    x = 0;

                    while (x < xextent)
                    {
                        rlevalue = 0;
                        sample0 = 0;
                        sample1 = 0;
                        sample2 = *(rawbuf + x);

                        abscount = 1;
                        absrem = xextent - (x + abscount);
                        sample1 = sample2;

                        while (absrem > 0 && abscount < maxabs)
                        {
                            sample0 = sample1;
                            sample1 = *(rawbuf + x + abscount);

                            if (sample0 == sample1)
                            {
                                break;
                            }

                            abscount++;
                            absrem--;
                        }

                        rlecount = 1;
                        rlerem = xextent - (x + rlecount);
                        sample1 = sample2;

                        while (rlerem > 0 && rlecount < maxrle)
                        {
                            sample0 = sample1;
                            sample1 = *(rawbuf + x + rlecount);

                            if (sample0 != sample1)
                            {
                                break;
                            }

                            rlecount++;
                            rlerem--;
                        }

                        if (abscount >= 3 && abscount >= rlecount)
                        {
                            rlevalue = abscount;

                            *dstbuf++ = 0x00;
                            *dstbuf++ = rlevalue;
                            bytesencoded++;
                            bytesencoded++;

                            for (uint32_t i = 0; i < rlevalue; ++i)
                            {
                                *dstbuf++ = *(rawbuf + x + i);
                                bytesencoded++;
                            }

                            // pad-byte
                            if ((rlevalue & 1))
                            {
                                *dstbuf++ = 0x00;
                                bytesencoded++;
                            }
                        }
                        else
                        {
                            rlevalue = rlecount;

                            if (sample0 == colorkey)
                            {
                                *dstbuf++ = 0x00;
                                *dstbuf++ = 0x02;
                                bytesencoded++;
                                bytesencoded++;

                                sample0 = 0;
                            }

                            *dstbuf++ = rlevalue;
                            *dstbuf++ = sample0;
                            bytesencoded++;
                            bytesencoded++;
                        }

                        x += rlevalue;
                    }

                    *dstbuf++ = 0x00;
                    *dstbuf++ = 0x00;
                    bytesencoded++;
                    bytesencoded++;

                    if (y != yextent)
                    {
                        rawptr += srcpitch;
                    }
                }

                // end of bitmap 
                *dstbuf++ = 0x00;
                *dstbuf++ = 0x01;
                bytesencoded++;
                bytesencoded++;
            } break;
            }
        }
        else            // everything else
        {
            while (y++ < yextent)
            {
                rawbuf = rawptr;
                memcpy(dstbuf, rawbuf, srcpitch);
                if (y != yextent)
                {
                    rawptr += srcpitch;
                }
                dstbuf += srcpitch + padbytes;
                bytesencoded += srcpitch + padbytes;
            }
        }
    }

    datasize = s_bmp_file_size + s_bmp_v3_info_size + dstpalettesize +
        bytesencoded;

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
GetInfoFromMemoryBMP(uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth,
    uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < s_bmp_file_size)
    {
        return false;
    }

    uint8_t* srcbuf = psrc;

    bmp_file_t bmpfile = {};
    bmp_v3_info_t bmpinfo = {};

    // file struct
    bmpfile.type        = ReadU16FromLE(srcbuf); srcbuf += 2;           // type - 0x4D42 = 'BM'
    bmpfile.size        = ReadU32FromLE(srcbuf); srcbuf += 4;           // file size
    bmpfile.reserved1   = ReadU16FromLE(srcbuf); srcbuf += 2;           // reserved1
    bmpfile.reserved2   = ReadU16FromLE(srcbuf); srcbuf += 2;           // reserved2
    bmpfile.offset      = ReadU32FromLE(srcbuf); srcbuf += 4;           // offset to data

    // info struct
    bmpinfo.size        = ReadU32FromLE(srcbuf); srcbuf += 4;           // info size
    bmpinfo.width       = ReadI32FromLE(srcbuf); srcbuf += 4;           // width
    bmpinfo.height      = ReadI32FromLE(srcbuf); srcbuf += 4;           // height
    bmpinfo.planes      = ReadU16FromLE(srcbuf); srcbuf += 2;           // planes
    bmpinfo.bits        = ReadU16FromLE(srcbuf); srcbuf += 2;           // bit count
    bmpinfo.compression = ReadU32FromLE(srcbuf); srcbuf += 4;           // compression
    bmpinfo.imagesize   = ReadU32FromLE(srcbuf); srcbuf += 4;           // imagesize
    bmpinfo.xresolution = ReadI32FromLE(srcbuf); srcbuf += 4;           // x-resolution
    bmpinfo.yresolution = ReadI32FromLE(srcbuf); srcbuf += 4;           // y-resolution
    bmpinfo.num_colours = ReadU32FromLE(srcbuf); srcbuf += 4;           // num_colours
    bmpinfo.num_colour_indexes = ReadU32FromLE(srcbuf); srcbuf += 4;    // num_colour_indexes

    uint32_t xsize = bmpinfo.width;
    uint32_t ysize = ABS(bmpinfo.height);
    uint32_t depth = bmpinfo.bits;

    if (bmpfile.type != 0x4D42 || bmpfile.reserved1 != 0 || bmpfile.reserved2 != 0 ||
        bmpinfo.planes != 1 || bmpinfo.compression > 2)
    {
        return false;
    }

    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = depth; }

    return true;
}

//------------------------------------------------------------------------------
// LoadBMP
//------------------------------------------------------------------------------
static bool
LoadFromMemoryBMP(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize,
    uint8_t* srcdepth)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < s_bmp_file_size)
    {
        fprintf(stderr, "BMP, Not a valid bitmap.\n");
        return false;
    }

    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;

    bmp_file_t bmpfile = {};
    bmp_v3_info_t bmpinfo = {};

    // file struct
    bmpfile.type        = ReadU16FromLE(srcbuf); srcbuf += 2;           // type - 0x4D42 = 'BM'
    bmpfile.size        = ReadU32FromLE(srcbuf); srcbuf += 4;           // file size
    bmpfile.reserved1   = ReadU16FromLE(srcbuf); srcbuf += 2;           // reserved1
    bmpfile.reserved2   = ReadU16FromLE(srcbuf); srcbuf += 2;           // reserved2
    bmpfile.offset      = ReadU32FromLE(srcbuf); srcbuf += 4;           // offset to data

    // info struct
    bmpinfo.size        = ReadU32FromLE(srcbuf); srcbuf += 4;           // info size
    bmpinfo.width       = ReadI32FromLE(srcbuf); srcbuf += 4;           // width
    bmpinfo.height      = ReadI32FromLE(srcbuf); srcbuf += 4;           // height
    bmpinfo.planes      = ReadU16FromLE(srcbuf); srcbuf += 2;           // planes
    bmpinfo.bits        = ReadU16FromLE(srcbuf); srcbuf += 2;           // bit count
    bmpinfo.compression = ReadU32FromLE(srcbuf); srcbuf += 4;           // compression
    bmpinfo.imagesize   = ReadU32FromLE(srcbuf); srcbuf += 4;           // imagesize
    bmpinfo.xresolution = ReadI32FromLE(srcbuf); srcbuf += 4;           // x-resolution
    bmpinfo.yresolution = ReadI32FromLE(srcbuf); srcbuf += 4;           // y-resolution
    bmpinfo.num_colours = ReadU32FromLE(srcbuf); srcbuf += 4;           // num_colours
    bmpinfo.num_colour_indexes = ReadU32FromLE(srcbuf); srcbuf += 4;    // num_colour_indexes

    if (bmpfile.type != 0x4D42)
    {
        fprintf(stderr, "BMP, version mismatch: %d.\n", bmpfile.type);
        return false;
    }

    if (bmpfile.reserved1 != 0 || bmpfile.reserved2 != 0)
    {
        fprintf(stderr, "BMP, reserved parameters non-zero: reserved1: %d, reserved2: \
            %d.\n", bmpfile.reserved1, bmpfile.reserved2);
        return false;
    }

    if (bmpinfo.planes != 1 || bmpinfo.compression > 2)
    {
        fprintf(stderr, "BMP, number of bit-planes must be 1: %d.\n", bmpinfo.planes);
        return false;
    }

    if (bmpinfo.compression > 2)
    {
        fprintf(stderr, "BMP, invalid compression format: %d.\n", bmpinfo.compression);
        return false;
    }

    if (bmpinfo.bits !=  1 && bmpinfo.bits !=  4 && bmpinfo.bits != 8 &&
        bmpinfo.bits != 24 && bmpinfo.bits != 32)
    {
        fprintf(stderr, "BMP, unsupported bits: %d.\n", bmpinfo.bits);
        return false;
    }

    if (bmpinfo.bits == 1 || bmpinfo.bits == 4 || bmpinfo.bits == 8)
    {
        uint8_t* palptr = srcptr + s_bmp_file_size + s_bmp_v3_info_size;
        uint32_t palnum = bmpinfo.num_colours;

        if (pdstpalette != 0)
        {
            for (uint32_t i = 0; i < palnum; ++i)
            {
                pdstpalette->data[i].b = *palptr++;
                pdstpalette->data[i].g = *palptr++;
                pdstpalette->data[i].r = *palptr++;
                pdstpalette->data[i].a = *palptr++;
            }

            pdstpalette->size = palnum;
            pdstpalette->bits = 32;

            uint32_t alpha = 0;

            for (size_t i = 0; i < pdstpalette->size; ++i)
            {
                alpha |= pdstpalette->data[i].a;
            }

            // if alpha is all 0s replace with 255s
            if (alpha == 0)
            {
                for (size_t i = 0; i < pdstpalette->size; ++i)
                {
                    pdstpalette->data[i].a = 255;
                }
            }
        }
    }

    srcbuf = srcptr + bmpfile.offset;            // ptr to data

    bool rle = ((bmpinfo.compression == BI_RLE8) || (bmpinfo.compression == BI_RLE4));          // BI_RLE4=2 && BI_RLE8=1
    int32_t xsize = bmpinfo.width;
    int32_t ysize = bmpinfo.height;
    float pixelsperbyte = PIXELS_PER_BYTE(bmpinfo.bits);
    int32_t pitch = (int32_t)(ceilf((float)(xsize) / pixelsperbyte));           // bytes per scanline of output
    int32_t widthbytes = ((xsize * bmpinfo.bits + 31) >> 5) * 4;           // 4-byte boundary
    int32_t padbytes = widthbytes - pitch;
    int32_t rlecount = 0;
    uint8_t sample = 0;

    uint8_t* pixels = (uint8_t*)malloc(ABS(ysize) * pitch);
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixels;

    if (pixels == NULL)
    {
        fprintf(stderr, "BMP, Out of memory\n");
        return false;
    }

    memset(pixels, 0, ABS(ysize) * pitch);

    // bottom-up dib
    if (ysize >= 0)
    {
        pixptr = (pixels + ((ABS(ysize)-1)) * pitch);
        pitch = -pitch;
    }

    ysize = ABS(ysize);

    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ABS(ysize); }
    if (srcdepth != NULL) { *srcdepth = (bmpinfo.bits & 0xFF); }

    if (rle)            // run-length encoding
    {
        int32_t y = ABS(ysize);

        while (y-- > 0)
        {
            pixbuf = pixptr;

            while (1)
            {    
                unsigned char data0 = *srcbuf++;
                unsigned char data1 = *srcbuf++;

                // encoded modes
                if (!data0)
                {
                    if (data1 >= 0x03 && data1 <= 0xFF)         // absolute run-length encoding
                    {    
                        // rle count word aligned
                        int rlebytes = (data1 + 1) & ~1;
                        rlecount = rlebytes >> 1;
                        padbytes = rlecount % 2;

                        switch (bmpinfo.bits)
                        {
                            case 4:
                            {
                                memcpy(pixbuf, srcbuf, rlecount);
                                pixbuf += rlecount;
                                srcbuf += rlecount + padbytes;
                            } break;
                            case 8:
                            {
                                memcpy(pixbuf, srcbuf, data1);
                                pixbuf += data1;
                                srcbuf += rlebytes;
                            } break;
                        }
                    }
                    else if (data1 == 0x02)         // delta encoding
                    {
                        int dx = *srcbuf++;
                        int dy = *srcbuf++;

                        y -= dy;
                        if (y <= 0) break;

                        if (ysize >= 0) { pixptr = pixels + ((ysize-y) * ABS(pitch)); }
                        else { pixptr = pixels + (y * ABS(pitch)); }
                        pixbuf = pixptr + (dx > 2 ? dx << 1 : dx);
                    }
                    else if (data1 == 0x01)         // end of bitmap
                    {
                        return true;
                    }
                    else            // end of scanline
                    {
                        break;
                    }
                }
                else            // run-length encoding
                {
                    // rle count
                    memset(pixbuf, data1, data0);
                    pixbuf += data0;
                }
            }

            if (y != 0)
            {
                pixptr += pitch;
            }
        }
    }
    else            // everything else
    {
        int32_t y = ABS(ysize);

        while (y-- > 0)
        {
            pixbuf = pixptr;
            memcpy(pixbuf, srcbuf, pitch);
            srcbuf += pitch + padbytes;

            if (y != 0)
            {
                pixptr += pitch;
            }
        }

        if (bmpinfo.bits == 32)
        {
            uint32_t alpha = 0;
            pixbuf = pixels;

            for (int i = 0; i < xsize * ABS(ysize); ++i, pixbuf += 4)
            {
                alpha |= pixbuf[3];
            }

            // if alpha is all 0's replace with 255's
            if (alpha == 0)
            {
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
//  1, 2, 4, 8-bit paletted and 24-bit saving and loading
//
//-----------------------------------------------------------------------------


typedef struct _pcx_v5_info_s
{
    uint8_t  identifier;         // PCX Id Number (Always 0x0A)
    uint8_t  version;            // Version Number
    uint8_t  encoding;           // Encoding Format
    uint8_t  bitsPerPixel;       // Bits Per Pixel
    uint16_t xMin;               // Left of image
    uint16_t yMin;               // Top of image
    uint16_t xMax;               // Right of image
    uint16_t yMax;               // Bottom of image
    uint16_t horzRes;            // Horizontal Resolution
    uint16_t vertRes;            // Vertical Resolution
    uint8_t  palette[48];        // 16-Color EGA Palette
    uint8_t  reserved1;          // Reserved (Always 0)
    uint8_t  numBitPlanes;       // Number of Bit Planes
    uint16_t bytesPerLine;       // Bytes Per Scan-line
    uint16_t paletteType;        // Palette Type
    uint16_t horzScreenSize;     // Horizontal Screen Size
    uint16_t vertScreenSize;     // Vertical Screen Size
    uint8_t  reserved2[54];      // Reserved (Always 0)
} pcx_v5_info_t;

// default 64 color ega palette
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
SaveToMemoryPCX(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec,
    uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize, uint8_t srcdepth,
    palette_t* psrcpalette)
{
    if (ppdst == NULL || ppdstsize == NULL)
    {
        fprintf(stderr, "PCX, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL)
    {
        fprintf(stderr, "PCX, Invalid src data.\n");
        return false;
    }

    if (srcdepth !=  1 && srcdepth != 2 && srcdepth !=  4 && srcdepth != 8 &&
        srcdepth != 24)
    {
        fprintf(stderr, "PCX, Unsupported src depth: %d.\n", srcdepth);
        return false;
    }

    uint8_t dstdepth = 0;

    switch (srcdepth)
    {
    case 24:
    {
        dstdepth = 8;
    } break;
    case 8:
    case 4:
    case 2:
    case 1:
    {
        dstdepth = srcdepth;
    } break;
    }

    if (dstdepth != 1 && dstdepth != 2 && dstdepth != 4 && dstdepth != 8)
    {
        fprintf(stderr, "PCX, Unsupported dst depth: %d.\n", dstdepth);
        return false;
    }

    uint32_t xextent = srcxsize;
    uint32_t yextent = srcysize;
    uint8_t colorplanes = 1;
    uint8_t palettetype = 0;

    if (srcdepth <= 8)
    {
        palettetype = 1;
    }
    else if (srcdepth == 24)
    {
        colorplanes = 3;
    }

    // dst stuff
    float dstpixelsperbyte = PIXELS_PER_BYTE(dstdepth);
    uint32_t dstpitch = (uint32_t)(ceilf((float)(xextent) / dstpixelsperbyte) + 1) & ~1;           // bytes per scanline (must be an even number)

    // src stuff
    uint32_t srcbytesperpixel = ((srcdepth == 24) ? 3 : 1);
    float srcpixelsperbyte = PIXELS_PER_BYTE(dstdepth * srcbytesperpixel);
    uint32_t srcpitch = (uint32_t)(ceilf((float)(xextent) / srcpixelsperbyte));

    if (srcdepth < 8)
    {
        xextent = srcpitch;
    }

    uint8_t* rawptr = psrc;
    uint8_t* rawbuf = psrc;

    // total bytes per scanline required to encode pcx data
    uint32_t dsttotalbytes = colorplanes * dstpitch;
    uint32_t dstpalettesize = 0;
    uint32_t dstpadbytes = dstpitch - srcpitch;
    
    // palette
    if (srcdepth == 8 && psrcpalette != NULL)
    {
        dstpalettesize = 768;
    }

    // big array for true-color images
    uint32_t datasize = s_pcx_v5_info_size + ((yextent * dsttotalbytes) * 2) + dstpalettesize;
    uint8_t* data = (uint8_t*)malloc(datasize);

    if (data == NULL)
    {
        fprintf(stderr, "PCX, Out of memory\n");
        return false;
    }

    memset(data, 0, datasize);

    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;

    // fill in file header
    *dstbuf++ = 0x0A;           // PCX Id Number (Always 0x0A)
    *dstbuf++ = 5;          // Version Number
    *dstbuf++ = 1;          // Encoding Format
    *dstbuf++ = dstdepth;          // Bits Per Pixel
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Left of image
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Top of image
    WriteU16ToLE(dstbuf, xextent-1);    dstbuf += 2;            // Right of image
    WriteU16ToLE(dstbuf, yextent-1);    dstbuf += 2;            // Bottom of image
    WriteU16ToLE(dstbuf, 72);           dstbuf += 2;            // Horizontal resolution (h-dpi) 
    WriteU16ToLE(dstbuf, 72);           dstbuf += 2;            // Vertical resolution (v-dpi)

    uint8_t* egaptr = dstbuf;           // set pointer to 16-Color EGA Palette
    dstbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)

    *dstbuf++ = colorplanes;            // Number of Bit Planes
    WriteU16ToLE(dstbuf, dstpitch);     dstbuf += 2;            // Bytes per scan-line (must be an even number)    
    WriteU16ToLE(dstbuf, palettetype);  dstbuf += 2;            // Palette Type
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Horizontal Screen Size
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Vertical Screen Size
    dstbuf += 54;           // Reserved2 (Always 0)

    uint32_t bytesencoded = 0;
    uint32_t colorplane = 0;
    uint32_t rlecount = 0;
    uint32_t abscount = 0;
    uint32_t rlevalue = 0;
    uint32_t maxrle = 0x3F;
    uint32_t maxabs = 0x01;
    int32_t absrem = 0;
    int32_t rlerem = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint8_t sample0 = 0;
    uint8_t sample1 = 0;
    uint8_t sample2 = 0;

    while (y++ < yextent)
    {
        colorplane = 0;

        do
        {
            rawbuf = rawptr;
            x = 0;

            while (x < xextent)
            {
                sample0 = 0;
                sample1 = 0;
                sample2 = *(rawbuf + ((x * srcbytesperpixel) + colorplane));

                abscount = 1;
                absrem = xextent - (x + abscount);
                sample1 = sample2;

                while (absrem > 0 && abscount < maxabs)
                {
                    sample0 = sample1;
                    sample1 = *(rawbuf + (((x + abscount) * srcbytesperpixel) +
                        colorplane));

                    if (sample0 == sample1)
                    {
                        break;
                    }

                    abscount++;
                    absrem--;
                }

                rlecount = 1;
                rlerem = xextent - (x + rlecount);
                sample1 = sample2;

                while (rlerem > 0 && rlecount < maxrle)
                {
                    sample0 = sample1;
                    sample1 = *(rawbuf + (((x + rlecount) * srcbytesperpixel) +
                        colorplane));

                    if (sample0 != sample1)
                    {
                        break;
                    }

                    rlecount++;
                    rlerem--;
                }

                if (abscount >= rlecount)
                {
                    rlevalue = abscount;

                    if ((0xC0 != (0xC0 & sample0)))
                    {
                        *dstbuf++ = sample0;
                        bytesencoded++;
                    }
                    else
                    {
                        *dstbuf++ = rlevalue | 0xC0;
                        *dstbuf++ = sample0;
                        bytesencoded++;
                        bytesencoded++;
                    }
                }
                else
                {
                    rlevalue = rlecount;

                    *dstbuf++ = rlevalue | 0xC0;
                    *dstbuf++ = sample0;
                    bytesencoded++;
                    bytesencoded++;
                }

                x += rlevalue;
            }

            dstbuf += dstpadbytes;
            bytesencoded += dstpadbytes;

            colorplane++;

        } while (colorplane < colorplanes);

        if (y != yextent)
        {
            rawptr += srcpitch;
        }
    }

    // palette
    if (srcdepth <= 8)
    {
        if (dstdepth == 1 || dstdepth == 2 || dstdepth == 4)
        {
            dstbuf = egaptr;            // 16-Color EGA Palette

            if (psrcpalette != NULL)
            {
                for (unsigned int i = 0; i < 16; ++i)
                {
                    *dstbuf++ = psrcpalette->data[i].r;
                    *dstbuf++ = psrcpalette->data[i].g;
                    *dstbuf++ = psrcpalette->data[i].b;
                }
            }
            else
            {
                for (unsigned int i = 0; i < 16; ++i)
                {
                    *dstbuf++ = ega_palette[i].r;
                    *dstbuf++ = ega_palette[i].g;
                    *dstbuf++ = ega_palette[i].b;
                }
            }
        }
        else
        {
            *dstbuf++ = 0x0C;           // 256-Color Palette code
            bytesencoded++;

            for (unsigned int i = 0; i < 256; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].r;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].b;
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
GetInfoFromMemoryPCX(uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth,
    uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < s_pcx_v5_info_size)
    {
        return false;
    }

    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;

    pcx_v5_info_t pcx = {};          // pcx header format

    pcx.identifier      = *srcbuf++;            // PCX Id number (Always 0x0A)
    pcx.version         = *srcbuf++;            // Version number
    pcx.encoding        = *srcbuf++;            // Encoding format
    pcx.bitsPerPixel    = *srcbuf++;            // Bits per pixel
    pcx.xMin            = ReadU16FromLE(srcbuf); srcbuf += 2;           // Left of image
    pcx.yMin            = ReadU16FromLE(srcbuf); srcbuf += 2;           // Top of image
    pcx.xMax            = ReadU16FromLE(srcbuf); srcbuf += 2;           // Right of image
    pcx.yMax            = ReadU16FromLE(srcbuf); srcbuf += 2;           // Bottom of image
    pcx.horzRes         = ReadU16FromLE(srcbuf); srcbuf += 2;           // Horizontal resolution
    pcx.vertRes         = ReadU16FromLE(srcbuf); srcbuf += 2;           // Vertical resolution

    uint8_t* egaptr = srcbuf;            // set pointer to 16-Color EGA Palette
    srcbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)

    pcx.numBitPlanes    = *srcbuf++;          // Number of bit planes
    pcx.bytesPerLine    = ReadU16FromLE(srcbuf); srcbuf += 2;           // Bytes per scan-line
    pcx.paletteType     = ReadU16FromLE(srcbuf); srcbuf += 2;           // Palette type
    pcx.horzScreenSize  = ReadU16FromLE(srcbuf); srcbuf += 2;           // Horizontal screen size
    pcx.vertScreenSize  = ReadU16FromLE(srcbuf); srcbuf += 2;           // Vertical screen size
    srcbuf += 54;           // Reserved2 (Always 0)

    if (pcx.identifier != 0x0A)
    {
        return false;
    }

    if (pcx.encoding != 1)
    {
        return false;
    }

    if (pcx.version != 0 && pcx.version != 2 && pcx.version != 3 &&
        pcx.version != 4 && pcx.version != 5)
    {
        return false;
    }

    if (pcx.bitsPerPixel != 1 && pcx.bitsPerPixel != 2 &&
        pcx.bitsPerPixel != 4 && pcx.bitsPerPixel != 8)
    {
        return false;
    }

    uint32_t xsize = (pcx.xMax - pcx.xMin) + 1;
    uint32_t ysize = (pcx.yMax - pcx.yMin) + 1;
    uint8_t depth = pcx.bitsPerPixel * pcx.numBitPlanes;

    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = depth; }

    return true;
}

//-----------------------------------------------------------------------------
// LoadPCX
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryPCX(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < s_pcx_v5_info_size)
    {
        fprintf(stderr, "PCX, Not a valid bitmap.\n");
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

    pcx_v5_info_t pcx = {};

    pcx.identifier      = *srcbuf++;
    pcx.version         = *srcbuf++;
    pcx.encoding        = *srcbuf++;
    pcx.bitsPerPixel    = *srcbuf++;
    pcx.xMin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    pcx.yMin            = ReadU16FromLE(srcbuf); srcbuf += 2;
    pcx.xMax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    pcx.yMax            = ReadU16FromLE(srcbuf); srcbuf += 2;
    pcx.horzRes         = ReadU16FromLE(srcbuf); srcbuf += 2;
    pcx.vertRes         = ReadU16FromLE(srcbuf); srcbuf += 2;

    uint8_t* egaptr = srcbuf;           // set pointer to 16-Color EGA Palette
    srcbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)

    pcx.numBitPlanes    = *srcbuf++;
    pcx.bytesPerLine    = ReadU16FromLE(srcbuf); srcbuf += 2;
    pcx.paletteType     = ReadU16FromLE(srcbuf); srcbuf += 2;
    pcx.horzScreenSize  = ReadU16FromLE(srcbuf); srcbuf += 2;
    pcx.vertScreenSize  = ReadU16FromLE(srcbuf); srcbuf += 2;
    srcbuf += 54;           // Reserved2 (Always 0)

    if (pcx.identifier != 0x0A)
    {
        fprintf(stderr, "PCX, Invalid manufacturer code: %d.\n",
            pcx.identifier);
        return false;
    }

    if (pcx.encoding != 1)
    {
        fprintf(stderr, "PCX, Invalid encoding value: %d.\n", pcx.encoding);
        return false;
    }

    if (pcx.version != 0 && pcx.version != 2 && pcx.version != 3 &&
        pcx.version != 4 && pcx.version != 5)
    {
        fprintf(stderr, "PCX, Version mismatch: %d.\n", pcx.version);
        return false;
    }

    if (pcx.bitsPerPixel != 1 && pcx.bitsPerPixel != 2 &&
        pcx.bitsPerPixel != 4 && pcx.bitsPerPixel != 8)
    {
        fprintf(stderr, "PCX, Unsupported depth: %d.\n", pcx.bitsPerPixel);
        return false;
    }

    if ((pcx.bitsPerPixel == 1 || pcx.bitsPerPixel == 2 ||
         pcx.bitsPerPixel == 4 || pcx.bitsPerPixel == 8) &&
         pcx.numBitPlanes == 1)         // paletted
    {
        uint32_t palnum = 0;
        uint8_t* palptr = NULL;

        switch (pcx.bitsPerPixel)
        {
        case 1:
        {
            palptr = egaptr;
            palnum = 2;
        } break;
        case 2:
        {
            palptr = egaptr;
            palnum = 4;
        } break;
        case 4:
        {
            palptr = egaptr;
            palnum = 16;
        } break;
        case 8:
        {
            palptr = srcend - 768;
            palnum = 256;
        } break;
        }

        if (pdstpalette != NULL)
        {
            for (uint32_t i = 0; i < palnum; ++i)
            {
                pdstpalette->data[i].r = *palptr++;
                pdstpalette->data[i].g = *palptr++;
                pdstpalette->data[i].b = *palptr++;
                pdstpalette->data[i].a = 255;
            }

            pdstpalette->size = palnum;
            pdstpalette->bits = 24;
        }
    }

    // apply a default palette
    if (pdstpalette != NULL)
    {
        if ((pcx.version == 0 || pcx.version == 2 || pcx.version == 3 ||
             pcx.version == 4) && pdstpalette->size == 0)
        {
            fprintf(stderr, "PCX, No palette information, defaulting to ega \
                palette.\n");

            memcpy(pdstpalette->data, &ega_palette, 16 * s_rgba_size);
            pdstpalette->size = 16;
            pdstpalette->bits = 24;
        }
    }

    int32_t totalbytes = pcx.numBitPlanes * pcx.bytesPerLine;
    int32_t ncolorplanes = pcx.numBitPlanes;
    int32_t xsize = (pcx.xMax - pcx.xMin) + 1;
    int32_t ysize = (pcx.yMax - pcx.yMin) + 1;
    int32_t bytesperscanline = pcx.bytesPerLine;
    int32_t pitch = bytesperscanline * ncolorplanes;
    int32_t rlecount = 0;
    int32_t subtotal = 0;
    int32_t colorplane = 0;

    uint8_t* pixels = (uint8_t*)malloc((ysize * pitch));
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixels;

    if (pixels == NULL)
    {
        fprintf(stderr, "PCX: Out of memory\n");
        return false;
    }

    memset(pixels, 0, ysize * pitch);

    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = pcx.bitsPerPixel * pcx.numBitPlanes; }

    int32_t y = 0;

    while (y < ysize)
    {
        pixbuf = pixptr;
        colorplane = 1;
        subtotal = 0;

        do
        {
            rlecount = 1;

            if ((0xC0 & *srcbuf) == 0xC0)
            {
                rlecount = (*srcbuf++ & 0x3F);
            }
        
            subtotal += rlecount;

            while (rlecount--)
            {
                *pixbuf = *srcbuf;
                pixbuf += ncolorplanes;
            }

            if (subtotal % bytesperscanline == 0)
            {
                pixbuf = pixptr + colorplane++;
            }

            srcbuf++;

        } while (subtotal < totalbytes);

        y++;

        if (y != ysize)
        {
            pixptr += pitch;
        }
    }

    return true;
}

#endif // #ifndef _PCX_H


//-----------------------------------------------------------------------------
// Resampling
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Point_32bit_32bit
//-----------------------------------------------------------------------------
static void
Point_32bit_32bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint32_t dstpitch = dstxsize * 4;
    uint32_t srcpitch = srcxsize * 4;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < dstysize)
    {
        y0 = lroundf(py);
        bufdst = rawdst;
        bufsrc = rawsrc + (y0 * srcpitch);
        x = 0;

        while (x < dstxsize)
        {
            x0 = lroundf(px);

            bufdst[x*4+0] = bufsrc[x0*4+0];
            bufdst[x*4+1] = bufsrc[x0*4+1];
            bufdst[x*4+2] = bufsrc[x0*4+2];
            bufdst[x*4+3] = bufsrc[x0*4+3];
            
            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Point_24bit_24bit
//-----------------------------------------------------------------------------
static void
Point_24bit_24bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint32_t dstpitch = dstxsize * 3;
    uint32_t srcpitch = srcxsize * 3;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < dstysize)
    {
        y0 = lroundf(py);
        bufdst = rawdst;
        bufsrc = rawsrc + (y0 * srcpitch);
        x = 0;

        while (x < dstxsize)
        {
            x0 = lroundf(px);

            bufdst[x*3+0] = bufsrc[x0*3+0];
            bufdst[x*3+1] = bufsrc[x0*3+1];
            bufdst[x*3+2] = bufsrc[x0*3+2];
            
            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Point_16bit_16bit
//-----------------------------------------------------------------------------
static void
Point_16bit_16bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint32_t dstpitch = dstxsize * 2;
    uint32_t srcpitch = srcxsize * 2;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < dstysize)
    {
        y0 = lroundf(py);
        bufdst = rawdst;
        bufsrc = rawsrc + (y0 * srcpitch);
        x = 0;

        while (x < dstxsize)
        {
            x0 = lroundf(px);

            bufdst[x*2+0] = bufsrc[x0*2+0];
            bufdst[x*2+1] = bufsrc[x0*2+1];
            
            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Point_8bit_8bit
//-----------------------------------------------------------------------------
static void
Point_8bit_8bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < dstysize)
    {
        y0 = lroundf(py);
        bufdst = rawdst;
        bufsrc = rawsrc + (y0 * srcxsize);
        x = 0;

        while (x < dstxsize)
        {
            x0 = lroundf(px);

            bufdst[x] = bufsrc[x0];
            
            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstxsize;
        }
    }
}

//-----------------------------------------------------------------------------
// Point_32bit_Nbit
//-----------------------------------------------------------------------------
static void
Point_32bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize * 4;
    uint32_t rshift = 0;
    uint32_t gshift = 0;
    uint32_t bshift = 0;
    uint32_t ashift = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    switch (srcformat)
    {
    case PIXELTYPE_RGBA:
    {
        rmask = 0xFF000000;
        gmask = 0x00FF0000;
        bmask = 0x0000FF00;
        amask = 0x000000FF;

        rshift = 24;
        gshift = 16;
        bshift =  8;
        ashift =  0;
    } break;
    case PIXELTYPE_ABGR:
    {
        amask = 0xFF000000;
        bmask = 0x00FF0000;
        gmask = 0x0000FF00;
        rmask = 0x000000FF;

        rshift =  0;
        gshift =  8;
        bshift = 16;
        ashift = 24;
    } break;
    case PIXELTYPE_BGRA:
    {
        bmask = 0xFF000000;
        gmask = 0x00FF0000;
        rmask = 0x0000FF00;
        amask = 0x000000FF;

        rshift =  8;
        gshift = 16;
        bshift = 24;
        ashift =  0;
    } break;
    }

    while (y < dstysize)
    {
        y0 = lroundf(py);
        bufdst = rawdst;
        bufsrc = rawsrc + (y0 * srcpitch);
        x = 0;

        while (x < dstxsize)
        {
            x0 = lroundf(px);

            uint32_t pixel = (bufsrc[(x0%srcxsize)*4+0] << 24)
                | (bufsrc[(x0%srcxsize)*4+1] << 16)
                | (bufsrc[(x0%srcxsize)*4+2] <<  8)
                | (bufsrc[(x0%srcxsize)*4+3] <<  0);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = ((pixel & rmask) >> rshift);
                bufdst[x*4+1] = ((pixel & gmask) >> gshift);
                bufdst[x*4+2] = ((pixel & bmask) >> bshift);
                bufdst[x*4+3] = ((pixel & amask) >> ashift);
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = ((pixel & amask) >> ashift);
                bufdst[x*4+1] = ((pixel & bmask) >> bshift);
                bufdst[x*4+2] = ((pixel & gmask) >> gshift);
                bufdst[x*4+3] = ((pixel & rmask) >> rshift);
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = ((pixel & bmask) >> bshift);
                bufdst[x*4+1] = ((pixel & gmask) >> gshift);
                bufdst[x*4+2] = ((pixel & rmask) >> rshift);
                bufdst[x*4+3] = ((pixel & amask) >> ashift);
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = ((pixel & rmask) >> rshift);
                bufdst[x*3+1] = ((pixel & gmask) >> gshift);
                bufdst[x*3+2] = ((pixel & bmask) >> bshift);
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = ((pixel & bmask) >> bshift);
                bufdst[x*3+1] = ((pixel & gmask) >> gshift);
                bufdst[x*3+2] = ((pixel & rmask) >> rshift);
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = ((pixel & rmask) >> rshift) * 0x1F / 0xFF;
                g = ((pixel & gmask) >> gshift) * 0x1F / 0xFF;
                b = ((pixel & bmask) >> bshift) * 0x1F / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)(((pixel & rmask) >> rshift) * 0.2990f +
                    ((pixel & gmask) >> gshift) * 0.5870f +
                    ((pixel & bmask) >> bshift) * 0.1140f);
                bufdst[x*2+1] = 255;
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                bufdst[x*1+0] = (uint8_t)(((pixel & rmask) >> rshift) * 0.2990f +
                    ((pixel & gmask) >> gshift) * 0.5870f +
                    ((pixel & bmask) >> bshift) * 0.1140f);
            } break;
            }
            
            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Point_24bit_Nbit
//-----------------------------------------------------------------------------
static void
Point_24bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize * 3;
    uint32_t rshift = 0;
    uint32_t gshift = 0;
    uint32_t bshift = 0;
    uint32_t ashift = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    switch (srcformat)
    {
    case PIXELTYPE_RGB:
    {
        rmask = 0x00FF0000;
        gmask = 0x0000FF00;
        bmask = 0x000000FF;
        
        rshift = 16;
        gshift =  8;
        bshift =  0;
    } break;
    case PIXELTYPE_BGR:
    {
        bmask = 0x00FF0000;
        gmask = 0x0000FF00;
        rmask = 0x000000FF;

        rshift =  0;
        gshift =  8;
        bshift = 16;
    } break;
    }

    while (y < dstysize)
    {
        y0 = lroundf(py);
        bufdst = rawdst;
        bufsrc = rawsrc + (y0 * srcpitch);
        x = 0;

        while (x < dstxsize)
        {
            x0 = lroundf(px);

            uint32_t pixel = (bufsrc[(x0%srcxsize)*3+0] << 16)
                | (bufsrc[(x0%srcxsize)*3+1] <<  8)
                | (bufsrc[(x0%srcxsize)*3+2] <<  0);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = ((pixel & rmask) >> rshift);
                bufdst[x*4+1] = ((pixel & gmask) >> gshift);
                bufdst[x*4+2] = ((pixel & bmask) >> bshift);
                bufdst[x*4+3] = ((pixel & amask) >> ashift);
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = ((pixel & amask) >> ashift);
                bufdst[x*4+1] = ((pixel & bmask) >> bshift);
                bufdst[x*4+2] = ((pixel & gmask) >> gshift);
                bufdst[x*4+3] = ((pixel & rmask) >> rshift);
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = ((pixel & bmask) >> bshift);
                bufdst[x*4+1] = ((pixel & gmask) >> gshift);
                bufdst[x*4+2] = ((pixel & rmask) >> rshift);
                bufdst[x*4+3] = ((pixel & amask) >> ashift);
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = ((pixel & rmask) >> rshift);
                bufdst[x*3+1] = ((pixel & gmask) >> gshift);
                bufdst[x*3+2] = ((pixel & bmask) >> bshift);
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = ((pixel & bmask) >> bshift);
                bufdst[x*3+1] = ((pixel & gmask) >> gshift);
                bufdst[x*3+2] = ((pixel & rmask) >> rshift);
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = ((pixel & rmask) >> rshift) * 0x1F / 0xFF;
                g = ((pixel & gmask) >> gshift) * 0x1F / 0xFF;
                b = ((pixel & bmask) >> bshift) * 0x1F / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)(((pixel & rmask) >> rshift) * 0.2990f +
                    ((pixel & gmask) >> gshift) * 0.5870f +
                    ((pixel & bmask) >> bshift) * 0.1140f);
                bufdst[x*2+1] = 255;
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                bufdst[x*1+0] = (uint8_t)(((pixel & rmask) >> rshift) * 0.2990f +
                    ((pixel & gmask) >> gshift) * 0.5870f +
                    ((pixel & bmask) >> bshift) * 0.1140f);
            } break;
            }
            
            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Point_16bit_Nbit
//-----------------------------------------------------------------------------
static void
Point_16bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float rmod = 0.0f;
    float gmod = 0.0f;
    float bmod = 0.0f;
    float amod = 0.0f;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize * 2;
    uint32_t rshift = 0;
    uint32_t gshift = 0;
    uint32_t bshift = 0;
    uint32_t ashift = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    switch (srcformat)
    {
    case PIXELTYPE_XBGR1555:
    {
        rmask = 0x7C00;
        gmask = 0x03E0;
        bmask = 0x001F;
        amask = 0x8000;

        rshift = 10;
        gshift =  5;
        bshift =  0;
        ashift = 15;

        rmod = 255.0f / 31.0f;
        gmod = 255.0f / 31.0f;
        bmod = 255.0f / 31.0f;
        amod = 0.0f;
    } break;
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        rmask = 0xFF00;
        gmask = 0xFF00;
        bmask = 0xFF00;
        amask = 0x00FF;

        rshift = 8;
        gshift = 8;
        bshift = 8;
        ashift = 0;

        rmod = 0.2990f;
        gmod = 0.5870f;
        bmod = 0.1140f;
        amod = 1.0f;
    } break;
    }

    while (y < dstysize)
    {
        y0 = lroundf(py);
        bufdst = rawdst;
        bufsrc = rawsrc + (y0 * srcpitch);
        x = 0;

        while (x < dstxsize)
        {
            x0 = lroundf(px);

            uint16_t pixel = (bufsrc[(x0%srcxsize)*2+0] <<  8)
                | (bufsrc[(x0%srcxsize)*2+1] <<  0);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = ((pixel & rmask) >> rshift) * rmod;
                bufdst[x*4+1] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*4+2] = ((pixel & bmask) >> bshift) * bmod;
                bufdst[x*4+3] = ((pixel & amask) >> ashift) * amod;
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = ((pixel & amask) >> ashift) * amod;
                bufdst[x*4+1] = ((pixel & bmask) >> bshift) * bmod;
                bufdst[x*4+2] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*4+3] = ((pixel & rmask) >> rshift) * rmod;
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = ((pixel & bmask) >> bshift) * bmod;
                bufdst[x*4+1] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*4+2] = ((pixel & rmask) >> rshift) * rmod;
                bufdst[x*4+3] = ((pixel & amask) >> ashift) * amod;
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = ((pixel & rmask) >> rshift) * rmod;
                bufdst[x*3+1] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*3+2] = ((pixel & bmask) >> bshift) * bmod;
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = ((pixel & bmask) >> bshift) * bmod;
                bufdst[x*3+1] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*3+2] = ((pixel & rmask) >> rshift) * rmod;
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = (((pixel & rmask) >> rshift) * rmod) * 0x1F / 0xFF;
                g = (((pixel & gmask) >> gshift) * gmod) * 0x1F / 0xFF;
                b = (((pixel & bmask) >> bshift) * bmod) * 0x1F / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)((((pixel & rmask) >> rshift) * rmod) * 0.2990f +
                    (((pixel & gmask) >> gshift) * gmod) * 0.5870f +
                    (((pixel & bmask) >> bshift) * bmod) * 0.1140f);
                bufdst[x*2+1] = 255;
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                bufdst[x*1+0] = (uint8_t)((((pixel & rmask) >> rshift) * rmod) * 0.2990f +
                    (((pixel & gmask) >> gshift) * gmod) * 0.5870f +
                    (((pixel & bmask) >> bshift) * bmod) * 0.1140f);
            } break;
            }
            
            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Point_8bit_Nbit
//-----------------------------------------------------------------------------
static void
Point_8bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;
    uint8_t v = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    }

    while (y < dstysize)
    {
        y0 = lroundf(py);
        bufdst = rawdst;
        bufsrc = rawsrc + (y0 * srcpitch);
        x = 0;

        while (x < dstxsize)
        {
            x0 = lroundf(px);

            v = bufsrc[x0*1+0];
            b = v;
            g = v;
            r = v;

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = r;
                bufdst[x*4+1] = g;
                bufdst[x*4+2] = b;
                bufdst[x*4+3] = a;
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = a;
                bufdst[x*4+1] = b;
                bufdst[x*4+2] = g;
                bufdst[x*4+3] = r;
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = b;
                bufdst[x*4+1] = g;
                bufdst[x*4+2] = r;
                bufdst[x*4+3] = a;
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = r;
                bufdst[x*3+1] = g;
                bufdst[x*3+2] = b;
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = b;
                bufdst[x*3+1] = g;
                bufdst[x*3+2] = r;
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = r * 0x1F / 0xFF;
                g = g * 0x1F / 0xFF;
                b = b * 0x1F / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f +
                    g * 0.5870f +
                    b * 0.1140f);
                bufdst[x*2+1] = a;
            } break;
            }
            
            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Point_PAL_Nbit
//-----------------------------------------------------------------------------
static void
Point_PAL_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat, palette_t* ppalette)
{
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize;
    uint32_t x0 = 0;
    uint32_t y0 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
    uint8_t v = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    while (y < dstysize)
    {
        y0 = lroundf(py);
        bufdst = rawdst;
        bufsrc = rawsrc + (y0 * srcpitch);
        x = 0;

        while (x < dstxsize)
        {
            x0 = lroundf(px);

            v = bufsrc[x0*1+0];
            b = ppalette->data[v].b;
            g = ppalette->data[v].g;
            r = ppalette->data[v].r;
            a = ppalette->data[v].a;

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = r;
                bufdst[x*4+1] = g;
                bufdst[x*4+2] = b;
                bufdst[x*4+3] = a;
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = a;
                bufdst[x*4+1] = b;
                bufdst[x*4+2] = g;
                bufdst[x*4+3] = r;
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = b;
                bufdst[x*4+1] = g;
                bufdst[x*4+2] = r;
                bufdst[x*4+3] = a;
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = r;
                bufdst[x*3+1] = g;
                bufdst[x*3+2] = b;
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = b;
                bufdst[x*3+1] = g;
                bufdst[x*3+2] = r;
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = r * 0x1F / 0xFF;
                g = g * 0x1F / 0xFF;
                b = b * 0x1F / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f +
                    g * 0.5870f +
                    b * 0.1140f);
                bufdst[x*2+1] = a;
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                bufdst[x*1+0] = (uint8_t)(r * 0.2990f +
                    g * 0.5870f +
                    b * 0.1140f);
            } break;
            }
            
            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Linear_32bit_32bit
//-----------------------------------------------------------------------------
static void
Linear_32bit_32bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint8_t* bufsrc0 = psrc;
    uint8_t* bufsrc1 = psrc;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint32_t dstpitch = dstxsize * 4;
    uint32_t srcpitch = srcxsize * 4;
    uint32_t x0 = 0, x1 = 0;
    uint32_t y0 = 0, y1 = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < dstysize)
    {
        bufdst = rawdst;
        y0 = FLOOR(py);
        y1 = y0 + 1;

        bufsrc0 = rawsrc + (y0 * srcpitch);
        bufsrc1 = rawsrc + (y1 * srcpitch);

        float y1_py = y1 - py;
        float py_y0 = py - y0;
        
        float x1_px = 0.0f;
        float px_x0 = 0.0f;

        px = 0.0f;
        x = 0;
        
        while (x < dstxsize)
        {
            x0 = FLOOR(px);
            x1 = x0 + 1;

            x1_px = x1 - px;
            px_x0 = px - x0;

            bufdst[x*4+0] = (y1_py * ((x1_px * bufsrc0[x0*4+0]) + (px_x0 * bufsrc0[x1*4+0]))) +
                (py_y0 * ((x1_px * bufsrc1[x0*4+0]) + (px_x0 * bufsrc1[x1*4+0])));
            bufdst[x*4+1] = (y1_py * ((x1_px * bufsrc0[x0*4+1]) + (px_x0 * bufsrc0[x1*4+1]))) +
                (py_y0 * ((x1_px * bufsrc1[x0*4+1]) + (px_x0 * bufsrc1[x1*4+1])));
            bufdst[x*4+2] = (y1_py * ((x1_px * bufsrc0[x0*4+2]) + (px_x0 * bufsrc0[x1*4+2]))) +
                (py_y0 * ((x1_px * bufsrc1[x0*4+2]) + (px_x0 * bufsrc1[x1*4+2])));
            bufdst[x*4+3] = (y1_py * ((x1_px * bufsrc0[x0*4+3]) + (px_x0 * bufsrc0[x1*4+3]))) +
                (py_y0 * ((x1_px * bufsrc1[x0*4+3]) + (px_x0 * bufsrc1[x1*4+3])));

            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Linear_24bit_24bit
//-----------------------------------------------------------------------------
static void
Linear_24bit_24bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint8_t* bufsrc0 = psrc;
    uint8_t* bufsrc1 = psrc;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint32_t dstpitch = dstxsize * 3;
    uint32_t srcpitch = srcxsize * 3;
    uint32_t x0 = 0, x1 = 0;
    uint32_t y0 = 0, y1 = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < dstysize)
    {
        bufdst = rawdst;
        y0 = FLOOR(py);
        y1 = y0 + 1;

        bufsrc0 = rawsrc + (y0 * srcpitch);
        bufsrc1 = rawsrc + (y1 * srcpitch);

        float y1_py = y1 - py;
        float py_y0 = py - y0;
        
        float x1_px = 0.0f;
        float px_x0 = 0.0f;

        px = 0.0f;
        x = 0;
        
        while (x < dstxsize)
        {
            x0 = FLOOR(px);
            x1 = x0 + 1;

            x1_px = x1 - px;
            px_x0 = px - x0;

            bufdst[x*3+0] = (y1_py * ((x1_px * bufsrc0[x0*3+0]) + (px_x0 * bufsrc0[x1*3+0]))) +
                (py_y0 * ((x1_px * bufsrc1[x0*3+0]) + (px_x0 * bufsrc1[x1*3+0])));
            bufdst[x*3+1] = (y1_py * ((x1_px * bufsrc0[x0*3+1]) + (px_x0 * bufsrc0[x1*3+1]))) +
                (py_y0 * ((x1_px * bufsrc1[x0*3+1]) + (px_x0 * bufsrc1[x1*3+1])));
            bufdst[x*3+2] = (y1_py * ((x1_px * bufsrc0[x0*3+2]) + (px_x0 * bufsrc0[x1*3+2]))) +
                (py_y0 * ((x1_px * bufsrc1[x0*3+2]) + (px_x0 * bufsrc1[x1*3+2])));

            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Linear_16bit_16bit
//-----------------------------------------------------------------------------
static void
Linear_16bit_16bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float rmod = 0.0f;
    float gmod = 0.0f;
    float bmod = 0.0f;
    float amod = 0.0f;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    float r0 = 0.0f, g0 = 0.0f, b0 = 0.0f, a0 = 0.0f;
    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f, a1 = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint8_t* bufsrc0 = psrc;
    uint8_t* bufsrc1 = psrc;
    uint32_t dstpitch = srcxsize * 2;
    uint32_t srcpitch = srcxsize * 2;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint32_t x0 = 0, x1 = 0;
    uint32_t y0 = 0, y1 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t rx0y0 = 0, gx0y0 = 0, bx0y0 = 0, ax0y0 = 0;
    uint8_t rx1y0 = 0, gx1y0 = 0, bx1y0 = 0, ax1y0 = 0;
    uint8_t rx0y1 = 0, gx0y1 = 0, bx0y1 = 0, ax0y1 = 0;
    uint8_t rx1y1 = 0, gx1y1 = 0, bx1y1 = 0, ax1y1 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (srcformat)
    {
    case PIXELTYPE_XBGR1555:
    {
        rmask = 0x7C00;
        gmask = 0x03E0;
        bmask = 0x001F;
        amask = 0x8000;

        rmod = 255.0f / 31.0f;
        gmod = 255.0f / 31.0f;
        bmod = 255.0f / 31.0f;
        amod = 0.0f;
    } break;
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        rmask = 0x00FF;
        gmask = 0x00FF;
        bmask = 0x00FF;
        amask = 0xFF00;

        rmod = 0.2990f;
        gmod = 0.5870f;
        bmod = 0.1140f;
        amod = 1.0f;
    } break;
    }

    while (y < dstysize)
    {
        bufdst = rawdst;
        y0 = FLOOR(py);
        y1 = y0 + 1;

        bufsrc0 = rawsrc + (y0 * srcpitch);
        bufsrc1 = rawsrc + (y1 * srcpitch);

        float y1_py = y1 - py;
        float py_y0 = py - y0;
        
        float x1_px = 0.0f;
        float px_x0 = 0.0f;

        px = 0.0f;
        x = 0;
        
        while (x < dstxsize)
        {
            x0 = FLOOR(px);
            x1 = x0 + 1;

            uint32_t pixel0 = (bufsrc0[(x0%srcxsize)*3+0] << 8)
                | (bufsrc0[(x0%srcxsize)*3+1] << 0);
            uint32_t pixel1 = (bufsrc0[(x1%srcxsize)*3+0] << 8)
                | (bufsrc0[(x1%srcxsize)*3+1] << 0);
            uint32_t pixel2 = (bufsrc1[(x0%srcxsize)*3+0] << 8)
                | (bufsrc1[(x0%srcxsize)*3+1] << 0);
            uint32_t pixel3 = (bufsrc1[(x1%srcxsize)*3+0] << 8)
                | (bufsrc1[(x1%srcxsize)*3+1] << 0);

            rx0y0 = (pixel0 & rmask) * rmod;
            gx0y0 = (pixel0 & gmask) * gmod;
            bx0y0 = (pixel0 & bmask) * bmod;
            ax0y0 = (pixel0 & amask) * amod;

            rx1y0 = (pixel1 & rmask) * rmod;
            gx1y0 = (pixel1 & gmask) * gmod;
            bx1y0 = (pixel1 & bmask) * bmod;
            ax1y0 = (pixel1 & amask) * amod;

            rx0y1 = (pixel2 & rmask) * rmod;
            gx0y1 = (pixel2 & gmask) * gmod;
            bx0y1 = (pixel2 & bmask) * bmod;
            ax0y1 = (pixel2 & amask) * amod;

            rx1y1 = (pixel3 & rmask) * rmod;
            gx1y1 = (pixel3 & gmask) * gmod;
            bx1y1 = (pixel3 & bmask) * bmod;
            ax1y1 = (pixel3 & amask) * amod;

            x1_px = x1 - px;
            px_x0 = px - x0;
            
            b0 = (x1_px * bx0y0) + (px_x0 * bx1y0);
            g0 = (x1_px * gx0y0) + (px_x0 * gx1y0);
            r0 = (x1_px * rx0y0) + (px_x0 * rx1y0);
            a0 = (x1_px * ax0y0) + (px_x0 * ax1y0);

            b1 = (x1_px * bx0y1) + (px_x0 * bx1y1);
            g1 = (x1_px * gx0y1) + (px_x0 * gx1y1);
            r1 = (x1_px * rx0y1) + (px_x0 * rx1y1);
            a1 = (x1_px * ax0y1) + (px_x0 * ax1y1);

            switch (dstformat)
            {
            case PIXELTYPE_XBGR1555:
            {
                r = ((y1_py * r0) + (py_y0 * r1) * 0x1F) / 0xFF;
                g = ((y1_py * g0) + (py_y0 * g1) * 0x1F) / 0xFF;
                b = ((y1_py * b0) + (py_y0 * b1) * 0x1F) / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF) >> 0;
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
                bufdst[x*2+1] = (y1_py * a0) + (py_y0 * a1);
            } break;
            }

            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Linear_8bit_8bit
//-----------------------------------------------------------------------------
static void
Linear_8bit_8bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint8_t* bufsrc0 = psrc;
    uint8_t* bufsrc1 = psrc;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    uint32_t x0 = 0, x1 = 0;
    uint32_t y0 = 0, y1 = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < dstysize)
    {
        bufdst = rawdst;
        y0 = FLOOR(py);
        y1 = y0 + 1;

        bufsrc0 = rawsrc + (y0 * srcxsize);
        bufsrc1 = rawsrc + (y1 * srcxsize);

        float y1_py = y1 - py;
        float py_y0 = py - y0;
        
        float x1_px = 0.0f;
        float px_x0 = 0.0f;

        px = 0.0f;
        x = 0;
        
        while (x < dstxsize)
        {
            x0 = FLOOR(px);
            x1 = x0 + 1;

            x1_px = x1 - px;
            px_x0 = px - x0;

            bufdst[x] = (y1_py * ((x1_px * bufsrc0[x0]) + (px_x0 * bufsrc0[x1]))) +
                (py_y0 * ((x1_px * bufsrc1[x0]) + (px_x0 * bufsrc1[x1])));

            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstxsize;
        }
    }
}

//-----------------------------------------------------------------------------
// Linear_32bit_Nbit
//-----------------------------------------------------------------------------
static void
Linear_32bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    float r0 = 0.0f, g0 = 0.0f, b0 = 0.0f, a0 = 0.0f;
    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f, a1 = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint8_t* bufsrc0 = psrc;
    uint8_t* bufsrc1 = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize * 4;
    uint32_t rshift = 0;
    uint32_t gshift = 0;
    uint32_t bshift = 0;
    uint32_t ashift = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint32_t x0 = 0, x1 = 0;
    uint32_t y0 = 0, y1 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t rx0y0 = 0, gx0y0 = 0, bx0y0 = 0, ax0y0 = 0;
    uint8_t rx1y0 = 0, gx1y0 = 0, bx1y0 = 0, ax1y0 = 0;
    uint8_t rx0y1 = 0, gx0y1 = 0, bx0y1 = 0, ax0y1 = 0;
    uint8_t rx1y1 = 0, gx1y1 = 0, bx1y1 = 0, ax1y1 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    switch (srcformat)
    {
    case PIXELTYPE_RGBA:
    {
        rmask = 0xFF000000;
        gmask = 0x00FF0000;
        bmask = 0x0000FF00;
        amask = 0x000000FF;

        rshift = 24;
        gshift = 16;
        bshift =  8;
        ashift =  0;
    } break;
    case PIXELTYPE_ABGR:
    {
        amask = 0xFF000000;
        bmask = 0x00FF0000;
        gmask = 0x0000FF00;
        rmask = 0x000000FF;

        rshift =  0;
        gshift =  8;
        bshift = 16;
        ashift = 24;
    } break;
    case PIXELTYPE_BGRA:
    {
        bmask = 0xFF000000;
        gmask = 0x00FF0000;
        rmask = 0x0000FF00;
        amask = 0x000000FF;

        rshift =  8;
        gshift = 16;
        bshift = 24;
        ashift =  0;
    } break;
    }

    while (y < dstysize)
    {
        bufdst = rawdst;
        y0 = FLOOR(py);
        y1 = y0 + 1;

        bufsrc0 = rawsrc + (y0 * srcpitch);
        bufsrc1 = rawsrc + (y1 * srcpitch);

        float y1_py = y1 - py;
        float py_y0 = py - y0;
        
        float x1_px = 0.0f;
        float px_x0 = 0.0f;

        px = 0.0f;
        x = 0;
        
        while (x < dstxsize)
        {
            x0 = FLOOR(px);
            x1 = x0 + 1;

            uint32_t pixel0 = (bufsrc0[(x0%srcxsize)*4+0] << 24)
                | (bufsrc0[(x0%srcxsize)*4+1] << 16)
                | (bufsrc0[(x0%srcxsize)*4+2] <<  8)
                | (bufsrc0[(x0%srcxsize)*4+3] <<  0);
            uint32_t pixel1 = (bufsrc0[(x1%srcxsize)*4+0] << 24)
                | (bufsrc0[(x1%srcxsize)*4+1] << 16)
                | (bufsrc0[(x1%srcxsize)*4+2] <<  8)
                | (bufsrc0[(x1%srcxsize)*4+3] <<  0);
            uint32_t pixel2 = (bufsrc1[(x0%srcxsize)*4+0] << 24)
                | (bufsrc1[(x0%srcxsize)*4+1] << 16)
                | (bufsrc1[(x0%srcxsize)*4+2] <<  8)
                | (bufsrc1[(x0%srcxsize)*4+3] <<  0);
            uint32_t pixel3 = (bufsrc1[(x1%srcxsize)*4+0] << 24)
                | (bufsrc1[(x1%srcxsize)*4+1] << 16)
                | (bufsrc1[(x1%srcxsize)*4+2] <<  8)
                | (bufsrc1[(x1%srcxsize)*4+3] <<  0);

            rx0y0 = ((pixel0 & rmask) >> rshift);
            gx0y0 = ((pixel0 & gmask) >> gshift);
            bx0y0 = ((pixel0 & bmask) >> bshift);
            ax0y0 = ((pixel0 & amask) >> ashift);

            rx1y0 = ((pixel1 & rmask) >> rshift);
            gx1y0 = ((pixel1 & gmask) >> gshift);
            bx1y0 = ((pixel1 & bmask) >> bshift);
            ax1y0 = ((pixel1 & amask) >> ashift);

            rx0y1 = ((pixel2 & rmask) >> rshift);
            gx0y1 = ((pixel2 & gmask) >> gshift);
            bx0y1 = ((pixel2 & bmask) >> bshift);
            ax0y1 = ((pixel2 & amask) >> ashift);

            rx1y1 = ((pixel3 & rmask) >> rshift);
            gx1y1 = ((pixel3 & gmask) >> gshift);
            bx1y1 = ((pixel3 & bmask) >> bshift);
            ax1y1 = ((pixel3 & amask) >> ashift);

            x1_px = x1 - px;
            px_x0 = px - x0;
            
            b0 = (x1_px * bx0y0) + (px_x0 * bx1y0);
            g0 = (x1_px * gx0y0) + (px_x0 * gx1y0);
            r0 = (x1_px * rx0y0) + (px_x0 * rx1y0);
            a0 = (x1_px * ax0y0) + (px_x0 * ax1y0);

            b1 = (x1_px * bx0y1) + (px_x0 * bx1y1);
            g1 = (x1_px * gx0y1) + (px_x0 * gx1y1);
            r1 = (x1_px * rx0y1) + (px_x0 * rx1y1);
            a1 = (x1_px * ax0y1) + (px_x0 * ax1y1);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = (y1_py * a0) + (py_y0 * a1);
                bufdst[x*4+1] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+2] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+3] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * b0) + (py_y0 * b1);
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = ((y1_py * r0) + (py_y0 * r1) * 0x1F) / 0xFF;
                g = ((y1_py * g0) + (py_y0 * g1) * 0x1F) / 0xFF;
                b = ((y1_py * b0) + (py_y0 * b1) * 0x1F) / 0xFF;

                rgb16 = 0;
                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
                bufdst[x*2+1] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*1+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
            } break;
            }

            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Linear_24bit_Nbit
//-----------------------------------------------------------------------------
static void
Linear_24bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    float r0 = 0.0f, g0 = 0.0f, b0 = 0.0f, a0 = 0.0f;
    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f, a1 = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint8_t* bufsrc0 = psrc;
    uint8_t* bufsrc1 = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize * 3;
    uint32_t rshift = 0;
    uint32_t gshift = 0;
    uint32_t bshift = 0;
    uint32_t ashift = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint32_t x0 = 0, x1 = 0;
    uint32_t y0 = 0, y1 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t rx0y0 = 0, gx0y0 = 0, bx0y0 = 0, ax0y0 = 0;
    uint8_t rx1y0 = 0, gx1y0 = 0, bx1y0 = 0, ax1y0 = 0;
    uint8_t rx0y1 = 0, gx0y1 = 0, bx0y1 = 0, ax0y1 = 0;
    uint8_t rx1y1 = 0, gx1y1 = 0, bx1y1 = 0, ax1y1 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    switch (srcformat)
    {
    case PIXELTYPE_RGB:
    {
        rmask = 0x00FF0000;
        gmask = 0x0000FF00;
        bmask = 0x000000FF;
        
        rshift = 16;
        gshift =  8;
        bshift =  0;
    } break;
    case PIXELTYPE_BGR:
    {
        bmask = 0x00FF0000;
        gmask = 0x0000FF00;
        rmask = 0x000000FF;

        rshift =  0;
        gshift =  8;
        bshift = 16;
    } break;
    }

    while (y < dstysize)
    {
        bufdst = rawdst;
        y0 = FLOOR(py);
        y1 = y0 + 1;

        bufsrc0 = rawsrc + (y0 * srcpitch);
        bufsrc1 = rawsrc + (y1 * srcpitch);

        float y1_py = y1 - py;
        float py_y0 = py - y0;
        
        float x1_px = 0.0f;
        float px_x0 = 0.0f;

        px = 0.0f;
        x = 0;
        
        while (x < dstxsize)
        {
            x0 = FLOOR(px);
            x1 = x0 + 1;

            uint32_t pixel0 = (bufsrc0[(x0%srcxsize)*3+0] << 16)
                | (bufsrc0[(x0%srcxsize)*3+1] <<  8)
                | (bufsrc0[(x0%srcxsize)*3+2] <<  0);
            uint32_t pixel1 = (bufsrc0[(x1%srcxsize)*3+0] << 16)
                | (bufsrc0[(x1%srcxsize)*3+1] <<  8)
                | (bufsrc0[(x1%srcxsize)*3+2] <<  0);
            uint32_t pixel2 = (bufsrc1[(x0%srcxsize)*3+0] << 16)
                | (bufsrc1[(x0%srcxsize)*3+1] <<  8)
                | (bufsrc1[(x0%srcxsize)*3+2] <<  0);
            uint32_t pixel3 = (bufsrc1[(x1%srcxsize)*3+0] << 16)
                | (bufsrc1[(x1%srcxsize)*3+1] <<  8)
                | (bufsrc1[(x1%srcxsize)*3+2] <<  0);

            rx0y0 = ((pixel0 & rmask) >> rshift);
            gx0y0 = ((pixel0 & gmask) >> gshift);
            bx0y0 = ((pixel0 & bmask) >> bshift);
            ax0y0 = 255;

            rx1y0 = ((pixel1 & rmask) >> rshift);
            gx1y0 = ((pixel1 & gmask) >> gshift);
            bx1y0 = ((pixel1 & bmask) >> bshift);
            ax1y0 = 255;

            rx0y1 = ((pixel2 & rmask) >> rshift);
            gx0y1 = ((pixel2 & gmask) >> gshift);
            bx0y1 = ((pixel2 & bmask) >> bshift);
            ax0y1 = 255;

            rx1y1 = ((pixel3 & rmask) >> rshift);
            gx1y1 = ((pixel3 & gmask) >> gshift);
            bx1y1 = ((pixel3 & bmask) >> bshift);
            ax1y1 = 255;

            x1_px = x1 - px;
            px_x0 = px - x0;
            
            b0 = (x1_px * bx0y0) + (px_x0 * bx1y0);
            g0 = (x1_px * gx0y0) + (px_x0 * gx1y0);
            r0 = (x1_px * rx0y0) + (px_x0 * rx1y0);
            a0 = (x1_px * ax0y0) + (px_x0 * ax1y0);

            b1 = (x1_px * bx0y1) + (px_x0 * bx1y1);
            g1 = (x1_px * gx0y1) + (px_x0 * gx1y1);
            r1 = (x1_px * rx0y1) + (px_x0 * rx1y1);
            a1 = (x1_px * ax0y1) + (px_x0 * ax1y1);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = (y1_py * a0) + (py_y0 * a1);
                bufdst[x*4+1] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+2] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+3] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * b0) + (py_y0 * b1);
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = ((y1_py * r0) + (py_y0 * r1) * 0x1F) / 0xFF;
                g = ((y1_py * g0) + (py_y0 * g1) * 0x1F) / 0xFF;
                b = ((y1_py * b0) + (py_y0 * b1) * 0x1F) / 0xFF;

                rgb16 = 0;
                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
                bufdst[x*2+1] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*1+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
            } break;
            }

            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Linear_16bit_Nbit
//-----------------------------------------------------------------------------
static void
Linear_16bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float rmod = 0.0f;
    float gmod = 0.0f;
    float bmod = 0.0f;
    float amod = 0.0f;
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    float r0 = 0.0f, g0 = 0.0f, b0 = 0.0f, a0 = 0.0f;
    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f, a1 = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint8_t* bufsrc0 = psrc;
    uint8_t* bufsrc1 = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize * 2;
    uint32_t rshift = 0;
    uint32_t gshift = 0;
    uint32_t bshift = 0;
    uint32_t ashift = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint32_t x0 = 0, x1 = 0;
    uint32_t y0 = 0, y1 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t rx0y0 = 0, gx0y0 = 0, bx0y0 = 0, ax0y0 = 0;
    uint8_t rx1y0 = 0, gx1y0 = 0, bx1y0 = 0, ax1y0 = 0;
    uint8_t rx0y1 = 0, gx0y1 = 0, bx0y1 = 0, ax0y1 = 0;
    uint8_t rx1y1 = 0, gx1y1 = 0, bx1y1 = 0, ax1y1 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    switch (srcformat)
    {
    case PIXELTYPE_XBGR1555:
    {
        rmask = 0x7C00;
        gmask = 0x03E0;
        bmask = 0x001F;
        amask = 0x8000;

        rshift = 10;
        gshift =  5;
        bshift =  0;
        ashift = 15;

        rmod = 255.0f / 31.0f;
        gmod = 255.0f / 31.0f;
        bmod = 255.0f / 31.0f;
        amod = 0.0f;
    } break;
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        rmask = 0xFF00;
        gmask = 0xFF00;
        bmask = 0xFF00;
        amask = 0x00FF;

        rshift = 8;
        gshift = 8;
        bshift = 8;
        ashift = 0;

        rmod = 0.2990f;
        gmod = 0.5870f;
        bmod = 0.1140f;
        amod = 1.0f;
    } break;
    }

    while (y < dstysize)
    {
        bufdst = rawdst;
        y0 = FLOOR(py);
        y1 = y0 + 1;

        bufsrc0 = rawsrc + (y0 * srcpitch);
        bufsrc1 = rawsrc + (y1 * srcpitch);

        float y1_py = y1 - py;
        float py_y0 = py - y0;
        
        float x1_px = 0.0f;
        float px_x0 = 0.0f;

        px = 0.0f;
        x = 0;
        
        while (x < dstxsize)
        {
            x0 = FLOOR(px);
            x1 = x0 + 1;

            uint32_t pixel0 = (bufsrc0[(x0%srcxsize)*3+0] << 8)
                | (bufsrc0[(x0%srcxsize)*3+1] << 0);
            uint32_t pixel1 = (bufsrc0[(x1%srcxsize)*3+0] << 8)
                | (bufsrc0[(x1%srcxsize)*3+1] << 0);
            uint32_t pixel2 = (bufsrc1[(x0%srcxsize)*3+0] << 8)
                | (bufsrc1[(x0%srcxsize)*3+1] << 0);
            uint32_t pixel3 = (bufsrc1[(x1%srcxsize)*3+0] << 8)
                | (bufsrc1[(x1%srcxsize)*3+1] << 0);

            rx0y0 = ((pixel0 & rmask) >> rshift) * rmod;
            gx0y0 = ((pixel0 & gmask) >> gshift) * gmod;
            bx0y0 = ((pixel0 & bmask) >> bshift) * bmod;
            ax0y0 = ((pixel0 & amask) >> ashift) * amod;

            rx1y0 = ((pixel1 & rmask) >> rshift) * rmod;
            gx1y0 = ((pixel1 & gmask) >> gshift) * gmod;
            bx1y0 = ((pixel1 & bmask) >> bshift) * bmod;
            ax1y0 = ((pixel1 & amask) >> ashift) * amod;

            rx0y1 = ((pixel2 & rmask) >> rshift) * rmod;
            gx0y1 = ((pixel2 & gmask) >> gshift) * gmod;
            bx0y1 = ((pixel2 & bmask) >> bshift) * bmod;
            ax0y1 = ((pixel2 & amask) >> ashift) * amod;

            rx1y1 = ((pixel3 & rmask) >> rshift) * rmod;
            gx1y1 = ((pixel3 & gmask) >> gshift) * gmod;
            bx1y1 = ((pixel3 & bmask) >> bshift) * bmod;
            ax1y1 = ((pixel3 & amask) >> ashift) * amod;

            x1_px = x1 - px;
            px_x0 = px - x0;
            
            b0 = (x1_px * bx0y0) + (px_x0 * bx1y0);
            g0 = (x1_px * gx0y0) + (px_x0 * gx1y0);
            r0 = (x1_px * rx0y0) + (px_x0 * rx1y0);
            a0 = (x1_px * ax0y0) + (px_x0 * ax1y0);

            b1 = (x1_px * bx0y1) + (px_x0 * bx1y1);
            g1 = (x1_px * gx0y1) + (px_x0 * gx1y1);
            r1 = (x1_px * rx0y1) + (px_x0 * rx1y1);
            a1 = (x1_px * ax0y1) + (px_x0 * ax1y1);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = (y1_py * a0) + (py_y0 * a1);
                bufdst[x*4+1] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+2] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+3] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * b0) + (py_y0 * b1);
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = ((y1_py * r0) + (py_y0 * r1) * 0x1F) / 0xFF;
                g = ((y1_py * g0) + (py_y0 * g1) * 0x1F) / 0xFF;
                b = ((y1_py * b0) + (py_y0 * b1) * 0x1F) / 0xFF;

                rgb16 = 0;
                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
                bufdst[x*2+1] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*1+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
            } break;
            }

            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Linear_8bit_Nbit
//-----------------------------------------------------------------------------
static void
Linear_8bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    float r0 = 0.0f, g0 = 0.0f, b0 = 0.0f, a0 = 0.0f;
    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f, a1 = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint8_t* bufsrc0 = psrc;
    uint8_t* bufsrc1 = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint32_t x0 = 0, x1 = 0;
    uint32_t y0 = 0, y1 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t vx0y0 = 0, vx1y0 = 0, vx0y1 = 0, vx1y1 = 0;
    uint8_t rx0y0 = 0, gx0y0 = 0, bx0y0 = 0, ax0y0 = 255;
    uint8_t rx1y0 = 0, gx1y0 = 0, bx1y0 = 0, ax1y0 = 255;
    uint8_t rx0y1 = 0, gx0y1 = 0, bx0y1 = 0, ax0y1 = 255;
    uint8_t rx1y1 = 0, gx1y1 = 0, bx1y1 = 0, ax1y1 = 255;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
    uint8_t v = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    }

    while (y < dstysize)
    {
        bufdst = rawdst;
        y0 = FLOOR(py);
        y1 = y0 + 1;

        bufsrc0 = rawsrc + (y0 * srcpitch);
        bufsrc1 = rawsrc + (y1 * srcpitch);

        float y1_py = y1 - py;
        float py_y0 = py - y0;
        
        float x1_px = 0.0f;
        float px_x0 = 0.0f;

        px = 0.0f;
        x = 0;
        
        while (x < dstxsize)
        {
            x0 = FLOOR(px);
            x1 = x0 + 1;

            vx0y0 = bufsrc0[x0*1+0];
            vx1y0 = bufsrc0[x1*1+0];
            vx0y1 = bufsrc1[x0*1+0];
            vx1y1 = bufsrc1[x1*1+0];

            bx0y0 = vx0y0;
            gx0y0 = vx0y0;
            rx0y0 = vx0y0;

            bx1y0 = vx1y0;
            gx1y0 = vx1y0;
            rx1y0 = vx1y0;

            bx0y1 = vx0y1;
            gx0y1 = vx0y1;
            rx0y1 = vx0y1;

            bx1y1 = vx1y1;
            gx1y1 = vx1y1;
            rx1y1 = vx1y1;

            x1_px = x1 - px;
            px_x0 = px - x0;
            
            b0 = (x1_px * bx0y0) + (px_x0 * bx1y0);
            g0 = (x1_px * gx0y0) + (px_x0 * gx1y0);
            r0 = (x1_px * rx0y0) + (px_x0 * rx1y0);
            a0 = (x1_px * ax0y0) + (px_x0 * ax1y0);

            b1 = (x1_px * bx0y1) + (px_x0 * bx1y1);
            g1 = (x1_px * gx0y1) + (px_x0 * gx1y1);
            r1 = (x1_px * rx0y1) + (px_x0 * rx1y1);
            a1 = (x1_px * ax0y1) + (px_x0 * ax1y1);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = (y1_py * a0) + (py_y0 * a1);
                bufdst[x*4+1] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+2] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+3] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * b0) + (py_y0 * b1);
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = ((y1_py * r0) + (py_y0 * r1) * 0x1F) / 0xFF;
                g = ((y1_py * g0) + (py_y0 * g1) * 0x1F) / 0xFF;
                b = ((y1_py * b0) + (py_y0 * b1) * 0x1F) / 0xFF;

                rgb16 = 0;
                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
                bufdst[x*2+1] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*1+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
            } break;
            }

            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Linear_PAL_Nbit
//-----------------------------------------------------------------------------
static void
Linear_PAL_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat, palette_t* ppalette)
{
    float dx = (float)srcxsize / (float)dstxsize;
    float dy = (float)srcysize / (float)dstysize;
    float px = 0.0f;
    float py = 0.0f;
    float r0 = 0.0f, g0 = 0.0f, b0 = 0.0f, a0 = 0.0f;
    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f, a1 = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint8_t* bufsrc0 = psrc;
    uint8_t* bufsrc1 = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint32_t x0 = 0, x1 = 0;
    uint32_t y0 = 0, y1 = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t vx0y0 = 0, vx1y0 = 0, vx0y1 = 0, vx1y1 = 0;
    uint8_t rx0y0 = 0, gx0y0 = 0, bx0y0 = 0, ax0y0 = 0;
    uint8_t rx1y0 = 0, gx1y0 = 0, bx1y0 = 0, ax1y0 = 0;
    uint8_t rx0y1 = 0, gx0y1 = 0, bx0y1 = 0, ax0y1 = 0;
    uint8_t rx1y1 = 0, gx1y1 = 0, bx1y1 = 0, ax1y1 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
    uint8_t v = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    while (y < dstysize)
    {
        bufdst = rawdst;
        y0 = FLOOR(py);
        y1 = y0 + 1;

        bufsrc0 = rawsrc + (y0 * srcpitch);
        bufsrc1 = rawsrc + (y1 * srcpitch);

        float y1_py = y1 - py;
        float py_y0 = py - y0;
        
        float x1_px = 0.0f;
        float px_x0 = 0.0f;

        px = 0.0f;
        x = 0;
        
        while (x < dstxsize)
        {
            x0 = FLOOR(px);
            x1 = x0 + 1;

            vx0y0 = bufsrc0[x0*1+0];
            vx1y0 = bufsrc0[x1*1+0];
            vx0y1 = bufsrc1[x0*1+0];
            vx1y1 = bufsrc1[x1*1+0];

            bx0y0 = ppalette->data[vx0y0].b;
            gx0y0 = ppalette->data[vx0y0].g;
            rx0y0 = ppalette->data[vx0y0].r;
            ax0y0 = ppalette->data[vx0y0].a;

            bx1y0 = ppalette->data[vx1y0].b;
            gx1y0 = ppalette->data[vx1y0].g;
            rx1y0 = ppalette->data[vx1y0].r;
            ax1y0 = ppalette->data[vx1y0].a;

            bx0y1 = ppalette->data[vx0y1].b;
            gx0y1 = ppalette->data[vx0y1].g;
            rx0y1 = ppalette->data[vx0y1].r;
            ax0y1 = ppalette->data[vx0y1].a;

            bx1y1 = ppalette->data[vx1y1].b;
            gx1y1 = ppalette->data[vx1y1].g;
            rx1y1 = ppalette->data[vx1y1].r;
            ax1y1 = ppalette->data[vx1y1].a;

            x1_px = x1 - px;
            px_x0 = px - x0;
            
            b0 = (x1_px * bx0y0) + (px_x0 * bx1y0);
            g0 = (x1_px * gx0y0) + (px_x0 * gx1y0);
            r0 = (x1_px * rx0y0) + (px_x0 * rx1y0);
            a0 = (x1_px * ax0y0) + (px_x0 * ax1y0);

            b1 = (x1_px * bx0y1) + (px_x0 * bx1y1);
            g1 = (x1_px * gx0y1) + (px_x0 * gx1y1);
            r1 = (x1_px * rx0y1) + (px_x0 * rx1y1);
            a1 = (x1_px * ax0y1) + (px_x0 * ax1y1);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = (y1_py * a0) + (py_y0 * a1);
                bufdst[x*4+1] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+2] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+3] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*4+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*4+2] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*4+3] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = (y1_py * r0) + (py_y0 * r1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * b0) + (py_y0 * b1);
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = (y1_py * b0) + (py_y0 * b1);
                bufdst[x*3+1] = (y1_py * g0) + (py_y0 * g1);
                bufdst[x*3+2] = (y1_py * r0) + (py_y0 * r1);
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = ((y1_py * r0) + (py_y0 * r1) * 0x1F) / 0xFF;
                g = ((y1_py * g0) + (py_y0 * g1) * 0x1F) / 0xFF;
                b = ((y1_py * b0) + (py_y0 * b1) * 0x1F) / 0xFF;

                rgb16 = 0;
                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF);
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
                bufdst[x*2+1] = (y1_py * a0) + (py_y0 * a1);
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                r = (y1_py * r0) + (py_y0 * r1);
                g = (y1_py * g0) + (py_y0 * g1);
                b = (y1_py * b0) + (py_y0 * b1);
                
                bufdst[x*1+0] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
            } break;
            }

            px += dx;
            x++;
        }

        py += dy;
        y++;

        if (y != dstysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Blit_32bit_32bit
//-----------------------------------------------------------------------------
static void
Blit_32bit_32bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = dstxsize * 4;
    uint32_t srcpitch = srcxsize * 4;
    uint32_t xsize = (srcxsize < dstxsize) ? dstxsize : srcxsize;
    uint32_t ysize = (srcysize < dstysize) ? dstysize : srcysize;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < ysize)
    {
        bufdst = rawdst;
        bufsrc = rawsrc + ((y%srcysize)*srcpitch);
        x = 0;

        while (x < xsize)
        {
            bufdst[x*4+0] = bufsrc[(x%srcxsize)*4+0];
            bufdst[x*4+1] = bufsrc[(x%srcxsize)*4+1];
            bufdst[x*4+2] = bufsrc[(x%srcxsize)*4+2];
            bufdst[x*4+3] = bufsrc[(x%srcxsize)*4+3];
            x++;
        }

        y++;

        if (y != ysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Blit_24bit_24bit
//-----------------------------------------------------------------------------
static void
Blit_24bit_24bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = dstxsize * 3;
    uint32_t srcpitch = srcxsize * 3;
    uint32_t xsize = (srcxsize < dstxsize) ? dstxsize : srcxsize;
    uint32_t ysize = (srcysize < dstysize) ? dstysize : srcysize;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < ysize)
    {
        bufdst = rawdst;
        bufsrc = rawsrc + ((y%srcysize)*srcpitch);
        x = 0;

        while (x < xsize)
        {
            bufdst[x*3+0] = bufsrc[(x%srcxsize)*3+0];
            bufdst[x*3+1] = bufsrc[(x%srcxsize)*3+1];
            bufdst[x*3+2] = bufsrc[(x%srcxsize)*3+2];
            x++;
        }

        y++;

        if (y != ysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Blit_16bit_16bit
//-----------------------------------------------------------------------------
static void
Blit_16bit_16bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = dstxsize * 2;
    uint32_t srcpitch = srcxsize * 2;
    uint32_t xsize = (srcxsize < dstxsize) ? dstxsize : srcxsize;
    uint32_t ysize = (srcysize < dstysize) ? dstysize : srcysize;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < ysize)
    {
        bufdst = rawdst;
        bufsrc = rawsrc + ((y%srcysize)*srcpitch);
        x = 0;

        while (x < xsize)
        {
            bufdst[x*2+0] = bufsrc[(x%srcxsize)*2+0];
            bufdst[x*2+1] = bufsrc[(x%srcxsize)*2+1];
            x++;
        }

        y++;

        if (y != ysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Blit_8bit_8bit
//-----------------------------------------------------------------------------
static void
Blit_8bit_8bit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t xsize = (srcxsize < dstxsize) ? dstxsize : srcxsize;
    uint32_t ysize = (srcysize < dstysize) ? dstysize : srcysize;
    uint32_t x = 0;
    uint32_t y = 0;

    while (y < ysize)
    {
        bufdst = rawdst;
        bufsrc = rawsrc + ((y%srcysize)*srcxsize);
        x = 0;

        while (x < xsize)
        {
            bufdst[x] = bufsrc[(x%srcxsize)];
            x++;
        }

        y++;

        if (y != ysize)
        {
            rawdst += dstxsize;
        }
    }
}

//-----------------------------------------------------------------------------
// Blit_32bit_Nbit
//-----------------------------------------------------------------------------
static void
Blit_32bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize * 4;
    uint32_t xsize = (srcxsize < dstxsize) ? dstxsize : srcxsize;
    uint32_t ysize = (srcysize < dstysize) ? dstysize : srcysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t rshift = 0;
    uint32_t gshift = 0;
    uint32_t bshift = 0;
    uint32_t ashift = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    switch (srcformat)
    {
    case PIXELTYPE_RGBA:
    {
        rmask = 0xFF000000;
        gmask = 0x00FF0000;
        bmask = 0x0000FF00;
        amask = 0x000000FF;

        rshift = 24;
        gshift = 16;
        bshift =  8;
        ashift =  0;
    } break;
    case PIXELTYPE_ABGR:
    {
        amask = 0xFF000000;
        bmask = 0x00FF0000;
        gmask = 0x0000FF00;
        rmask = 0x000000FF;

        rshift =  0;
        gshift =  8;
        bshift = 16;
        ashift = 24;
    } break;
    case PIXELTYPE_BGRA:
    {
        bmask = 0xFF000000;
        gmask = 0x00FF0000;
        rmask = 0x0000FF00;
        amask = 0x000000FF;

        rshift =  8;
        gshift = 16;
        bshift = 24;
        ashift =  0;
    } break;
    }

    while (y < ysize)
    {
        bufdst = rawdst;
        bufsrc = rawsrc + ((y%srcysize) * srcpitch);
        x = 0;

        while (x < xsize)
        {
            uint32_t pixel = (bufsrc[(x%srcxsize)*4+0] << 24)
                | (bufsrc[(x%srcxsize)*4+1] << 16)
                | (bufsrc[(x%srcxsize)*4+2] <<  8)
                | (bufsrc[(x%srcxsize)*4+3] <<  0);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = (pixel & rmask) >> rshift;
                bufdst[x*4+1] = (pixel & gmask) >> gshift;
                bufdst[x*4+2] = (pixel & bmask) >> bshift;
                bufdst[x*4+3] = (pixel & amask) >> ashift;
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = (pixel & amask) >> rshift;
                bufdst[x*4+1] = (pixel & bmask) >> gshift;
                bufdst[x*4+2] = (pixel & gmask) >> bshift;
                bufdst[x*4+3] = (pixel & rmask) >> ashift;
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = (pixel & bmask) >> bshift;
                bufdst[x*4+1] = (pixel & gmask) >> gshift;
                bufdst[x*4+2] = (pixel & rmask) >> rshift;
                bufdst[x*4+3] = (pixel & amask) >> ashift;
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = (pixel & rmask) >> rshift;
                bufdst[x*3+1] = (pixel & gmask) >> gshift;
                bufdst[x*3+2] = (pixel & bmask) >> bshift;
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = (pixel & bmask) >> bshift;
                bufdst[x*3+1] = (pixel & gmask) >> gshift;
                bufdst[x*3+2] = (pixel & rmask) >> rshift;
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = (((pixel & rmask) >> rshift) * 0x1F) / 0xFF;
                g = (((pixel & gmask) >> gshift) * 0x1F) / 0xFF;
                b = (((pixel & bmask) >> bshift) * 0x1F) / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF) >> 0;
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)(((pixel & rmask) >> rshift) * 0.2990f +
                    ((pixel & gmask) >> gshift) * 0.5870f +
                    ((pixel & bmask) >> bshift) * 0.1140f);
                bufdst[x*2+1] = (pixel & amask);
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                bufdst[x*1+0] = (uint8_t)(((pixel & rmask) >> rshift) * 0.2990f +
                    ((pixel & gmask) >> gshift) * 0.5870f +
                    ((pixel & bmask) >> bshift) * 0.1140f);
            } break;
            }

            x++;
        }

        y++;

        if (y != ysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Blit_24bit_Nbit
//-----------------------------------------------------------------------------
static void
Blit_24bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize * 3;
    uint32_t xsize = (srcxsize < dstxsize) ? dstxsize : srcxsize;
    uint32_t ysize = (srcysize < dstysize) ? dstysize : srcysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t rshift = 0;
    uint32_t gshift = 0;
    uint32_t bshift = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    switch (srcformat)
    {
    case PIXELTYPE_RGB:
    {
        rmask = 0x00FF0000;
        gmask = 0x0000FF00;
        bmask = 0x000000FF;
        
        rshift = 16;
        gshift =  8;
        bshift =  0;
    } break;
    case PIXELTYPE_BGR:
    {
        bmask = 0x00FF0000;
        gmask = 0x0000FF00;
        rmask = 0x000000FF;

        rshift =  0;
        gshift =  8;
        bshift = 16;
    } break;
    }

    while (y < ysize)
    {
        bufdst = rawdst;
        bufsrc = rawsrc + ((y%srcysize) * srcpitch);
        x = 0;

        while (x < xsize)
        {
            uint32_t pixel = (bufsrc[(x%srcxsize)*3+0] << 16)
                | (bufsrc[(x%srcxsize)*3+1] <<  8)
                | (bufsrc[(x%srcxsize)*3+2] <<  0);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = (pixel & rmask) >> rshift;
                bufdst[x*4+1] = (pixel & gmask) >> gshift;
                bufdst[x*4+2] = (pixel & bmask) >> bshift;
                bufdst[x*4+3] = 255;
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = 255;
                bufdst[x*4+1] = (pixel & bmask) >> bshift;
                bufdst[x*4+2] = (pixel & gmask) >> gshift;
                bufdst[x*4+3] = (pixel & rmask) >> rshift;
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = (pixel & bmask) >> bshift;
                bufdst[x*4+1] = (pixel & gmask) >> gshift;
                bufdst[x*4+2] = (pixel & rmask) >> rshift;
                bufdst[x*4+3] = 255;
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = (pixel & rmask) >> rshift;
                bufdst[x*3+1] = (pixel & gmask) >> gshift;
                bufdst[x*3+2] = (pixel & bmask) >> bshift;
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = (pixel & bmask) >> bshift;
                bufdst[x*3+1] = (pixel & gmask) >> gshift;
                bufdst[x*3+2] = (pixel & rmask) >> rshift;
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = (((pixel & rmask) >> rshift) * 0x1F) / 0xFF;
                g = (((pixel & gmask) >> gshift) * 0x1F) / 0xFF;
                b = (((pixel & bmask) >> bshift) * 0x1F) / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF) >> 0;
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)(((pixel & rmask) >> rshift) * 0.2990f +
                    ((pixel & gmask) >> gshift) * 0.5870f +
                    ((pixel & bmask) >> bshift) * 0.1140f);
                bufdst[x*2+1] = 255;
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                bufdst[x*1+0] = (uint8_t)(((pixel & rmask) >> rshift) * 0.2990f +
                    ((pixel & gmask) >> gshift) * 0.5870f +
                    ((pixel & bmask) >> bshift) * 0.1140f);
            } break;
            }

            x++;
        }

        y++;

        if (y != ysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Blit_16bit_Nbit
//-----------------------------------------------------------------------------
static void
Blit_16bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    float rmod = 0.0f;
    float gmod = 0.0f;
    float bmod = 0.0f;
    float amod = 0.0f;
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize * 2;
    uint32_t xsize = (srcxsize < dstxsize) ? dstxsize : srcxsize;
    uint32_t ysize = (srcysize < dstysize) ? dstysize : srcysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t rshift = 0;
    uint32_t gshift = 0;
    uint32_t bshift = 0;
    uint32_t ashift = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    switch (srcformat)
    {
    case PIXELTYPE_XBGR1555:
    {
        rmask = 0x7C00;
        gmask = 0x03E0;
        bmask = 0x001F;
        amask = 0x8000;

        rshift = 10;
        gshift =  5;
        bshift =  0;
        ashift = 15;

        rmod = 255.0f / 31.0f;
        gmod = 255.0f / 31.0f;
        bmod = 255.0f / 31.0f;
        amod = 0.0f;
    } break;
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        rmask = 0xFF00;
        gmask = 0xFF00;
        bmask = 0xFF00;
        amask = 0x00FF;

        rshift = 8;
        gshift = 8;
        bshift = 8;
        ashift = 0;

        rmod = 0.2990f;
        gmod = 0.5870f;
        bmod = 0.1140f;
        amod = 1.0f;
    } break;
    }

    while (y < ysize)
    {
        bufdst = rawdst;
        bufsrc = rawsrc + ((y%srcysize) * srcpitch);
        x = 0;

        while (x < xsize)
        {
            uint16_t pixel = (bufsrc[(x%srcxsize)*2+0] << 8)
                | (bufsrc[(x%srcxsize)*2+1] << 0);

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = ((pixel & rmask) >> rshift) * rmod;
                bufdst[x*4+1] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*4+2] = ((pixel & bmask) >> bshift) * bmod;
                bufdst[x*4+3] = ((pixel & amask) >> ashift) * amod;
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = ((pixel & amask) >> ashift) * amod;
                bufdst[x*4+1] = ((pixel & bmask) >> bshift) * bmod;
                bufdst[x*4+2] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*4+3] = ((pixel & rmask) >> rshift) * rmod;
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = ((pixel & bmask) >> bshift) * bmod;
                bufdst[x*4+1] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*4+2] = ((pixel & rmask) >> rshift) * rmod;
                bufdst[x*4+3] = ((pixel & amask) >> ashift) * amod;
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = ((pixel & rmask) >> rshift) * rmod;
                bufdst[x*3+1] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*3+2] = ((pixel & bmask) >> bshift) * bmod;
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = ((pixel & bmask) >> bshift) * bmod;
                bufdst[x*3+1] = ((pixel & gmask) >> gshift) * gmod;
                bufdst[x*3+2] = ((pixel & rmask) >> rshift) * rmod;
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = (((pixel & rmask) >> rshift) * 0x1F) / 0xFF;
                g = (((pixel & gmask) >> gshift) * 0x1F) / 0xFF;
                b = (((pixel & bmask) >> bshift) * 0x1F) / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF) >> 0;
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)(((pixel & rmask) >> rshift) * 0.2990f +
                    ((pixel & gmask) >> gshift) * 0.5870f +
                    ((pixel & bmask) >> bshift) * 0.1140f);
                bufdst[x*2+1] = 255;
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                bufdst[x*1+0] = (uint8_t)((((pixel & rmask) >> rshift) * rmod) * 0.2990f +
                    (((pixel & gmask) >> gshift) * gmod) * 0.5870f +
                    (((pixel & bmask) >> bshift) * bmod) * 0.1140f);
            } break;
            }

            x++;
        }

        y++;

        if (y != ysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Blit_8bit_Nbit
//-----------------------------------------------------------------------------
static void
Blit_8bit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize;
    uint32_t xsize = (srcxsize < dstxsize) ? dstxsize : srcxsize;
    uint32_t ysize = (srcysize < dstysize) ? dstysize : srcysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;
    uint8_t v = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    }

    while (y < ysize)
    {
        bufdst = rawdst;
        bufsrc = rawsrc + ((y%srcysize) * srcpitch);
        x = 0;

        while (x < xsize)
        {
            v = bufsrc[(x%srcxsize)*1+0];
            b = v;
            g = v;
            r = v;

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = r;
                bufdst[x*4+1] = g;
                bufdst[x*4+2] = b;
                bufdst[x*4+3] = a;
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = a;
                bufdst[x*4+1] = b;
                bufdst[x*4+2] = g;
                bufdst[x*4+3] = r;
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = b;
                bufdst[x*4+1] = g;
                bufdst[x*4+2] = r;
                bufdst[x*4+3] = a;
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = r;
                bufdst[x*3+1] = g;
                bufdst[x*3+2] = b;
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = b;
                bufdst[x*3+1] = g;
                bufdst[x*3+2] = r;
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = (r * 0x1F) / 0xFF;
                g = (g * 0x1F) / 0xFF;
                b = (b * 0x1F) / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF) >> 0;
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f +
                    g * 0.5870f +
                    b * 0.1140f);
                bufdst[x*2+1] = a;
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                bufdst[x*1+0] = (uint8_t)(r * 0.2990f +
                    g * 0.5870f +
                    b * 0.1140f);
            } break;
            }

            x++;
        }

        y++;

        if (y != ysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// Blit_PAL_Nbit
//-----------------------------------------------------------------------------
static void
Blit_PAL_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat, palette_t* ppalette)
{
    uint8_t* rawdst = pdst;
    uint8_t* rawsrc = psrc;
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = 0;
    uint32_t srcpitch = srcxsize;
    uint32_t xsize = (srcxsize < dstxsize) ? dstxsize : srcxsize;
    uint32_t ysize = (srcysize < dstysize) ? dstysize : srcysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint16_t rgb16 = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
    uint8_t v = 0;

    switch (dstformat)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    {
        dstpitch = dstxsize * 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstpitch = dstxsize * 3;
    } break;
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstpitch = dstxsize * 2;
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dstpitch = dstxsize;
    } break;
    }

    while (y < ysize)
    {
        bufdst = rawdst;
        bufsrc = rawsrc + ((y%srcysize) * srcpitch);
        x = 0;

        while (x < xsize)
        {
            v = bufsrc[(x%srcxsize)*1+0];
            b = ppalette->data[v].b;
            g = ppalette->data[v].g;
            r = ppalette->data[v].r;
            a = ppalette->data[v].a;

            switch (dstformat)
            {
            case PIXELTYPE_RGBA:
            {
                bufdst[x*4+0] = r;
                bufdst[x*4+1] = g;
                bufdst[x*4+2] = b;
                bufdst[x*4+3] = a;
            } break;
            case PIXELTYPE_ABGR:
            {
                bufdst[x*4+0] = a;
                bufdst[x*4+1] = b;
                bufdst[x*4+2] = g;
                bufdst[x*4+3] = r;
            } break;
            case PIXELTYPE_BGRA:
            {
                bufdst[x*4+0] = b;
                bufdst[x*4+1] = g;
                bufdst[x*4+2] = r;
                bufdst[x*4+3] = a;
            } break;
            case PIXELTYPE_RGB:
            {
                bufdst[x*3+0] = r;
                bufdst[x*3+1] = g;
                bufdst[x*3+2] = b;
            } break;
            case PIXELTYPE_BGR:
            {
                bufdst[x*3+0] = b;
                bufdst[x*3+1] = g;
                bufdst[x*3+2] = r;
            } break;
            case PIXELTYPE_XBGR1555:
            {
                r = (r * 0x1F) / 0xFF;
                g = (g * 0x1F) / 0xFF;
                b = (b * 0x1F) / 0xFF;

                rgb16 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                bufdst[x*2+0] = (rgb16 & 0x00FF) >> 0;
                bufdst[x*2+1] = (rgb16 & 0xFF00) >> 8;
            } break;
            case PIXELTYPE_LUMINANCE_ALPHA:
            {
                bufdst[x*2+0] = (uint8_t)(r * 0.2990f +
                    g * 0.5870f +
                    b * 0.1140f);
                bufdst[x*2+1] = a;
            } break;
            case PIXELTYPE_LUMINANCE:
            {
                bufdst[x*1+0] = (uint8_t)(r * 0.2990f +
                    g * 0.5870f +
                    b * 0.1140f);
            } break;
            }

            x++;
        }

        y++;

        if (y != ysize)
        {
            rawdst += dstpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// ResampleImage
//-----------------------------------------------------------------------------
static bool
ResampleImage(image_t* pdstimage, rect_t* pdstrect, image_t* psrcimage,
    palette_t* ppalette, rect_t* psrcrect, uint32_t filter)
{
    if (pdstimage == NULL || psrcimage == NULL)
    {
        return false;
    }

    // dst stuff
    rect_t dstrect = {};

    if (pdstrect == NULL)
    {
        dstrect.min[0] = 0;
        dstrect.min[1] = 0;
        dstrect.max[0] = pdstimage->xsize;
        dstrect.max[1] = pdstimage->ysize;
    }
    else
    {
        dstrect = *pdstrect;
    }

    uint32_t dstxorigin = dstrect.min[0] < 0 ? 0 : dstrect.min[0];
    uint32_t dstyorigin = dstrect.min[1] < 0 ? 0 : dstrect.min[1];
    uint32_t dstxextent = ABS(dstrect.max[0]) - dstxorigin;
    uint32_t dstyextent = ABS(dstrect.max[1]) - dstyorigin;

    if (dstxextent > pdstimage->xsize || dstyextent > pdstimage->ysize)
    {
        fprintf(stderr, "ResampleImage, dst rectangle exceeds image bounds.\n");
        return false;
    }

    int32_t dstbytesperpixel = 0;

    switch (pdstimage->pixeltype)
    {
    case PIXELTYPE_ABGR:
    case PIXELTYPE_RGBA:
    case PIXELTYPE_BGRA:
    {
        dstbytesperpixel = 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        dstbytesperpixel = 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dstbytesperpixel = 2;
    } break;
    default:
    {
        dstbytesperpixel = 1;
    }
    }

    if (pdstimage->data == NULL)
    {
        pdstimage->data = (uint8_t*)malloc(dstxextent * dstyextent * dstbytesperpixel);
        memset(pdstimage->data, 0, dstxextent * dstyextent * dstbytesperpixel);
    }

    int32_t dstpitch = dstxextent * dstbytesperpixel;
    uint8_t* dstbuf = pdstimage->data + (dstyorigin * dstpitch) +
        (dstxorigin * dstbytesperpixel);            // start of current row

    rect_t srcrect = {};

    if (psrcrect == NULL)
    {
        srcrect.min[0] = 0;
        srcrect.min[1] = 0;
        srcrect.max[0] = psrcimage->xsize;
        srcrect.max[1] = psrcimage->ysize;
    }
    else
    {
        srcrect = *psrcrect;
    }

    // src stuff
    uint32_t srcxorigin = srcrect.min[0] < 0 ? 0 : srcrect.min[0];
    uint32_t srcyorigin = srcrect.min[1] < 0 ? 0 : srcrect.min[1];
    uint32_t srcxextent = ABS(srcrect.max[0]) - srcxorigin;
    uint32_t srcyextent = ABS(srcrect.max[1]) - srcyorigin;

    if (srcxextent > psrcimage->xsize || srcyextent > psrcimage->ysize)
    {
        fprintf(stderr, "ResampleImage, src rectangle exceeds image bounds.\n");
        return false;
    }

    int32_t srcbytesperpixel = 0;
    
    switch (psrcimage->pixeltype)
    {
    case PIXELTYPE_ABGR:
    case PIXELTYPE_RGBA:
    case PIXELTYPE_BGRA:
    {
        srcbytesperpixel = 4;
    } break;
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        srcbytesperpixel = 3;
    } break;
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        srcbytesperpixel = 2;
    } break;
    default:
    {
        srcbytesperpixel = 1;
    }
    }

    int32_t srcpitch = psrcimage->xsize * srcbytesperpixel;          // bytes per src span
    uint8_t* srcbuf = psrcimage->data + (srcyorigin * srcpitch) +
        (srcxorigin * srcbytesperpixel);            // start of current row

    bool result = true;

    if (dstxorigin == srcxorigin && dstxextent == srcxextent &&
        dstyorigin == srcyorigin && dstyextent == srcyextent &&
        pdstimage->pixeltype == psrcimage->pixeltype)
    {
        memcpy(dstbuf, srcbuf, dstyextent * dstpitch);
    }
    else
    {
        switch (filter)
        {
        case FILTER_POINT:
        {
            if (pdstimage->pixeltype == psrcimage->pixeltype)
            {
                if (srcbytesperpixel == 4)
                {
                    Point_32bit_32bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 3)
                {
                    Point_24bit_24bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 2)
                {
                    Point_16bit_16bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else
                {
                    Point_8bit_8bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
            }
            else
            {
                if (srcbytesperpixel == 4)
                {
                    Point_32bit_Nbit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 3)
                {
                    Point_24bit_Nbit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 2)
                {
                    Point_16bit_Nbit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else
                {
                    if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX)
                    {
                        Point_PAL_Nbit(dstbuf, dstxextent, dstyextent,
                            pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                            psrcimage->pixeltype, ppalette);
                    }
                    else
                    {
                        Point_8bit_Nbit(dstbuf, dstxextent, dstyextent,
                            pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                            psrcimage->pixeltype);
                    }
                }
            }
        } break;
        case FILTER_LINEAR:
        {
            if (pdstimage->pixeltype == psrcimage->pixeltype)
            {
                if (srcbytesperpixel == 4)
                {
                    Linear_32bit_32bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 3)
                {
                    Linear_24bit_24bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 2)
                {
                    Linear_16bit_16bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else
                {
                    Linear_8bit_8bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
            }
            else
            {
                if (srcbytesperpixel == 4)
                {
                    Linear_32bit_Nbit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 3)
                {
                    Linear_24bit_Nbit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 2)
                {
                    Linear_16bit_Nbit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else
                {
                    if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX)
                    {
                        Linear_PAL_Nbit(dstbuf, dstxextent, dstyextent,
                            pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                            psrcimage->pixeltype, ppalette);
                    }
                    else
                    {
                        Linear_8bit_Nbit(dstbuf, dstxextent, dstyextent,
                            pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                            psrcimage->pixeltype);
                    }
                }
            }
        } break;
        default:
        {
            if (pdstimage->pixeltype == psrcimage->pixeltype)
            {
                if (srcbytesperpixel == 4)
                {
                    Blit_32bit_32bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 3)
                {
                    Blit_24bit_24bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 2)
                {
                    Blit_16bit_16bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else
                {
                    Blit_8bit_8bit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
            }
            else
            {
                if (srcbytesperpixel == 4)
                {
                    Blit_32bit_Nbit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 3)
                {
                    Blit_24bit_Nbit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else if (srcbytesperpixel == 2)
                {
                    Blit_16bit_Nbit(dstbuf, dstxextent, dstyextent,
                        pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                        psrcimage->pixeltype);
                }
                else
                {
                    if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX)
                    {
                        Blit_PAL_Nbit(dstbuf, dstxextent, dstyextent,
                            pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                            psrcimage->pixeltype, ppalette);
                    }
                    else
                    {
                        Blit_8bit_Nbit(dstbuf, dstxextent, dstyextent,
                            pdstimage->pixeltype, srcbuf, srcxextent, srcyextent,
                            psrcimage->pixeltype);
                    }
                }
            }
        } break;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
// ReplaceColor
//-----------------------------------------------------------------------------
static void
ReplaceColor(image_t* image, palette_t* ppalette, rgba_t dstcolorkey,
    rgba_t srccolorkey)
{
    uint8_t* rawsrc = NULL;
    uint8_t* bufdst = NULL;
    uint8_t* pixels = NULL;
    uint32_t xsize = 0;
    uint32_t ysize = 0;
    uint32_t pitch = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    if (image == NULL)
    {
        return;
    }

    if (image->data == NULL)
    {
        return;
    }

    rawsrc = image->data;
    xsize = image->xsize;
    ysize = image->ysize;

    if (image->pixeltype == PIXELTYPE_RGBA)
    {
        pitch = xsize * 4;
        while (y < ysize)
        {
            bufdst = rawsrc;
            x = 0;
            while (x < xsize)
            {
                if (bufdst[x*4+0] == srccolorkey.r &&
                    bufdst[x*4+1] == srccolorkey.g &&
                    bufdst[x*4+2] == srccolorkey.b &&
                    bufdst[x*4+3] == srccolorkey.a)
                {
                    bufdst[x*4+0] = dstcolorkey.r;
                    bufdst[x*4+1] = dstcolorkey.g;
                    bufdst[x*4+2] = dstcolorkey.b;
                    bufdst[x*4+3] = dstcolorkey.a;
                }
                x++;
            }
            y++;

            if (y != ysize)
            {
                rawsrc += pitch;
            }
        }
    }
    else if (image->pixeltype == PIXELTYPE_RGB)
    {
        pitch = xsize * 3;
        while (y < ysize)
        {
            bufdst = rawsrc;
            x = 0;
            while (x < xsize)
            {
                if (bufdst[x*3+0] == srccolorkey.r &&
                    bufdst[x*3+1] == srccolorkey.g &&
                    bufdst[x*3+2] == srccolorkey.b)
                {
                    bufdst[x*3+0] = dstcolorkey.r;
                    bufdst[x*3+1] = dstcolorkey.g;
                    bufdst[x*3+2] = dstcolorkey.b;
                }
                x++;
            }
            y++;

            if (y != ysize)
            {
                rawsrc += pitch;
            }
        }
    }
    else if (image->pixeltype == PIXELTYPE_ABGR)
    {
        pitch = xsize * 4;
        while (y < ysize)
        {
            bufdst = rawsrc;
            x = 0;
            while (x < xsize)
            {
                if (bufdst[x*4+0] == srccolorkey.a &&
                    bufdst[x*4+1] == srccolorkey.b &&
                    bufdst[x*4+2] == srccolorkey.g &&
                    bufdst[x*4+3] == srccolorkey.r)
                {
                    bufdst[x*4+0] = dstcolorkey.a;
                    bufdst[x*4+1] = dstcolorkey.b;
                    bufdst[x*4+2] = dstcolorkey.g;
                    bufdst[x*4+3] = dstcolorkey.r;
                }
                x++;
            }
            y++;

            if (y != ysize)
            {
                rawsrc += pitch;
            }
        }
    }
    else if (image->pixeltype == PIXELTYPE_BGRA)
    {
        pitch = xsize * 4;
        while (y < ysize)
        {
            bufdst = rawsrc;
            x = 0;
            while (x < xsize)
            {
                if (bufdst[x*4+0] == srccolorkey.b &&
                    bufdst[x*4+1] == srccolorkey.g &&
                    bufdst[x*4+2] == srccolorkey.r &&
                    bufdst[x*4+3] == srccolorkey.a)
                {
                    bufdst[x*4+0] = dstcolorkey.b;
                    bufdst[x*4+1] = dstcolorkey.g;
                    bufdst[x*4+2] = dstcolorkey.r;
                    bufdst[x*4+3] = dstcolorkey.a;
                }
                x++;
            }
            y++;

            if (y != ysize)
            {
                rawsrc += pitch;
            }
        }
    }
    else if (image->pixeltype == PIXELTYPE_BGR)
    {
        pitch = xsize * 3;
        while (y < ysize)
        {
            bufdst = rawsrc;
            x = 0;
            while (x < xsize)
            {
                if (bufdst[x*3+0] == srccolorkey.b &&
                    bufdst[x*3+1] == srccolorkey.g &&
                    bufdst[x*3+2] == srccolorkey.r)
                {
                    bufdst[x*3+0] = dstcolorkey.b;
                    bufdst[x*3+1] = dstcolorkey.g;
                    bufdst[x*3+2] = dstcolorkey.r;
                }
                x++;
            }
            y++;

            if (y != ysize)
            {
                rawsrc += pitch;
            }
        }
    }
    else if (image->pixeltype == PIXELTYPE_XBGR1555)
    {
        pitch = xsize * 2;
        uint8_t r0 = (dstcolorkey.r * 0x1F) / 0xFF;
        uint8_t g0 = (dstcolorkey.g * 0x1F) / 0xFF;
        uint8_t b0 = (dstcolorkey.b * 0x1F) / 0xFF;
        uint8_t r1 = (srccolorkey.r * 0x1F) / 0xFF;
        uint8_t g1 = (srccolorkey.g * 0x1F) / 0xFF;
        uint8_t b1 = (srccolorkey.b * 0x1F) / 0xFF;
        uint16_t dstcolor = ((r0 & 0xFF) << 10) | ((g0 & 0xFF) << 5) | (b0 & 0xFF);
        uint16_t srccolor = ((r1 & 0xFF) << 10) | ((g1 & 0xFF) << 5) | (b1 & 0xFF);

        while (y < ysize)
        {
            bufdst = rawsrc;
            x = 0;
            while (x < xsize)
            {                
                if (bufdst[x*2+0] == (srccolor & 0x00FF) &&
                    bufdst[x*2+1] == (srccolor & 0xFF00) >> 8)
                {

                    bufdst[x*2+0] = (dstcolor & 0x00FF);
                    bufdst[x*2+1] = (dstcolor & 0xFF00) >> 8;
                }
                x++;
            }
            y++;

            if (y != ysize)
            {
                rawsrc += pitch;
            }
        }
    }
    else if (image->pixeltype == PIXELTYPE_LUMINANCE_ALPHA)
    {
        pitch = xsize * 2;
        while (y < ysize)
        {
            bufdst = rawsrc;
            x = 0;
            while (x < xsize)
            {
                if (bufdst[x*2+0] == srccolorkey.r &&
                    bufdst[x*2+1] == srccolorkey.a)
                {
                    bufdst[x*2+0] = dstcolorkey.r;
                    bufdst[x*2+1] = dstcolorkey.a;
                }
                x++;
            }
            y++;

            if (y != ysize)
            {
                rawsrc += pitch;
            }
        }
    }
    else if (image->pixeltype == PIXELTYPE_LUMINANCE)
    {
        pitch = xsize * 1;
        while (y < ysize)
        {
            bufdst = rawsrc;
            x = 0;
            while (x < xsize)
            {
                if (bufdst[x+0] == srccolorkey.r)
                {
                    bufdst[x+0] = dstcolorkey.r;
                }
                x++;
            }
            y++;

            if (y != ysize)
            {
                rawsrc += pitch;
            }
        }
    }
    else if (image->pixeltype == PIXELTYPE_COLOUR_INDEX)
    {
        if (ppalette == NULL)
        {
            return;
        }

        pitch = xsize * 1;
        int32_t dstcolorindex = -1;
        int32_t srccolorindex = -1;

        for (uint32_t i = 0; i < ppalette->size; ++i)
        {
            if (ppalette->data[i].r == dstcolorkey.r &&
                ppalette->data[i].g == dstcolorkey.g &&
                ppalette->data[i].b == dstcolorkey.b &&
                ppalette->data[i].a == dstcolorkey.a)
            {
                dstcolorindex = i;
                break;
            }
        }

        for (uint32_t i = 0; i < ppalette->size; ++i)
        {
            if (ppalette->data[i].r == srccolorkey.r &&
                ppalette->data[i].g == srccolorkey.g &&
                ppalette->data[i].b == srccolorkey.b &&
                ppalette->data[i].a == srccolorkey.a)
            {
                srccolorindex = i;
                break;
            }
        }

        if ((dstcolorindex != srccolorindex) && dstcolorindex != -1 &&
            srccolorindex != -1)
        {
            while (y < ysize)
            {
                bufdst = rawsrc;
                x = 0;
                while (x < xsize)
                {
                    if (bufdst[x+0] == srccolorindex)
                    {
                        bufdst[x+0] = dstcolorindex;
                    }
                    x++;
                }
                y++;

                if (y != ysize)
                {
                    rawsrc += pitch;
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// SaveImageToMemory
//-----------------------------------------------------------------------------
bool
SaveImageToMemory(uint8_t** ppdst, uint32_t* ppdstsize, file_format_t format,
    encode_t dstcodec, image_t* psrcimage, palette_t* psrcpalette,
    rect_t* psrcrect)
{
    bool result = false;
    image_t dstimage = {
        NULL,
        0,
        0,
        PIXELTYPE_UNKNOWN
    };
    uint32_t depthbits = 0;
    pixel_t dstformat = PIXELTYPE_UNKNOWN;

    if (psrcimage != NULL)
    {
        switch (format)
        {
            case FILEFORMAT_PNG:
            {
                // supported output formats
                if (psrcimage->pixeltype == PIXELTYPE_RGBA
                    || psrcimage->pixeltype == PIXELTYPE_ABGR
                    || psrcimage->pixeltype == PIXELTYPE_BGRA)
                {
                    dstformat = PIXELTYPE_RGBA;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_RGB
                    || psrcimage->pixeltype == PIXELTYPE_BGR
                    || psrcimage->pixeltype == PIXELTYPE_XBGR1555)
                {
                    dstformat = PIXELTYPE_RGB;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_LUMINANCE_ALPHA)
                {
                    dstformat = PIXELTYPE_LUMINANCE_ALPHA;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_LUMINANCE)
                {
                    dstformat = PIXELTYPE_LUMINANCE;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX)
                {
                    dstformat = PIXELTYPE_COLOUR_INDEX;
                }
                else
                {
                    fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        convert src data to dst format.\n");
                    return false;
                }
            } break;
            case FILEFORMAT_BMP:
            {
                // supported output formats
                if (psrcimage->pixeltype == PIXELTYPE_RGBA
                    || psrcimage->pixeltype == PIXELTYPE_ABGR
                    || psrcimage->pixeltype == PIXELTYPE_BGRA)
                {
                    dstformat = PIXELTYPE_BGRA;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_RGB
                    || psrcimage->pixeltype == PIXELTYPE_BGR
                    || psrcimage->pixeltype == PIXELTYPE_XBGR1555
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE_ALPHA
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE)
                {
                    dstformat = PIXELTYPE_BGR;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX)
                {
                    dstformat = PIXELTYPE_COLOUR_INDEX;
                }
                else
                {
                    fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        convert src data to dst format.\n");
                    return false;
                }
            } break;
            case FILEFORMAT_PCX:
            {
                // supported output formats
                if (psrcimage->pixeltype == PIXELTYPE_RGBA
                    || psrcimage->pixeltype == PIXELTYPE_RGB
                    || psrcimage->pixeltype == PIXELTYPE_XBGR1555
                    || psrcimage->pixeltype == PIXELTYPE_ABGR
                    || psrcimage->pixeltype == PIXELTYPE_BGRA
                    || psrcimage->pixeltype == PIXELTYPE_BGR
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE_ALPHA
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE)
                {
                    dstformat = PIXELTYPE_RGB;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX)
                {
                    dstformat = PIXELTYPE_COLOUR_INDEX;
                }
                else
                {
                    fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        convert src data to dst format.\n");
                    return false;
                }
            } break;
            case FILEFORMAT_TGA:
            {
                // supported output formats
                if (psrcimage->pixeltype == PIXELTYPE_RGBA
                    || psrcimage->pixeltype == PIXELTYPE_ABGR
                    || psrcimage->pixeltype == PIXELTYPE_BGRA)
                {
                    dstformat = PIXELTYPE_BGRA;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_RGB
                    || psrcimage->pixeltype == PIXELTYPE_BGR)
                {
                    dstformat = PIXELTYPE_BGR;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_XBGR1555)
                {
                    dstformat = PIXELTYPE_XBGR1555;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_LUMINANCE_ALPHA
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE)
                {
                    dstformat = PIXELTYPE_LUMINANCE;
                }
                else if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX)
                {
                    dstformat = PIXELTYPE_COLOUR_INDEX;
                }
                else
                {
                    fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        convert src data to dst format.\n");
                    return false;
                }
            } break;
            default:
            {
                fprintf(stderr, "SaveImage, Unsupported image format\n");
            }
        }

        switch (dstformat)
        {
        case PIXELTYPE_RGBA:
        case PIXELTYPE_ABGR:
        case PIXELTYPE_BGRA:
        {
            depthbits = 32;
        } break;
        case PIXELTYPE_RGB:
        case PIXELTYPE_BGR:
        {
            depthbits = 24;
        } break;
        case PIXELTYPE_XBGR1555:
        case PIXELTYPE_LUMINANCE_ALPHA:
        {
            depthbits = 16;
        } break;
        case PIXELTYPE_LUMINANCE:
        case PIXELTYPE_COLOUR_INDEX:
        {
            depthbits = 8;
        } break;
        }

        dstimage.xsize = psrcimage->xsize;
        dstimage.ysize = psrcimage->ysize;
        dstimage.pixeltype = dstformat;

        // convert to dst format
        result = ResampleImage(&dstimage, psrcrect, psrcimage, psrcpalette,
            psrcrect, FILTER_NONE);

        if (dstimage.pixeltype != PIXELTYPE_COLOUR_INDEX)
        {
            psrcpalette = NULL;
        }

        if (result == true)
        {
            switch (format)
            {
            case FILEFORMAT_PNG:
            {
                result = SaveToMemoryPNG(ppdst, ppdstsize, dstcodec, dstimage.data,
                    dstimage.xsize, dstimage.ysize, depthbits, psrcpalette, NULL);
            } break;
            case FILEFORMAT_BMP:
            {
                result = SaveToMemoryBMP(ppdst, ppdstsize, dstcodec, dstimage.data,
                    dstimage.xsize, dstimage.ysize, depthbits, psrcpalette, NULL, false);
            } break;
            case FILEFORMAT_PCX:
            {
                result = SaveToMemoryPCX(ppdst, ppdstsize, dstcodec, dstimage.data,
                    dstimage.xsize, dstimage.ysize, depthbits, psrcpalette);
            } break;
            case FILEFORMAT_TGA:
            {
                result = SaveToMemoryTGA(ppdst, ppdstsize, dstcodec, dstimage.data,
                    dstimage.xsize, dstimage.ysize, depthbits, psrcpalette, false,
                    false);
            } break;
            default:
            {
                fprintf(stderr, "SaveImage, Unsupported image format\n");
            }
            }
        }
    }

    if (dstimage.data != NULL)
    {
        free(dstimage.data);
        dstimage.data = NULL;
    }

    return result;
}

//-----------------------------------------------------------------------------
// SaveImageToFile
//-----------------------------------------------------------------------------
bool
SaveImageToFile(const char* pdstfile, file_format_t dstformat, encode_t dstcodec,
    image_t* psrcimage, palette_t* psrcpalette, rect_t* psrcrect)
{
    bool result = false;

    if (pdstfile == NULL || strlen(pdstfile) == 0)
    {
        return result;
    }

    // open file
    FILE* hFile = fopen(pdstfile, "wb");

    uint32_t dstsize = 0;
    uint8_t* rawdst = NULL;
    void* dstbuf = rawdst;

    if (hFile != NULL)
    {
        // save image
        result = SaveImageToMemory(&rawdst, &dstsize, dstformat, dstcodec,
            psrcimage, psrcpalette, psrcrect);
    }
    else
    {
        fprintf(stderr, "SaveImage, Couldn't open file %s.\n", pdstfile);
    }

    size_t fileSize = dstsize;

    // file write
    size_t bytesWritten = 0;

    if (hFile != NULL && fileSize != 0)
    {
        if (dstbuf != NULL)
        {
            bytesWritten = fwrite(dstbuf, sizeof(uint8_t), fileSize, hFile);
        }

        fclose(hFile);
        hFile = NULL;
    }

    if (dstbuf != NULL)
    {
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
GetImageInfoFromMemory(image_info_t* psrcinfo, uint8_t* psrc, uint32_t psrcsize)
{
    bool result = false;
    pixel_t pixeltype = PIXELTYPE_UNKNOWN;
    uint32_t xsize = 0;
    uint32_t ysize = 0;
    uint8_t depthbits = 0;
    uint8_t colormap = 0;
    file_format_t format = FILEFORMAT_NONE;

    if (psrcinfo != NULL) { psrcinfo->fileformat = FILEFORMAT_NONE; }
    if ((result = GetInfoFromMemoryPNG(&colormap, &xsize, &ysize, &depthbits,
        psrc, psrcsize)) == true)
    {
        if (depthbits == 32) { pixeltype = PIXELTYPE_RGBA; }
        else if (depthbits == 24) { pixeltype = PIXELTYPE_RGB; }
        else if (depthbits == 16) { pixeltype = PIXELTYPE_LUMINANCE_ALPHA; }
        else if (depthbits <=  8 && colormap == 0) { pixeltype = PIXELTYPE_LUMINANCE; }
        else if (depthbits <=  8 && colormap == 1) { pixeltype = PIXELTYPE_COLOUR_INDEX; }
        format = FILEFORMAT_PNG;
    }
    else if ((result = GetInfoFromMemoryBMP(&xsize, &ysize, &depthbits, psrc,
        psrcsize)) == true)
    {
        pixeltype = (depthbits == 32) ? PIXELTYPE_BGRA :
                (depthbits == 24) ? PIXELTYPE_BGR : PIXELTYPE_COLOUR_INDEX;
        format = FILEFORMAT_BMP;
    }
    else if ((result = GetInfoFromMemoryPCX(&xsize, &ysize, &depthbits, psrc,
        psrcsize)) == true)
    {
        pixeltype = (depthbits <= 8) ? PIXELTYPE_COLOUR_INDEX : PIXELTYPE_RGB;        
        format = FILEFORMAT_PCX;
    }
    else if ((result = GetInfoFromMemoryTGA(&colormap, &xsize, &ysize,
        &depthbits, psrc, psrcsize)) == true)
    {
        if (depthbits == 32) { pixeltype = PIXELTYPE_BGRA; }
        else if (depthbits == 24) { pixeltype = PIXELTYPE_BGR; }
        else if (depthbits == 16) { pixeltype = PIXELTYPE_XBGR1555; }
        else if (depthbits ==  8 && colormap == 0) { pixeltype = PIXELTYPE_LUMINANCE; }
        else if (depthbits ==  8 && colormap == 1) { pixeltype = PIXELTYPE_COLOUR_INDEX; }
        format = FILEFORMAT_TGA;
    }
    else { fprintf(stderr, "GetImageInfo, Unsupported image format\n"); }

    if (psrcinfo != NULL)
    {
        psrcinfo->pixeltype = pixeltype;
        psrcinfo->xsize = xsize;
        psrcinfo->ysize = ysize;
        psrcinfo->depth = depthbits;
        psrcinfo->fileformat = format;
    }

    return result;
}

//-----------------------------------------------------------------------------
// GetImageInfoFromFile
//-----------------------------------------------------------------------------
bool
GetImageInfoFromFile(image_info_t* psrcinfo, const char* psrcfile)
{
    bool result = false;

    if (psrcfile == NULL || strlen(psrcfile) == 0)
    {
        return result;
    }

    // open file
    FILE* hFile = fopen(psrcfile, "rb");
    long end = 0;
    long pos = 0;

    // file size
    if (hFile != NULL)
    {
        pos = ftell(hFile);
        fseek(hFile,  0L, SEEK_END);
        end = ftell(hFile);
        fseek(hFile, pos, SEEK_SET);
    }
    
    size_t fileSize = (end - pos);

    // allocate memory
    uint8_t* rawsrc = NULL;
    void* srcbuf = rawsrc;

    if (fileSize != 0)
    {
        srcbuf = malloc(((fileSize + 2) & ~1));
        memset(srcbuf, 0, ((fileSize + 2) & ~1));
    }

    rawsrc = (uint8_t*)srcbuf;

    // file read
    size_t bytesRead = 0;

    if (hFile != NULL && fileSize != 0)
    {
        if (srcbuf != NULL)
        {
            bytesRead = fread(srcbuf, sizeof(uint8_t), fileSize, hFile);
        }

        fclose(hFile);
        hFile = NULL;
    }

    if (fileSize == bytesRead && srcbuf != NULL)
    {
        // image info
        result = GetImageInfoFromMemory(psrcinfo, rawsrc, (uint32_t)bytesRead);
    }

    if (srcbuf != NULL)
    {
        free(srcbuf);
        srcbuf = NULL;
    }

    rawsrc = NULL;

    return result;
}

//-----------------------------------------------------------------------------
// LoadImageFromMemory
//-----------------------------------------------------------------------------
bool
LoadImageFromMemory(image_t* pdstimage, palette_t* pdstpalette,
    rect_t* pdstrect, uint8_t* psrc, uint32_t srcsize, pixel_t dstformat,
    rect_t* psrcrect, uint32_t filter, rgba_t colorkey,
    image_info_t* psrcinfo)
{
    bool result = false;
    palette_t srcpalette = {};
    image_t srcimage = {
        NULL,
        0,
        0,
        PIXELTYPE_UNKNOWN
    };
    uint8_t depthbits = 0;
    file_format_t format = FILEFORMAT_NONE;

    if (pdstimage != NULL)
    {
        rgba_t pngcolorkey = { 0, 0, 0, 0 };

        if ((result = LoadFromMemoryPNG(&srcimage.data, &srcpalette, psrc,
            srcsize, &srcimage.xsize, &srcimage.ysize, &depthbits, &pngcolorkey)) == true)
        {
            if (depthbits == 32) { srcimage.pixeltype = PIXELTYPE_RGBA; }
            else if (depthbits == 24) { srcimage.pixeltype = PIXELTYPE_RGB; }
            else if (depthbits == 16) { srcimage.pixeltype = PIXELTYPE_LUMINANCE_ALPHA; }
            else if (depthbits <=  8 && srcpalette.size == 0) { srcimage.pixeltype = PIXELTYPE_LUMINANCE; }
            else if (depthbits <=  8 && srcpalette.size != 0) { srcimage.pixeltype = PIXELTYPE_COLOUR_INDEX; }
            if (pngcolorkey.b != 0 && pngcolorkey.g != 0 && pngcolorkey.r != 0 && pngcolorkey.a != 0)
            {
                colorkey = pngcolorkey;
            }
            format = FILEFORMAT_PNG;
        }
        else if ((result = LoadFromMemoryBMP(&srcimage.data, &srcpalette, psrc,
            srcsize, &srcimage.xsize, &srcimage.ysize, &depthbits)) == true)
        {
            if (depthbits < 8)          // expand packed type
            {
                float srcpixelsperbyte = PIXELS_PER_BYTE(depthbits);
                int32_t srcpitch = (int32_t)(ceilf((float)(srcimage.xsize) / srcpixelsperbyte));
                int32_t dstpitch = (int32_t)(ceilf((float)(srcimage.xsize) / srcpixelsperbyte));

                uint32_t runcount = 0;
                uint32_t bpp = 0;
                uint32_t bit = 0;
                uint32_t padbytes = srcpitch - dstpitch;

                if (depthbits == 1)
                {
                    bpp = 8;
                    bit = 7;
                }
                else if (depthbits == 2)
                {
                    bpp = 4;
                    bit = 6;
                }
                else if (depthbits == 4)
                {
                    bpp = 2;
                    bit = 4;
                }

                uint8_t* pixels = (uint8_t*)malloc((srcimage.xsize * srcimage.ysize));
                uint8_t* pixptr = pixels;
                uint8_t* pixbuf = pixels;
                memset(pixels, 0, srcimage.xsize * srcimage.ysize);

                uint8_t* srcbuf = srcimage.data;
                uint32_t x = 0;
                uint32_t y = 0;

                while (y < srcimage.ysize)
                {
                    pixbuf = pixptr;
                    runcount = bpp;
                    x = srcimage.xsize;

                    while (x > 0)
                    {
                        if (x < runcount) { runcount = x; }
                        ExpandNbitsToIndex8(pixbuf, 1, *srcbuf, runcount, bit);
                        pixbuf += runcount;
                        srcbuf++;
                        x -= runcount;
                    }

                    srcbuf += padbytes;
                    y++;

                    if (y != srcimage.ysize)
                    {
                        pixptr += srcimage.xsize;
                    }
                }

                free(srcimage.data);
                srcimage.data = pixels;
                depthbits = 8;
            }

            srcimage.pixeltype = (depthbits == 32) ? PIXELTYPE_BGRA :
                (depthbits == 24) ? PIXELTYPE_BGR : PIXELTYPE_COLOUR_INDEX;
            format = FILEFORMAT_BMP;
        }
        else if ((result = LoadFromMemoryPCX(&srcimage.data, &srcpalette, psrc,
            srcsize, &srcimage.xsize, &srcimage.ysize, &depthbits)) == true)
        {
            if (depthbits < 8)          // expand packed type
            {
                float srcpixelsperbyte = PIXELS_PER_BYTE(depthbits);
                int32_t srcpitch = (int32_t)(ceilf((float)(srcimage.xsize) / srcpixelsperbyte) + 1) & ~1;
                int32_t dstpitch = (int32_t)(ceilf((float)(srcimage.xsize) / srcpixelsperbyte));

                uint32_t runcount = 0;
                uint32_t bpp = 0;
                uint32_t bit = 0;
                uint32_t padbytes = srcpitch - dstpitch;

                if (depthbits == 1)
                {
                    bpp = 8;
                    bit = 7;
                }
                else if (depthbits == 2)
                {
                    bpp = 4;
                    bit = 6;
                }
                else if (depthbits == 4)
                {
                    bpp = 2;
                    bit = 4;
                }

                uint8_t* pixels = (uint8_t*)malloc((srcimage.xsize * srcimage.ysize));
                uint8_t* pixptr = pixels;
                uint8_t* pixbuf = pixels;
                memset(pixels, 0, srcimage.xsize * srcimage.ysize);

                uint8_t* srcbuf = srcimage.data;
                uint32_t x = 0;
                uint32_t y = 0;

                while (y < srcimage.ysize)
                {
                    pixbuf = pixptr;
                    runcount = bpp;
                    x = srcimage.xsize;

                    while (x > 0)
                    {
                        if (x < runcount) { runcount = x; }
                        ExpandNbitsToIndex8(pixbuf, 1, *srcbuf, runcount, bit);
                        pixbuf += runcount;
                        srcbuf++;
                        x -= runcount;
                    }

                    srcbuf += padbytes;
                    y++;

                    if (y != srcimage.ysize)
                    {
                        pixptr += srcimage.xsize;
                    }
                }

                free(srcimage.data);
                srcimage.data = pixels;
                depthbits = 8;
            }

            srcimage.pixeltype = (depthbits <= 8) ? PIXELTYPE_COLOUR_INDEX : PIXELTYPE_RGB;
            format = FILEFORMAT_PCX;
        }
        else if ((result = LoadFromMemoryTGA(&srcimage.data, &srcpalette, psrc,
            srcsize, &srcimage.xsize, &srcimage.ysize, &depthbits)) == true)
        {
            if (depthbits == 32) { srcimage.pixeltype = PIXELTYPE_BGRA; }
            else if (depthbits == 24) { srcimage.pixeltype = PIXELTYPE_BGR; }
            else if (depthbits == 16) { srcimage.pixeltype = PIXELTYPE_XBGR1555; }
            else if (depthbits ==  8 && srcpalette.size == 0) { srcimage.pixeltype = PIXELTYPE_LUMINANCE; }
            else if (depthbits ==  8 && srcpalette.size != 0) { srcimage.pixeltype = PIXELTYPE_COLOUR_INDEX; }
            format = FILEFORMAT_TGA;
        }
        else { fprintf(stderr, "LoadImage, Unsupported image format\n"); }

        if (result == true && psrcinfo != NULL)
        {
            psrcinfo->xsize = srcimage.xsize;
            psrcinfo->ysize = srcimage.ysize;
            psrcinfo->pixeltype = srcimage.pixeltype;
            psrcinfo->depth = depthbits;
            psrcinfo->fileformat = format;
        }

        // convert to format and filter
        if (result == true)
        {
            pdstimage->xsize = srcimage.xsize;
            pdstimage->ysize = srcimage.ysize;
            pdstimage->pixeltype = dstformat == PIXELTYPE_UNKNOWN ? PIXELTYPE_RGBA : dstformat;

            result = ResampleImage(pdstimage, pdstrect, &srcimage,
                &srcpalette, psrcrect, filter);

            if (result == true)
            {
                if (colorkey.b != 0 || colorkey.g != 0 || colorkey.r != 0 ||
                    colorkey.a != 0)
                {
                    const rgba_t transparent_black = { 0, 0, 0, 0 };
                    ReplaceColor(pdstimage, &srcpalette, transparent_black, colorkey);
                }

                if (pdstpalette != NULL)
                {
                    for (uint32_t i = 0; i < srcpalette.size; ++i)
                    {
                        pdstpalette->data[i] = srcpalette.data[i];
                    }

                    pdstpalette->size = srcpalette.size;
                    pdstpalette->bits = srcpalette.bits;
                }
            }
        }

        if (srcimage.data != NULL)
        {
            free(srcimage.data);
            srcimage.data = NULL;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
// LoadImageFromFile
//-----------------------------------------------------------------------------
bool
LoadImageFromFile(image_t* pdstimage, palette_t* pdstpalette, rect_t* pdstrect,
    const char* psrcfile, pixel_t format, rect_t* psrcrect, uint32_t filter,
    rgba_t colorkey, image_info_t* psrcinfo)
{
    bool result = false;

    if (psrcfile == NULL || strlen(psrcfile) == 0)
    {
        return result;
    }

    // open file
    FILE* hFile = fopen(psrcfile, "rb");
    long end = 0;
    long pos = 0;

    // file size
    if (hFile != NULL)
    {
        pos = ftell(hFile);
        fseek(hFile,  0L, SEEK_END);
        end = ftell(hFile);
        fseek(hFile, pos, SEEK_SET);
    }
    else
    {
        fprintf(stderr, "LoadImage, Couldn't open file %s.\n", psrcfile);
    }
    
    size_t fileSize = (end - pos);

    // allocate memory
    uint8_t* rawsrc = NULL;
    void* srcbuf = rawsrc;

    if (fileSize != 0)
    {
        srcbuf = malloc(((fileSize + 2) & ~1));
        memset(srcbuf, 0, ((fileSize + 2) & ~1));
    }

    rawsrc = (uint8_t*)srcbuf;

    // file read
    size_t bytesRead = 0;

    if (hFile != NULL && fileSize != 0)
    {
        if (srcbuf != NULL)
        {
            bytesRead = fread(srcbuf, sizeof(uint8_t), fileSize, hFile);
        }

        fclose(hFile);
        hFile = NULL;
    }

    if (fileSize == bytesRead && srcbuf != NULL)
    {
        // load image
        result = LoadImageFromMemory(pdstimage, pdstpalette, pdstrect, rawsrc,
            (uint32_t)bytesRead, format, psrcrect, filter, colorkey, psrcinfo);
    }

    if (srcbuf != NULL)
    {
        free(srcbuf);
        srcbuf = NULL;
    }

    rawsrc = NULL;

    return result;
}

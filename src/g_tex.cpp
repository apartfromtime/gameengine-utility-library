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

#include "..\inc\GEUL\g_tex.h"
#include "g_geul.h"


//-----------------------------------------------------------------------------
// Sampling
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Get32BPPSample
//-----------------------------------------------------------------------------
void
Get32BPPSample(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample0,
    int32_t* sample1, int32_t* sample2, int32_t* sample3)
{
    int32_t s0 = INT_MAX;
    int32_t s1 = INT_MAX;
    int32_t s2 = INT_MAX;
    int32_t s3 = INT_MAX;

    if (x < width)
    {
        s0 = *(ppix + ((x * 4) + 0));
        s1 = *(ppix + ((x * 4) + 1));
        s2 = *(ppix + ((x * 4) + 2));
        s3 = *(ppix + ((x * 4) + 3));
    }

    if (s0 != INT_MAX)
    {
        *sample0 = s0;
        *sample1 = s1;
        *sample2 = s2;
        *sample3 = s3;
    }
}

//-----------------------------------------------------------------------------
// Get24BPPSample
//-----------------------------------------------------------------------------
void
Get24BPPSample(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample0,
    int32_t* sample1, int32_t* sample2)
{
    int32_t s0 = INT_MAX;
    int32_t s1 = INT_MAX;
    int32_t s2 = INT_MAX;

    if (x < width)
    {
        s0 = *(ppix + ((x * 3) + 0));
        s1 = *(ppix + ((x * 3) + 1));
        s2 = *(ppix + ((x * 3) + 2));
    }

    if (s0 != INT_MAX)
    {
        *sample0 = s0;
        *sample1 = s1;
        *sample2 = s2;
    }
}

//-----------------------------------------------------------------------------
// Get24BPPSampleR
//-----------------------------------------------------------------------------
void
Get24BPPSampleR(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample)
{
    int32_t s = INT_MAX;

    if (x < width)
    {
        s = *(ppix + ((x * 3) + 0));
    }

    if (s != INT_MAX)
    {
        *sample = s;
    }
}

//-----------------------------------------------------------------------------
// Get24BPPSampleG
//-----------------------------------------------------------------------------
void
Get24BPPSampleG(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample)
{
    int32_t s = INT_MAX;

    if (x < width)
    {
        s = *(ppix + ((x * 3) + 1));
    }

    if (s != INT_MAX)
    {
        *sample = s;
    }
}

//-----------------------------------------------------------------------------
// Get24BPPSampleB
//-----------------------------------------------------------------------------
void
Get24BPPSampleB(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample)
{
    int32_t s = INT_MAX;

    if (x < width)
    {
        s = *(ppix + ((x * 3) + 2));
    }

    if (s != INT_MAX)
    {
        *sample = s;
    }
}

//-----------------------------------------------------------------------------
// Get16BPPSample
//-----------------------------------------------------------------------------
void
Get16BPPSample(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample)
{
    int32_t s = INT_MAX;

    if (x < width)
    {
        s = 0;
        s = *(ppix + ((x * 2) + 0));
        s = *(ppix + ((x * 2) + 1)) << 8;
    }

    if (s != INT_MAX)
    {
        *sample = s;
    }
}

//-----------------------------------------------------------------------------
// Get8BPPSample
//-----------------------------------------------------------------------------
void
Get8BPPSample(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample)
{
    int32_t s = INT_MAX;

    if (x < width)
    {
        s = 0; s = *(ppix + x);
    }

    if (s != INT_MAX)
    {
        *sample = s;
    }
}

//-----------------------------------------------------------------------------
// Get4BPPSample
//-----------------------------------------------------------------------------
void
Get4BPPSample(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample)
{
    int32_t s = INT_MAX;

    if ((x + 0) < width) {s = 0; s |= *(ppix + x + 0) << 4;}
    if ((x + 1) < width) {s |= *(ppix + x + 1);}

    if (s != INT_MAX)
    {
        *sample = s;
    }
}

//-----------------------------------------------------------------------------
// Get2BPPSample
//-----------------------------------------------------------------------------
void
Get2BPPSample(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample)
{
    int32_t s = INT_MAX;

    if ((x + 0) < width) {s = 0; s |= *(ppix + x + 0) << 6;}
    if ((x + 1) < width) {s |= *(ppix + x + 1) << 4;}
    if ((x + 2) < width) {s |= *(ppix + x + 2) << 2;}
    if ((x + 3) < width) {s |= *(ppix + x + 3);}

    if (s != INT_MAX)
    {
        *sample = s;
    }
}

//-----------------------------------------------------------------------------
// Get1BPPSample
//-----------------------------------------------------------------------------
void
Get1BPPSample(uint8_t* ppix, int32_t x, int32_t width, int32_t* sample)
{
    int32_t s = INT_MAX;

    if ((x + 0) < width) {s = 0; s |= *(ppix + x + 0) << 7;}
    if ((x + 1) < width) {s |= *(ppix + x + 1) << 6;}
    if ((x + 2) < width) {s |= *(ppix + x + 2) << 5;}
    if ((x + 3) < width) {s |= *(ppix + x + 3) << 4;}
    if ((x + 4) < width) {s |= *(ppix + x + 4) << 3;}
    if ((x + 5) < width) {s |= *(ppix + x + 5) << 2;}
    if ((x + 6) < width) {s |= *(ppix + x + 6) << 1;}
    if ((x + 7) < width) {s |= *(ppix + x + 7);}

    if (s != INT_MAX)
    {
        *sample = s;
    }
}

//-----------------------------------------------------------------------------
// blit N-bits per byte to 8-bits per index
//-----------------------------------------------------------------------------
void BlitNbitsToIndex8(uint8_t* pdst, uint32_t dstskip, uint8_t sample,
    uint8_t count, uint8_t bitstart)
{
    static unsigned char mask_table[8] = { 0, 0, 0, 0, 0x0F, 0, 0x03, 0x01 };

    uint8_t dstofs = 0;
    uint8_t srcofs = bitstart;
    uint8_t srcdec = 8-bitstart;
    uint8_t srcbit = mask_table[bitstart];

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

#define PIXELS_PER_BYTE(depthbits) ((8.0f / (float)(depthbits)))

typedef uint8_t   byte;
typedef uint16_t  word;
typedef uint32_t dword;


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

#pragma pack(push, 1)
typedef struct _png_chunk_s
{
    uint32_t size;          // size
    uint32_t type;          // type
} png_chunk_t;

typedef struct _png_header
{
    uint32_t width;         // width
    uint32_t height;            // height
    uint8_t depthbits;          // depth in bits
    uint8_t colourtype;            // colour type
    uint8_t compression;            // compression method
    uint8_t filter;         // filter method
    uint8_t interlace;          // interlace method
} png_header_t;
#pragma pack(pop)


//------------------------------------------------------------------------------
// CRC (from the specification)
//------------------------------------------------------------------------------
static unsigned long g_crc_table[256];          // Table of CRCs of all 8-bit messages

// Flag: has the table been computed? Initially false.
bool g_crc_table_computed = false;

// Make the table for a fast CRC.
void MakeCrcTable(void)
{
    unsigned long c = 0;

    memset(g_crc_table, 0, 256 * sizeof(unsigned long));

    for (int n = 0; n < 256; n++)
    {
        c = (unsigned long)n;

        for (int k = 0; k < 8; k++)
        {
            if (c & 1)
            {
                c = 0xEDB88320L ^ (c >> 1);
            }
            else
            {
                c = c >> 1;
            }
        }

        g_crc_table[n] = c;
    }

    g_crc_table_computed = true;
}

//------------------------------------------------------------------------------
// Update a running CRC with the bytes buf[0..len-1]--the CRC
// should be initialized to all 1's, and the transmitted value
// is the 1's complement of the final running CRC (see the
// crc() routine below)).
//------------------------------------------------------------------------------
unsigned long UpdateCrc(unsigned long crc, unsigned char* buf, int len)
{
    unsigned long c = crc;

    if (g_crc_table_computed == false)
    {
        MakeCrcTable();
    }

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
// Critical and acilliary chunk definitions
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
void
ShrinkPNG(uint8_t* pdst, uint32_t* pdstlen, uint8_t filtertype, uint32_t srcxskip,
    uint32_t srcyskip, uint32_t srcxsize, uint32_t srcysize, uint32_t srcdepthbits,
    uint8_t* psrc)
{
    // image layout - indexed, grayscale, truecolor
    // alpha channel - 8-bit per sample only

    // filtering
    uint32_t bytesencoded = 0;
    uint32_t xsize = 0;         // xsize
    uint32_t ysize = 0;         // ysize
    uint32_t x = 0;
    uint32_t y = 0;
    int32_t run = 0;            // run-length
    int32_t bit = 0;            // bit-offset start or shift-bit start
    int32_t inc = 0;            // pixel-to-pixel increment (interlace pattern increment)
    uint8_t bytesperpixel = 0;

    // pixels
    uint16_t sample = 0;
    uint8_t* raw0buf = 0;           // Raw(x) buffer
    uint8_t* raw1buf = 0;           // Raw(x-bpp) buffer
    uint8_t* pri0buf = 0;           // Pri(x) buffer
    uint8_t* pri1buf = 0;           // Pri(x-bpp) buffer
    uint8_t* dstbuf = pdst;
    uint8_t* srcbuf = psrc;

    // empty image
    if (srcxsize != 0 && srcysize != 0)
    {
        ysize = srcysize;

        if (srcdepthbits < 8)
        {
            if (srcdepthbits == 1)         // 1-bit indexed
            {
                run = 8;
                bit = 7;
            }
            else if (srcdepthbits == 2)            // 2-bit indexed
            {
                run = 4;
                bit = 6;
            }
            else if (srcdepthbits == 4)            // 4-bit indexed
            {
                run = 2;
                bit = 4;
            }

            bytesperpixel = 1;
            xsize = ((srcxsize * srcdepthbits) + 7) >> 3;          // width in bytes
            inc = srcxskip * run;
        }
        else            // 8bit
        {
            bytesperpixel = (srcdepthbits >> 3);
            xsize = srcxsize;
            inc = srcxskip * (srcdepthbits >> 3);
        }

        // filtering - None = 0; Sub = 1; Up = 2; Average = 3; Paeth = 4
        uint8_t filtermode = (filtertype >= PNG_FILTER_COUNT) ? 0 : filtertype;

        while (y++ < ysize)
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
            uint16_t s = 0;
            uint8_t enc = filtermode;

            if ((y-1)!=0) { pri1buf = raw1buf - srcyskip; }
            if ((y-1)!=0) { pri0buf = raw0buf - srcyskip; }

            if (filtermode == PNG_FILTER_ADAPTIVE)
            {
                uint32_t sum[2] = { UINT32_MAX };
                enc = 0;

                for (int f = 0; f < PNG_FILTER_COUNT-1; ++f)
                {
                    x = 0;
                    sum[1] = 0;

                    while (x++ < xsize)
                    {
                        bpp = 0;
                        raw0 = 0;
                        raw1 = 0;
                        pri0 = 0;
                        pri1 = 0;
                        s = 0;

                        while (bpp++ < bytesperpixel)
                        {
                            if (srcdepthbits < 8)
                            {
                                if ((y-1) != 0 && (x-1) != 0)
                                {
                                    BlitNbitsToIndex8((uint8_t*)&pri1, srcxskip, *pri1buf, run, bit);
                                }


                                if ((y-1) != 0)
                                {
                                    BlitNbitsToIndex8((uint8_t*)&pri0, srcxskip, *pri0buf, run, bit);
                                }

                                if ((x-1) != 0)
                                {
                                    BlitNbitsToIndex8((uint8_t*)&raw1, srcxskip, *raw1buf, run, bit);
                                }

                                BlitNbitsToIndex8((uint8_t*)&raw0, srcxskip, *raw0buf, run, bit);
                            }
                            else
                            {
                                if ((y-1) != 0 && (x-1) != 0)
                                {
                                    pri1 = pri1buf[s];
                                }

                                if ((y-1) != 0)
                                {
                                    pri0 = pri0buf[s];
                                }

                                if ((x-1) != 0)
                                {
                                    raw1 = raw1buf[s];
                                }

                                raw0 = raw0buf[s++];
                            }

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
                        }
                    }

                    if (sum[1] < sum[0])
                    {
                        sum[0] = sum[1];
                        enc = f;
                    }
                }

                x = 0;
            }

            *dstbuf++ = enc;           // filter-type byte
            bytesencoded++;

            while (x++ < xsize)
            {
                bpp = 0;
                raw0 = 0;
                raw1 = 0;
                pri0 = 0;
                pri1 = 0;
                s = 0;

                while (bpp++ < bytesperpixel)
                {
                    if (srcdepthbits < 8)
                    {
                        if ((y-1) != 0 && (x-1) != 0)
                        {
                            BlitNbitsToIndex8((uint8_t*)&pri1, srcxskip, *pri1buf, run, bit);
                        }

                        if ((y-1) != 0)
                        {
                            BlitNbitsToIndex8((uint8_t*)&pri0, srcxskip, *pri0buf, run, bit);
                        }


                        if ((x-1) != 0)
                        {
                            BlitNbitsToIndex8((uint8_t*)&raw1, srcxskip, *raw1buf, run, bit);
                        }

                        BlitNbitsToIndex8((uint8_t*)&raw0, srcxskip, *raw0buf, run, bit);
                    }
                    else
                    {
                        if ((y-1) != 0 && (x-1) != 0)
                        {
                            pri1 = pri1buf[s];
                        }

                        if ((y-1) != 0)
                        {
                            pri0 = pri0buf[s];
                        }

                        if ((x-1) != 0)
                        {
                            raw1 = raw1buf[s];
                        }

                        raw0 = raw0buf[s++];
                    }

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

                    *dstbuf++ = sample;
                    bytesencoded++;
                }
            
                if ((y-1) != 0 && (x-1) != 0) { pri1buf += inc; }
                if ((x-1) != 0) { raw1buf += inc; }
                if (x!=xsize) { pri0buf += inc; }
                if (x!=xsize) { raw0buf += inc; }
            }

            if (y != ysize)
            {
                srcbuf += srcyskip;
            }
        }
    }

    if (pdstlen != NULL) { *pdstlen = bytesencoded; }
}

//------------------------------------------------------------------------------
// ShrinkInterlacedPNG
//------------------------------------------------------------------------------
void
ShrinkInterlacedPNG(uint8_t* dstptr, uint32_t* pdstlen, uint8_t filtermode,
    uint32_t srcxsize, uint32_t srcysize, uint32_t srcdepthbits, uint8_t* srcptr)
{
    uint32_t srcbytesperpixel = (srcdepthbits < 8) ? 1 : (srcdepthbits >> 3);
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

        srcbuf = srcptr + (i_yorigin[pass] * srcpitch) + (i_xorigin[pass] * srcbytesperpixel);
        dstbuf = dstptr + dstlen;

        ShrinkPNG(dstptr + dstlen, &bytesencoded, filtermode, i_xextent[pass],
            i_yextent[pass] * srcpitch, x, y, srcdepthbits, srcbuf);

        dstlen += bytesencoded;
    }

    if (pdstlen != NULL) { *pdstlen = dstlen; }
}

//------------------------------------------------------------------------------
// SavePNG
//------------------------------------------------------------------------------
bool
SaveToMemoryPNG(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec,
    uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize, uint8_t srcdepthbits,
    palette_t* psrcpalette, rgba_t* pcolorkey)
{
    if (ppdst == NULL || ppdstsize == 0)
    {
        fprintf(stderr, "PNG, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL)
    {
        fprintf(stderr, "PNG, Invalid src data.\n");
        return false;
    }

    if (srcdepthbits !=  1 && srcdepthbits !=  2 && srcdepthbits !=  4 &&
        srcdepthbits !=  8 && srcdepthbits != 24 && srcdepthbits != 32)
    {
        fprintf(stderr, "PNG, Unsupported depth: %d.\n", srcdepthbits);
        return false;
    }

    uint8_t dstdepth = 0;

    switch (srcdepthbits)
    {
    case 32:
    case 24:
    case 16:
    {
        dstdepth = 8;
    } break;
    case  8:
    case  4:
    case  2:
    case  1:
    {
        dstdepth = srcdepthbits;

        if (psrcpalette != NULL)
        {
            if (psrcpalette->size <= 256 && psrcpalette->size > 16)
            {
                dstdepth = 8;
            }
            else if (psrcpalette->size <= 16 && psrcpalette->size > 4)
            {
                dstdepth = 4;
            }
            else if (psrcpalette->size <= 4 && psrcpalette->size > 2)
            {
                dstdepth = 2;
            }
            else if (psrcpalette->size <= 2)
            {
                dstdepth = 1;
            }
        }
    } break;
    }

    int32_t xsize = srcxsize;
    int32_t ysize = srcysize;

    // color-type interpretation of the image and number of
    // bytes-per-pixel
    uint8_t colortype = 0;

    switch (srcdepthbits)
    {
        case 32:            // rgba
        {
            colortype = 6;
        } break;
        case 24:         // rgb
        {
            colortype = 2;
        } break;
        case 16:         // greyscale + alpha
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
    int srcbytesperpixel = ((srcdepthbits == 32) ? 4 :
        (srcdepthbits == 24) ? 3 :
        (srcdepthbits == 16) ? 2 : 1);
    int pitch = srcxsize * srcbytesperpixel;
    uint8_t* rawptr = psrc;         // start of current row
    uint8_t* rawbuf = psrc;         // current row

    // dst stuff
    float dstpixelsperbyte = PIXELS_PER_BYTE(dstdepth * srcbytesperpixel);
    int32_t dstpitch = (int32_t)(ceilf((float)(xsize) / dstpixelsperbyte) + 1) & ~1;           // bytes per scanline

    uint32_t bytesperpixel = (uint32_t)(1.0f / dstpixelsperbyte);         // bytes per pixel

    uint32_t chunksize = sizeof(png_chunk_t);           // chunk size
    uint32_t headersize = sizeof(png_header_t);         // header size
    uint32_t palettesize = 0;          // colormap_length
    uint32_t signaturesize = sizeof(PNG_IDENTIFIER) / sizeof(PNG_IDENTIFIER[0]);
    uint32_t crcsize = sizeof(uint32_t);
    uint32_t crc = 0;

    // palette
    if (srcdepthbits <= 8)
    {
        if (psrcpalette != 0)
        {
            // the PLTE chunk contains from 1 to 256 palette entries, each
            // three-bytes
            palettesize = psrcpalette->size * 3;
        }
    }

    // 8-byte signature + IHDR + (PLTE) + IDAT + IEND

    int byteswritten = 0;
    // big array for true-color images
    int datasize = signaturesize + (chunksize + headersize + crcsize) +
        (chunksize + palettesize + crcsize) +
        (chunksize + ((ysize * dstpitch) * 2) + crcsize) +
        (chunksize + crcsize);          // png data size
    uint8_t* data = (uint8_t*)malloc(datasize);
    memset(data, 0, datasize * sizeof(uint8_t));

    uint32_t dstlen = 0;
    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;
    uint8_t* dstend = data + dstlen;

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
    size = headersize;
    type = BigU32(IHDR);

    WriteU32ToBE(dstbuf, size); dstbuf += 4;         // chunk size
    uint8_t* crcbuf = dstbuf;            // set pointer for crc calculation
    WriteU32ToBE(dstbuf, type); dstbuf += 4;         // chunk type
    byteswritten += 8;

    WriteU32ToBE(dstbuf, xsize); dstbuf += 4;           // width
    WriteU32ToBE(dstbuf, ysize); dstbuf += 4;           // height

    // check for invalid color-type and depth
    if (colortype == 3 && dstdepth >= 16)
    {
        fprintf(stderr, "PNG, bad color-type (%d) and bit-depth (%d).\n",
            colortype, dstdepth);

        if (data != 0)
        {
            free(data);
            data = 0;
        }

        return false;
    }
    
    uint8_t compression = 0;
    uint8_t filter = 0;
    // 0 (no interlace) or 1 (Adam7 interlace)
    uint8_t interlace = (codec == ENCODE_INTERLACE) ? 1 : 0;

    interlace = (interlace < 2) ? interlace : 0;

    *dstbuf++ = dstdepth;          // depth bits
    *dstbuf++ = colortype;         // colortype
    *dstbuf++ = compression;           // deflate\inflate compression
    *dstbuf++ = filter;            // filter method 0 (adaptive filtering with five basic filter types)
    *dstbuf++ = interlace;         // 0 (no interlace) or 1 (Adam7 interlace)
    byteswritten += 13;

    // CRC
    crc = Crc(crcbuf, (dstbuf - crcbuf));
    WriteU32ToBE(dstbuf, crc); dstbuf += 4;
    byteswritten += 4;

    // palette
    if (palettesize)
    {
        // PLTE chunk
        size = palettesize;
        type = BigU32(PLTE);

        WriteU32ToBE(dstbuf, size); dstbuf += 4;         // chunk size
        crcbuf = dstbuf;            // set pointer for crc calculation
        WriteU32ToBE(dstbuf, type); dstbuf += 4;         // chunk type
        byteswritten += 8;

        for (unsigned int i = 0; i < psrcpalette->size; ++i)
        {
            *dstbuf++ = psrcpalette->data[i].r;
            *dstbuf++ = psrcpalette->data[i].g;
            *dstbuf++ = psrcpalette->data[i].b;
            byteswritten += 3;
        }

        // CRC
        crc = Crc(crcbuf, (dstbuf - crcbuf));
        WriteU32ToBE(dstbuf, crc); dstbuf += 4;
        byteswritten += 4;
    }

    // transparency
    if (pcolorkey != NULL)
    {
        // tRNS chunk
        type = BigU32(tRNS);

        switch(colortype)
        {
        case 0:
        {
            // single gray level value, stored in the format:
            // Gray:  2 bytes, range 0 .. (2^bitdepth)-1
            size = 2;
            WriteU32ToBE(dstbuf, size); dstbuf += 4;
            crcbuf = dstbuf;            // set pointer for crc calculation
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
            crcbuf = dstbuf;            // set pointer for crc calculation
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
            crcbuf = dstbuf;            // set pointer for crc calculation
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

            if (data != 0)
            {
                free(data);
                data = 0;
            }

            return false;
        }
        }

        // CRC
        crc = Crc(crcbuf, (dstbuf - crcbuf));
        WriteU32ToBE(dstbuf, crc); dstbuf += 4;
        byteswritten += 4;
    }

    // uncompressed data, bytes per line, per component, plus filter per row
    uint32_t idatlen = (ysize * (dstpitch + 1)) + ((interlace > 0) ? ysize : 0);
    uint8_t* idatptr = (uint8_t*)malloc(((idatlen + 1) & ~1));
    uint8_t* idatbuf = idatptr;
    memset(idatptr, 0, ((idatlen + 1) & ~1) * sizeof(uint8_t));

    // filter selection (adaptive filtering with five basic filter types)
    int filtermode = PNG_FILTER_ADAPTIVE;

    if (filtermode == PNG_FILTER_ADAPTIVE)
    {
        if (colortype == 3 || dstdepth < 8)
        {
            filtermode = PNG_FILTER_NONE;
        }
    }

    uint32_t depthbits = dstdepth < 8 ? dstdepth : dstdepth * bytesperpixel;

    // IDAT chunk
    if (codec == ENCODE_INTERLACE)           // interlacing
    {
        ShrinkInterlacedPNG(idatbuf, &idatlen, filtermode, xsize, ysize,
            depthbits, rawbuf);
    }
    else            // no-interlacing
    {
        ShrinkPNG(idatbuf, &idatlen, filtermode, 1, pitch, xsize, ysize,
            depthbits, rawbuf);
    }

    idatbuf = idatptr;

    uint32_t bytesencoded = 0;       // number of data bytes encoded
    uint32_t bytesdecoded = 0;       // number of data bytes decoded

    // ZLIB compression
    z_stream deflator = {};

    // deflate\inflate compression levels 0-10
    uint8_t compressionlevel = 6;

    int status = deflateInit(&deflator, MIN(compressionlevel, 9));

    if (status != Z_OK)
    {
        fprintf(stderr, "PNG, Deflate: failed with status %i.\n",
            status);

        if (idatptr != NULL)
        {
            free(idatptr);
            idatptr = NULL;
        }
    }

    // deflate data
    uint32_t odatlen = idatlen;           // deflate size
    uint8_t* odatptr = (uint8_t*)malloc(((odatlen + 1) & ~1));
    uint8_t* odatbuf = odatptr;
    memset(odatptr, 0, ((odatlen + 1) & ~1) * sizeof(uint8_t));

    size_t oabsrem = 0;         // absolute remaining output
    size_t odatrem = 0;         // relative remaining output
    size_t iabsrem = 0;         // absolute remaining input
    size_t idatrem = 0;         // relative remaining input

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

        if (status <= Z_STREAM_END)
        {
            if (status == Z_STREAM_END)
            {
                fprintf(stdout, "PNG, Deflate: completed successfully\n");
            }
            else
            {
                fprintf(stderr, "PNG, Deflate: failed with status %i.\n",
                    status);
            }

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
    if (idatptr != NULL)
    {
        free(idatptr);
        idatptr = NULL;
    }

    // compression failed
    if (status != Z_STREAM_END)
    {
        if (odatptr != NULL)
        {
            free(odatptr);
            odatptr = NULL;
        }
    }
    else
    {
        // data
        oabsrem = bytesencoded;          // remaining bytes
        odatrem = MIN(32768, oabsrem);            // number of bytes

        while (oabsrem > 0)
        {
            oabsrem = oabsrem - odatrem;

            // IDAT chunk
            size = odatrem;
            type = BigU32(IDAT);

            WriteU32ToBE(dstbuf, size); dstbuf += 4;         // chunk size
            crcbuf = dstbuf;            // set pointer for crc calculation
            WriteU32ToBE(dstbuf, type); dstbuf += 4;         // chunk type
            byteswritten += 8;

            // IDAT data
            for (size_t i = 0; i < odatrem; ++i)
            {
                *dstbuf++ = *odatptr++;
                byteswritten++;
            }

            // CRC
            crc = Crc(crcbuf, (dstbuf - crcbuf));
            WriteU32ToBE(dstbuf, crc); dstbuf += 4;
            byteswritten += 4;
        }
    }

    // IEND chunk
    size = 0;
    type = BigU32(IEND);

    WriteU32ToBE(dstbuf, size); dstbuf += 4;         // chunk size
    crcbuf = dstbuf;            // set pointer for crc calculation
    WriteU32ToBE(dstbuf, type); dstbuf += 4;         // chunk type
    byteswritten += 8;

    // CRC
    crc = Crc(crcbuf, (dstbuf - crcbuf));
    WriteU32ToBE(dstbuf, crc); dstbuf += 4;
    byteswritten += 4;

    // actual png data size
    *ppdstsize = byteswritten;
    // reallocate array to a actual size
    *ppdst = (uint8_t*)realloc(data, ((*ppdstsize + 2) & ~1));

    return true;
}

//-----------------------------------------------------------------------------
// GetInfoFromMemory
//-----------------------------------------------------------------------------
bool
GetInfoFromMemoryPNG(uint8_t* srccolormap, uint32_t* srcxsize,
    uint32_t* srcysize, uint8_t* srcdepthbits, uint8_t* psrc, uint32_t psrcsize)
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

    uint32_t xsize = 0;          // image width
    uint32_t ysize = 0;          // image height
    uint32_t depth = 0;          // bits per pixel component
    uint32_t colortype = 0;         // colortype format
    uint32_t compression = 0;           // compression method
    uint32_t filter = 0;         // preprocess filter (0 = adaptive)
    uint32_t interlace = 0;      // interlace method (0 = no-interlace, 1 = Adam7)
    uint32_t gamma = 0;         // gamma correction
    uint32_t palnum = 0;           // number of palette colors
    uint8_t colorkey[3] = {};         // transparency color

    // A valid PNG image must contain an IHDR chunk, one or more IDAT chunks, and an
    // IEND chunk.

    bool endfound = false;
    bool first = true;          // image header chunk must precede all other chunks
    bool dattest = false;           // zero-length chunks are valid
    bool paltest = false;           // gamma precedes palette

    while (srcbuf < srcend && endfound == false)
    {
        // chunk length and type
        uint32_t l = ReadU32FromBE(srcbuf); srcbuf += 4;
        uint32_t t = ReadU32FromBE(srcbuf); srcbuf += 4;

        switch (BigU32(t))
        {
            case IHDR:
            {
                // check for multiple IHDR
                if (first == false)
                {
                    fprintf(stderr, "PNG, multiple IHDR.\n");
                    return false;
                }

                fprintf(stdout, "IHDR\n");

                first = false;

                // check chunk is valid length
                if (l != sizeof(png_header_t))
                {
                    fprintf(stderr, "PNG, bad IHDR len.\n");
                    return false;
                }

                xsize = ReadU32FromBE(srcbuf); srcbuf += 4;
                ysize = ReadU32FromBE(srcbuf); srcbuf += 4;

                // zero is an invalid value
                if (xsize == 0 || ysize == 0)
                {
                    fprintf(stderr, "PNG, 0-pixel image: x: %d, y: %d\n",
                        xsize, ysize);
                    return false;
                }

                depth = *srcbuf++;

                if (depth !=  1 && depth != 2 && depth != 4 && depth != 8)
                {
                    fprintf(stderr, "PNG, unsupported bits: %d.\n", depth);
                    return false;
                }
                
                colortype = *srcbuf++;

                // check for invalid color-type and depth
                if (colortype == 3 && depth >= 16)
                {
                    fprintf(stderr, "PNG, bad color-type (%d) and bit-depth (%d).\n",
                        colortype, depth);
                    return false;
                }

                // color-type interpretation of the image and number of
                // bytes-per-pixel
                switch (colortype)
                {
                    case 0:         // grayscale
                    case 2:         // rgb
                    case 3:         // palette
                    case 4:         // grayscale + alpha
                    case 6:         // rgba
                    {} break;
                    default:
                    {
                        fprintf(stderr, "PNG, unsupported color type: %d.\n",
                            colortype);
                        return false;
                    }
                }

                compression = *srcbuf++;

                // compression method
                if (compression != 0)
                {
                    fprintf(stderr, "PNG, unrecognised compression code: %d.\n",
                        compression);
                    return false;
                }

                filter = *srcbuf++;

                // pre-processing method applied before compression
                if (filter != 0)
                {
                    fprintf(stderr, "PNG, bad filter method: %d\n", filter);
                    return false;
                }

                interlace = *srcbuf++;
                
                // transmission order of image
                if (interlace > 1)
                {
                    fprintf(stderr, "PNG, bad interlace method: %d\n",
                        interlace);
                    return false;
                }

            } break;
            case gAMA:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "gAMA\n");

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

                srcbuf += l;
            } break;
            case PLTE:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                // the PLTE chunk must precede the first IDAT chunk.
                if (dattest)
                {
                    fprintf(stderr, "PNG, PLTE after IDAT.\n");
                    return false;
                }

                fprintf(stdout, "PLTE\n");

                // the PLTE chunk contains from 1 to 256 palette entries, each
                // three-bytes
                // the number of entries is determined from the chunk length.
                if (l > (256 * 3))
                {
                    fprintf(stderr, "PNG, Invalid PLTE length: %d\n", l);
                    return false;
                }

                // a chunk length not divisible by 3 is an error.
                palnum = l / 3;

                if ((palnum * 3) != l)
                {    
                    fprintf(stderr, "PNG, Invalid PLTE entries: %d\n",
                        palnum);
                    return false;
                }
                
                if (paltest == false)
                {
                    paltest = true;
                }

                srcbuf += l;
            } break;
            case tRNS:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "tRNS\n");

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
                    if (colortype == 4 || colortype == 6)           // grayscale + alpha or rgba
                    {
                        fprintf(stderr, "PNG, tRNS with alpha.\n");
                        return false;
                    }
                }
                
                srcbuf += l;
            } break;
            case IDAT:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "IDAT\n");

                if (dattest == false)
                {
                    dattest = true;
                }

                srcbuf += l;
            } break;
            case cHRM:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "cHRM\n");

                srcbuf += l;
            } break;
            case sRGB:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "sRGB\n");

                srcbuf += l;
            } break;
            case iCCP:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "iCCP\n");

                srcbuf += l;
            } break;
            case iTXt:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "iTXt\n");

                srcbuf += l;
            } break;
            case tEXt:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "tEXt\n");

                srcbuf += l;
            } break;
            case zTXt:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "zTXt\n");

                srcbuf += l;
            } break;
            case bKGD:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "bKGD\n");

                srcbuf += l;
            } break;
            case pHYs:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "pHYs\n");

                srcbuf += l;
            } break;
            case sBIT:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "sBIT\n");

                srcbuf += l;
            } break;
            case sPLT:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "sPLT\n");

                srcbuf += l;
            } break;
            case hIST:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "hIST\n");

                srcbuf += l;
            } break;
            case tIME:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                fprintf(stdout, "tIME\n");

                srcbuf += l;
            } break;
            case IEND:
            {
                fprintf(stdout, "IEND\n");

                // end of PNG chunks
                endfound = true;
            } break;
            default:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");

                    return false;
                }

                char fourcc[4] = {};
                fourcc[0] = (uint8_t)((t >> 24) & 0xFF);
                fourcc[1] = (uint8_t)((t >> 16) & 0xFF);
                fourcc[2] = (uint8_t)((t >>  8) & 0xFF);
                fourcc[3] = (uint8_t)((t >>  0) & 0xFF);
                
                fprintf(stderr, "PNG, PNG chunk not supported: %s\n", fourcc);

                srcbuf += l;
            } break;
        }

        // skip CRC
        srcbuf += 4;
    }

    uint8_t depthbits = 0;
    uint8_t bytesperpixel = 0;

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

    depthbits = depth * bytesperpixel;

    if (srccolormap != NULL) { *srccolormap = paltest; }
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepthbits != NULL) { *srcdepthbits = depthbits; }

    return true;
}

//------------------------------------------------------------------------------
// ExpandPNG
//------------------------------------------------------------------------------
void
ExpandPNG(uint8_t* pdst, uint32_t ppdstsize, uint32_t dstxskip,
    uint32_t dstyskip, uint32_t dstxsize, uint32_t dstysize, uint32_t dstpitch,
    uint32_t dstdepth, uint32_t dstbytesperpixel, uint8_t** ppsrc)
{
    // image layout - indexed, grayscale, truecolor
    // alpha channel - equal to 8bits per sample only

    uint32_t depth = dstdepth * dstbytesperpixel;

    // filtering
    uint32_t xsize = 0;            // xsize
    uint32_t ysize = 0;            // ysize
    uint32_t x = 0;
    uint32_t y = 0;
    int32_t run = 0;            // run-length
    int32_t bit = 0;            // bit-offset start or shift-bit start
    int32_t inc = 0;            // pixel-to-pixel increment (interlace pattern increment)
    uint8_t filtermode = 0;         // filtering - None = 0; Sub = 1; Up = 2; Average = 3; Paeth = 4
    uint8_t bytesperpixel = 0;

    // pixels
    uint16_t sample = 0;
    uint8_t* raw0buf = 0;            // Raw(x) buffer
    uint8_t* raw1buf = 0;            // Raw(x-bpp) buffer
    uint8_t* pri0buf = 0;            // Pri(x) buffer
    uint8_t* pri1buf = 0;            // Pri(x-bpp) buffer
    uint8_t* dstbuf = pdst;
    uint8_t* srcbuf = *ppsrc;

    // empty image
    if (dstxsize != 0 && dstysize != 0)
    {
        ysize = dstysize;

        if (depth < 8)
        {
            if (depth == 1)         // 1-bit indexed
            {
                run = 8;
                bit = 7;
            }
            else if (depth == 2)            // 2-bit indexed
            {
                run = 4;
                bit = 6;
            }
            else if (depth == 4)            // 4-bit indexed
            {
                run = 2;
                bit = 4;
            }

            bytesperpixel = 1;
            xsize = ((dstxsize * depth) + 7) >> 3;         // width in bytes
            inc = dstxskip * run;
        }
        else            // 8bit
        {
            bytesperpixel = (depth >> 3);
            xsize = dstxsize;
            inc = dstxskip * (depth >> 3);
        }

        while (y < ysize)
        {
            filtermode = *(*ppsrc)++;           // filter enc
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
            uint16_t s = 0;

            if (y!=0) { pri1buf = raw1buf - dstyskip; }
            if (y!=0) { pri0buf = raw0buf - dstyskip; }

            while (x < xsize)
            {
                bpp = 0;
                raw0 = 0;
                pri1 = 0;
                raw1 = 0;
                pri0 = 0;
                s = 0;

                while (bpp < bytesperpixel)
                {
                    if (y != 0 && x != 0)
                    {
                        pri1 = pri1buf[s];
                    }

                    if (y != 0)
                    {
                        pri0 = pri0buf[s];
                    }

                    if (x != 0)
                    {
                        raw1 = raw1buf[s];
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
                        BlitNbitsToIndex8(raw0buf, dstxskip, sample, run, bit);
                    }
                    else
                    {
                        raw0buf[s++] = sample;
                    }

                    bpp++;
                }

                if (y!=0 && x!=0) { pri1buf += inc; }
                if (x!=0) { raw1buf += inc; }
                pri0buf += inc;
                raw0buf += inc;

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
void
ExpandInterlacedPNG(uint8_t* dstptr, uint32_t dstlen, uint32_t dstxsize,
    uint32_t dstysize, uint32_t dstpitch, uint32_t dstdepth,
    uint32_t dstbytesperpixel, uint8_t** ppsrc)
{
    // image layout - indexed, grayscale, truecolor
    // alpha channel - equal to 8bits per sample only

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
            h, dstpitch, dstdepth, dstbytesperpixel, ppsrc);
    }
}

//-----------------------------------------------------------------------------
// LoadPNG
//-----------------------------------------------------------------------------
bool
LoadFromMemoryPNG(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize,
    uint8_t* srcdepthbits, rgba_t* pcolorkey)
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

    // pixels
    uint32_t bytesperpixel = 0;         // bytes per pixel

    // image
    pixel_t pixeltype = PIXELTYPE_UNKNOWN;          // pixeltype format
    uint32_t xsize = 0;         // image width
    uint32_t ysize = 0;         // image height
    uint32_t depth = 0;         // bits per pixel component
    uint32_t colortype = 0;         // colortype format
    uint32_t compression = 0;           // compression method
    uint32_t pngfilter = 0;         // preprocess filter (0 = adaptive)
    uint32_t interlace = 0;         // interlace method (0 = no-interlace, 1 = Adam7)
    uint32_t gamma = 0;         // image gamma
    uint32_t palnum = 0;           // number of palette colors
    uint8_t colorkey[3] = {};           // transparency color

    // idat chunks
    uint32_t idatlen = 0;           // total size of idat chunks

    // A valid PNG image must contain an IHDR chunk, one or more IDAT chunks, and an
    // IEND chunk.
    bool endfound = false;
    bool first = true;
    bool dattest = false;           // zero-length chunks are valid
    bool paltest = false;           // gamma precedes palette

    while (srcbuf < srcend && endfound == false)
    {
        // chunk length and type
        uint32_t l = ReadU32FromBE(srcbuf); srcbuf += 4;
        uint32_t t = ReadU32FromBE(srcbuf); srcbuf += 4;

        switch (BigU32(t))
        {
            case IHDR:
            {
                // check for multiple IHDR
                if (first == false)
                {
                    fprintf(stderr, "PNG, multiple IHDR.\n");
                    return false;
                }

                first = false;

                // check chunk is valid length
                if (l != sizeof(png_header_t))
                {
                    fprintf(stderr, "PNG, bad IHDR len.\n");
                    return false;
                }

                xsize = ReadU32FromBE(srcbuf); srcbuf += 4;
                ysize = ReadU32FromBE(srcbuf); srcbuf += 4;

                // zero is an invalid value
                if (xsize == 0 || ysize == 0)
                {
                    fprintf(stderr, "PNG, 0-pixel image: x: %d, y: %d\n",
                        xsize, ysize);
                    return false;
                }

                depth = *srcbuf++;

                if (depth !=  1 && depth != 2 && depth != 4 && depth != 8)
                {
                    fprintf(stderr, "PNG, unsupported bits: %d.\n", depth);
                    return false;
                }
                
                colortype = *srcbuf++;

                // check for invalid color-type and depth
                if (colortype == 3 && depth >= 16)
                {
                    fprintf(stderr, "PNG, bad color-type (%d) and bit-depth (%d).\n",
                        colortype, depth);
                    return false;
                }

                // color-type interpretation of the image and number of
                // bytes-per-pixel
                switch (colortype)
                {
                    case 0:         // grayscale
                    case 2:         // rgb
                    case 3:         // palette
                    case 4:         // grayscale + alpha
                    case 6:         // rgba
                    {} break;
                    default:
                    {
                        fprintf(stderr, "PNG, unsupported color type: %d.\n",
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
                    fprintf(stderr, "PNG, unrecognised compression code: %d.\n",
                        compression);
                    return false;
                }

                pngfilter = *srcbuf++;

                // pre-processing method applied before compression
                if (pngfilter != 0)
                {
                    fprintf(stderr, "PNG, bad filter method: %d\n", pngfilter);
                    return false;
                }

                interlace = *srcbuf++;
                
                // transmission order of image
                if (interlace > 1)
                {
                    fprintf(stderr, "PNG, bad interlace method: %d\n",
                        interlace);
                    return false;
                }

            } break;
            case gAMA:
            {
                if (first)
                {
                    fprintf(stderr, "PNG, first not IHDR.\n");
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
                    fprintf(stderr, "PNG, first not IHDR.\n");
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
                if (l > (256 * 3))
                {
                    fprintf(stderr, "PNG, Invalid PLTE length: %d\n", l);
                    return false;
                }

                // a chunk length not divisible by 3 is an error.
                palnum = l / 3;

                if ((palnum * 3) != l)
                {    
                    fprintf(stderr, "PNG, Invalid PLTE entries: %d\n",
                        palnum);
                    return false;
                }

                if (paltest == false)
                {
                    paltest = true;
                }

                // the palette chunk must appear for color type 3, and can appear for color types
                // 2 and 6; it must not appear for color types 0 and 4.
                if (pdstpalette != 0)
                {
                    memset(&pdstpalette->data, 0, 256 * sizeof(rgba_t));

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
                    fprintf(stderr, "PNG, first not IHDR.\n");
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

                    if (l > palnum)
                    {
                        fprintf(stderr, "PNG, bad tRNS len: %d\n", l);
                        return false;
                    }

                    if (pdstpalette != 0)
                    {
                        for (uint32_t i = 0; i < l; ++i)
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

                    if (pcolorkey != 0)
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
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                if (dattest == false)
                {
                    dattest = true;
                }

                idatlen += l; srcbuf += l;
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
                    fprintf(stderr, "PNG, first not IHDR.\n");
                    return false;
                }

                char fourcc[5] = {};
                fourcc[0] = (uint8_t)((t >> 24) & 0xFF);
                fourcc[1] = (uint8_t)((t >> 16) & 0xFF);
                fourcc[2] = (uint8_t)((t >>  8) & 0xFF);
                fourcc[3] = (uint8_t)((t >>  0) & 0xFF);
                fourcc[4] = '\0';
                
                fprintf(stderr, "PNG, chunk not supported: %s\n", fourcc);

                srcbuf += l;
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
    uint32_t idatofs = 0;           // offset of idat chunk
    uint8_t* idatptr = (uint8_t*)malloc(((idatlen + 1) & ~1) * sizeof(uint8_t));
    uint8_t* idatbuf = idatptr;
    memset(idatptr, 0, ((idatlen + 1) & ~1) * sizeof(uint8_t));

    srcbuf = srcptr;            // reset current to begining
    endfound = false;

    // skip signature
    srcbuf += 8;

    while (srcbuf < srcend && endfound == false)
    {
        // chunk length and type
        uint32_t l = ReadU32FromBE(srcbuf); srcbuf += 4;
        uint32_t t = ReadU32FromBE(srcbuf); srcbuf += 4;

        switch (BigU32(t))
        {
            case IDAT:
            {
                if ((uint32_t)(srcend - srcbuf) >= l)
                {
                    memcpy(idatbuf + idatofs, srcbuf, l);
                    srcbuf += l;
                }
                else
                {
                    fprintf(stderr, "PNG, out of data.\n");

                    if (idatptr != NULL)
                    {
                        free(idatptr);
                        idatptr = NULL;
                    }

                    return false;
                }

                idatofs += l;
            } break;
            case IEND:
            {
                // end of PNG chunks
                endfound = true;
            } break;
            default:
            {
                srcbuf += l;
            } break;
        }

        // skip CRC
        srcbuf += 4;
    }

    // filtered image output buffer
    uint32_t bytesencoded = 0;       // number of data bytes encoded
    uint32_t bytesdecoded = 0;       // number of data bytes decoded

    // ZLIB decompression
    z_stream inflator = {};
    int status = inflateInit(&inflator);

    if (status != Z_OK)
    {
        fprintf(stderr, "PNG, Inflate: failed with status %i.\n", status);

        if (idatptr != NULL)
        {
            free(idatptr);
            idatptr = NULL;
        }

        return false;
    }

    // inflate data, bytes per line, per component, plus filter per row
    uint32_t odatlen = (ysize * (dstpitch + 1)) + ((interlace > 0) ? ysize : 0);
    uint8_t* odatptr = (uint8_t*)malloc(((odatlen + 1) & ~1));
    uint8_t* odatbuf = odatptr;
    memset(odatptr, 0, ((odatlen + 1) & ~1) * sizeof(uint8_t));

    size_t oabsrem = odatlen - inflator.total_out;         // absolute remaining output
    size_t odatrem = 0;         // relative remaining output
    size_t iabsrem = idatlen - inflator.total_in;         // absolute remaining input
    size_t idatrem = 0;         // relative remaining input

    idatrem = MIN(32767, idatlen);
    odatrem = MIN(32767, odatlen);

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
        idatrem = MIN(32767, iabsrem);
        bytesdecoded = inflator.total_in;

        // output
        oabsrem = odatlen - inflator.total_out;
        odatbuf = odatptr + inflator.total_out;
        odatrem = MIN(32767, oabsrem);
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
    if (idatptr != NULL)
    {
        free(idatptr);
        idatptr = NULL;
    }

    // decompression failed
    if (status != Z_STREAM_END)
    {
        fprintf(stderr, "PNG, Inflate: failed with status %i.\n",
            status);

        // free un-compressed data and return
        if (odatptr != NULL)
        {
            free(odatptr);
            odatptr = NULL;
        }

        return false;
    }

    // pixels
    uint32_t pixlen = xsize * ysize * bytesperpixel;
    uint8_t* pixels = (uint8_t*)malloc(pixlen * sizeof(uint8_t));
    uint8_t* rawptr = pixels;         // current scanline
    uint8_t* rawbuf = pixels;         // scanline buffer
    uint32_t rawlen = 0;
    uint32_t pitch = xsize * bytesperpixel;          // bytes per image
    memset(pixels, 0, pixlen * sizeof(uint8_t));

    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepthbits != NULL)
    {
        // color-type interpretation of the image and number of
        // bytes-per-pixel
        switch (colortype)
        {
            case 0:         // grayscale
            {
                *srcdepthbits = depth;
            } break;
            case 2:         // rgb
            {
                *srcdepthbits = 24;
            } break;
            case 3:         // palette
            {
                *srcdepthbits = depth;
            } break;
            case 4:         // grayscale + alpha
            {
                *srcdepthbits = 16;
            } break;
            case 6:         // rgba
            {
                *srcdepthbits = 32;
            } break;
        }
    }

    uint32_t datlen = (ysize * (dstpitch + 1)) + ((interlace > 0) ? ysize : 0);
    uint8_t* datptr = (uint8_t*)malloc(((datlen + 1) & ~1));
    memcpy(datptr, odatptr, datlen);          // filtered image data
    uint8_t* datbuf = datptr;

    if (odatptr != NULL)
    {
        free(odatptr);
        odatptr = NULL;
    }

    // interlace and filter
    if (interlace == 1)
    {
        ExpandInterlacedPNG(rawptr, pixlen, xsize, ysize, pitch, depth,
            bytesperpixel, &datbuf);
    }
    else
    {
        ExpandPNG(rawbuf, pixlen, 1, pitch, xsize, ysize, pitch, depth,
            bytesperpixel, &datbuf);
    }

    if (datptr != NULL)
    {
        free(datptr);
        datptr = NULL;
    }

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

#pragma pack(push, 1)
typedef struct _tga_file
{
    byte    id_length;
    byte    colormap_type;
    byte    image_type;
    word    colormap_index;
    word    colormap_length;
    byte    colormap_size;
    word    x_origin;
    word    y_origin;
    word    width;
    word    height;
    byte    pixel_size;
    byte    image_descriptor;
} tga_file_t;
#pragma pack(pop)

//------------------------------------------------------------------------------
// SaveTGA
//------------------------------------------------------------------------------
bool
SaveToMemoryTGA(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec,
    uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize, uint8_t srcdepthbits,
    palette_t* psrcpalette, bool invertX, bool invertY)
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

    if (srcdepthbits != 8 && srcdepthbits != 16 && srcdepthbits != 24 && srcdepthbits != 32)
    {
        fprintf(stderr, "TGA, Unsupported depth: %d.\n", srcdepthbits);
        return false;
    }

    uint8_t dstdepth = 0;

    switch (srcdepthbits)
    {
        case 32:
        {
            dstdepth = 32;
        } break;
        case 24:
        {
            dstdepth = 24;
        } break;
        case 16:
        {
            dstdepth = 16;
        } break;
        case 8:
        {
            dstdepth = 8;
        } break;
    }

    int32_t xorigin = 0;
    int32_t yorigin = 0;
    int32_t xextent = srcxsize;
    int32_t yextent = srcysize;

    // dst stuff
    float dstpixelsperbyte = PIXELS_PER_BYTE(dstdepth);
    int32_t dstpitch = (int32_t)(ceilf((float)(xextent) / dstpixelsperbyte) + 1) & ~1;           // bytes per scanline

    // src stuff
    int32_t srcbytesperpixel = ((srcdepthbits == 32) ? 4 :
        (srcdepthbits == 24) ? 3 :
        (srcdepthbits == 16) ? 2 : 1);
    int32_t srcpitch = srcxsize * srcbytesperpixel;
    uint8_t* rawptr = psrc;         // start of current row
    uint8_t* rawbuf = psrc;         // current row

    uint8_t id_length = 0;
    uint8_t colormap_type = (psrcpalette != NULL) ? 1 : 0;
    uint8_t image_type = TGA_NO_IMAGE_DATA;
    uint32_t colormap_index = 0;
    int32_t colormap_length = 0;            // colormap length (number of palette entries)
    int32_t colormap_size = 0;
    uint8_t pixel_size = dstdepth;
    uint8_t image_descriptor = 0;
    
    // palette
    if (srcdepthbits == 8)
    {
        if (psrcpalette != NULL)
        {
            colormap_length = psrcpalette->size;
            colormap_size = psrcpalette->bits;
        }
    }

    if (psrc == 0)
    {
        image_type = TGA_NO_IMAGE_DATA;
    }
    else
    {
        if (codec == ENCODE_RLE)
        {
            switch(srcdepthbits)
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
            switch (srcdepthbits)
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

    int32_t bytesencoded = 0;
    // big array for true-color images
    int32_t datasize = sizeof(tga_file_t) + ((yextent * dstpitch) * 2 *
        sizeof(unsigned char)) + colormap_length;           // tga data size
    uint8_t* data = (uint8_t*)malloc(datasize);
    memset(data, 0, datasize * sizeof(uint8_t));

    uint32_t dstlen = datasize;
    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;
    uint8_t* dstend = data + dstlen;
    
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

    *dstbuf++ = tgafile.id_length;          // id length
    *dstbuf++ = tgafile.colormap_type;          // colormap type
    *dstbuf++ = tgafile.image_type;         // image type
    WriteU16ToLE(dstbuf, tgafile.colormap_index);   dstbuf += 2;            // colormap index
    WriteU16ToLE(dstbuf, tgafile.colormap_length);  dstbuf += 2;            // colormap length
    *dstbuf++ = tgafile.colormap_size;          // colormap pixel size
    WriteU16ToLE(dstbuf, tgafile.x_origin);         dstbuf += 2;            // x-origin
    WriteU16ToLE(dstbuf, tgafile.y_origin);         dstbuf += 2;            // y-origin
    WriteU16ToLE(dstbuf, tgafile.width);            dstbuf += 2;            // x-size
    WriteU16ToLE(dstbuf, tgafile.height);           dstbuf += 2;            // y-size
    *dstbuf++ = tgafile.pixel_size;         // pixel size
    *dstbuf++ = tgafile.image_descriptor;           // image descriptor

    dstbuf += id_length;            // skip the image descriptor 
    
    if (colormap_size != 0 && psrcpalette != 0)
    {
        // fill in the palette
        if (colormap_size == 15 || colormap_size == 16)
        {            
            for (unsigned short i = colormap_index; i < colormap_length; ++i)
            {
                uint16_t r = psrcpalette->data[i].r;
                uint16_t g = psrcpalette->data[i].g;
                uint16_t b = psrcpalette->data[i].b;

                r = r * 0x1F / 0xFF;
                g = g * 0x1F / 0xFF;
                b = b * 0x1F / 0xFF;
                
                uint16_t rgb1555 = ((r & 0xFF) << 10) | ((g & 0xFF) << 5) | (b & 0xFF);

                WriteU16ToLE(dstbuf, rgb1555); dstbuf += 2;
            }
        }
        else if (colormap_size == 24)
        {            
            for (unsigned short i = colormap_index; i < colormap_length; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].b;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].r;
            }
        }
        else if (colormap_size == 32)
        {            
            for (unsigned short i = colormap_index; i < colormap_length; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].b;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].r;
                *dstbuf++ = psrcpalette->data[i].a;
            }
        }
    }

    int32_t x = 0;
    int32_t y = 0;
    int32_t maxrle = 0x80;          // max run-length count is 0x80
    int32_t maxabs = 0xFF;          // max abs-length count is 0xFF

    // encoding
    if (psrc != 0)
    {
        if (srcdepthbits == 32)
        {
            switch (image_type)
            {
            case TGA_RGB_RLE:
            {
                int32_t abscount = 0;
                int32_t rlecount = 0;
                int32_t rlevalue = 0;           // run-length value
                int32_t sample0 = 0;
                int32_t sample1 = 0;
                int32_t sample2 = 0;
                int32_t sample3 = 0;
                int32_t sample4 = 0;
                int32_t sample5 = 0;
                int32_t sample6 = 0;
                int32_t sample7 = 0;
                int32_t s0 = 0;
                int32_t s1 = 0;
                int32_t s2 = 0;
                int32_t s3 = 0;
                int32_t absrem = 0;
                int32_t rlerem = 0;

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
                        sample4 = INT_MAX;
                        sample5 = INT_MAX;
                        sample6 = INT_MAX;
                        sample7 = INT_MAX;
                        s0 = INT_MAX;
                        s1 = INT_MAX;
                        s2 = INT_MAX;
                        s3 = INT_MAX;

                        Get32BPPSample(rawbuf, x + 0, xextent, &s0, &s1, &s2, &s3);

                        abscount = 1;
                        absrem = xextent - (x + abscount);
                        sample4 = s0;
                        sample5 = s1;
                        sample6 = s2;
                        sample7 = s3;

                        while (absrem > 0 && abscount < maxabs)
                        {
                            sample0 = sample4;
                            sample1 = sample5;
                            sample2 = sample6;
                            sample3 = sample7;
                            sample4 = INT_MAX;
                            sample5 = INT_MAX;
                            sample6 = INT_MAX;
                            sample7 = INT_MAX;

                            Get32BPPSample(rawbuf, (x + abscount), xextent, &sample4, &sample5, &sample6, &sample7);
            
                            if (sample0 == sample4 ||
                                sample1 != sample5 ||
                                sample2 != sample6 ||
                                sample3 != sample7)
                            {
                                break;
                            }

                            abscount++;
                            absrem--;
                        }

                        rlecount = 1;
                        rlerem = xextent - (x + rlecount);
                        sample4 = s0;
                        sample5 = s1;
                        sample6 = s2;
                        sample7 = s3;

                        while (rlerem > 0 && rlecount < maxrle)
                        {
                            sample0 = sample4;
                            sample1 = sample5;
                            sample2 = sample6;
                            sample3 = sample7;
                            sample4 = INT_MAX;
                            sample5 = INT_MAX;
                            sample6 = INT_MAX;
                            sample7 = INT_MAX;

                            Get32BPPSample(rawbuf, (x + rlecount), xextent, &sample4, &sample5, &sample6, &sample7);
            
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
                            rlevalue = (abscount == 0) ? 1 : abscount;

                            *dstbuf++ = (0 << 7) | (rlevalue - 1);
                            bytesencoded++;

                            for (int i = 0; i < rlevalue; ++i)
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
                            rlevalue = (rlecount == 0) ? 1 : rlecount;

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
                        rawptr += srcpitch;           // set the next span
                    }
                }
            } break;
            case TGA_RGB:
            {
                memcpy(dstbuf, rawbuf, yextent * srcpitch);
                dstbuf  += yextent * srcpitch;
                bytesencoded += yextent * srcpitch;
            } break;
            }
        }
        else if (srcdepthbits == 24)
        {
            switch (image_type)
            {
            case TGA_RGB_RLE:
            {
                int32_t abscount = 0;
                int32_t rlecount = 0;
                int32_t rlevalue = 0;
                int32_t sample0 = 0;
                int32_t sample1 = 0;
                int32_t sample2 = 0;
                int32_t sample3 = 0;
                int32_t sample4 = 0;
                int32_t sample5 = 0;
                int32_t s0 = 0;
                int32_t s1 = 0;
                int32_t s2 = 0;
                int32_t absrem = 0;
                int32_t rlerem = 0;

                while (y++ < yextent)
                {
                    rawbuf = rawptr;
                    x = 0;

                    while (x < xextent)
                    {
                        sample0 = 0;
                        sample1 = 0;
                        sample2 = 0;
                        sample3 = INT_MAX;
                        sample4 = INT_MAX;
                        sample5 = INT_MAX;
                        s0 = INT_MAX;
                        s1 = INT_MAX;
                        s2 = INT_MAX;

                        Get24BPPSample(rawbuf, x + 0, xextent, &s0, &s1, &s2);

                        abscount = 1;
                        absrem = xextent - (x + abscount);
                        sample3 = s0;
                        sample4 = s1;
                        sample5 = s2;

                        while (absrem > 0 && abscount < maxabs)
                        {
                            sample0 = sample3;
                            sample1 = sample4;
                            sample2 = sample5;
                            sample3 = INT_MAX;
                            sample4 = INT_MAX;
                            sample5 = INT_MAX;

                            Get24BPPSample(rawbuf, (x + abscount), xextent, &sample3, &sample4, &sample5);
            
                            if (sample0 == sample3 ||
                                sample1 != sample4 ||
                                sample2 != sample5)
                            {
                                break;
                            }

                            abscount++;
                            absrem--;
                        }

                        rlecount = 1;
                        rlerem = xextent - (x + rlecount);
                        sample3 = s0;
                        sample4 = s1;
                        sample5 = s2;

                        while (rlerem > 0 && rlecount < maxrle)
                        {
                            sample0 = sample3;
                            sample1 = sample4;
                            sample2 = sample5;
                            sample3 = INT_MAX;
                            sample4 = INT_MAX;
                            sample5 = INT_MAX;

                            Get24BPPSample(rawbuf, (x + rlecount), xextent, &sample3, &sample4, &sample5);
            
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
                            rlevalue = (abscount == 0) ? 1 : abscount;

                            *dstbuf++ = (0 << 7) | (rlevalue - 1);
                            bytesencoded++;

                            for (int i = 0; i < rlevalue; ++i)
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
                            rlevalue = (rlecount == 0) ? 1 : rlecount;

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
                        rawptr += srcpitch;           // set the next span
                    }
                }
            } break;
            case TGA_RGB:
            {
                memcpy(dstbuf, rawbuf, yextent * srcpitch);
                dstbuf  += yextent * srcpitch;
                bytesencoded += yextent * srcpitch;
            } break;
            }
        }
        else if (srcdepthbits == 16)
        {
            switch (image_type)
            {
            case TGA_RGB_RLE:
            {
                int32_t abscount = 0;
                int32_t rlecount = 0;
                int32_t rlevalue = 0;
                int32_t sample0 = 0;
                int32_t sample1 = 0;
                int32_t s0 = 0;
                int32_t absrem = 0;
                int32_t rlerem = 0;

                while (y++ < yextent)
                {
                    rawbuf = rawptr;
                    x = 0;

                    while (x < xextent)
                    {
                        sample0 = 0;
                        sample1 = INT_MAX;
                        s0 = INT_MAX;

                        Get16BPPSample(rawbuf, x + 0, xextent, &s0);

                        abscount = 1;
                        absrem = xextent - (x + abscount);
                        sample1 = s0;

                        while (absrem > 0 && abscount < maxabs)
                        {
                            sample0 = sample1;
                            sample1 = INT_MAX;

                            Get16BPPSample(rawbuf, (x + abscount), xextent, &sample1);
            
                            if (sample0 == sample1)
                            {
                                break;
                            }

                            abscount++;
                            absrem--;
                        }

                        rlecount = 1;
                        rlerem = xextent - (x + rlecount);
                        sample1 = s0;

                        while (rlerem > 0 && rlecount < maxrle)
                        {
                            sample0 = sample1;
                            sample1 = INT_MAX;

                            Get16BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
            
                            if (sample0 != sample1)
                            {
                                break;
                            }

                            rlecount++;
                            rlerem--;
                        }

                        if (abscount >= rlecount)
                        {
                            rlevalue = (abscount == 0) ? 1 : abscount;

                            *dstbuf++ = (0 << 7) | (rlevalue - 1);
                            bytesencoded++;

                            for (int i = 0; i < rlecount; ++i)
                            {
                                *dstbuf++ = *(rawbuf + ((x * 2) + i) + 0);
                                *dstbuf++ = *(rawbuf + ((x * 2) + i) + 1);
                                bytesencoded++;
                                bytesencoded++;
                            }
                        }
                        else
                        {
                            rlevalue = (rlecount == 0) ? 1 : rlecount;

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
                        rawptr += srcpitch;           // set the next span
                    }
                }
            } break;
            case TGA_RGB:
            {
                memcpy(dstbuf, rawbuf, yextent * srcpitch);
                dstbuf  += yextent * srcpitch;
                bytesencoded += yextent * srcpitch;
            } break;
            }
        }        
        else if (srcdepthbits == 8 && psrcpalette == NULL)
        {
            switch (image_type)
            {
            case TGA_BLACK_AND_WHITE_RLE:
            {
                int32_t abscount = 0;
                int32_t rlecount = 0;
                int32_t rlevalue = 0;
                int32_t sample0 = 0;
                int32_t sample1 = 0;
                int32_t s0 = 0;
                int32_t absrem = 0;
                int32_t rlerem = 0;

                while (y++ < yextent)
                {
                    rawbuf = rawptr;
                    x = 0;

                    while (x < xextent)
                    {
                        sample0 = 0;
                        sample1 = INT_MAX;
                        s0 = INT_MAX;

                        Get8BPPSample(rawbuf, x + 0, xextent, &s0);

                        abscount = 1;
                        absrem = xextent - (x + abscount);
                        sample1 = s0;

                        while (absrem > 0 && abscount < maxabs)
                        {
                            sample0 = sample1;
                            sample1 = INT_MAX;

                            Get8BPPSample(rawbuf, (x + abscount), xextent, &sample1);
            
                            if (sample0 == sample1)
                            {
                                break;
                            }

                            abscount++;
                            absrem--;
                        }

                        rlecount = 1;
                        rlerem = xextent - (x + rlecount);
                        sample1 = s0;

                        while (rlerem > 0 && rlecount < maxrle)
                        {
                            sample0 = sample1;
                            sample1 = INT_MAX;

                            Get8BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
            
                            if (sample0 != sample1)
                            {
                                break;
                            }

                            rlecount++;
                            rlerem--;
                        }

                        if (abscount >= rlecount)
                        {
                            rlevalue = (abscount == 0) ? 1 : abscount;

                            *dstbuf++ = (0 << 7) | (rlevalue - 1);
                            bytesencoded++;

                            for (int i = 0; i < rlecount; ++i)
                            {
                                *dstbuf++ = *(rawbuf + x + i);
                                bytesencoded++;
                            }
                        }
                        else
                        {
                            rlevalue = (rlecount == 0) ? 1 : rlecount;

                            *dstbuf++ = (1 << 7) | (rlevalue - 1);
                            *dstbuf++ = sample0;
                            bytesencoded++;
                            bytesencoded++;
                        }

                        x += rlevalue;
                    }

                    if (y != yextent)
                    {
                        rawptr += srcpitch;           // set the next span
                    }
                }
            } break;
            case TGA_BLACK_AND_WHITE:
            {
                memcpy(dstbuf, rawbuf, yextent * srcpitch);
                dstbuf  += yextent * srcpitch;
                bytesencoded += yextent * srcpitch;
            } break;
            }
        }
        else if (srcdepthbits == 8 && psrcpalette != NULL)
        {
            switch (image_type)
            {
            case TGA_MAPPED_RLE:
            {
                int32_t abscount = 0;
                int32_t rlecount = 0;
                int32_t rlevalue = 0;
                int32_t sample0 = 0;
                int32_t sample1 = 0;
                int32_t s0 = 0;
                int32_t absrem = 0;
                int32_t rlerem = 0;

                while (y++ < yextent)
                {
                    rawbuf = rawptr;
                    x = 0;

                    while (x < xextent)
                    {
                        sample0 = 0;
                        sample1 = INT_MAX;
                        s0 = INT_MAX;

                        Get8BPPSample(rawbuf, x + 0, xextent, &s0);

                        abscount = 1;
                        absrem = xextent - (x + abscount);
                        sample1 = s0;

                        while (absrem > 0 && abscount < maxabs)
                        {
                            sample0 = sample1;
                            sample1 = INT_MAX;

                            Get8BPPSample(rawbuf, (x + abscount), xextent, &sample1);
            
                            if (sample0 == sample1)
                            {
                                break;
                            }

                            abscount++;
                            absrem--;
                        }

                        rlecount = 1;
                        rlerem = xextent - (x + rlecount);
                        sample1 = s0;

                        while (rlerem > 0 && rlecount < maxrle)
                        {
                            sample0 = sample1;
                            sample1 = INT_MAX;

                            Get8BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
            
                            if (sample0 != sample1)
                            {
                                break;
                            }

                            rlecount++;
                            rlerem--;
                        }

                        if (abscount >= rlecount)
                        {
                            rlevalue = (abscount == 0) ? 1 : abscount;

                            *dstbuf++ = (0 << 7) | (rlevalue - 1);
                            bytesencoded++;

                            for (int i = 0; i < rlecount; ++i)
                            {
                                *dstbuf++ = *(rawbuf + x + i);
                                bytesencoded++;
                            }
                        }
                        else
                        {
                            rlevalue = (rlecount == 0) ? 1 : rlecount;

                            *dstbuf++ = (1 << 7) | (rlevalue - 1);
                            *dstbuf++ = sample0;
                            bytesencoded++;
                            bytesencoded++;
                        }

                        x += rlevalue;
                    }

                    if (y != yextent)
                    {
                        rawptr += srcpitch;           // set the next span
                    }
                }
            } break;
            case TGA_MAPPED:
            {
                memcpy(dstbuf, rawbuf, yextent * srcpitch);
                dstbuf  += yextent * srcpitch;
                bytesencoded += yextent * srcpitch;
            } break;
            }
        }
    }

    // actual tga data size
    *ppdstsize = sizeof(tga_file_t) + id_length +
        colormap_length * ((colormap_size==15 ? 16 : colormap_size) >> 3) +
        (bytesencoded * sizeof(uint8_t));         // ptr to data
    // reallocate array to a actual size
    *ppdst = (uint8_t*)realloc(data, ((*ppdstsize + 2) & ~1));
    
    return true;
}

//-----------------------------------------------------------------------------
// GetInfo
//-----------------------------------------------------------------------------
bool
GetInfoFromMemoryTGA(uint8_t* srccolormap, uint32_t* srcxsize,
    uint32_t* srcysize, uint8_t* srcdepthbits, uint8_t* psrc,
    uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < sizeof(tga_file_t))
    {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

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
    uint8_t depthbits = tgafile.pixel_size;
    uint8_t colormap = tgafile.colormap_type;

    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepthbits != NULL) { *srcdepthbits = depthbits; }
    if (srccolormap != NULL) { *srccolormap = colormap; }

    return true;
}

//-----------------------------------------------------------------------------
// LoadTGA
//-----------------------------------------------------------------------------
bool
LoadFromMemoryTGA(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize,
    uint8_t* srcdepthbits)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < sizeof(tga_file_t))
    {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

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

    if (tgafile.colormap_type == 1 &&
       (tgafile.image_type == TGA_MAPPED || tgafile.image_type == TGA_MAPPED_RLE))
    {
        if (tgafile.pixel_size == 8)
        {
            uint8_t* palptr = srcbuf + tgafile.id_length;
            uint32_t palnum = tgafile.colormap_length < 256 ?
                tgafile.colormap_length : 256;
            
            if (pdstpalette != 0)
            {
                memset(&pdstpalette->data, 0, 256 * sizeof(rgba_t));

                if (tgafile.colormap_size == 15 || tgafile.colormap_size == 16)
                {
                    for (size_t i = 0; i < palnum; ++i)
                    {
                        uint16_t pixel = 0;
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

    srcbuf = srcbuf + tgafile.id_length + tgafile.colormap_length *
        ((tgafile.colormap_size==15 ? 16 : tgafile.colormap_size) >> 3);          // ptr to data

    int32_t xsize = tgafile.width;         // image width 
    int32_t ysize = tgafile.height;            // image height
    int32_t rlevalue = 0;           // run-length value
    int32_t rlecount = 0;           // run-length count
    int32_t pitch = xsize * (tgafile.pixel_size>>3);           // pitch in bytes
    int32_t bytes = 0;

    // decoding
    uint8_t pixel8[4];
    uint8_t comp = tgafile.pixel_size >> 3;
    uint32_t rgb1555 = 0;
    
    if (tgafile.pixel_size !=  8 && tgafile.pixel_size != 16 &&
        tgafile.pixel_size != 24 && tgafile.pixel_size != 32)
    {
        fprintf(stderr, "TGA, unsupported bits: %d.\n", tgafile.pixel_size);
        return false;
    }

    uint8_t* pixels = (uint8_t*)malloc(xsize * ysize * ((tgafile.pixel_size == 16 ? 24 : tgafile.pixel_size)>>3));
    uint8_t* rawptr = pixels;           // start of current dst row
    uint8_t* rawbuf = pixels;           // current dst row
    
    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepthbits != NULL) { *srcdepthbits = tgafile.pixel_size; }

    switch (tgafile.image_type)
    {
        case TGA_MAPPED_RLE:
        case TGA_RGB_RLE:
        case TGA_BLACK_AND_WHITE_RLE:
        {
            int32_t y = ysize;
            int32_t x = xsize;
            
            // 5-bits per color component
            if (tgafile.pixel_size == 16)           // there is no 16-bit greyscale
            {
                while (y-- > 0)
                {
                    rawbuf = rawptr;
                    rlecount = 0;
                    rlevalue = 0;
                    bytes = 0;
                    
                    while (bytes < x)
                    {                        
                        // get rle count
                        if (rlecount == 0)
                        {
                            rlevalue = *srcbuf++;
                            rlecount = (rlevalue & 0x7F) + 1;

                            rgb1555 = 0;
                            rgb1555 |= *srcbuf++;
                            rgb1555 |= *srcbuf++ << 8;
                        }
                        else if (!(rlevalue & 0x80))
                        {
                            // if this is not a repeating count read next pixel of
                            // component size
                            rgb1555 = 0;
                            rgb1555 |= *srcbuf++;
                            rgb1555 |= *srcbuf++ << 8;
                        }

                        *rawbuf++ = (((rgb1555 >>  0) & 0x1F) * 0xFF) / 0x1F;
                        *rawbuf++ = (((rgb1555 >>  5) & 0x1F) * 0xFF) / 0x1F;
                        *rawbuf++ = (((rgb1555 >> 10) & 0x1F) * 0xFF) / 0x1F;
                        
                        bytes++;
                        rlecount--;
                    }

                    if (y != 0)
                    {
                        rawptr += pitch;
                    }
                }
            }
            else
            {
                while (y-- > 0)
                {
                    rawbuf = rawptr;
                    rlecount = 0;
                    rlevalue = 0;
                    bytes = 0;
                    
                    while (bytes < x)
                    {                        
                        // get rle count
                        if (rlecount == 0)
                        {
                            rlevalue = *srcbuf++;
                            rlecount = (rlevalue & 0x7F) + 1;

                            for (int i = 0; i < comp; ++i)
                            {
                                pixel8[i] = *srcbuf++;
                            }
                        }
                        else if (!(rlevalue & 0x80))
                        {
                            // if this is not a repeating count read next pixel of
                            // component size
                            for (int i = 0; i < comp; ++i)
                            {
                                pixel8[i] = *srcbuf++;
                            }
                        }
                        
                        memcpy(rawbuf, pixel8, comp);
                        rawbuf += comp;
                        
                        bytes++;
                        rlecount--;
                    }
                    
                    if (y != 0)
                    {
                        rawptr += pitch;
                    }
                }
            }
        } break;
        case TGA_MAPPED:
        case TGA_RGB:
        case TGA_BLACK_AND_WHITE:
        {
            int32_t y = ysize;
            int32_t x = xsize;

            // 5-bits per color component
            if (tgafile.pixel_size == 16)
            {
                while (y-- > 0)
                {
                    rawbuf = rawptr;
                    bytes = 0;
                    
                    while (bytes < x)
                    {
                        rgb1555 = 0;
                        rgb1555 |= *srcbuf++;
                        rgb1555 |= *srcbuf++ << 8;

                        *rawbuf++ = (((rgb1555 >>  0) & 0x1F) * 0xFF) / 0x1F;
                        *rawbuf++ = (((rgb1555 >>  5) & 0x1F) * 0xFF) / 0x1F;
                        *rawbuf++ = (((rgb1555 >> 10) & 0x1F) * 0xFF) / 0x1F;

                        bytes++;
                    }

                    if (y != 0)
                    {
                        rawptr += pitch;
                    }
                }
            }
            else
            {
                memcpy(rawbuf, srcbuf, y * pitch);
            }
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

#pragma pack(push, 1)
typedef struct _bmp_file
{
    unsigned short  type;
    unsigned long   size;
    unsigned short  reserved1;
    unsigned short  reserved2;
    unsigned long   offset;
} bmp_file_t;

typedef struct _bmp_info
{
    unsigned long   size;
    long            width;
    long            height;
    unsigned short  planes;
    unsigned short  bits;
    unsigned long   compression;
    unsigned long   imagesize;
    long            xresolution;
    long            yresolution;
    unsigned long   num_colours;
    unsigned long   num_colour_indexes;
} bmp_info_t;

typedef struct _bmp_v3_header
{
    unsigned long   size;
    long            width;
    long            height;
    unsigned short  planes;
    unsigned short  bits;
    unsigned long   compression;
    unsigned long   imagesize;
    long            xresolution;
    long            yresolution;
    unsigned long   num_colours;
    unsigned long   num_colour_indexes;
} bmp_v3_header_t;

typedef struct _bmp_v4_header
{
  unsigned long size;
  long          width;
  long          height;
  short         planes;
  short         bitCount;
  unsigned long compression;
  unsigned long sizeImage;
  long          xPelsPerMeter;
  long          yPelsPerMeter;
  unsigned long clrUsed;
  unsigned long clrImportant;
  unsigned long redMask;
  unsigned long greenMask;
  unsigned long blueMask;
  unsigned long alphaMask;
  unsigned long csType;
  long          endpoints[9];         // CIEXYZTRIPLE 3x3 fixed-point long type
  unsigned long gammaRed;
  unsigned long gammaGreen;
  unsigned long gammaBlue;
} bmp_v4_header_t;

typedef struct _bmp_v5_header
{
  unsigned long size;
  long          width;
  long          height;
  short         planes;
  short         bitCount;
  unsigned long compression;
  unsigned long sizeImage;
  long          xPelsPerMeter;
  long          yPelsPerMeter;
  unsigned long clrUsed;
  unsigned long clrImportant;
  unsigned long redMask;
  unsigned long greenMask;
  unsigned long blueMask;
  unsigned long alphaMask;
  unsigned long csType;
  long          endpoints[9];         // CIEXYZTRIPLE 3x3 fixed-point long type
  unsigned long gammaRed;
  unsigned long gammaGreen;
  unsigned long gammaBlue;
  unsigned long intent;
  unsigned long profileData;
  unsigned long profileSize;
  unsigned long reserved;
} bmp_v5_header_t;
#pragma pack(pop)

//------------------------------------------------------------------------------
// SaveBMP
//------------------------------------------------------------------------------
bool
SaveToMemoryBMP(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec,
    uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize, uint32_t srcdepthbits,
    palette_t* psrcpalette, rgba_t* pcolorkey, bool invertY)
{
    if (ppdst == NULL || ppdstsize == 0)
    {
        fprintf(stderr, "BMP, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL)
    {
        printf("BMP, Invalid src data.\n");
        return false;
    }

    if (srcdepthbits !=  1 && srcdepthbits !=  4 && srcdepthbits != 8 &&
        srcdepthbits != 24 && srcdepthbits != 32)
    {
        printf("BMP, Unsupported depth: %d.\n", srcdepthbits);
        return false;
    }

    uint8_t dstdepth = 0;

    switch (srcdepthbits)
    {
    case 32:
    {
        dstdepth = 32;
    } break;
    case 24:
    {
        dstdepth = 24;
    } break;
    case 8:
    case 4:
    case 1:
    {
        dstdepth = srcdepthbits;

        if (psrcpalette != 0)
        {
            if (psrcpalette->size <= 256 && psrcpalette->size > 16)
            {
                dstdepth = 8;
            }
            else if (psrcpalette->size <= 16 && psrcpalette->size > 2)
            {
                dstdepth = 4;
            }
            else if (psrcpalette->size <= 2)
            {
                dstdepth = 1;
            }
        }
    } break;
    }

    int32_t xorigin = 0;
    int32_t yorigin = 0;
    int32_t xextent = srcxsize;
    int32_t yextent = srcysize;

    // dst stuff
    float dstpixelsperbyte = PIXELS_PER_BYTE(dstdepth);
    int32_t dstpitch = (int32_t)(ceilf((float)(xextent) / dstpixelsperbyte) + 1) & ~1;           // bytes per scanline

    uint32_t compression = BI_RGB;

    if (codec == ENCODE_RLE)
    {
        if (dstdepth == 8)
        {
            compression = BI_RLE8;
        }
        else if (dstdepth == 4)
        {
            compression = BI_RLE4;
        }
    }

    int32_t widthbytes = ((xextent * dstdepth + 31) >> 5) * 4;
    int32_t xsizebytes = (xextent * dstdepth + 7) >> 3;
    int32_t padbytes = widthbytes - xsizebytes;

    // src stuff
    uint32_t srcbytesperpixel = ((srcdepthbits == 32) ? 4 : (srcdepthbits == 24) ? 3 : 1);
    uint32_t srcpitch = srcxsize * srcbytesperpixel;
    uint8_t* rawptr = psrc + (yorigin * srcpitch) + (xorigin * srcbytesperpixel);         // start of current row
    uint8_t* rawbuf = psrc + (yorigin * srcpitch) + (xorigin * srcbytesperpixel);         // current row

    int32_t dstpalettesize = 0;           // palette
    
    // palette
    if (srcdepthbits <= 8)
    {
        if (psrcpalette != 0)
        {
            if (dstdepth != 1 || dstdepth != 2 || dstdepth != 4)
            {
                dstpalettesize = psrcpalette->size * 4;
            }
        }
    }

    int32_t type = 0x4D42;          // 'BM'
    // total bytes per scanline required to encode bmp data
    // big array for true-color images
    int32_t datasize = sizeof(bmp_file_t) + sizeof(bmp_info_t) +
        ((yextent * dstpitch) * sizeof(unsigned char)) + dstpalettesize;          // bmp data size
    uint8_t* data = (uint8_t*)malloc(datasize);
    memset(data, 0, datasize * sizeof(uint8_t));

    uint32_t dstlen = datasize;
    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;
    uint8_t* dstend = data + dstlen;

    // fill in file and info header
    bmp_file_t bmpfile = {};
    bmpfile.type = type;
    bmpfile.size = datasize;
    bmpfile.offset = sizeof(bmp_file_t) + sizeof(bmp_info_t) + dstpalettesize;

    bmp_info_t bmpinfo = {};
    bmpinfo.size = sizeof(bmp_info_t);
    bmpinfo.width = xextent;
    bmpinfo.height = (invertY == true) ? yextent : -yextent;          // bottom-up dib
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
    if (srcdepthbits <= 8)
    {
        if (psrcpalette != 0)
        {
            for (int32_t i = 0; i < dstpalettesize >> 2; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].b;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].r;
                *dstbuf++ = psrcpalette->data[i].a;
            }
        }
    }

    int32_t bytesencoded = 0;
    int32_t x = 0;
    int32_t y = 0;
    bool rle = compression == ENCODE_RLE;            // ENCODING_RGB = 1 || ENCODE_RLE = 2

    // encoding
    if (psrc != 0)
    {
        if (srcdepthbits == 32)
        {
            while (y++ < yextent)
            {
                rawbuf = rawptr;
                x = 0;

                while (x < xextent)
                {
                    *dstbuf++ = *(rawbuf + ((x * srcbytesperpixel) + 0));         // b
                    *dstbuf++ = *(rawbuf + ((x * srcbytesperpixel) + 1));         // g
                    *dstbuf++ = *(rawbuf + ((x * srcbytesperpixel) + 2));         // r
                    *dstbuf++ = *(rawbuf + ((x * srcbytesperpixel) + 3));         // a
                    x++;
                }

                if (y != yextent)
                {
                    rawptr += srcpitch;           // set the next span
                }
                dstbuf += padbytes;
                bytesencoded += srcpitch + padbytes;
            }
        }
        else if (srcdepthbits == 24)
        {
            while (y++ < yextent)
            {
                rawbuf = rawptr;
                x = 0;

                while (x < xextent)
                {
                    *dstbuf++ = *(rawbuf + ((x * srcbytesperpixel) + 0));         // b
                    *dstbuf++ = *(rawbuf + ((x * srcbytesperpixel) + 1));         // g
                    *dstbuf++ = *(rawbuf + ((x * srcbytesperpixel) + 2));         // r
                    x++;
                }

                if (y != yextent)
                {
                    rawptr += srcpitch;           // set the next span
                }
                dstbuf += padbytes;
                bytesencoded += srcpitch + padbytes;
            }
        }
        else if (srcdepthbits <= 8)
        {
            if (rle)            // run-length encoding
            {
                int32_t colorkey = -1;

                if (pcolorkey != 0 && psrcpalette != 0)
                {
                    for (int i = 0; i < dstpalettesize >> 2; ++i)
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

                int32_t abscount = 0;
                int32_t rlecount = 0;           // run-length count
                int32_t rlevalue = 0;           // run-length value
                int32_t sample0 = 0;
                int32_t sample1 = 0;
                int32_t sample2 = 0;
                int32_t sample3 = 0;
                int32_t s0 = 0;
                int32_t s1 = 0;
                int32_t s2 = 0;
                int32_t s3 = 0;
                int32_t maxrle = 0xFF;          // max run-length count is 0xFF
                int32_t maxabs = 0xFF;          // max abs-length count is 0xFF
                int32_t absrem = 0;
                int32_t rlerem = 0;

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
                                sample0 = 0;            // sample 1
                                sample1 = INT_MAX;          // sample 2
                                sample2 = INT_MAX;          // sample 3

                                // pre-sample 3 pixels
                                Get4BPPSample(rawbuf, x + 0, xextent, &sample2);

                                abscount = 1;
                                absrem = xextent - (x + abscount);
                                sample1 = sample2;

                                while (absrem > 0 && abscount < maxabs)
                                {
                                    sample0 = sample1;
                                    sample1 = INT_MAX;

                                    Get4BPPSample(rawbuf, (x + abscount), xextent, &sample1);

                                    if (sample0 == sample1)
                                    {
                                        break;
                                    }

                                    abscount++;
                                    absrem--;
                                    if (abscount + 1 < maxabs) {abscount++;absrem--;}
                                }

                                rlecount = 1;
                                rlerem = xextent - (x + rlecount);
                                sample1 = sample2;

                                while (rlerem > 0 && rlecount < maxrle)
                                {
                                    sample0 = sample1;
                                    sample1 = INT_MAX;

                                    Get4BPPSample(rawbuf, (x + rlecount), xextent, &sample1);

                                    if (sample0 != sample1)
                                    {
                                        break;
                                    }

                                    rlecount++;
                                    rlerem--;
                                    if (rlecount + 1 < maxabs) {rlecount++;rlerem--;}
                                }

                                if (abscount >= 3 && abscount >= rlecount)
                                {
                                    rlevalue = (abscount == 0) ? 1 : abscount;
                                    
                                    *dstbuf++ = 0x00;
                                    *dstbuf++ = rlevalue;
                                    bytesencoded++;
                                    bytesencoded++;

                                    for (int i = 0; i < (rlevalue >> 1); ++i)
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
                                    rlevalue = (rlecount == 0) ? 1 : rlecount;

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
                                rawptr += srcpitch;           // set the next span
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
                                sample0 = 0;            // sample 1
                                sample1 = INT_MAX;            // sample 2
                                sample2 = INT_MAX;            // sample 3

                                // pre-sample 4 pixels
                                Get8BPPSample(rawbuf, x + 0, xextent, &sample1);

                                abscount = 1;
                                absrem = xextent - (x + abscount);
                                sample1 = sample2;

                                while (absrem > 0 && abscount < maxabs)
                                {
                                    sample0 = sample1;
                                    sample1 = INT_MAX;

                                    Get8BPPSample(rawbuf, (x + abscount), xextent, &sample1);

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
                                    sample1 = INT_MAX;

                                    Get8BPPSample(rawbuf, (x + rlecount), xextent, &sample1);

                                    if (sample0 != sample1)
                                    {
                                        break;
                                    }

                                    rlecount++;
                                    rlerem--;
                                }

                                if (abscount >= 3 && abscount >= rlecount)
                                {
                                    rlevalue = (abscount == 0) ? 1 : abscount;

                                    *dstbuf++ = 0x00;
                                    *dstbuf++ = rlevalue;
                                    bytesencoded++;
                                    bytesencoded++;

                                    for (int i = 0; i < rlevalue; ++i)
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
                                    rlevalue = (rlecount == 0) ? 1 : rlecount;

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
                                rawptr += srcpitch;           // set the next span
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
            else            // index
            {
                int32_t sample = 0;

                switch (dstdepth)
                {
                    case 1:         // 1-bit encoding
                    case 4:         // 4-bit encoding
                    {
                        // for each row in image 
                        while (y++ < yextent)
                        {
                            rawbuf = rawptr;
                            x = 0;

                            // all other samples
                            while (x < xextent)
                            {
                                sample = 0;

                                if (dstdepth == 1) {
                                    Get1BPPSample(rawbuf, x, xextent, &sample);
                                } else {
                                    Get4BPPSample(rawbuf, x, xextent, &sample);
                                }

                                x += (dstdepth == 1) ? 8 : 2;

                                *dstbuf++ = sample;
                                bytesencoded++;
                            }

                            if (y != yextent)
                            {
                                rawptr += srcpitch;           // set the next span
                            }

                            dstbuf += padbytes;
                            bytesencoded += padbytes;
                        }
                    } break;
                    case  8:        // 8-bit encoding
                    case 24:        // 24bit encoding
                    case 32:        // 32bit encoding
                    {
                        while (y++ < yextent)
                        {
                            rawbuf = rawptr;
                            memcpy(dstbuf, rawbuf, srcpitch);
                            if (y != yextent)
                            {
                                rawptr += srcpitch;           // set the next span
                            }
                            dstbuf += srcpitch + padbytes;
                            bytesencoded += srcpitch + padbytes;
                        }
                    } break;
                }
            }
        }
    }

    datasize = sizeof(bmp_file_t) + sizeof(bmp_info_t) + dstpalettesize +
        (bytesencoded * sizeof(uint8_t));

    // rewrite file struct size
    dstbuf = dstptr + 2;
    WriteU32ToLE(dstbuf, datasize); dstbuf += 4;

    // actual bmp data size
    *ppdstsize = datasize;
    // reallocate array to a actual size
    *ppdst = (uint8_t*)realloc(data, (*ppdstsize + 2) & ~1);

    return true;
}

//------------------------------------------------------------------------------
// GetInfo
//------------------------------------------------------------------------------
bool
GetInfoFromMemoryBMP(uint32_t* srcxsize, uint32_t* srcysize,
    uint8_t* srcdepthbits, uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < sizeof(bmp_file_t))
    {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

    bmp_file_t bmpfile = {};
    bmp_info_t bmpinfo = {};

    // file struct
    bmpfile.type    = ReadU16FromLE(srcbuf);    srcbuf += 2;           // type - 0x4D4 = 'BM'
    bmpfile.size    = ReadU32FromLE(srcbuf);    srcbuf += 4;           // file size
    srcbuf += 4;            // reserved1 and reserved2
    bmpfile.offset  = ReadU32FromLE(srcbuf);    srcbuf += 4;            // offset to data

    // info struct
    bmpinfo.size        = ReadU32FromLE(srcbuf);        srcbuf += 4;            // info size
    bmpinfo.width       = ReadI32FromLE(srcbuf);        srcbuf += 4;            // width
    bmpinfo.height      = ReadI32FromLE(srcbuf);        srcbuf += 4;            // height
    bmpinfo.planes      = ReadU16FromLE(srcbuf);        srcbuf += 2;            // planes
    bmpinfo.bits        = ReadU16FromLE(srcbuf);        srcbuf += 2;            // bit count
    bmpinfo.compression = ReadU32FromLE(srcbuf);        srcbuf += 4;            // compression
    bmpinfo.imagesize   = ReadU32FromLE(srcbuf);        srcbuf += 4;            // imagesize
    bmpinfo.xresolution = ReadI32FromLE(srcbuf);        srcbuf += 4;            // x-resolution
    bmpinfo.yresolution = ReadI32FromLE(srcbuf);        srcbuf += 4;            // y-resolution
    bmpinfo.num_colours = ReadU32FromLE(srcbuf);        srcbuf += 4;            // num_colours
    bmpinfo.num_colour_indexes = ReadU32FromLE(srcbuf); srcbuf += 4;            // num_colour_indexes

    uint32_t xsize = bmpinfo.width;         // image width
    uint32_t ysize = ABS(bmpinfo.height);           // image height
    uint32_t depthbits = bmpinfo.bits;          // bits per pixel component

    if (bmpfile.type != 0x4D42 || bmpfile.reserved1 != 0 ||
        bmpfile.reserved2 != 0 || bmpinfo.planes != 1 || bmpinfo.compression > 2)
    {
        return false;
    }

    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepthbits != NULL) { *srcdepthbits = depthbits; }

    return true;
}

//------------------------------------------------------------------------------
// LoadBMP
//------------------------------------------------------------------------------
bool
LoadFromMemoryBMP(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize,
    uint8_t* srcdepthbits)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < sizeof(bmp_file_t))
    {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

    bmp_file_t bmpfile = {};
    bmp_info_t bmpinfo = {};

    // file struct
    bmpfile.type    = ReadU16FromLE(srcbuf);    srcbuf += 2;           // type - 0x4D4 = 'BM'
    bmpfile.size    = ReadU32FromLE(srcbuf);    srcbuf += 4;           // file size
    srcbuf += 4;            // reserved1 and reserved2
    bmpfile.offset  = ReadU32FromLE(srcbuf);    srcbuf += 4;            // offset to data

    // info struct
    bmpinfo.size        = ReadU32FromLE(srcbuf);        srcbuf += 4;            // info size
    bmpinfo.width       = ReadI32FromLE(srcbuf);        srcbuf += 4;            // width
    bmpinfo.height      = ReadI32FromLE(srcbuf);        srcbuf += 4;            // height
    bmpinfo.planes      = ReadU16FromLE(srcbuf);        srcbuf += 2;            // planes
    bmpinfo.bits        = ReadU16FromLE(srcbuf);        srcbuf += 2;            // bit count
    bmpinfo.compression = ReadU32FromLE(srcbuf);        srcbuf += 4;            // compression
    bmpinfo.imagesize   = ReadU32FromLE(srcbuf);        srcbuf += 4;            // imagesize
    bmpinfo.xresolution = ReadI32FromLE(srcbuf);        srcbuf += 4;            // x-resolution
    bmpinfo.yresolution = ReadI32FromLE(srcbuf);        srcbuf += 4;            // y-resolution
    bmpinfo.num_colours = ReadU32FromLE(srcbuf);        srcbuf += 4;            // num_colours
    bmpinfo.num_colour_indexes = ReadU32FromLE(srcbuf); srcbuf += 4;            // num_colour_indexes

    if (bmpfile.type != 0x4D42 || bmpfile.reserved1 != 0 ||
        bmpfile.reserved2 != 0 || bmpinfo.planes != 1 || bmpinfo.compression > 2)
    {
        return false;
    }

    if (bmpinfo.bits == 1 || bmpinfo.bits == 4 || bmpinfo.bits == 8)
    {
        uint8_t* palptr = srcptr + sizeof(bmp_file_t) +
            sizeof(bmp_info_t);
        uint32_t palnum = bmpinfo.num_colours;

        if (pdstpalette != 0)
        {
            memset(&pdstpalette->data, 0, 256 * sizeof(rgba_t));

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
    int32_t xsize = bmpinfo.width;         // image width
    int32_t ysize = bmpinfo.height;            // image height
    int32_t rlecount = 0;           // run-length count
    int32_t pitch = xsize * (bmpinfo.bits < 8 ? 1 : (bmpinfo.bits >> 3));          // bytes per scanline of output
    int32_t widthbytes = ((xsize * bmpinfo.bits + 31) >> 5) * 4;           // 4-byte boundary
    int32_t xsizebytes = (xsize * bmpinfo.bits + 7) >> 3;          // bytes per scanline of input 
    int32_t padbytes = widthbytes - xsizebytes;
    uint8_t sample = 0;
    int32_t run = 0;            // run-length
    int32_t max = 0;            // maximum run-length
    int32_t bit = 0;            // bit-offset

    uint8_t* pixels = (uint8_t*)malloc(xsize * ABS(ysize) *
        (bmpinfo.bits < 8 ? 1 : (bmpinfo.bits >> 3)));
    uint8_t* pixptr = pixels;           // start of current dst row
    uint8_t* pixbuf = pixels;           // current dst row
    memset(pixels, 0, (xsize * ysize * (bmpinfo.bits < 8 ? 1 : (bmpinfo.bits >> 3))));

    // bottom-up dib
    if (ysize >= 0)
    {
        pixptr = (pixels + ((ABS(ysize)-1)) * pitch);
        pitch = -pitch;
    }

    ysize = ABS(ysize);

    *ppdst = pixels;
    if (srcxsize != 0) { *srcxsize = xsize; }
    if (srcysize != 0) { *srcysize = ABS(ysize); }
    if (srcdepthbits != 0) { *srcdepthbits = bmpinfo.bits; }

    if (rle)            // run-length encoding
    {
        int32_t y = ABS(ysize);
        int32_t x = xsize;

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
                            case 4:         // 4-bit
                            {
                                for (int i = 0; i < rlecount; ++i)
                                {
                                    sample = *srcbuf++;
                                    int d1 = sample & 0xF;
                                    int d0 = sample >> 4;
                                    *pixbuf++ = d0;
                                    if ((i * 2) + 1 == data1) break;
                                    *pixbuf++ = d1;
                                }
                                
                                srcbuf += padbytes;
                            } break;
                            case 8:         // 8-bit
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
                        else { pixptr = pixels + (y * pitch); }
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
                    rlecount = data0;
                    sample = data1;

                    switch (bmpinfo.bits)
                    {
                        case 4:         // 4-bit
                        {
                            for (int i = 0; i < rlecount; ++i)
                            {
                                int d1 = sample & 0xF;
                                int d0 = sample >> 4;
                                if (i & 1) { *pixbuf++ = d1; }
                                else { *pixbuf++ = d0; }
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

            if (y != 0)
            {
                pixptr += pitch;
            }
        }
    }
    else            // index
    {
        switch (bmpinfo.bits)
        {
            // decode bmp
            case 1:         // 1-bit
            case 4:         // 4-bit
            {
                if (bmpinfo.bits == 1)         // 1bit indexed
                {
                    max = 8;
                    bit = 7;
                }
                else            // 4-bit indexed
                {
                    max = 2;
                    bit = 4;
                }

                int32_t x = xsize;
                int32_t y = ysize;

                while (y-- > 0)
                {
                    pixbuf = pixptr;
                    run = max;
                    x = xsize;

                    while (x > 0)
                    {
                        BlitNbitsToIndex8(pixbuf, 1, *srcbuf, run, bit);
                        pixbuf += run;
                        srcbuf++;
                        x -= run;
                    }

                    if (y != 0)
                    {
                        pixptr += pitch;
                        srcbuf += padbytes;
                    }
                }
            } break;
            case  8:        // 8-bit
            case 24:        // 24bit
            {
                int32_t y = ysize;
                int32_t x = xsize;

                while (y-- > 0)
                {
                    pixbuf = pixptr;
                    memcpy(pixbuf, srcbuf, ABS(pitch));
                    srcbuf += ABS(pitch) + padbytes;

                    if (y != 0)
                    {
                        pixptr += pitch;
                    }
                }   
            } break;
            case 32:        // 32bit
            {
                uint32_t alpha = 0;
                int32_t x = 0;
                int32_t y = 0;
                uint8_t r = 0;
                uint8_t g = 0;
                uint8_t b = 0;
                uint8_t a = 0;

                x = xsize;
                y = ysize;

                while (y-- > 0)
                {
                    pixbuf = pixptr;

                    for (int32_t i = 0; i < x; ++i)
                    {
                        b = *srcbuf++;
                        g = *srcbuf++;
                        r = *srcbuf++;
                        a = *srcbuf++;

                        alpha |= a;

                        *pixbuf++ = b;
                        *pixbuf++ = g;
                        *pixbuf++ = r;
                        *pixbuf++ = a;
                    }

                    srcbuf += padbytes;

                    if (y != 0)
                    {
                        pixptr += pitch;
                    }
                }

                // if alpha is all 0's replace with 255's
                if (alpha == 0)
                {
                    pixbuf = pixels;

                    for (int i = 0; i < xsize * ysize; ++i, pixbuf += 4)
                    {
                        pixbuf[3] = 255;
                    }
                }

            } break;
            default:
            {
                // should never get here!
                fprintf(stderr, "BMP, Corrupt\n");
                return false;
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


#pragma pack(push, 1)
typedef struct _pcx_v5_info_s
{
    byte identifier;         // PCX Id Number (Always 0x0A)
    byte version;            // Version Number
    byte encoding;           // Encoding Format
    byte bitsPerPixel;       // Bits Per Pixel
    word xMin;               // Left of image
    word yMin;               // Top of image
    word xMax;               // Right of image
    word yMax;               // Bottom of image
    word horzRes;            // Horizontal Resolution
    word vertRes;            // Vertical Resolution
    byte palette[48];        // 16-Color EGA Palette
    byte reserved1;          // Reserved (Always 0)
    byte numBitPlanes;       // Number of Bit Planes
    word bytesPerLine;       // Bytes Per Scan-line
    word paletteType;        // Palette Type
    word horzScreenSize;     // Horizontal Screen Size
    word vertScreenSize;     // Vertical Screen Size
    byte reserved2[54];      // Reserved (Always 0)
} pcx_v5_info_t;
#pragma pack(pop)

// default 64 color ega palette
static rgba_t ega_palette[64] = {
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

//-----------------------------------------------------------------------------
// SavePCX
//-----------------------------------------------------------------------------
bool
SaveToMemoryPCX(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec,
    uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize, uint8_t srcdepthbits,
    palette_t* psrcpalette)
{
    if (ppdst == NULL || ppdstsize == 0)
    {
        fprintf(stderr, "PCX, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL)
    {
        fprintf(stderr, "PCX, Invalid src data.\n");
        return false;
    }

    if (srcdepthbits !=  1 && srcdepthbits != 2 &&
        srcdepthbits !=  4 && srcdepthbits != 8 &&
        srcdepthbits != 24)
    {
        fprintf(stderr, "PCX, Unsupported bits: %d.\n", srcdepthbits);
        return false;
    }

    uint8_t dstdepth = 0;

    switch (srcdepthbits)
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
        dstdepth = srcdepthbits;

        if (psrcpalette != 0)
        {
            if (psrcpalette->size <= 256 && psrcpalette->size > 16)
            {
                dstdepth = 8;
            }
            else if (psrcpalette->size <= 16 && psrcpalette->size > 4)
            {
                dstdepth = 4;
            }
            else if (psrcpalette->size <= 4 && psrcpalette->size > 2)
            {
                dstdepth = 2;
            }
            else if (psrcpalette->size <= 2)
            {
                dstdepth = 1;
            }
        }
    } break;
    }

    if (dstdepth != 1 && dstdepth != 2 && dstdepth != 4 && dstdepth != 8)
    {
        fprintf(stderr, "PCX, unsupported dst bits: %d.\n", dstdepth);
        return false;
    }

    int32_t xorigin = 0;
    int32_t yorigin = 0;
    int32_t xextent = srcxsize;
    int32_t yextent = srcysize;

    uint16_t colorplanes = 1;            // color-planes
    uint16_t palettetype = 0;            // palette-type

    // version and number of color planes
    if (srcdepthbits <= 8)
    {
        palettetype = 1;
    }
    else if (srcdepthbits == 24)
    {
        colorplanes = 3;
    }

    // dst stuff
    float dstpixelsperbyte = PIXELS_PER_BYTE(dstdepth);
    int32_t dstpitch = (int32_t)(ceilf((float)(xextent) / dstpixelsperbyte) + 1) & ~1;           // bytes per scanline (must be an even number)
    
    // src stuff
    int32_t srcbytesperpixel = ((srcdepthbits == 24) ? 3 : 1);
    int32_t srcpitch = srcxsize * srcbytesperpixel;            // bytes per src span
    uint8_t* rawptr = psrc;         // start of current row
    uint8_t* rawbuf = psrc;         // current row

    // total bytes per scanline required to encode pcx data
    int32_t dsttotalbytes = colorplanes * dstpitch;
    int32_t dstpalettesize = 0;
    int32_t dstpadbytes = dstpitch - srcxsize;
    
    // palette
    if (srcdepthbits <= 8)
    {
        if (psrcpalette != 0)
        {
            if (dstdepth != 1 || dstdepth != 2 || dstdepth != 4)
            {
                dstpalettesize = psrcpalette->size * 3;
            }
        }
    }

    // big array for true-color images
    int32_t datasize = sizeof(pcx_v5_info_t) + ((yextent * dsttotalbytes) * 2 *
        sizeof(uint8_t)) + dstpalettesize;            // pcx data size
    uint8_t* data = (uint8_t*)malloc(datasize);
    memset(data, 0, datasize * sizeof(uint8_t));

    uint32_t dstlen = datasize;
    uint8_t* dstptr = data;
    uint8_t* dstbuf = data;
    uint8_t* dstend = data + dstlen;

    // fill in file header
    pcx_v5_info_t pcx = {};

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
    WriteU16ToLE(dstbuf, dstpitch);         dstbuf += 2;            // Bytes per scan-line (must be an even number)    
    WriteU16ToLE(dstbuf, palettetype);      dstbuf += 2;            // Palette Type
    WriteU16ToLE(dstbuf, 0);                dstbuf += 2;            // Horizontal Screen Size
    WriteU16ToLE(dstbuf, 0);                dstbuf += 2;            // Vertical Screen Size
    dstbuf += 54;           // Reserved2 (Always 0)

    // encoding
    int32_t bytesencoded = 0;           // number of data bytes encoded
    int32_t colorplane = 0;         // true color bit plane
    int32_t rlecount = 0;           // run-length count
    int32_t abscount = 0;           // abs-length count
    int32_t rlevalue = 0;           // run-length value
    int32_t sample0 = 0;
    int32_t sample1 = 0;
    int32_t sample2 = 0;
    int32_t maxrle = 0x3F;          // max rle count is 0x3F
    int32_t maxabs = 0x01;          // max abs count is 0x01
    int32_t absrem = 0;
    int32_t rlerem = 0;
    int32_t x = 0;
    int32_t y = 0;

    if (srcdepthbits == 24)
    {
        // for each row in image
        while (y++ < yextent)
        {
            // get the image row, clear color-plane and subtotal
            colorplane = 0;

            do
            {
                rawbuf = rawptr;
                x = 0;

                // all other samples
                while (x < xextent)
                {
                    sample0 = 0;
                    sample1 = INT_MAX;
                    sample2 = INT_MAX;

                    switch (colorplane)
                    {
                    case 0:
                    {
                        Get24BPPSampleR(rawbuf, x + 0, xextent, &sample2);
                    } break;
                    case 1:
                    {
                        Get24BPPSampleG(rawbuf, x + 0, xextent, &sample2);
                    } break;
                    case 2:
                    {
                        Get24BPPSampleB(rawbuf, x + 0, xextent, &sample2);
                    } break;
                    }

                    abscount = 1;
                    absrem = xextent - (x + abscount);
                    sample1 = sample2;

                    while (absrem > 0 && abscount < maxabs)
                    {
                        sample0 = sample1;
                        sample1 = INT_MAX;

                        switch (colorplane)
                        {
                        case 0:
                        {
                            Get24BPPSampleR(rawbuf, (x + abscount), xextent, &sample1);
                        } break;
                        case 1:
                        {
                            Get24BPPSampleG(rawbuf, (x + abscount), xextent, &sample1);
                        } break;
                        case 2:
                        {
                            Get24BPPSampleB(rawbuf, (x + abscount), xextent, &sample1);
                        } break;
                        }

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
                        sample1 = INT_MAX;

                        switch (colorplane)
                        {
                        case 0:
                        {
                            Get24BPPSampleR(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        case 1:
                        {
                            Get24BPPSampleG(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        case 2:
                        {
                            Get24BPPSampleB(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        }

                        if (sample0 != sample1)
                        {
                            break;
                        }

                        rlecount++;
                        rlerem--;
                    }

                    if (abscount >= rlecount)
                    {
                        rlevalue = (abscount == 0) ? 1 : abscount;

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
                        rlevalue = (rlecount == 0) ? 1 : rlecount;

                        *dstbuf++ = rlevalue | 0xC0;
                        *dstbuf++ = sample0;
                        bytesencoded++;
                        bytesencoded++;
                    }

                    x += rlevalue;
                }

                dstbuf += dstpadbytes;
                bytesencoded += dstpadbytes;

                // bump the color-plane and restart the output row
                colorplane++;

            } while (colorplane < colorplanes);         // no encoding break at the end of each color-plane

            if (y != yextent)
            {
                rawptr += srcpitch;           // set the next span
            }
        }
    }
    else if (srcdepthbits <= 8)
    {
        // for each row in image
        while (y++ < yextent)
        {
            // get the image row, clear color-plane and subtotal
            colorplane = 0;

            do
            {
                rawbuf = rawptr;
                x = 0;

                // all other samples
                while (x < xextent)
                {
                    sample0 = 0;
                    sample1 = INT_MAX;
                    sample2 = INT_MAX;

                    switch (dstdepth)
                    {
                    case 1:
                    {
                        Get1BPPSample(rawbuf, x + 0, xextent, &sample2);
                    } break;
                    case 2:
                    {
                        Get2BPPSample(rawbuf, x + 0, xextent, &sample2);
                    } break;
                    case 4:
                    {
                        Get4BPPSample(rawbuf, x + 0, xextent, &sample2);
                    } break;
                    case 8:
                    {
                        Get8BPPSample(rawbuf, x + 0, xextent, &sample2);
                    } break;
                    }
                
                    abscount = 1;
                    absrem = xextent - (x + abscount);
                    sample1 = sample2;

                    while (absrem > 0 && abscount < maxabs)
                    {
                        sample0 = sample1;
                        sample1 = INT_MAX;

                        switch (dstdepth)
                        {
                        case 1:
                        {
                            Get1BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        case 2:
                        {
                            Get2BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        case 4:
                        {
                            Get4BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        case 8:
                        {
                            Get8BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        }

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
                        sample1 = INT_MAX;

                        switch (dstdepth)
                        {
                        case 1:
                        {
                            Get1BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        case 2:
                        {
                            Get2BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        case 4:
                        {
                            Get4BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        case 8:
                        {
                            Get8BPPSample(rawbuf, (x + rlecount), xextent, &sample1);
                        } break;
                        }

                        if (sample0 != sample1)
                        {
                            break;
                        }

                        rlecount++;
                        rlerem--;
                    }

                    if (abscount >= rlecount)
                    {
                        rlevalue = (abscount == 0) ? 1 : abscount;

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
                        rlevalue = (rlecount == 0) ? 1 : rlecount;

                        *dstbuf++ = rlevalue | 0xC0;
                        *dstbuf++ = sample0;
                        bytesencoded++;
                        bytesencoded++;
                    }

                    x += rlevalue;
                }

                dstbuf += dstpadbytes;
                bytesencoded += dstpadbytes;

                // bump the color-plane and restart the output row
                colorplane++;

            } while (colorplane < colorplanes);         // no encoding break at the end of each color-plane

            if (y != yextent)
            {
                rawptr += srcpitch;           // set the next span
            }
        }
    }

    // palette
    if (srcdepthbits <= 8)
    {
        if (dstdepth == 1 || dstdepth == 2 || dstdepth == 4)
        {
            dstbuf = egaptr;            // 16-Color EGA Palette

            for (unsigned int i = 0; i < 16; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].r;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].b;
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

            bytesencoded += 256 * 3;
        }
    }

    // actual pcx data size
    *ppdstsize = sizeof(pcx_v5_info_t) + (bytesencoded * sizeof(uint8_t));
    // reallocate array to actual size
    *ppdst = (uint8_t*)realloc(data, *ppdstsize + 2 & ~1);

    return true;
}

//-----------------------------------------------------------------------------
// GetInfoPCX
//-----------------------------------------------------------------------------
bool
GetInfoFromMemoryPCX(uint32_t* srcxsize, uint32_t* srcysize,
    uint8_t* srcdepthbits, uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < sizeof(pcx_v5_info_t))
    {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

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
        fprintf(stderr, "PCX, invalid manufacturer code: %d.\n",
            pcx.identifier);
        return false;
    }

    if (pcx.encoding != 1)
    {
        fprintf(stderr, "PCX, invalid encoding value: %d.\n", pcx.encoding);
        return false;
    }

    if (pcx.version != 0 && pcx.version != 2 && pcx.version != 3 &&
        pcx.version != 4 && pcx.version != 5)
    {
        fprintf(stderr, "PCX, version mismatch: %d.\n", pcx.version);
        return false;
    }

    if (pcx.bitsPerPixel != 1 && pcx.bitsPerPixel != 2 &&
        pcx.bitsPerPixel != 4 && pcx.bitsPerPixel != 8)
    {
        fprintf(stderr, "PCX, unsupported bits: %d.\n", pcx.bitsPerPixel);
        return false;
    }

    uint32_t xsize = (pcx.xMax - pcx.xMin) + 1;
    uint32_t ysize = (pcx.yMax - pcx.yMin) + 1;
    uint8_t depthbits = pcx.bitsPerPixel * pcx.numBitPlanes;

    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepthbits != NULL) { *srcdepthbits = depthbits; }

    return true;
}

//-----------------------------------------------------------------------------
// LoadPCX
//-----------------------------------------------------------------------------
bool LoadFromMemoryPCX(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize,
    uint8_t* srcdepthbits)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < sizeof(pcx_v5_info_t))
    {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

    pcx_v5_info_t pcx = {};          // pcx header format

    pcx.identifier      = *srcbuf++;            // PCX Id Number (Always 0x0A)
    pcx.version         = *srcbuf++;            // Version Number
    pcx.encoding        = *srcbuf++;            // Encoding Format
    pcx.bitsPerPixel    = *srcbuf++;            // Bits Per Pixel
    pcx.xMin            = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Left of image
    pcx.yMin            = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Top of image
    pcx.xMax            = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Right of image
    pcx.yMax            = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Bottom of image
    pcx.horzRes         = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Horizontal Resolution
    pcx.vertRes         = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Vertical Resolution

    uint8_t* egaptr = srcbuf;           // set pointer to 16-Color EGA Palette
    srcbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)

    pcx.numBitPlanes    = *srcbuf++;          // Number of Bit Planes
    pcx.bytesPerLine    = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Bytes Per Scan-line
    pcx.paletteType     = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Palette Type
    pcx.horzScreenSize  = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Horizontal Screen Size
    pcx.vertScreenSize  = ReadU16FromLE(srcbuf);    srcbuf += 2;            // Vertical Screen Size
    srcbuf += 54;           // Reserved2 (Always 0)

    if (pcx.identifier != 0x0A || pcx.encoding != 1)
    {
        return false;
    }

    if (pcx.bitsPerPixel != 1 && pcx.bitsPerPixel != 2 &&
        pcx.bitsPerPixel != 4 && pcx.bitsPerPixel != 8)
    {
        fprintf(stderr, "PCX, unsupported bits: %d.\n", pcx.bitsPerPixel);
        return false;
    }

    if ((pcx.bitsPerPixel == 1 || pcx.bitsPerPixel == 2 ||
         pcx.bitsPerPixel == 4 || pcx.bitsPerPixel == 8) &&
         pcx.numBitPlanes == 1)         // paletted
    {
        uint32_t palnum = 0;           // number of palette colours
        uint8_t* palptr = 0;

        switch (pcx.bitsPerPixel)
        {
        case 1:         // 1-bit monochrome
        {
            palptr = egaptr;
            palnum = 2;
        } break;
        case 2:         // 2-bit, 4 color indexed
        {
            palptr = egaptr;
            palnum = 4;
        } break;
        case 4:         // 4-bit, 16 color indexed
        {
            palptr = egaptr;
            palnum = 16;
        } break;
        case 8:         // 8-bit 256 color indexed
        {
            palptr = srcend - 768;
            palnum = 256;
        } break;
        }

        if (pdstpalette != 0)
        {
            memset(&pdstpalette->data, 0, 256 * sizeof(rgba_t));

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
    if (pdstpalette != 0)
    {
        if ((pcx.version == 0 || pcx.version == 2 || pcx.version == 3 ||
             pcx.version == 4) && pdstpalette->size == 0)
        {
            fprintf(stderr, "PCX, No palette information, defaulting to ega \
                palette.\n");

            memcpy(&pdstpalette->data, &ega_palette, 16 * sizeof(rgba_t));
            pdstpalette->size = 16;
            pdstpalette->bits = 24;
        }
    }

    int32_t totalbytes = pcx.numBitPlanes * pcx.bytesPerLine;           // bytes per scanline
    int32_t ncolorplanes = pcx.numBitPlanes;            // number of color-planes
    int32_t xsize = (pcx.xMax - pcx.xMin) + 1;          // image width
    int32_t ysize = (pcx.yMax - pcx.yMin) + 1;          // image height
    int32_t bytesperscanline = pcx.bytesPerLine;            // bytes per scanline
    int32_t rlecount = 0;           // run-length count
    int32_t subtotal = 0;           // running sub-total
    int32_t colorplane = 0;         // current color plane
    int32_t pitch = xsize * ncolorplanes;           // bytes per span
    int32_t rem = 0;            // pixels remaining
    int32_t run = 0;            // run-length
    int32_t max = 0;            // number of pixels read
    int32_t bit = 0;            // bit-offset

    uint8_t* pixels = (uint8_t*)malloc((xsize * ysize * ncolorplanes));
    uint8_t* pixptr = pixels;           // start of current dst row
    uint8_t* pixbuf = pixels;           // current dst row
    memset(pixels, 0, xsize * ysize * ncolorplanes);

    *ppdst = pixels;
    if (srcxsize != 0) { *srcxsize = xsize; }
    if (srcysize != 0) { *srcysize = ysize; }
    if (srcdepthbits != 0) {
        *srcdepthbits = pcx.bitsPerPixel * pcx.numBitPlanes;
    }

    if (pcx.bitsPerPixel < 8)
    {
        if (pcx.bitsPerPixel == 1)         // 1-bit indexed
        {
            max = 8;
            bit = 7;
        }
        else if (pcx.bitsPerPixel == 2)            // 2-bit indexed
        {
            max = 4;
            bit = 6;
        }
        else if (pcx.bitsPerPixel == 4)            // 4-bit indexed
        {
            max = 2;
            bit = 4;
        }
    }
    else            // 8-bit indexed or true-color
    {
        switch (ncolorplanes)
        {
        case 3:
        {
            max = 3;
        } break;
        case 2:
        {
            max = 2;
        } break;
        case 1:
        {
            max = 1;
        } break;
        default:
        {
            fprintf(stderr, "PCX, unsupported bitplanes: %d.\n",
                pcx.numBitPlanes);
            return false;
        }
        }
    }

    int32_t y = 0;

    // for each span
    while (y < ysize)
    {
        // start the span, color-plane and subtotal and
        pixbuf = pixptr;
        colorplane = 1;
        subtotal = 0;
        rem = pitch;
        run = max;

        do
        {
            rlecount = 1;           // assume a rle of 1 unless

            // this is a rle encoding, then
            if ((0xC0 & *srcbuf) == 0xC0)
            {
                rlecount = (*srcbuf++ & 0x3F);           // get the rle count and
            }
        
            subtotal += rlecount;           // update the subtotal, then

            // write the rle pixel run and
            while (rlecount-- && rem > 0)
            {
                if (rem < run) { run = rem; }
                if (pcx.bitsPerPixel < 8)
                {
                    BlitNbitsToIndex8(pixbuf, 1, *srcbuf, run, bit);
                }
                else
                {
                    *pixbuf = *srcbuf;
                }
                pixbuf += run;
                rem -= run;
            }

            // break at the end of each color-plane. then
            if (subtotal % bytesperscanline == 0)
            {
                pixbuf = pixptr + colorplane++;          // restart the span and
                rem = pitch;
            }

            srcbuf++;         // get the next byte of data, then

        } while (subtotal < totalbytes);            // break at the end of the scanline and

        y++;

        if (y != ysize)
        {
            pixptr += pitch;           // set the next span
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
void
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
bool
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

    switch (filter)
    {
    case FILTER_POINT:
    {
        if (pdstimage->pixeltype == psrcimage->pixeltype)
        {
            if (srcbytesperpixel == 4)
            {
                Point_32bit_32bit(dstbuf, dstxextent, dstyextent, pdstimage->pixeltype,
                    srcbuf, srcxextent, srcyextent, psrcimage->pixeltype);
            }
            else if (srcbytesperpixel == 3)
            {
                Point_24bit_24bit(dstbuf, dstxextent, dstyextent, pdstimage->pixeltype,
                    srcbuf, srcxextent, srcyextent, psrcimage->pixeltype);
            }
            else if (srcbytesperpixel == 2)
            {
                Point_16bit_16bit(dstbuf, dstxextent, dstyextent, pdstimage->pixeltype,
                    srcbuf, srcxextent, srcyextent, psrcimage->pixeltype);
            }
            else
            {
                Point_8bit_8bit(dstbuf, dstxextent, dstyextent, pdstimage->pixeltype,
                    srcbuf, srcxextent, srcyextent, psrcimage->pixeltype);
            }
        }
        else
        {
            if (srcbytesperpixel == 4)
            {
                Point_32bit_Nbit(dstbuf, dstxextent, dstyextent, pdstimage->pixeltype,
                    srcbuf, srcxextent, srcyextent, psrcimage->pixeltype);
            }
            else if (srcbytesperpixel == 3)
            {
                Point_24bit_Nbit(dstbuf, dstxextent, dstyextent, pdstimage->pixeltype,
                    srcbuf, srcxextent, srcyextent, psrcimage->pixeltype);
            }
            else if (srcbytesperpixel == 2)
            {
                Point_16bit_Nbit(dstbuf, dstxextent, dstyextent, pdstimage->pixeltype,
                    srcbuf, srcxextent, srcyextent, psrcimage->pixeltype);
            }
            else
            {
                if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX)
                {
                    Point_PAL_Nbit(dstbuf, dstxextent, dstyextent, pdstimage->pixeltype,
                        srcbuf, srcxextent, srcyextent, psrcimage->pixeltype,
                        ppalette);
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

    return result;
}

//-----------------------------------------------------------------------------
// ReplaceColor
//-----------------------------------------------------------------------------
void
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
// SaveImage
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
// SaveImage
//-----------------------------------------------------------------------------
bool
SaveImage(const char* pdstfile, file_format_t dstformat, encode_t dstcodec,
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
// GetImageInfo
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
// GetImageInfo
//-----------------------------------------------------------------------------
bool
GetImageInfo(image_info_t* psrcinfo, const char* psrcfile)
{
    bool result = false;

    if (psrcfile == NULL || strlen(psrcfile) == 0)
    {
        return result;
    }

    // open file
    FILE* hFile = fopen(psrcfile, "rb");
    size_t end = 0;
    size_t pos = 0;

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
        result = GetImageInfoFromMemory(psrcinfo, rawsrc, bytesRead);
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
// LoadImage
//-----------------------------------------------------------------------------
bool
LoadImageFromMemory(image_t* pdstimage, palette_t* pdstpalette,
    rect_t* pdstrect, uint8_t* psrc, uint32_t srcsize, pixel_t dstformat,
    rect_t* psrcrect, uint32_t filter, rgba_t colorkey,
    image_info_t* psrcinfo)
{
    bool result = false;
    palette_t srcpalette;
    image_t srcimage = {
        NULL,
        0,
        0,
        PIXELTYPE_UNKNOWN
    };
    uint8_t depthbits = 0;
    file_format_t format = FILEFORMAT_NONE;

    memset(&srcpalette, 0, sizeof(palette_t));

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
            srcimage.pixeltype = (depthbits == 32) ? PIXELTYPE_BGRA :
                (depthbits == 24) ? PIXELTYPE_BGR : PIXELTYPE_COLOUR_INDEX;
            format = FILEFORMAT_BMP;
        }
        else if ((result = LoadFromMemoryPCX(&srcimage.data, &srcpalette, psrc,
            srcsize, &srcimage.xsize, &srcimage.ysize, &depthbits)) == true)
        {
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

        if (result == true &&& psrcinfo != NULL)
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
            pdstimage->pixeltype = dstformat;

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
// LoadImage
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
    size_t end = 0;
    size_t pos = 0;

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
            bytesRead, format, psrcrect, filter, colorkey, psrcinfo);
    }

    if (srcbuf != NULL)
    {
        free(srcbuf);
        srcbuf = NULL;
    }

    rawsrc = NULL;

    return result;
}

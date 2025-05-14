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

static const uint32_t s_rgba_size = 4;

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
ShrinkPNG(uint8_t* pdst, uint32_t* pdstlen, uint32_t srcxsize,
    uint32_t srcysize, uint32_t srcdepth, uint8_t interlaced, uint8_t filtertype,
    uint8_t* psrc)
{
    uint8_t* dstbuf = pdst;
    uint8_t* pixptr = psrc;
    uint32_t dstofs = 0;
    uint32_t srcbytes = (srcdepth + 7) >> 3;
    float srcpixelsperbyte = (8.0f / (float)srcdepth);
    uint32_t srcpitch = WidthInBytes(srcxsize, srcdepth);
    uint32_t srcxskip = 1;
    uint32_t srcyskip = 1;
    uint32_t srcxorig = 0;
    uint32_t srcyorig = 0;
    uint32_t xsize = srcxsize;
    uint32_t ysize = srcysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t passes = interlaced != 1 ? 1 : 7;
    uint8_t mask[4] = { 0x01, 0x03, 0, 0x0F };
    uint16_t sample = 0;
    uint16_t pae0 = 0;
    uint16_t raw0 = 0;
    uint16_t raw1 = 0;
    uint16_t pri0 = 0;
    uint16_t pri1 = 0;
    uint16_t bpp = 0;
    uint16_t pa = 0;
    uint16_t pb = 0;
    uint16_t pc = 0;
    uint8_t filter = 0;
    uint8_t bitX = 0;
    uint8_t bit0 = 0;
    uint8_t bit1 = 0;

    filtertype = (filtertype < PNG_FILTER_COUNT) ? ((srcdepth >= 8) ? filtertype : 0) : 0;

    for (uint32_t pass = 0; pass < passes; ++pass)
    {
        if (interlaced) {

            const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
            const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
            const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
            const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };

            xsize = (srcxsize - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
            ysize = (srcysize - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];
            srcxskip = i_xextent[pass];
            srcyskip = i_yextent[pass];
            srcxorig = i_xorigin[pass];
            srcyorig = i_yorigin[pass];
            dstbuf = pdst + dstofs;
        }

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
                        int32_t x0 = (((x - 0) * srcxskip) + srcxorig);
                        int32_t x1 = (((x - 1) * srcxskip) + srcxorig);
                        int32_t y0 = (((y - 0) * srcyskip) + srcyorig);
                        int32_t y1 = (((y - 1) * srcyskip) + srcyorig);
                        int32_t xx = (uint32_t)(x0 / srcpixelsperbyte);
                        int32_t yy = y0 * srcpitch;
                        bpp = 0;

                        while (bpp < srcbytes)
                        {
                            pri0 = 0;
                            pri1 = 0;
                            raw1 = 0;

                            raw0 = sample = (pixptr + yy + xx)[bpp];

                            if ((x1 >= 0) && (y1 >= 0)) {
                                pri1 = (pixptr + (y1 * srcpitch) + (x1 * srcbytes))[bpp];
                            }

                            if ((y1 >= 0)) {
                                pri0 = (pixptr + (y1 * srcpitch) + (x0 * srcbytes))[bpp];
                            }

                            if ((x1 >= 0)) {
                                raw1 = (pixptr + (y0 * srcpitch) + (x1 * srcbytes))[bpp];
                            }

                            switch (fltr)
                            {
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
                                pa = ABS(pri0 - pri1);
                                pb = ABS(raw1 - pri1);
                                pc = ABS(raw1 + pri0 - (2 * pri1));

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
                        x++;
                    }

                    if (sum[1] < sum[0]) {
                        sum[0] = sum[1];
                        filter = fltr;
                    }
                }
            }

            *dstbuf++ = filter; dstofs++;
            sample = 0;
            x = 0;

            while (x < xsize)
            {
                int32_t x0 = (((x - 0) * srcxskip) + srcxorig);
                int32_t x1 = (((x - 1) * srcxskip) + srcxorig);
                int32_t y0 = (((y - 0) * srcyskip) + srcyorig);
                int32_t y1 = (((y - 1) * srcyskip) + srcyorig);
                int32_t xx = (uint32_t)(x0 / srcpixelsperbyte);
                int32_t yy = y0 * srcpitch;
                uint8_t mod = srcpixelsperbyte - 1;
                uint8_t curX = ( x & mod);
                uint8_t cur0 = (x0 & mod);
                bpp = 0;

                while (bpp < srcbytes)
                {
                    pri0 = 0;
                    pri1 = 0;
                    raw1 = 0;

                    bitX = (mod - curX) * srcdepth;
                    bit0 = (mod - cur0) * srcdepth;

                    if (srcdepth <= 4) {
                        sample |= ((*(pixptr+yy+xx)>>bit0)&mask[srcdepth-1])<<bitX;
                    } else {
                        raw0 = sample = (pixptr + yy + xx)[bpp];
                    }

                    if (srcdepth >= 8) {
                        if ((x1 >= 0) && (y1 >= 0)) {
                            pri1 = (pixptr + (y1 * srcpitch) + (x1 * srcbytes))[bpp];
                        }

                        if ((y1 >= 0)) {
                            pri0 = (pixptr + (y1 * srcpitch) + (x0 * srcbytes))[bpp];
                        }

                        if ((x1 >= 0)) {
                            raw1 = (pixptr + (y0 * srcpitch) + (x1 * srcbytes))[bpp];
                        }

                        switch (filter)
                        {
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
                            pa = ABS(pri0 - pri1);
                            pb = ABS(raw1 - pri1);
                            pc = ABS(raw1 + pri0 - (2 * pri1));

                            if (pa <= pb && pa <= pc) {
                                pae0 = (raw0 - raw1) & 0xFF;
                            } else if (pb <= pc) {
                                pae0 = (raw0 - pri0) & 0xFF;
                            } else {
                                pae0 = (raw0 - pri1) & 0xFF;
                            }

                            sample = pae0;
                        } break;
                        }
                    }

                    if (srcdepth >= 8 || (srcdepth <= 4 && curX == mod)) {
                        *dstbuf++ = (sample & 0xFF); dstofs++; sample = 0;
                    }
                    bpp++;
                }
                x++;
            }
            y++;
        }
    }

    if (pdstlen != NULL) { *pdstlen = dstofs; }
}

//------------------------------------------------------------------------------
// SavePNG
//------------------------------------------------------------------------------
static bool
SaveToMemoryPNG(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec, uint8_t* psrc,
    uint32_t srcxsize, uint32_t srcysize, uint8_t srcdepth, uint8_t srcsampledepth,
    palette_t* psrcpalette, rgba_t* pcolorkey)
{
    if (ppdst == NULL || ppdstsize == NULL) {
        fprintf(stderr, "PNG, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL) {
        fprintf(stderr, "PNG, Invalid src data.\n");
        return false;
    }

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
            if (psrcpalette == NULL) {
                colortype = 0;          // greyscale
            } else {
                colortype = 3;          // palette
            }
        } break;
    }

    // src stuff
    uint8_t* srcptr = psrc;
    uint32_t srcpitch = WidthInBytes(srcxsize, srcdepth);
    uint32_t palettesize = 0;
    uint32_t crc = 0;

    // palette
    if (srcdepth <= 8) {
        if (psrcpalette != NULL) {
            // the PLTE chunk contains from 1 to 256 palette entries, each
            // three-bytes
            palettesize = psrcpalette->size * 3;
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
    uint8_t interlace = (codec == ENCODE_INTERLACE) ? 1 : 0;

    interlace = (interlace < 2) ? interlace : 0;

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

        for (uint32_t i = 0; i < psrcpalette->size; ++i)
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
    ShrinkPNG(idatbuf, &idatlen, srcxsize, srcysize, srcdepth, interlace, filtermode,
        srcptr);

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

    // deflate data
    uint32_t odatlen = idatlen;           // deflate size
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
    uint8_t* srcdepth, uint8_t* srcsampledepth, uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < 8) {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

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
    uint8_t  colorkey[3] = {};
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
ExpandPNG(uint8_t** ppdst, uint32_t dstxsize, uint32_t dstysize, uint32_t dstdepth,
    uint8_t interlaced, uint8_t* psrc)
{
    uint8_t* srcbuf = psrc;
    uint32_t srcofs = 0;
    uint32_t dstbytes = (dstdepth + 7) >> 3;
    float dstpixelsperbyte = (8.0f / (float)dstdepth);
    uint32_t dstpitch = WidthInBytes(dstxsize, dstdepth);
    uint32_t dstxskip = 1;
    uint32_t dstyskip = 1;
    uint32_t dstxorig = 0;
    uint32_t dstyorig = 0;
    uint32_t xsize = dstxsize;
    uint32_t ysize = dstysize;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t passes = interlaced != 1 ? 1 : 7;
    uint8_t mask[4] = { 0x01, 0x03, 0, 0x0F };
    uint8_t* pixptr = *ppdst;
    uint16_t sample = 0;
    uint16_t pae0 = 0;
    uint16_t raw0 = 0;
    uint16_t raw1 = 0;
    uint16_t pri0 = 0;
    uint16_t pri1 = 0;
    uint16_t bpp = 0;
    uint16_t pa = 0;
    uint16_t pb = 0;
    uint16_t pc = 0;
    uint8_t filter = 0;
    uint8_t bitX = 0;
    uint8_t bit0 = 0;
    uint8_t bit1 = 0;

    for (uint32_t pass = 0; pass < passes; ++pass)
    {
        if (interlaced) {

            const uint8_t i_xorigin[7] = { 0, 4, 0, 2, 0, 1, 0 };
            const uint8_t i_xextent[7] = { 8, 8, 4, 4, 2, 2, 1 };
            const uint8_t i_yorigin[7] = { 0, 0, 4, 0, 2, 0, 1 };
            const uint8_t i_yextent[7] = { 8, 8, 8, 4, 4, 2, 2 };

            xsize = (dstxsize - i_xorigin[pass] + i_xextent[pass] - 1) / i_xextent[pass];
            ysize = (dstysize - i_yorigin[pass] + i_yextent[pass] - 1) / i_yextent[pass];
            dstxskip = i_xextent[pass];
            dstyskip = i_yextent[pass];
            dstxorig = i_xorigin[pass];
            dstyorig = i_yorigin[pass];
            srcbuf = psrc + srcofs;
        }
        
        y = 0;

        while (y < ysize)
        {
            filter = *srcbuf++; srcofs++;
            x = 0;

            while (x < xsize)
            {
                int32_t x0 = (((x - 0) * dstxskip) + dstxorig);
                int32_t x1 = (((x - 1) * dstxskip) + dstxorig);
                int32_t y0 = (((y - 0) * dstyskip) + dstyorig);
                int32_t y1 = (((y - 1) * dstyskip) + dstyorig);
                int32_t xx = (uint32_t)(x0 / dstpixelsperbyte);
                int32_t yy = y0 * dstpitch;
                uint8_t mod = dstpixelsperbyte - 1;
                uint8_t curX = ( x & mod);
                uint8_t cur0 = (x0 & mod);
                uint8_t cur1 = (x1 & mod);
                bpp = 0;

                while (bpp < dstbytes)
                {
                    pri0 = 0;
                    pri1 = 0;
                    raw1 = 0;

                    if (dstdepth >= 8 || (dstdepth <= 4 && curX == 0)) {
                        raw0 = sample = *srcbuf++; srcofs++;
                    }

                    if (dstdepth <= 4) {

                        bitX = (mod - curX) * dstdepth;
                        bit0 = (mod - cur0) * dstdepth;
                        bit1 = (mod - cur1) * dstdepth;

                        if ((x1 >= 0) && (y1 >= 0)) {                     
                            pri1 = (*(pixptr + (y1 * dstpitch) +
                                (uint32_t)(x1 / dstpixelsperbyte))>>bit1)&
                                mask[dstdepth-1];
                        }
                        if ((x1 >= 0)) {
                            raw1 = (*(pixptr + (y0 * dstpitch) +
                                (uint32_t)(x1 / dstpixelsperbyte))>>bit1)&
                                mask[dstdepth-1];
                        }
                        if ((y1 >= 0)) {
                            pri0 = (*(pixptr + (y1 * dstpitch) +
                                (uint32_t)(x0 / dstpixelsperbyte))>>bit0)&
                                mask[dstdepth-1];
                        }
                    } else {
                        if ((x1 >= 0) && (y1 >= 0)) {
                            pri1 = (pixptr + (y1 * dstpitch) + (x1 * dstbytes))[bpp];
                        }
                        if ((x1 >= 0)) {
                            raw1 = (pixptr + (y0 * dstpitch) + (x1 * dstbytes))[bpp];
                        }
                        if ((y1 >= 0)) {
                            pri0 = (pixptr + (y1 * dstpitch) + (x0 * dstbytes))[bpp];
                        }
                    }

                    switch (filter)
                    {
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
                        pa = ABS(pri0 - pri1);
                        pb = ABS(raw1 - pri1);
                        pc = ABS(raw1 + pri0 - (2 * pri1));

                        if (pa <= pb && pa <= pc) {
                            pae0 = (raw0 + raw1) & 0xFF;
                        } else if (pb <= pc) {
                            pae0 = (raw0 + pri0) & 0xFF;
                        } else {
                            pae0 = (raw0 + pri1) & 0xFF;
                        }

                        sample = pae0;
                    } break;
                    }

                    if (dstdepth <= 4) {
                        (pixptr + yy + xx)[0] |= ((sample >> bitX) &
                            mask[dstdepth-1]) << bit0;
                    } else {
                        (pixptr + yy + xx)[bpp] = (sample & 0xFF);
                    }
                    bpp++;
                }
                x++;
            }
            y++;
        }
    }
}

//-----------------------------------------------------------------------------
// LoadPNG
//-----------------------------------------------------------------------------
static bool
LoadFromMemoryPNG(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth,
    uint8_t* srcsampledepth, rgba_t* pcolorkey)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < 8) {
        return false;
    }

    uint32_t srclen = psrcsize;
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + srclen;

    // check png header
    for (int i = 0; i < 8; ++i)
    {
        if (*srcbuf++ != PNG_IDENTIFIER[i]) {
            fprintf(stderr, "PNG, Bad signature.\n");
            return false;
        }
    }

    // image
    pixel_t pixeltype = PIXELTYPE_UNKNOWN;
    uint32_t xsize = 0;
    uint32_t ysize = 0;
    uint32_t depth = 0;
    uint32_t bytesperpixel = 0;
    uint32_t gamma = 0;
    uint32_t palnum = 0;
    uint32_t idatlen = 0;
    uint8_t colortype = 0;
    uint8_t compression = 0;
    uint8_t filter = 0;
    uint8_t interlace = 0;
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
                if (first == false) {
                    fprintf(stderr, "PNG, Multiple IHDR.\n");
                    return false;
                }

                first = false;

                // check chunk is valid length
                if (size != s_png_headersize) {
                    fprintf(stderr, "PNG, Bad IHDR len.\n");
                    return false;
                }

                xsize = ReadU32FromBE(srcbuf); srcbuf += 4;
                ysize = ReadU32FromBE(srcbuf); srcbuf += 4;

                // zero is an invalid value
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
                
                colortype = *srcbuf++;

                // check for invalid color-type and depth
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
                if (compression == 0) {
                    compression = ENCODE_RGB;
                } else {
                    fprintf(stderr, "PNG, Unrecognised compression code: %d.\n",
                        compression);
                    return false;
                }

                filter = *srcbuf++;

                // pre-processing method applied before compression
                if (filter != 0) {
                    fprintf(stderr, "PNG, Bad filter method: %d\n", filter);
                    return false;
                }

                interlace = *srcbuf++;
                
                // transmission order of image
                if (interlace > 1) {
                    fprintf(stderr, "PNG, Bad interlace method: %d\n", interlace);
                    return false;
                }

            } break;
            case gAMA:
            {
                if (first) {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                // the gAMA chunk must precede the first IDAT chunk.
                if (dattest) {
                    fprintf(stderr, "PNG, gAMA after IDAT.\n");
                    return false;
                }

                // the gAMA chunk must precede the first IDAT chunk.
                if (paltest) {
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
                if (dattest) {
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

                if (paltest == false) {
                    paltest = true;
                }

                // the palette chunk must appear for color type 3, and can appear for color types
                // 2 and 6; it must not appear for color types 0 and 4.
                if (pdstpalette != NULL) {

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
                if (first) {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                // the tRNS chunk must precede the first IDAT chunk.
                if (dattest) {
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

                    if (pdstpalette != NULL) {

                        for (uint32_t i = 0; i < size; ++i)
                        {
                            pdstpalette->data[i].a = *srcbuf++;
                        }

                        pdstpalette->bits = 32;
                    }
                } else {
                    // check for transparency in images with alpha
                    // grayscale + alpha or rgba
                    if (colortype == 4 || colortype == 6) {
                        fprintf(stderr, "PNG, tRNS with alpha.\n");
                        return false;
                    }

                    for (uint32_t i = 0; i < bytesperpixel; ++i)
                    {
                        colorkey[i] = (uint8_t)(ReadU16FromBE(srcbuf) & 255);
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
                if (first) {
                    fprintf(stderr, "PNG, First not IHDR.\n");
                    return false;
                }

                if (dattest == false) {
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
                if (first) {
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
                if (first) {
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

    uint32_t dstpitch = (WidthInBytes(xsize, (depth * bytesperpixel)) + 1) & ~1;

    // idat chunks
    uint8_t* idatptr = (uint8_t*)malloc(((idatlen + 1) & ~1));
    uint8_t* idatbuf = idatptr;

    if (idatptr == NULL) {
        fprintf(stderr, "PNG, Out of memory.\n");
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

    if (status != Z_OK) {

        fprintf(stderr, "PNG, Inflate: failed with status %i.\n", status);
        free(idatptr);
        idatptr = NULL;

        return false;
    }

    // inflate data, bytes per line, per component, plus filter per row
    uint32_t odatlen = ((ysize + 1) * (dstpitch + 1)) + (ysize + 1);
    uint8_t* odatptr = (uint8_t*)malloc(((odatlen + 1) & ~1));
    uint8_t* odatbuf = odatptr;

    if (odatptr == NULL) {

        fprintf(stderr, "PNG, Out of memory.\n");
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

    } while (status == Z_OK);

    inflateEnd(&inflator);

    // free compressed data
    free(idatptr);
    idatptr = NULL;

    // decompression failed
    if (status != Z_STREAM_END) {

        fprintf(stderr, "PNG, Inflate: failed with status %i.\n", status);
        free(odatptr);          // free un-compressed data and return
        odatptr = NULL;

        return false;
    } else {
        fprintf(stdout, "PNG, Inflate: completed successfully\n");
    }

    uint32_t datlen = odatlen;
    uint8_t* datptr = (uint8_t*)malloc(((datlen + 1) & ~1));

    if (datptr == NULL) {

        fprintf(stderr, "PNG, Out of memory.\n");
        free(odatptr);
        odatptr = NULL;

        return false;
    }

    memcpy(datptr, odatptr, datlen);          // filtered image data
    uint8_t* datbuf = datptr;

    free(odatptr);
    odatptr = NULL;

    // deinterlace and filter
    uint32_t pixlen = xsize * ysize * bytesperpixel;
    uint8_t* pixels = (uint8_t*)malloc(pixlen);

    if (pixels == NULL) {
        
        fprintf(stderr, "PNG, Out of memory.\n");
        free(datptr);
        datptr = NULL;

        return false;
    }

    memset(pixels, 0, xsize * ysize * bytesperpixel);

    ExpandPNG(&pixels, xsize, ysize, depth * bytesperpixel, interlace, datbuf);

    free(datptr);
    datptr = NULL;

    uint8_t* ppix = pixels;

    if (gamma != 0) {

        float fgamma = (gamma * (1.0f / 2.2f)) / 100000.0f;

        // grayscale
        if ((colortype == 0 || colortype == 4) && depth == 8) {
            for (unsigned int i = 0; i < xsize * ysize; ++i)
            {
                ppix[0] = powf((ppix[0] / 255.0f), fgamma) * 255.0f;
                ppix += bytesperpixel;
            }
        }

        // rgb
        if ((colortype == 2 || colortype == 6) && depth == 8) {
            for (unsigned int i = 0; i < xsize * ysize; ++i)
            {
                ppix[0] = powf((ppix[0] / 255.0f), fgamma) * 255.0f;
                ppix[1] = powf((ppix[1] / 255.0f), fgamma) * 255.0f;
                ppix[2] = powf((ppix[2] / 255.0f), fgamma) * 255.0f;
                ppix += bytesperpixel;
            }
        }
    }

    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) {
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
    if (srcsampledepth != NULL) { *srcsampledepth = depth; }

    return true;
}

#endif // #ifndef _PNG_H_


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
SaveToMemoryTGA(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec, uint8_t* psrc,
    uint32_t srcxsize, uint32_t srcysize, uint8_t srcdepth, palette_t* psrcpalette,
    bool invertX, bool invertY)
{
    if (ppdst == NULL || ppdstsize == NULL) {
        fprintf(stderr, "TGA, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL) {
        fprintf(stderr, "TGA, Invalid src data.\n");
        // return false;
    }

    if (srcdepth != 8 && srcdepth != 16 && srcdepth != 24 && srcdepth != 32) {
        fprintf(stderr, "TGA, Unsupported depth: %d.\n", srcdepth);
        return false;
    }

    // src stuff
    uint32_t bytesperpixel = srcdepth >> 3;
    uint32_t pitch = srcxsize * bytesperpixel;
    uint8_t* rawptr = psrc;
    uint8_t* rawbuf = psrc;
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
        if (psrcpalette != NULL) {
            colormap_type = 1;
            colormap_length = psrcpalette->size;
            colormap_size = psrcpalette->bits;
        }
    }

    if (psrc == NULL) {
        image_type = TGA_NO_IMAGE_DATA;
    } else {
        if (codec == ENCODE_RLE) {
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
                    if (psrcpalette == NULL) {
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
                    if (psrcpalette == NULL) {
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
    
    if (colormap_length != 0 && psrcpalette != NULL) {
        // fill in the palette
        if (colormap_size == 15 || colormap_size == 16) {
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
        } else if (colormap_size == 24) {
            for (uint16_t i = colormap_index; i < colormap_length; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].b;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].r;
            }
        } else if (colormap_size == 32) {            
            for (uint16_t i = colormap_index; i < colormap_length; ++i)
            {
                *dstbuf++ = psrcpalette->data[i].b;
                *dstbuf++ = psrcpalette->data[i].g;
                *dstbuf++ = psrcpalette->data[i].r;
                *dstbuf++ = psrcpalette->data[i].a;
            }
        }
    }

    if (psrc != NULL) {

        uint32_t x = 0;
        uint32_t y = 0;

        if (codec == ENCODE_RLE) {          // run-length encoding

            uint32_t abscount = 0;
            uint32_t rlecount = 0;
            uint32_t rlevalue = 0;
            uint32_t sample0 = 0;
            uint32_t sample1 = 0;

            switch (image_type)
            {
            case TGA_RGB_RLE:
            case TGA_MAPPED_RLE:
            case TGA_BLACK_AND_WHITE_RLE:
            {
                while (y < srcysize)
                {
                    rawbuf = rawptr;
                    x = 0;

                    while (x < srcxsize)
                    {
                        abscount = 1;
                        sample0 = 0;
                        sample1 = 0;

                        memcpy(&sample0, (rawbuf + (x * bytesperpixel)), bytesperpixel);

                        sample1 = sample0;

                        while ((x + abscount) < srcxsize && abscount < 0xFF)
                        {
                            sample0 = sample1;
                            sample1 = 0;

                            memcpy(&sample1, (rawbuf + ((x + abscount) * bytesperpixel)),
                                bytesperpixel);

                            if (sample0 == sample1) {
                                break;
                            }

                            abscount++;
                        }

                        rlecount = 1;
                        sample0 = 0;
                        sample1 = 0;

                        memcpy(&sample0, (rawbuf + (x * bytesperpixel)), bytesperpixel);

                        sample1 = sample0;

                        while ((x + rlecount) < srcxsize && rlecount < 0x80)
                        {
                            sample0 = sample1;
                            sample1 = 0;

                            memcpy(&sample1, (rawbuf + ((x + rlecount) * bytesperpixel)),
                                bytesperpixel);

                            if (sample0 != sample1) {
                                break;
                            }

                            rlecount++;
                        }

                        if (abscount >= rlecount) {

                            rlevalue = abscount;

                            *dstbuf++ = (0 << 7) | (rlevalue - 1);
                            bytesencoded++;

                            memcpy(&dstbuf, (rawbuf + (x * bytesperpixel) * rlevalue), bytesperpixel);
                            dstbuf += bytesperpixel * rlevalue;
                            bytesencoded += bytesperpixel * rlevalue;
                        } else {
                            rlevalue = rlecount;

                            *dstbuf++ = (1 << 7) | (rlevalue - 1);
                            bytesencoded++;

                            memcpy(dstbuf, &sample0, bytesperpixel);
                            dstbuf += bytesperpixel;
                            bytesencoded += bytesperpixel;
                        }
                        x += rlevalue;
                    }
                    rawptr += pitch;
                    y++;
                }
            } break;
            }
        } else {            // everything else
            memcpy(dstbuf, rawbuf, srcysize* pitch);
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
    uint8_t* srcdepth, uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < s_tga_file_size) {
        return false;
    }

    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;

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
        } else if (colormap_size !=  8 &&
                 colormap_size != 15 &&
                 colormap_size != 16 &&
                 colormap_size != 24 &&
                 colormap_size != 32) {
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
LoadFromMemoryTGA(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < s_tga_file_size) {
        fprintf(stderr, "TGA, Not a valid bitmap.\n");
        return false;
    }

    // src stuff
    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;

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

    if (pixel_size !=  8 && pixel_size != 16 && pixel_size != 24 &&
        pixel_size != 32) {
        fprintf(stderr, "TGA, Unsupported bits: %d.\n", pixel_size);
        return false;
    }

    if (colormap_type == 1) {
        if (image_type != TGA_MAPPED &&
            image_type != TGA_MAPPED_RLE) {
            fprintf(stderr, "TGA, Colour map type and image type mismatch.\n");
            return false;
        } else if (colormap_size !=  8 &&
                 colormap_size != 15 &&
                 colormap_size != 16 &&
                 colormap_size != 24 &&
                 colormap_size != 32) {
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
            uint8_t* palptr = srcbuf + id_length;
            uint32_t palnum = colormap_length < 256 ?
                colormap_length : 256;
            
            if (pdstpalette != NULL) {
                if (colormap_size == 15 || colormap_size == 16) {

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
                } else if (colormap_size == 24) {
                    for (size_t i = 0; i < palnum; ++i)
                    {
                        pdstpalette->data[i].b = *palptr++;
                        pdstpalette->data[i].g = *palptr++;
                        pdstpalette->data[i].r = *palptr++;
                        pdstpalette->data[i].a = 255;
                    }
                    
                    pdstpalette->size = palnum;
                    pdstpalette->bits = 24;
                } else if (colormap_size == 32) {
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
    srcbuf = srcbuf + id_length + colormap_length *
        ((colormap_size==15 ? 16 : colormap_size) >> 3);

    // dst stuff
    uint32_t xsize = width;
    uint32_t ysize = height;
    uint32_t bytesperpixel = pixel_size >> 3;
    uint32_t pitch = xsize * bytesperpixel;
    uint32_t rlevalue = 0;
    uint32_t rlecount = 0;
    uint8_t rgba[4] = {};

    uint8_t* pixels = (uint8_t*)malloc(xsize * ysize * bytesperpixel);
    uint8_t* rawptr = pixels;           // start of current dst row
    uint8_t* rawbuf = pixels;           // current dst row

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
                rawbuf = rawptr;
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

                    memcpy(rawbuf, rgba, bytesperpixel);
                    rawbuf += bytesperpixel;
                    rlecount--;
                    x++;
                }
                rawptr += pitch;
                y++;
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
#define BI_BITFIELDS    3L
#endif // #ifdef _WIN32

#define BMP_IDENTIFIER          0x4D42

static const uint32_t s_bmp_file_size = 14;
static const uint32_t s_bmp_v3_info_size = 40;

//------------------------------------------------------------------------------
// SaveBMP
//------------------------------------------------------------------------------
static bool
SaveToMemoryBMP(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec, uint8_t* psrc,
    uint32_t srcxsize, uint32_t srcysize, uint32_t srcdepth, palette_t* psrcpalette,
    rgba_t* pcolorkey, bool invertY)
{
    if (ppdst == NULL || ppdstsize == NULL) {
        fprintf(stderr, "BMP, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL) {
        fprintf(stderr, "BMP, Invalid src data.\n");
        //return false;
    }

    if (srcdepth !=  1 && srcdepth !=  4 && srcdepth != 8 && srcdepth != 24 &&
        srcdepth != 32) {
        fprintf(stderr, "BMP, Unsupported depth: %d.\n", srcdepth);
        return false;
    }

    uint32_t xextent = srcxsize;
    uint32_t yextent = srcysize;
    uint8_t dstdepth = srcdepth;

    // dst stuff
    uint32_t dstpitch = (WidthInBytes(xextent, dstdepth) + 3) & ~3;           // 4-byte boundary
    uint32_t dstpalettesize = 0;

    // palette
    if (srcdepth <= 8) {
        if (psrcpalette != NULL) {
            dstpalettesize = psrcpalette->size * 4;
        }
    }

    // src stuff
    uint32_t srcpitch = WidthInBytes(xextent, srcdepth);
    uint8_t* rawsrc = psrc;
    uint8_t* rawptr = psrc;
    uint8_t* rawbuf = psrc;
    uint32_t padbytes = dstpitch - srcpitch;

    // byte encoded array
    int32_t datasize = s_bmp_file_size + s_bmp_v3_info_size + ((yextent * dstpitch)) +
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
    int32_t  width = xextent;
    int32_t  height = (invertY == true) ? yextent : -(int32_t)(yextent);          // bottom-up dib
    uint16_t planes = 1;
    uint16_t bits = dstdepth;
    uint32_t compression = BI_RGB;

    if (codec == ENCODE_RLE) {
        if (srcdepth == 8) {
            compression = BI_RLE8;
        }
        else if (srcdepth == 4) {
            compression = BI_RLE4;
        }
    }

    uint32_t imagesize = xextent * yextent * (dstdepth < 8 ? 1 : (dstdepth >> 3));
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
    WriteU32ToLE(dstbuf, imagesize);    dstbuf += 4;            // image size
    WriteI32ToLE(dstbuf, xresolution);  dstbuf += 4;            // xresolution
    WriteI32ToLE(dstbuf, yresolution);  dstbuf += 4;            // yresolution
    WriteU32ToLE(dstbuf, num_colours);  dstbuf += 4;            // num_colours
    WriteU32ToLE(dstbuf, num_colour_indexes); dstbuf += 4;      // num_colour_indexes

    // fill in the palette
    if (srcdepth <= 8) {
        if (psrcpalette != NULL) {
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

    if (psrc != NULL) {
        if (compression != BI_RGB) {            // run-length encoding
            int32_t colorkey = -1;

            if (pcolorkey != NULL && psrcpalette != NULL) {
                for (uint32_t i = 0; i < dstpalettesize >> 2; ++i)
                {
                    if (psrcpalette->data[i].r == pcolorkey->r &&
                        psrcpalette->data[i].g == pcolorkey->g &&
                        psrcpalette->data[i].b == pcolorkey->b &&
                        psrcpalette->data[i].a == pcolorkey->a) {
                        colorkey = i;
                        break;
                    }
                }
            }

            uint32_t abscount = 0;
            uint32_t rlecount = 0;
            uint32_t rlevalue = 0;
            uint8_t sample0 = 0;
            uint8_t sample1 = 0;

            switch (dstdepth)
            {
            case 4:         // 4-bit encoding
            {
                uint32_t runcount = 0;
                x = 0;
                y = 0;
                uint8_t* pixels = (uint8_t*)malloc(srcxsize * srcysize);
                uint8_t* pixbuf = pixels;
                uint8_t* srcbuf = rawbuf;
                uint8_t sample = 0;

                memset(pixels, 0, srcxsize * srcysize);

                while (y < srcysize)
                {
                    x = 0;
                    while (x < srcxsize)
                    {
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

                srcpitch = xextent;
                x = 0;
                y = 0;
                rawsrc = pixels;
                rawbuf = pixels;
                rawptr = pixels;
            };
            case 8:         // 8-bit encoding
            {
                uint8_t sample = 0;

                while (y < yextent)
                {
                    rawbuf = rawptr;
                    x = 0;

                    if (*rawbuf == colorkey) {

                        uint32_t count = 0;

                        while (count < xextent * (yextent - y))
                        {
                            if (*(rawbuf + count) != colorkey) {
                                break;
                            }

                            count++;
                        }

                        if (count >= xextent) {

                            uint32_t dx = 0;
                            uint32_t dy = 0;

                            while (count >= xextent)
                            {
                                count -= xextent;
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

                            if (y < yextent) {
                                rawptr = psrc + (y * srcpitch);
                                rawbuf = rawptr + x;
                            }
                        }
                    }

                    while (x < xextent)
                    {
                        abscount = 1;
                        sample0 = *(rawbuf + x);
                        sample1 = sample0;

                        while ((x + abscount) < xextent && abscount < 0xFF)
                        {
                            sample0 = sample1;
                            sample1 = *(rawbuf + x + abscount);

                            if (sample0 == sample1) {
                                break;
                            }

                            abscount++;
                        }

                        rlecount = 1;
                        sample0 = *(rawbuf + x);
                        sample1 = sample0;

                        while ((x + rlecount) < xextent && rlecount < 0xFF)
                        {
                            sample0 = sample1;
                            sample1 = *(rawbuf + x + rlecount);

                            if (sample0 != sample1) {
                                break;
                            }

                            rlecount++;
                        }

                        if (abscount >= 3 && abscount >= rlecount) {
                            for (uint32_t i = 0; i < abscount; i++)
                            {
                                if (*(rawbuf + x + i) == colorkey) {
                                    abscount = i;
                                    break;
                                }
                            }
                        }

                        if (abscount >= 3 && abscount >= rlecount) {

                            rlevalue = abscount;

                            *dstbuf++ = 0x00;
                            *dstbuf++ = rlevalue;
                            bytesencoded++;
                            bytesencoded++;

                            uint32_t count = rlevalue;

                            if (dstdepth == 4) {
                                count = (rlevalue >> 1);
                            }

                            for (uint32_t i = 0; i < count; ++i)
                            {
                                sample = 0;

                                switch (dstdepth)
                                {
                                case 4:
                                {
                                    sample |= *(rawbuf + x + i + 0) << 4;
                                    sample |= *(rawbuf + x + i + 1);
                                } break;
                                case 8:
                                {
                                    sample = *(rawbuf + x + i);
                                } break;
                                }

                                *dstbuf++ = sample;
                                bytesencoded++;
                            }

                            // pad-byte
                            if ((rlevalue & 1)) {
                                *dstbuf++ = 0x00;
                                bytesencoded++;
                            }
                        } else {
                            rlevalue = rlecount;

                            sample = 0;

                            if (sample0 == colorkey) {
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
                                    int d1 = sample0 & 0xF;
                                    int d0 = sample0 << 4;

                                    sample = d0 + d1;
                                } break;
                                case 8:
                                {
                                    sample = sample0;
                                } break;
                                }
                            }

                            *dstbuf++ = rlevalue;
                            *dstbuf++ = sample;
                            bytesencoded++;
                            bytesencoded++;
                        }

                        x += rlevalue;
                    }

                    *dstbuf++ = 0x00;
                    *dstbuf++ = 0x00;
                    bytesencoded++;
                    bytesencoded++;

                    rawptr += srcpitch;
                    y++;
                }

                // end of bitmap 
                *dstbuf++ = 0x00;
                *dstbuf++ = 0x01;
                bytesencoded++;
                bytesencoded++;

                if (dstdepth == 4)
                {
                    free(rawsrc);
                    rawsrc = NULL;
                }
            } break;
            }
        } else {            // everything else
            while (y < yextent)
            {
                rawbuf = rawptr;
                memcpy(dstbuf, rawbuf, srcpitch);
                rawptr += srcpitch;
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
GetInfoFromMemoryBMP(uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth,
    uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < s_bmp_file_size) {
        return false;
    }

    uint8_t* srcbuf = psrc;

    // file struct
    uint16_t type        = ReadU16FromLE(srcbuf); srcbuf += 2;
    srcbuf += 4; // file size
    uint16_t reserved1   = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t reserved2   = ReadU16FromLE(srcbuf); srcbuf += 2;
    srcbuf += 4; // offset

    // info struct
    srcbuf += 4; // info size
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
LoadFromMemoryBMP(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < s_bmp_file_size) {
        fprintf(stderr, "BMP, Not a valid bitmap.\n");
        return false;
    }

    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;

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

        if (pdstpalette != 0) {

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
            if (alpha == 0) {
                for (size_t i = 0; i < pdstpalette->size; ++i)
                {
                    pdstpalette->data[i].a = 255;
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
SaveToMemoryPCX(uint8_t** ppdst, uint32_t* ppdstsize, encode_t codec, uint8_t* psrc,
    uint32_t srcxsize, uint32_t srcysize, uint8_t srcdepth, palette_t* psrcpalette)
{
    if (ppdst == NULL || ppdstsize == NULL) {
        fprintf(stderr, "PCX, Invalid dst data.\n");
        return false;
    }

    if (psrc == NULL) {
        fprintf(stderr, "PCX, Invalid src data.\n");
        return false;
    }

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
    uint8_t dstdepth = (srcdepth == 24) ? 8 : srcdepth;
    uint32_t dstpitch = (WidthInBytes(srcxsize, dstdepth) + 1) & ~1;           // must be an even number

    // src stuff
    uint32_t srcbytesperpixel = ((srcdepth == 24) ? 3 : 1);
    uint32_t srcpitch = WidthInBytes(srcxsize, srcdepth);

    if (srcdepth < 8) {
        xsize = srcpitch;
    }

    uint8_t* rawptr = psrc;
    uint8_t* rawbuf = psrc;

    uint32_t dstpadbytes = dstpitch - (srcpitch / srcbytesperpixel);
    uint32_t dstpalettesize = 0;

    // palette
    if (srcdepth == 8 && psrcpalette != NULL) {
        dstpalettesize = 768;
    } else {
        fprintf(stderr, "PCX, Missing palette.\n");
        return false;
    }

    // byte encoded array
    uint32_t datasize = s_pcx_v5_info_size + (ysize * (srcbytesperpixel * dstpitch)) +
        dstpalettesize;
    uint8_t* data = (uint8_t*)malloc(datasize);

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

    *dstbuf++ = srcbytesperpixel;            // Number of Bit Planes
    WriteU16ToLE(dstbuf, dstpitch);     dstbuf += 2;            // Bytes per scan-line (must be an even number)    
    WriteU16ToLE(dstbuf, palettetype);  dstbuf += 2;            // Palette Type
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Horizontal Screen Size
    WriteU16ToLE(dstbuf, 0);            dstbuf += 2;            // Vertical Screen Size
    dstbuf += 54;           // Reserved2 (Always 0)

    uint32_t bytesencoded = 0;
    uint32_t colorplane = 0;
    uint32_t rlecount = 0;
    uint32_t rlevalue = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint8_t sample0 = 0;
    uint8_t sample1 = 0;

    while (y < ysize)
    {
        colorplane = 0;

        do
        {
            rawbuf = rawptr;
            x = 0;

            while (x < xsize)
            {
                rlecount = 1;
                sample0 = *(rawbuf + ((x * srcbytesperpixel) + colorplane));
                sample1 = sample0;

                while ((x + rlecount) < xsize && rlecount < 0x3F)
                {
                    sample0 = sample1;
                    sample1 = *(rawbuf + (((x + rlecount) * srcbytesperpixel) +
                        colorplane));

                    if (sample0 != sample1) {
                        break;
                    }

                    rlecount++;
                }

                rlevalue = rlecount;

                if (rlecount == 1) {
                    if ((0xC0 != (0xC0 & sample0))) {
                        *dstbuf++ = sample0;
                        bytesencoded++;
                    } else {
                        *dstbuf++ = rlevalue | 0xC0;
                        *dstbuf++ = sample0;
                        bytesencoded++;
                        bytesencoded++;
                    }
                } else {
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

        } while (colorplane < srcbytesperpixel);

        rawptr += srcpitch;
        y++;
    }

    // palette
    if (srcbytesperpixel == 1) {

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
            palptr = (psrcpalette == NULL) ? ega_palette : psrcpalette->data;
        } break;
        case 8:
        {
            *dstbuf++ = 0x0C;           // 256-Color Palette Code
            bytesencoded++;
            palnum = 256;
            palptr = psrcpalette->data;
        } break;
        }

        if (psrcpalette != NULL) {
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
GetInfoFromMemoryPCX(uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth,
    uint8_t* psrc, uint32_t psrcsize)
{
    if (psrc == NULL || psrcsize < s_pcx_v5_info_size) {
        return false;
    }

    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;

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
LoadFromMemoryPCX(uint8_t** ppdst, palette_t* pdstpalette, uint8_t* psrc,
    uint32_t psrcsize, uint32_t* srcxsize, uint32_t* srcysize, uint8_t* srcdepth)
{
    if (ppdst == NULL || psrc == NULL || psrcsize < s_pcx_v5_info_size) {
        fprintf(stderr, "PCX, Not a valid bitmap.\n");
        return false;
    }

    uint8_t* srcptr = psrc;
    uint8_t* srcbuf = psrc;
    uint8_t* srcend = psrc + psrcsize;

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

    uint8_t* egaptr = srcbuf;           // set pointer to 16-Color EGA Palette
    srcbuf += 49;           // 16-Color EGA Palette + Reserved1 (Always 0)

    uint8_t  numBitPlanes    = *srcbuf++;
    uint16_t bytesPerLine    = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t paletteType     = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t horzScreenSize  = ReadU16FromLE(srcbuf); srcbuf += 2;
    uint16_t vertScreenSize  = ReadU16FromLE(srcbuf); srcbuf += 2;
    srcbuf += 54;           // Reserved2 (Always 0)

    if (identifier != 0x0A) {
        fprintf(stderr, "PCX, Invalid manufacturer code: %d.\n", identifier);
        return false;
    }

    if (encoding != 1) {
        fprintf(stderr, "PCX, Invalid encoding value: %d.\n", encoding);
        return false;
    }

    if (version != 5) {
        fprintf(stderr, "PCX, Version mismatch: %d.\n", version);
        return false;
    }

    if (bitsPerPixel != 1 && bitsPerPixel != 2 && bitsPerPixel != 4 &&
        bitsPerPixel != 8) {
        fprintf(stderr, "PCX, Unsupported depth: %d.\n", bitsPerPixel);
        return false;
    }

    uint32_t ncolorplanes = numBitPlanes;
    uint32_t xsize = (xMax - xMin) + 1;
    uint32_t ysize = (yMax - yMin) + 1;
    uint32_t bytesperscanline = bytesPerLine;
    uint32_t pitch = WidthInBytes(xsize, bitsPerPixel);
    uint32_t padbytes = bytesperscanline - pitch;
    uint32_t totalbytes = numBitPlanes * pitch;
    uint32_t rlecount = 0;
    uint32_t subtotal = 0;
    uint32_t colorplane = 0;

    uint8_t* pixels = (uint8_t*)malloc(ysize * pitch);
    uint8_t* pixptr = pixels;
    uint8_t* pixbuf = pixels;

    if (pixels == NULL) {
        fprintf(stderr, "PCX: Out of memory.\n");
        return false;
    }

    memset(pixels, 0, ysize * pitch);

    *ppdst = pixels;
    if (srcxsize != NULL) { *srcxsize = xsize; }
    if (srcysize != NULL) { *srcysize = ysize; }
    if (srcdepth != NULL) { *srcdepth = bitsPerPixel * numBitPlanes; }

    uint32_t y = 0;

    while (y < ysize)
    {
        pixbuf = pixptr;
        colorplane = 1;
        subtotal = 0;

        do
        {
            rlecount = 1;

            if ((0xC0 & *srcbuf) == 0xC0) {
                rlecount = (*srcbuf++ & 0x3F);
            }
        
            subtotal += rlecount;

            while (rlecount--)
            {
                *pixbuf = *srcbuf;
                pixbuf += ncolorplanes;
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
    if (numBitPlanes == 1) {

        uint32_t palnum = 0;
        uint8_t* palptr = NULL;

        switch (bitsPerPixel)
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

        if (pdstpalette != NULL) {
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

    return true;
}

#endif // #ifndef _PCX_H

//-----------------------------------------------------------------------------
// Resampling
//-----------------------------------------------------------------------------

typedef struct _filter_info
{
    float pixelN;         // number
    float pixelW;         // weight
} filter_info_t;

typedef struct _filter_list
{
    filter_info_t*  info;          // info of weights
    uint32_t        size;          // size of info
} filter_list_t;

static const float BOX_SUPPORT = 0.5f;

static float 
BoxFilter(float t)
{
    if ((t > -0.5f) && (t <= 0.5f)) return (1.0f);
    return 0.0f;
}

static const float TRIANGLE_SUPPORT = 1.0f;

static float
TriangleFilter(float t)
{
    if (t < 0.0f) t = -t;
    if (t < 1.0f) return (1.0f - t);
    return 0.0f;
}

static const float BELL_SUPPORT = 1.5f;

static float
BellFilter(float t)
{
    if (t < 0.0f) t = -t;
    if (t < 0.5f) return (0.75f - (t * t));
    if (t < 1.5f) {
        t = (t - 1.5f);
        return (0.5f * (t * t));
    }
    return 0.0f;
}

static const float B_SPLINE_SUPPORT = 2.0f;

static float
BSplineFilter(float t)
{
    float tt;

    if (t < 0.0f) t = -t;
    if (t < 1.0f) {
        tt = t * t;
        return ((0.5f * tt * t) - tt + (2.0f / 3.0f));
    } else if (t < 2.0f) {
        t = 2.0f - t;
        return ((1.0f / 6.0f) * (t * t * t));
    }
    return 0.0f;
}

static const float CUBIC_SUPPORT = 1.0f;

static float
CubicFilter(float t)
{
    if (t < 0.0f) t = -t;
    if (t < 1.0f) return ((2.0f * t - 3.0f) * t * t + 1.0f);
    return 0.0f;
}

static float
Sinc(float x)
{
    x *= G_PI;
    if (x != 0.0f) return (sinf(x) / x);
    return 1.0f;
}

static const float LANCZOS3_SUPPORT = 3.0f;

static float
Lanczos3Filter(float t)
{
    if (t < 0.0f) t = -t;
    if (t < 3.0) return(Sinc(t) * Sinc(t / 3.0f));
    return 0.0f;
}

static const float MITCHELL_SUPPORT = 2.0f;

#define	B	(1.0f / 3.0f)
#define	C	(1.0f / 3.0f)

static float
MitchellFilter(float t)
{
    float tt = t * t;
    if (t < 0.0f) t = -t;
    if (t < 1.0f) {
        t = (((12.0f - 9.0f * B - 6.0f * C) * (t * tt))
            + ((-18.0f + 12.0f * B + 6.0f * C) * tt)
            + (6.0f - 2.0f * B));
        return (t / 6.0f);
    } else if (t < 2.0f) {
        t = (((-1.0f * B - 6.0f * C) * (t * tt))
            + ((6.0f * B + 30.0f * C) * tt)
            + ((-12.0f * B - 48.0f * C) * t)
            + (8.0f * B + 24.0f * C));
        return (t / 6.0f);
    }
    return 0.0f;
}

//-----------------------------------------------------------------------------
// Filter
//-----------------------------------------------------------------------------
static bool
Filter(uint8_t** pdst, uint32_t dstxsize, uint32_t dstysize, uint32_t dstbytes,
    uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize, uint32_t srcbytes,
    uint32_t filtertype)
{
    if (psrc == NULL) {
        return false;
    }

    // dst stuff
    if (*pdst == NULL) {
        *pdst = (uint8_t*)malloc(dstxsize * dstysize * dstbytes);
        memset(*pdst, 0, dstxsize * dstysize * dstbytes);
    }

    uint32_t dstpitch = dstxsize * dstbytes;
    uint8_t* dstbuf = pdst[0];

    // src stuff
    uint32_t srcpitch = srcxsize * srcbytes;
    uint8_t* srcbuf = psrc;

    bool result = true;

    // do filtering and scaling
    typedef float (*filter_f)(float);
    filter_f filter_func = NULL;
    float filterwidth = 0.0f;

    switch (filtertype)
    {
    case FILTER_BOX:
    {
        filter_func = BoxFilter;
        filterwidth = BOX_SUPPORT;
    } break;
    case FILTER_TRIANGLE:
    {
        filter_func = TriangleFilter;
        filterwidth = TRIANGLE_SUPPORT;
    } break;
    case FILTER_BELL:
    {
        filter_func = BellFilter;
        filterwidth = BELL_SUPPORT;
    } break;
    case FILTER_BSPLINE:
    {
        filter_func = BSplineFilter;
        filterwidth = B_SPLINE_SUPPORT;
    } break;
    case FILTER_CUBIC:
    {
        filter_func = CubicFilter;
        filterwidth = CUBIC_SUPPORT;
    } break;
    case FILTER_LANCZOS:
    {
        filter_func = Lanczos3Filter;
        filterwidth = LANCZOS3_SUPPORT;
    } break;
    case FILTER_MITCHELL:
    {
        filter_func = MitchellFilter;
        filterwidth = MITCHELL_SUPPORT;
    } break;
    }

    uint8_t* tmpptr = NULL;
    uint8_t* tmpbuf = NULL;
    uint8_t* raster = NULL;         // a row or column of pixels
    float xscale, yscale;           // scale factors
    float pixelN, pixelW;           // pixel number and weight
    float fwidth, fscale;           // filter calculation variables
    float mid, min, max;

    // create intermediate image
    tmpptr = (uint8_t*)malloc(srcysize * dstpitch);
    tmpbuf = tmpptr;

    xscale = (float)dstxsize / (float)srcxsize;
    yscale = (float)dstysize / (float)srcysize;

    // pre-calculate filter contributions for a row
    filter_list_t* filter = (filter_list_t*)malloc((dstxsize + 4) *
        sizeof(filter_list_t));

    if (filter == NULL) {
        
        fprintf(stderr, "Filter: Out of memory\n");
        free(tmpptr);
        
        return false;
    }

    memset(filter, 0, (dstxsize + dstbytes) * sizeof(filter_list_t));

    fwidth = filterwidth;
    fscale = 1.0f;

    if (xscale < 1.0f) {
        fwidth /= xscale;
        fscale /= xscale;
    }

    for (int i = 0; i < (int)dstxsize; ++i)
    {
        filter[i].size = 0;
        filter[i].info = (filter_info_t*)malloc((int)((fwidth * 2) + 1) *
            sizeof(filter_info_t));

        if (filter[i].info == NULL) {

            for (int j = 0; j < (int)dstxsize - i; ++j)
            {
                free(filter[j].info);
            }

            fprintf(stderr, "Filter: Out of memory\n");
            
            free(filter);
            free(tmpptr);

            return false;
        }

        memset(filter[i].info, 0, (int)((fwidth * 2) + 1) * sizeof(filter_info_t));

        mid = (float)i / xscale;
        min =  ceilf(mid - fwidth);
        max = floorf(mid + fwidth);

        for (int j = (int)min; j <= (int)max; ++j)
        {
            pixelN = 0;
            pixelW = mid - (float)j;
            pixelW = filter_func(pixelW / fscale) / fscale;

            if (j < 0) {
                pixelN = -j;
            } else if (j >= (int)srcxsize) {
                pixelN = (srcxsize - j) + (srcxsize - 1);
            } else {
                pixelN = j;
            }

            if (filter[i].size < ((fwidth * 2) + 1)) {

                filter[i].info[filter[i].size].pixelN = pixelN;
                filter[i].info[filter[i].size].pixelW = pixelW;
                filter[i].size++;
            }
        }
    }

    // apply filter to horizontal axis from src to tmp
    raster = (uint8_t*)malloc((srcxsize + srcbytes) * srcbytes);

    if (raster == NULL) {
        
        fprintf(stderr, "Filter: Out of memory\n");

        for (int i = 0; i < (int)dstxsize; ++i)
        {
            free(filter[i].info);
        }

        free(filter);
        free(tmpptr);

        return false;
    }

    memset(raster, 0, (srcxsize + srcbytes) * srcbytes);

    for (int k = 0; k < (int)srcysize; ++k)
    {
        // get pixel row
        memset(raster, 0, (srcxsize + srcbytes) * srcbytes);
        int n = (k - ((k / srcysize) * srcysize));

        memcpy(raster, (srcbuf + (n * srcpitch)), srcpitch);

        for (int i = 0; i < (int)dstxsize; ++i)
        {
            for (size_t bpp = 0; bpp < srcbytes; bpp++)
            {
                pixelW = 0.0f;

                for (uint32_t j = 0; j < filter[i].size; ++j)
                {
                    pixelW += raster[((int)filter[i].info[j].pixelN *
                        srcbytes) + bpp] * filter[i].info[j].pixelW;
                }

                tmpptr[(k * dstpitch) + (i * dstbytes) + bpp] =
                    CLAMP(pixelW, 0, 255);
            }
        }
    }

    // free the memory allocated for horizontal filter weights
    for (int i = 0; i < (int)dstxsize; ++i)
    {
        free(filter[i].info);
    }

    free(filter);
    free(raster);

    // pre-calculate filter contributions for a column
    filter = (filter_list_t*)malloc((dstysize + dstbytes) * sizeof(filter_list_t));

    if (filter == NULL) {

        fprintf(stderr, "Filter: Out of memory\n");
        free(tmpptr);

        return false;
    }

    memset(filter, 0, (dstysize + dstbytes) * sizeof(filter_list_t));

    fwidth = filterwidth;
    fscale = 1.0f;

    if (yscale < 1.0f) {
        fwidth /= yscale;
        fscale /= yscale;
    }

    for (int i = 0; i < (int)dstysize; ++i)
    {
        filter[i].size = 0;
        filter[i].info = (filter_info_t*)malloc((int)((fwidth * 2) + 1) *
            sizeof(filter_info_t));

        if (filter[i].info == NULL) {
            
            fprintf(stderr, "Filter: Out of memory\n");

            for (int j = 0; j < (int)dstysize - i; ++j)
            {
                free(filter[j].info);
            }

            free(filter);
            free(tmpptr);

            return false;
        }

        memset(filter[i].info, 0, (int)((fwidth * 2) + 1) *
            sizeof(filter_info_t));

        mid = (float)i / yscale;
        min =  ceilf(mid - fwidth);
        max = floorf(mid + fwidth);

        for (int j = (int)min; j <= (int)max; ++j)
        {
            pixelN = 0;
            pixelW = mid - (float)j;
            pixelW = filter_func(pixelW / fscale) / fscale;

            if (j < 0) {
                pixelN = -j;
            } else if (j >= (int)srcysize) {
                pixelN = (srcysize - j) + (srcysize - 1);
            } else {
                pixelN = j;
            }

            if (filter[i].size < ((fwidth * 2) + 1)) {

                filter[i].info[filter[i].size].pixelN = pixelN;
                filter[i].info[filter[i].size].pixelW = pixelW;
                filter[i].size++;
            }
        }
    }

    // apply filter to vertical axis from tmp to dst
    raster = (uint8_t*)malloc((srcysize + dstbytes) * dstbytes);

    if (raster == NULL) {

        fprintf(stderr, "Filter: Out of memory\n");

        for (int i = 0; i < (int)dstysize; ++i)
        {
            free(filter[i].info);
        }

        free(filter);
        free(tmpptr);

        return false;
    }

    memset(raster, 0, (srcysize + dstbytes) * dstbytes);

    for (int k = 0; k < (int)dstxsize; ++k)
    {
        uint8_t* pbuf = tmpbuf + (k * dstbytes);
        uint8_t* rbuf = raster;

        for (int y = srcysize; y > 0; y--)
        {
            memcpy(rbuf, pbuf, dstbytes);
            rbuf += dstbytes;
            pbuf += dstpitch;
        }

        for (int i = 0; i < (int)dstysize; ++i)
        {
            for (size_t bpp = 0; bpp < dstbytes; bpp++)
            {
                pixelW = 0.0f;

                for (uint32_t j = 0; j < filter[i].size; ++j)
                {
                    pixelW += raster[((int)filter[i].info[j].pixelN *
                        dstbytes) + bpp] * filter[i].info[j].pixelW;
                }

                dstbuf[(i * dstpitch) + (k * dstbytes) + bpp] =
                    CLAMP(pixelW, 0, 255);
            }
        }
    }

    // free the memory allocated for vertical filter weights
    for (int i = 0; i < (int)dstysize; ++i)
    {
        free(filter[i].info);
    }

    free(filter);
    free(raster);
    free(tmpptr);

    return result;
}

static uint8_t
GetBytesForPixelFormat(pixel_t format)
{
    uint8_t bytes = 1;

    switch (format)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    { bytes++; }
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    { bytes++; }
    case PIXELTYPE_XBGR1555:
    case PIXELTYPE_LUMINANCE_ALPHA:
    { bytes++; }
    }

    return bytes;
}

void
GetMaskForPixelFormat(pixel_t format, uint32_t& rmask, uint32_t& gmask,
    uint32_t& bmask, uint32_t& amask)
{
    uint32_t pmask[4] = { 0 };

    switch (format)
    {
    case PIXELTYPE_RGBA:
    case PIXELTYPE_ABGR:
    case PIXELTYPE_BGRA:
    case PIXELTYPE_RGB:
    case PIXELTYPE_BGR:
    {
        pmask[0] = 0x000000FF;
        pmask[1] = 0x0000FF00;
        pmask[2] = 0x00FF0000;
        pmask[3] = 0xFF000000;
    } break;
    case PIXELTYPE_XBGR1555:
    {
        pmask[0] = 0x8000;
        pmask[1] = 0x7C00;
        pmask[2] = 0x03E0;
        pmask[3] = 0x001F;
    } break;
    case PIXELTYPE_LUMINANCE_ALPHA:
    case PIXELTYPE_LUMINANCE:
    {
        pmask[0] = 0x00FF;
        pmask[1] = 0x00FF;
        pmask[2] = 0x00FF;
        pmask[3] = 0xFF00;
    } break;
    }

    switch (format)
    {
    case PIXELTYPE_RGBA:
    {
        rmask = pmask[0];
        gmask = pmask[1];
        bmask = pmask[2];
        amask = pmask[3];
    } break;
    case PIXELTYPE_ABGR:
    {
        amask = pmask[0];
        bmask = pmask[1];
        gmask = pmask[2];
        rmask = pmask[3];
    } break;
    case PIXELTYPE_BGRA:
    {
        bmask = pmask[0];
        gmask = pmask[1];
        rmask = pmask[2];
        amask = pmask[3];
    } break;
    case PIXELTYPE_RGB:
    {
        rmask = pmask[0];
        gmask = pmask[1];
        bmask = pmask[2];
        amask = pmask[3];
    } break;
    case PIXELTYPE_BGR:
    {
        bmask = pmask[0];
        gmask = pmask[1];
        rmask = pmask[2];
        amask = pmask[3];
    } break;
    case PIXELTYPE_XBGR1555:
    {
        amask = pmask[0];
        bmask = pmask[1];
        gmask = pmask[2];
        rmask = pmask[3];
    } break;
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        rmask = pmask[0];
        gmask = pmask[1];
        bmask = pmask[2];
        amask = pmask[3];
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        rmask = pmask[0];
        gmask = pmask[1];
        bmask = pmask[2];
        amask = pmask[3];
    } break;
    }
}

//-----------------------------------------------------------------------------
// Blit_Nbit_Nbit
//-----------------------------------------------------------------------------
static void
Blit_Nbit_Nbit(uint8_t* pdst, uint32_t dstxsize, uint32_t dstysize,
    pixel_t dstformat, uint8_t* psrc, uint32_t srcxsize, uint32_t srcysize,
    pixel_t srcformat, palette_t* ppalette)
{
    uint8_t* bufdst = pdst;
    uint8_t* bufsrc = psrc;
    uint32_t dstpitch = dstxsize;
    uint32_t srcpitch = srcxsize;
    uint32_t xsize = (srcxsize < dstxsize) ? srcxsize : dstxsize;
    uint32_t ysize = (srcysize < dstysize) ? srcysize : dstysize;
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
    uint8_t a = 255;
    uint8_t v = 0;
    uint8_t dstbytes = GetBytesForPixelFormat(dstformat);
    uint8_t srcbytes = GetBytesForPixelFormat(srcformat);

    dstpitch = dstxsize * dstbytes;
    srcpitch = srcxsize * srcbytes;

    GetMaskForPixelFormat(srcformat, rmask, gmask, bmask, amask);

    for (size_t mask = rmask; (mask & 0x01) != 1; mask >>= 1) rshift++;
    for (size_t mask = gmask; (mask & 0x01) != 1; mask >>= 1) gshift++;
    for (size_t mask = bmask; (mask & 0x01) != 1; mask >>= 1) bshift++;
    for (size_t mask = amask; (mask & 0x01) != 1; mask >>= 1) ashift++;

    if (srcformat == PIXELTYPE_LUMINANCE || srcbytes == 2 || srcbytes == 3 ||
        srcbytes == 4) {
        
        float fmod = 1.0f;
        float amod = 1.0f;

        if (srcbytes == 3 || srcbytes == 4) {
            if (dstformat == PIXELTYPE_XBGR1555) {
                fmod = 31.0f / 255.0f;
                amod = 1.0f;
            }
        }

        if (srcbytes == 2) {
            fmod = 255.0f / 31.0f;
            amod = 255.0f;
            if (srcformat == PIXELTYPE_LUMINANCE_ALPHA) {
                fmod = 1.0f;
                amod = 1.0f;
                if (dstformat == PIXELTYPE_XBGR1555) {
                    amod = 1.0f / 255.0f;
                }
            }
        }

        while (y++ < ysize)
        {
            x = 0;
            while (x < xsize)
            {
                uint32_t pixel = UINT_MAX;

                memcpy(&pixel, (bufsrc + (x * srcbytes)), srcbytes);

                switch (dstformat)
                {
                case PIXELTYPE_RGBA:
                {
                    bufdst[x * 4 + 0] = ((pixel & rmask) >> rshift) * fmod;
                    bufdst[x * 4 + 1] = ((pixel & gmask) >> gshift) * fmod;
                    bufdst[x * 4 + 2] = ((pixel & bmask) >> bshift) * fmod;
                    bufdst[x * 4 + 3] = ((pixel & amask) >> ashift) * amod;
                } break;
                case PIXELTYPE_ABGR:
                {
                    bufdst[x * 4 + 0] = ((pixel & amask) >> ashift) * amod;
                    bufdst[x * 4 + 1] = ((pixel & bmask) >> bshift) * fmod;
                    bufdst[x * 4 + 2] = ((pixel & gmask) >> gshift) * fmod;
                    bufdst[x * 4 + 3] = ((pixel & rmask) >> rshift) * fmod;
                } break;
                case PIXELTYPE_BGRA:
                {
                    bufdst[x * 4 + 0] = ((pixel & bmask) >> bshift) * fmod;
                    bufdst[x * 4 + 1] = ((pixel & gmask) >> gshift) * fmod;
                    bufdst[x * 4 + 2] = ((pixel & rmask) >> rshift) * fmod;
                    bufdst[x * 4 + 3] = ((pixel & amask) >> ashift) * amod;
                } break;
                case PIXELTYPE_RGB:
                {
                    bufdst[x * 3 + 0] = ((pixel & rmask) >> rshift) * fmod;
                    bufdst[x * 3 + 1] = ((pixel & gmask) >> gshift) * fmod;
                    bufdst[x * 3 + 2] = ((pixel & bmask) >> bshift) * fmod;
                } break;
                case PIXELTYPE_BGR:
                {
                    bufdst[x * 3 + 0] = ((pixel & bmask) >> bshift) * fmod;
                    bufdst[x * 3 + 1] = ((pixel & gmask) >> gshift) * fmod;
                    bufdst[x * 3 + 2] = ((pixel & rmask) >> rshift) * fmod;
                } break;
                case PIXELTYPE_XBGR1555:
                {
                    r = ((pixel & rmask) >> rshift) * fmod;
                    g = ((pixel & gmask) >> gshift) * fmod;
                    b = ((pixel & bmask) >> bshift) * fmod;
                    a = ((pixel & amask) >> ashift) * amod;

                    rgb16 = (((a & 0xFF) << 15) | (r & 0xFF) << 10) |
                        ((g & 0xFF) << 5) | (b & 0xFF);

                    bufdst[x * 2 + 0] = (rgb16 & 0x00FF) >> 0;
                    bufdst[x * 2 + 1] = (rgb16 & 0xFF00) >> 8;
                } break;
                case PIXELTYPE_LUMINANCE_ALPHA:
                {
                    bufdst[x * 2 + 0] = (uint8_t)(
                        (((pixel & rmask) >> rshift) * fmod) * 0.2990f +
                        (((pixel & gmask) >> gshift) * fmod) * 0.5870f +
                        (((pixel & bmask) >> bshift) * fmod) * 0.1140f);
                    bufdst[x * 2 + 1] = (pixel & amask) * amod;
                } break;
                case PIXELTYPE_LUMINANCE:
                {
                    bufdst[x] = (uint8_t)(
                        (((pixel & rmask) >> rshift) * fmod) * 0.2990f +
                        (((pixel & gmask) >> gshift) * fmod) * 0.5870f +
                        (((pixel & bmask) >> bshift) * fmod) * 0.1140f);
                } break;
                }
                x++;
            }
            bufdst += dstpitch;
            bufsrc += srcpitch;
        }
    } else {
        float fmod = 31.0f / 255.0f;
        float amod = 1.0f;

        while (y++ < ysize)
        {
            x = 0;
            while (x < xsize)
            {
                v = bufsrc[x * 1 + 0];
                b = ppalette->data[v].b;
                g = ppalette->data[v].g;
                r = ppalette->data[v].r;
                a = ppalette->data[v].a;

                switch (dstformat)
                {
                case PIXELTYPE_RGBA:
                {
                    bufdst[x * 4 + 0] = r;
                    bufdst[x * 4 + 1] = g;
                    bufdst[x * 4 + 2] = b;
                    bufdst[x * 4 + 3] = a;
                } break;
                case PIXELTYPE_ABGR:
                {
                    bufdst[x * 4 + 0] = a;
                    bufdst[x * 4 + 1] = b;
                    bufdst[x * 4 + 2] = g;
                    bufdst[x * 4 + 3] = r;
                } break;
                case PIXELTYPE_BGRA:
                {
                    bufdst[x * 4 + 0] = b;
                    bufdst[x * 4 + 1] = g;
                    bufdst[x * 4 + 2] = r;
                    bufdst[x * 4 + 3] = a;
                } break;
                case PIXELTYPE_RGB:
                {
                    bufdst[x * 3 + 0] = r;
                    bufdst[x * 3 + 1] = g;
                    bufdst[x * 3 + 2] = b;
                } break;
                case PIXELTYPE_BGR:
                {
                    bufdst[x * 3 + 0] = b;
                    bufdst[x * 3 + 1] = g;
                    bufdst[x * 3 + 2] = r;
                } break;
                case PIXELTYPE_XBGR1555:
                {
                    r = r * fmod;
                    g = g * fmod;
                    b = b * fmod;
                    a = a * amod;

                    rgb16 = ((a & 0xFF) << 15) | ((r & 0xFF) << 10) |
                        ((g & 0xFF) << 5) | (b & 0xFF);

                    bufdst[x * 2 + 0] = (rgb16 & 0x00FF) >> 0;
                    bufdst[x * 2 + 1] = (rgb16 & 0xFF00) >> 8;
                } break;
                case PIXELTYPE_LUMINANCE_ALPHA:
                {
                    bufdst[x * 2 + 0] = (uint8_t)(r * 0.2990f + g * 0.5870f +
                        b * 0.1140f);
                    bufdst[x * 2 + 1] = a;
                } break;
                case PIXELTYPE_LUMINANCE:
                {
                    bufdst[x] = (uint8_t)(r * 0.2990f + g * 0.5870f + b * 0.1140f);
                } break;
                }
                x++;
            }
            bufdst += dstpitch;
            bufsrc += srcpitch;
        }
    }
}

//-----------------------------------------------------------------------------
// FastFill
//-----------------------------------------------------------------------------
static void
FastFill(image_t* pimage, palette_t* ppalette, rgba_t dst, rgba_t src)
{
    uint8_t* bufdst = NULL;
    uint8_t* pixels = NULL;
    uint32_t bytes = 1;
    uint32_t pitch = 0;
    uint32_t dc = 0;
    uint32_t sc = 0;
    uint32_t pc = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    if (pimage == NULL || pimage->data == NULL) {
        return;
    }

    bytes = GetBytesForPixelFormat(pimage->pixeltype);
    pitch = pimage->xsize * bytes;

    switch (pimage->pixeltype)
    {
    case PIXELTYPE_RGBA:
    {
        dc = (dst.a << 24) + (dst.b << 16) + (dst.g << 8) + dst.r;
        sc = (src.a << 24) + (src.b << 16) + (src.g << 8) + src.r;
    } break;
    case PIXELTYPE_ABGR:
    {
        dc = (dst.r << 24) + (dst.g << 16) + (dst.b << 8) + dst.a;
        sc = (src.r << 24) + (src.g << 16) + (src.b << 8) + src.a;
    } break;
    case PIXELTYPE_BGRA:
    {
        dc = (dst.a << 24) + (dst.r << 16) + (dst.g << 8) + dst.b;
        sc = (src.a << 24) + (src.r << 16) + (src.g << 8) + src.b;
    } break;
    case PIXELTYPE_RGB:
    {
        dc = (    0 << 24) + (dst.b << 16) + (dst.g << 8) + dst.r;
        sc = (    0 << 24) + (src.b << 16) + (src.g << 8) + src.r;
    } break;
    case PIXELTYPE_BGR:
    {
        dc = (    0 << 24) + (dst.r << 16) + (dst.g << 8) + dst.b;
        sc = (    0 << 24) + (src.r << 16) + (src.g << 8) + src.b;
    } break;
    case PIXELTYPE_XBGR1555:
    {
        float fmod = 31.0f / 255.0f;
        float amod = 1.0f;
        uint8_t r0 = dst.r * fmod;
        uint8_t g0 = dst.g * fmod;
        uint8_t b0 = dst.b * fmod;
        uint8_t a0 = dst.a * amod;
        uint8_t r1 = src.r * fmod;
        uint8_t g1 = src.g * fmod;
        uint8_t b1 = src.b * fmod;
        uint8_t a1 = src.a * amod;

        dc = ((a0 & 0xFF) << 15) | ((r0 & 0xFF) << 10) | ((g0 & 0xFF) << 5) | (b0 & 0xFF);
        sc = ((a1 & 0xFF) << 15) | ((r1 & 0xFF) << 10) | ((g1 & 0xFF) << 5) | (b1 & 0xFF);
    } break;
    case PIXELTYPE_LUMINANCE_ALPHA:
    {
        dc = (dst.a << 8) + (uint8_t)(dst.r * 0.2990f + dst.g * 0.5870f +
            dst.b * 0.1140f);
        sc = (dst.a << 8) + (uint8_t)(src.r * 0.2990f + src.g * 0.5870f +
            src.b * 0.1140f);
    } break;
    case PIXELTYPE_LUMINANCE:
    {
        dc = (    0 << 8) + (uint8_t)(dst.r * 0.2990f + dst.g * 0.5870f +
            dst.b * 0.1140f);
        sc = (    0 << 8) + (uint8_t)(src.r * 0.2990f + src.g * 0.5870f +
            src.b * 0.1140f);
    } break;
    case PIXELTYPE_COLOUR_INDEX:
    {
        int32_t dstindex = -1;
        int32_t srcindex = -1;

        if (ppalette == NULL) {
            return;
        }

        for (uint32_t i = 0; i < ppalette->size; ++i)
        {
            if (ppalette->data[i].r == dst.r &&
                ppalette->data[i].g == dst.g &&
                ppalette->data[i].b == dst.b &&
                ppalette->data[i].a == dst.a) {
                dstindex = i;
                break;
            }
        }

        for (uint32_t i = 0; i < ppalette->size; ++i)
        {
            if (ppalette->data[i].r == src.r &&
                ppalette->data[i].g == src.g &&
                ppalette->data[i].b == src.b &&
                ppalette->data[i].a == src.a) {
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

    bufdst = pimage->data;

    while (y < pimage->ysize)
    {
        x = (pimage->xsize + 3) / 4;
        switch (pimage->xsize % 4)
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
SaveImageToMemory(uint8_t** ppdst, uint32_t* ppdstsize, file_format_t format,
    encode_t dstcodec, image_t* psrcimage, palette_t* psrcpalette, rect_t* psrcrect)
{
    image_t dstimage = {
        NULL,
        0,
        0,
        PIXELTYPE_UNKNOWN
    };
    image_t* pdstimage = &dstimage;
    uint32_t depth = 8;
    pixel_t dstformat = PIXELTYPE_UNKNOWN;
    bool result = false;

    if (psrcimage != NULL) {
        // supported output formats
        switch (format)
        {
            case FILEFORMAT_PNG:
            {
                if (psrcimage->pixeltype == PIXELTYPE_RGBA
                    || psrcimage->pixeltype == PIXELTYPE_ABGR
                    || psrcimage->pixeltype == PIXELTYPE_BGRA) {
                    dstformat = PIXELTYPE_RGBA;
                } else if (psrcimage->pixeltype == PIXELTYPE_RGB
                    || psrcimage->pixeltype == PIXELTYPE_BGR) {
                    dstformat = PIXELTYPE_RGB;
                } else if (psrcimage->pixeltype == PIXELTYPE_XBGR1555) {
                    dstformat = PIXELTYPE_XBGR1555;
                } else if (psrcimage->pixeltype == PIXELTYPE_LUMINANCE_ALPHA) {
                    dstformat = PIXELTYPE_LUMINANCE_ALPHA;
                } else if (psrcimage->pixeltype == PIXELTYPE_LUMINANCE) {
                    dstformat = PIXELTYPE_LUMINANCE;
                } else if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX) {
                    dstformat = PIXELTYPE_COLOUR_INDEX;
                } else {
                    fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        convert src data to dst format.\n");
                    return false;
                }
            } break;
            case FILEFORMAT_BMP:
            {
                if (psrcimage->pixeltype == PIXELTYPE_RGBA
                    || psrcimage->pixeltype == PIXELTYPE_ABGR
                    || psrcimage->pixeltype == PIXELTYPE_BGRA) {
                    dstformat = PIXELTYPE_BGRA;
                } else if (psrcimage->pixeltype == PIXELTYPE_RGB
                    || psrcimage->pixeltype == PIXELTYPE_BGR
                    || psrcimage->pixeltype == PIXELTYPE_XBGR1555
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE_ALPHA
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE) {
                    dstformat = PIXELTYPE_BGR;
                } else if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX) {
                    dstformat = PIXELTYPE_COLOUR_INDEX;
                } else {
                    fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        convert src data to dst format.\n");
                    return false;
                }
            } break;
            case FILEFORMAT_PCX:
            {
                if (psrcimage->pixeltype == PIXELTYPE_RGBA
                    || psrcimage->pixeltype == PIXELTYPE_RGB
                    || psrcimage->pixeltype == PIXELTYPE_XBGR1555
                    || psrcimage->pixeltype == PIXELTYPE_ABGR
                    || psrcimage->pixeltype == PIXELTYPE_BGRA
                    || psrcimage->pixeltype == PIXELTYPE_BGR
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE_ALPHA
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE) {
                    dstformat = PIXELTYPE_RGB;
                } else if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX) {
                    dstformat = PIXELTYPE_COLOUR_INDEX;
                } else {
                    fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        convert src data to dst format.\n");
                    return false;
                }
            } break;
            case FILEFORMAT_TGA:
            {
                if (psrcimage->pixeltype == PIXELTYPE_RGBA
                    || psrcimage->pixeltype == PIXELTYPE_ABGR
                    || psrcimage->pixeltype == PIXELTYPE_BGRA) {
                    dstformat = PIXELTYPE_BGRA;
                } else if (psrcimage->pixeltype == PIXELTYPE_RGB
                    || psrcimage->pixeltype == PIXELTYPE_BGR) {
                    dstformat = PIXELTYPE_BGR;
                } else if (psrcimage->pixeltype == PIXELTYPE_XBGR1555) {
                    dstformat = PIXELTYPE_XBGR1555;
                } else if (psrcimage->pixeltype == PIXELTYPE_LUMINANCE_ALPHA
                    || psrcimage->pixeltype == PIXELTYPE_LUMINANCE) {
                    dstformat = PIXELTYPE_LUMINANCE;
                } else if (psrcimage->pixeltype == PIXELTYPE_COLOUR_INDEX) {
                    dstformat = PIXELTYPE_COLOUR_INDEX;
                } else {
                    fprintf(stderr, "SaveImage, Invalid src format, cannot \
                        convert src data to dst format.\n");
                    return false;
                }
            } break;
            default:
            {
                fprintf(stderr, "SaveImage, Unsupported image format.\n");
                return false;
            }
        }

        depth *= GetBytesForPixelFormat(dstformat);

        // src stuff
        rect_t srcrect = {};

        if (psrcrect == NULL) {
            srcrect.min.x = 0;
            srcrect.min.y = 0;
            srcrect.max.x = psrcimage->xsize;
            srcrect.max.y = psrcimage->ysize;
        } else {
            srcrect = *psrcrect;
        }

        uint8_t srcbytes = GetBytesForPixelFormat(psrcimage->pixeltype);
        uint32_t srcxorigin = MAX(0, MIN((uint32_t)ABS(srcrect.min.x),
            psrcimage->xsize));
        uint32_t srcyorigin = MAX(0, MIN((uint32_t)ABS(srcrect.min.y),
            psrcimage->ysize));
        uint32_t srcxextent = MIN((uint32_t)ABS(srcrect.max.x), psrcimage->xsize);
        uint32_t srcyextent = MIN((uint32_t)ABS(srcrect.max.y), psrcimage->ysize);
        uint32_t srcpitch = srcxextent * srcbytes;
        uint32_t srcxsize = srcxextent - srcxorigin;
        uint32_t srcysize = srcyextent - srcyorigin;

        // dst stuff
        uint32_t dstpitch = srcxsize * srcbytes;

        dstimage.xsize = srcxsize;
        dstimage.ysize = srcysize;
        dstimage.pixeltype = dstformat;
        dstimage.data = (uint8_t*)malloc(srcysize * srcpitch);

        if (dstimage.data == NULL) {
            result = false;
            fprintf(stderr, "SaveImage: Out of memory.\n");
        } else {

            memset(dstimage.data, 0, srcysize * dstpitch);

            // copy to destination
            uint8_t* bufdst = pdstimage->data;
            uint8_t* bufsrc = psrcimage->data + (srcyorigin * srcpitch) +
                (srcxorigin * srcbytes);
            uint32_t xsize = srcxsize;
            uint32_t ysize = srcysize;
            uint32_t pitch = srcxsize * srcbytes;
            uint32_t y = 0;

            while (y++ < ysize)
            {
                memcpy(bufdst, bufsrc, pitch);
                bufdst += dstpitch;
                bufsrc += srcpitch;
            }

            // convert to dstformat
            if (dstformat != psrcimage->pixeltype) {

                uint8_t* tmpptr = (uint8_t*)malloc(srcysize * dstpitch);

                Blit_Nbit_Nbit(tmpptr, srcxsize, srcysize, dstformat, pdstimage->data,
                    srcxsize, srcysize, psrcimage->pixeltype, psrcpalette);

                free(pdstimage->data);
                pdstimage->data = tmpptr;
            }

            result = true;
        }

        if (dstimage.pixeltype != PIXELTYPE_COLOUR_INDEX) {
            psrcpalette = NULL;
        }

        if (result == true) {

            switch (format)
            {
            case FILEFORMAT_PNG:
            {
                result = SaveToMemoryPNG(ppdst, ppdstsize, dstcodec, dstimage.data,
                    dstimage.xsize, dstimage.ysize, depth, 8, psrcpalette, NULL);
            } break;
            case FILEFORMAT_BMP:
            {
                result = SaveToMemoryBMP(ppdst, ppdstsize, dstcodec, dstimage.data,
                    dstimage.xsize, dstimage.ysize, depth, psrcpalette, NULL, false);
            } break;
            case FILEFORMAT_PCX:
            {
                result = SaveToMemoryPCX(ppdst, ppdstsize, dstcodec, dstimage.data,
                    dstimage.xsize, dstimage.ysize, depth, psrcpalette);
            } break;
            case FILEFORMAT_TGA:
            {
                result = SaveToMemoryTGA(ppdst, ppdstsize, dstcodec, dstimage.data,
                    dstimage.xsize, dstimage.ysize, depth, psrcpalette, false,
                    false);
            } break;
            }
        }

        if (dstimage.data != NULL) {
            free(dstimage.data);
            dstimage.data = NULL;
        }
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

    if (pdstfile == NULL || strlen(pdstfile) == 0) {
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
            psrcimage, psrcpalette, psrcrect);
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
GetImageInfoFromMemory(image_info_t* psrcinfo, uint8_t* psrc, uint32_t psrcsize)
{
    bool result = false;
    pixel_t pixeltype = PIXELTYPE_UNKNOWN;
    uint32_t xsize = 0;
    uint32_t ysize = 0;
    uint8_t depth = 0;
    uint8_t colormap = 0;
    file_format_t format = FILEFORMAT_NONE;

    if (psrcinfo != NULL) { psrcinfo->fileformat = FILEFORMAT_NONE; }
    if ((result = GetInfoFromMemoryPNG(&colormap, &xsize, &ysize, &depth, NULL,
        psrc, psrcsize)) == true) {
        if (depth == 32) { pixeltype = PIXELTYPE_RGBA; }
        else if (depth == 24) { pixeltype = PIXELTYPE_RGB; }
        else if (depth == 16) { pixeltype = PIXELTYPE_LUMINANCE_ALPHA; }
        else if (depth <=  8 && colormap == 0) { pixeltype = PIXELTYPE_LUMINANCE; }
        else if (depth <=  8 && colormap == 1) { pixeltype = PIXELTYPE_COLOUR_INDEX; }
        format = FILEFORMAT_PNG;
    } else if ((result = GetInfoFromMemoryBMP(&xsize, &ysize, &depth, psrc,
        psrcsize)) == true) {
        pixeltype = (depth == 32) ? PIXELTYPE_BGRA :
            (depth == 24) ? PIXELTYPE_BGR : PIXELTYPE_COLOUR_INDEX;
        format = FILEFORMAT_BMP;
    } else if ((result = GetInfoFromMemoryPCX(&xsize, &ysize, &depth, psrc,
        psrcsize)) == true) {
        pixeltype = (depth <= 8) ? PIXELTYPE_COLOUR_INDEX : PIXELTYPE_RGB;
        format = FILEFORMAT_PCX;
    } else if ((result = GetInfoFromMemoryTGA(&colormap, &xsize, &ysize,
        &depth, psrc, psrcsize)) == true) {
        if (depth == 32) { pixeltype = PIXELTYPE_BGRA; }
        else if (depth == 24) { pixeltype = PIXELTYPE_BGR; }
        else if (depth == 16) { pixeltype = PIXELTYPE_XBGR1555; }
        else if (depth ==  8 && colormap == 0) { pixeltype = PIXELTYPE_LUMINANCE; }
        else if (depth ==  8 && colormap == 1) { pixeltype = PIXELTYPE_COLOUR_INDEX; }
        format = FILEFORMAT_TGA;
    } else { fprintf(stderr, "GetImageInfo, Unsupported image format\n"); }

    if (psrcinfo != NULL) {
        psrcinfo->pixeltype = pixeltype;
        psrcinfo->xsize = xsize;
        psrcinfo->ysize = ysize;
        psrcinfo->depth = depth;
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
LoadImageFromMemory(image_t* pdstimage, palette_t* pdstpalette, rect_t* pdstrect,
    uint8_t* psrc, uint32_t srcsize, pixel_t dstformat, rect_t* psrcrect,
    uint32_t filtertype, rgba_t colorkey, image_info_t* psrcinfo)
{
    bool result = false;
    palette_t srcpalette = {};
    image_t srcimage = {
        NULL,
        0,
        0,
        PIXELTYPE_UNKNOWN
    };
    image_t* psrcimage = &srcimage;
    uint8_t depth = 0;
    file_format_t format = FILEFORMAT_NONE;

    if (pdstimage != NULL) {

        rgba_t pngcolorkey = { 0, 0, 0, 0 };
        if ((result = LoadFromMemoryPNG(&srcimage.data, &srcpalette, psrc,
            srcsize, &srcimage.xsize, &srcimage.ysize, &depth, NULL,
            &pngcolorkey)) == true) {
            if (depth == 32) { srcimage.pixeltype = PIXELTYPE_RGBA; }
            else if (depth == 24) { srcimage.pixeltype = PIXELTYPE_RGB; }
            else if (depth == 16) { srcimage.pixeltype = PIXELTYPE_LUMINANCE_ALPHA; }
            else if (depth <= 8 && srcpalette.size == 0) { srcimage.pixeltype = PIXELTYPE_LUMINANCE; }
            else if (depth <= 8 && srcpalette.size != 0) { srcimage.pixeltype = PIXELTYPE_COLOUR_INDEX; }
            if (pngcolorkey.b != 0 && pngcolorkey.g != 0 && pngcolorkey.r != 0 && pngcolorkey.a != 0) {
                colorkey = pngcolorkey;
            }
            format = FILEFORMAT_PNG;
        } else if ((result = LoadFromMemoryBMP(&srcimage.data, &srcpalette, psrc,
            srcsize, &srcimage.xsize, &srcimage.ysize, &depth)) == true) {
            srcimage.pixeltype = (depth == 32) ? PIXELTYPE_BGRA :
                (depth == 24) ? PIXELTYPE_BGR : PIXELTYPE_COLOUR_INDEX;
            format = FILEFORMAT_BMP;
        } else if ((result = LoadFromMemoryPCX(&srcimage.data, &srcpalette, psrc,
            srcsize, &srcimage.xsize, &srcimage.ysize, &depth)) == true) {
            srcimage.pixeltype = (depth <= 8) ? PIXELTYPE_COLOUR_INDEX : PIXELTYPE_RGB;
            format = FILEFORMAT_PCX;
        } else if ((result = LoadFromMemoryTGA(&srcimage.data, &srcpalette, psrc,
            srcsize, &srcimage.xsize, &srcimage.ysize, &depth)) == true) {
            if (depth == 32) { srcimage.pixeltype = PIXELTYPE_BGRA; }
            else if (depth == 24) { srcimage.pixeltype = PIXELTYPE_BGR; }
            else if (depth == 16) { srcimage.pixeltype = PIXELTYPE_XBGR1555; }
            else if (depth == 8 && srcpalette.size == 0) { srcimage.pixeltype = PIXELTYPE_LUMINANCE; }
            else if (depth == 8 && srcpalette.size != 0) { srcimage.pixeltype = PIXELTYPE_COLOUR_INDEX; }
            format = FILEFORMAT_TGA;
        } else { fprintf(stderr, "LoadImage, Unsupported image format\n"); }

        if (result == true && psrcinfo != NULL) {
            psrcinfo->xsize = srcimage.xsize;
            psrcinfo->ysize = srcimage.ysize;
            psrcinfo->pixeltype = srcimage.pixeltype;
            psrcinfo->depth = depth;
            psrcinfo->fileformat = format;
        }

        // expand packed type
        if ((format == FILEFORMAT_PNG || format == FILEFORMAT_BMP ||
             format == FILEFORMAT_PCX) && depth <= 4 && result == true) {

            uint32_t x = 0;
            uint32_t y = 0;
            uint8_t* pixels = (uint8_t*)malloc(srcimage.xsize * srcimage.ysize);
            uint8_t* pixbuf = pixels;
            uint8_t* srcbuf = srcimage.data;
            uint8_t pixelsperbyte =  8 / depth;
            uint8_t maskbits[4] = { 0x01, 0x03, 0, 0x0F };
            uint8_t scalebit[4] = { 0xFF, 0x55, 0, 0x11 };
            uint8_t mask = maskbits[depth-1];
            uint8_t resetbit = 8 - depth;
            uint8_t bit = resetbit;

            memset(pixels, 0, srcimage.xsize * srcimage.ysize);

            while (y < srcimage.ysize)
            {
                x = (srcimage.xsize / pixelsperbyte);

                do
                {
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

                switch (srcimage.xsize & (pixelsperbyte - 1))
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

            pixbuf = pixels;

            if (srcimage.pixeltype == PIXELTYPE_LUMINANCE) {
                for (unsigned int i = 0; i < srcimage.xsize * srcimage.ysize; ++i)
                {
                    *pixbuf++ *= scalebit[depth-1];
                }
            }

            free(srcimage.data);
            srcimage.data = pixels;
            depth = 8;
        }

        // dst stuff
        rect_t dstrect = {};

        if (pdstrect == NULL) {
            dstrect.min.x = 0;
            dstrect.min.y = 0;
            dstrect.max.x = psrcimage->xsize;
            dstrect.max.y = psrcimage->ysize;
        } else {
            dstrect = *pdstrect;
        }

        dstformat = (dstformat == PIXELTYPE_UNKNOWN) ? PIXELTYPE_RGBA : dstformat;
        uint8_t dstbytes = GetBytesForPixelFormat(dstformat);
        uint32_t dstxorigin = MAX(0, ABS(dstrect.min.x));
        uint32_t dstyorigin = MAX(0, ABS(dstrect.min.y));
        uint32_t dstxextent = ABS(dstrect.max.x);
        uint32_t dstyextent = ABS(dstrect.max.y);
        uint32_t dstxsize = dstxextent - dstxorigin;
        uint32_t dstysize = dstyextent - dstyorigin;
        uint32_t dstpitch = dstxextent * dstbytes;

        if (pdstimage->data == NULL) {
            pdstimage->data = (uint8_t*)malloc(dstyextent * dstpitch);
            memset(pdstimage->data, 0, dstyextent * dstpitch);
        }

        pdstimage->xsize = dstxextent;
        pdstimage->ysize = dstyextent;
        pdstimage->pixeltype = dstformat;

        // src stuff
        rect_t srcrect = {};

        if (psrcrect == NULL) {
            srcrect.min.x = 0;
            srcrect.min.y = 0;
            srcrect.max.x = psrcimage->xsize;
            srcrect.max.y = psrcimage->ysize;
        } else {
            srcrect = *psrcrect;
        }

        uint8_t srcbytes = GetBytesForPixelFormat(srcimage.pixeltype);
        uint32_t srcxorigin = MAX(0, MIN((uint32_t)ABS(srcrect.min.x),
            psrcimage->xsize));
        uint32_t srcyorigin = MAX(0, MIN((uint32_t)ABS(srcrect.min.y),
            psrcimage->ysize));
        uint32_t srcxextent = psrcimage->xsize;
        uint32_t srcyextent = psrcimage->ysize;
        uint32_t srcxsize = srcxextent - srcxorigin;
        uint32_t srcysize = srcyextent - srcyorigin;
        uint32_t srcpitch = srcxextent * srcbytes;

        // convert to dstformat
        if (pdstimage->pixeltype != psrcimage->pixeltype) {

            uint8_t* tmpptr = (uint8_t*)malloc(srcyextent * srcxextent * dstbytes);

            Blit_Nbit_Nbit(tmpptr, srcxextent, srcyextent, dstformat, psrcimage->data,
                srcxextent, srcyextent, psrcimage->pixeltype, &srcpalette);

            srcpitch = srcxextent * dstbytes;
            
            free(psrcimage->data);
            psrcimage->data = tmpptr;
        }

        // filter and scale
        if (filtertype != FILTER_NONE && dstformat != PIXELTYPE_COLOUR_INDEX) {

            uint8_t* tmpptr = NULL;

            result = Filter(&tmpptr, dstxextent, dstyextent, dstbytes, psrcimage->data,
                srcxextent, srcyextent, dstbytes, filtertype);

            srcxsize = dstxextent - srcxorigin;
            srcysize = dstyextent - srcyorigin;
            srcpitch = dstxextent * dstbytes;

            free(psrcimage->data);
            psrcimage->data = tmpptr;
        }

        // copy to destination
        if (result == true) {

            uint8_t* bufdst = pdstimage->data + (dstyorigin * dstpitch) +
                (dstxorigin * dstbytes);
            uint8_t* bufsrc = psrcimage->data + (srcyorigin * srcpitch) +
                (srcxorigin * dstbytes);
            uint32_t xsize = MIN(srcxsize, dstxsize);
            uint32_t ysize = MIN(srcysize, dstysize);
            uint32_t pitch = xsize * dstbytes;
            uint32_t y = 0;

            while (y++ < ysize)
            {
                memcpy(bufdst, bufsrc, pitch);
                bufdst += dstpitch;
                bufsrc += srcpitch;
            }
        }

        // transparency
        if (result == true) {
            if (colorkey.b != 0 || colorkey.g != 0 || colorkey.r != 0 ||
                colorkey.a != 0) {
                const rgba_t transparent_black = { 0, 0, 0, 0 };
                FastFill(pdstimage, &srcpalette, transparent_black, colorkey);
            }
        }

        // palette
        if (pdstpalette != NULL && dstformat == PIXELTYPE_COLOUR_INDEX) {
            for (uint32_t i = 0; i < srcpalette.size; ++i)
            {
                pdstpalette->data[i] = srcpalette.data[i];
            }

            pdstpalette->size = srcpalette.size;
            pdstpalette->bits = srcpalette.bits;
        }

        if (srcimage.data != NULL) {
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
        result = LoadImageFromMemory(pdstimage, pdstpalette, pdstrect, rawsrc,
            (uint32_t)bytesRead, format, psrcrect, filter, colorkey, psrcinfo);

        free(srcbuf);
        srcbuf = NULL;
        rawsrc = NULL;
    }

    return result;
}

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


#ifndef _G_D3DTEX_H_
#define _G_D3DTEX_H_


//-----------------------------------------------------------------------------
//
// Direct3D texture functions.
//
// Support:
// 8bit formats only (pcx, bmp, tga, png)
//
// Requirements:
// zlib for png compression
//
//-----------------------------------------------------------------------------


#include "g_d3dmath.h"


// pixel type
enum pixel_t
{
    PIXELTYPE_UNKNOWN = 0,
    PIXELTYPE_RGBA,
    PIXELTYPE_RGB,
    PIXELTYPE_ABGR,
    PIXELTYPE_BGRA,
    PIXELTYPE_BGR,
    PIXELTYPE_XBGR1555,
    PIXELTYPE_LUMINANCE_ALPHA,
    PIXELTYPE_LUMINANCE,
    PIXELTYPE_COLOUR_INDEX,
    PIXELTYPE_COUNT
};

// filter type
enum filter_t
{
    FILTER_NONE = 0,
    FILTER_POINT,
    FILTER_LINEAR,
    FILTER_COUNT
};

// encode type
enum encode_t
{
    ENCODE_RGB = 0,
    ENCODE_RLE,
    ENCODE_INTERLACE,         // png only
    ENCODE_COUNT
};

// file format
enum file_format_t
{
    FILEFORMAT_NONE = 0,
    FILEFORMAT_PNG,
    FILEFORMAT_BMP,
    FILEFORMAT_PCX,
    FILEFORMAT_TGA
};

// rgba type
typedef struct _rgb_quad
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
} rgb_quad_t;

// palette
typedef struct _palette
{
    rgb_quad_t data[256];
    uint32_t size;
    uint32_t bits;
} palette_t;

// image
typedef struct _image
{
    uint8_t* data;
    uint32_t xsize;
    uint32_t ysize;
    pixel_t  pixeltype;
} image_t;

// image info
typedef struct _image_info
{
    uint32_t xsize;
    uint32_t ysize;
    uint32_t depth;
    pixel_t  pixeltype;
    file_format_t fileformat;
} image_info_t;

//-----------------------------------------------------------------------------
// SaveImageToMemory
//
// Saves an image to a file in memory.
//
// [out   ] ppdst       - Pointer to the file in memory from which to save the
//                        surface.
// [in    ] ppdstsize   - Size of the file in memory, in bytes.
// [in    ] dstformat   - Member of the file_format_t enumerated type
//                        describing the requested file format for the image
// [in    ] dstcodec    - Member of the encode_t enumerated type decribing
//                        the requested encoding for the image.
// [in    ] psrcimage   - Pointer to a image_t structure containing the image
//                        to be saved.
// [in    ] psrcpalette - Pointer to a pallete_t structure, the source palette
//                        of 256 colors or null.
// [in    ] psrcrect    - Pointer to a rect_t structure. Specifies the source
//                        rectangle. Set this parameter to null to specify the
//                        entire image.
//-----------------------------------------------------------------------------
bool
SaveImageToMemory(uint8_t** ppdst, uint32_t* ppdstsize, file_format_t dstformat,
    encode_t dstcodec, image_t* psrcimage, palette_t* psrcpalette,
    rect_t* psrcrect);

//-----------------------------------------------------------------------------
// SaveImage
//
// Saves an image to a file.
//
// [in    ] pdstfile    - Pointer to a string that specifies the filename.
// [in    ] dstformat   - Member of the file_format_t enumerated type
//                        describing the requested file format for the image
// [in    ] dstcodec    - Member of the encode_t enumerated type decribing
//                        the requested encoding for the image.
// [in    ] psrcimage   - Pointer to a image_t structure containing the image
//                        to be saved.
// [in    ] psrcpalette - Pointer to a pallete_t structure, the source palette
//                        of 256 colors or null.
// [in    ] psrcrect    - Pointer to a rect_t structure. Specifies the source
//                        rectangle. Set this parameter to null to specify the
//                        entire image.
//-----------------------------------------------------------------------------
bool
SaveImage(const char* pdstfile, file_format_t dstformat, encode_t dstcodec,
    image_t* psrcimage, palette_t* psrcpalette, rect_t* psrcrect);

//-----------------------------------------------------------------------------
// GetImageInfoFromMemory
//
// Retrieves information about a given image file in memory.
//
// [out] psrcinfo   - Pointer to a image_info_t structure to be filled with the
//                    desription of the data in the source file.
// [in ] psrc       - Pointer to source file in memory.
// [in ] psrcsize   - Size of file in memory, in bytes.
//-----------------------------------------------------------------------------
bool
GetImageInfoFromMemory(image_info_t* psrcinfo, uint8_t* psrc, uint32_t psrcsize);

//-----------------------------------------------------------------------------
// GetImageInfo
//
// Retrieves information about a given image file.
//
// [out   ] psrcinfo    - Pointer to a image_info_t structure to be filled with the
//                        desription of the data in the source file.
// [in    ] psrcfile    - Pointer to a string that specifies the filename.
//-----------------------------------------------------------------------------
bool
GetImageInfo(image_info_t* psrcinfo, const char* psrcfile);

//-----------------------------------------------------------------------------
// LoadImageFromMemory
//
// Loads an image from a file in memory.
//
// [in    ] pdstimage   - Pointer to image structure. Specifies the destination
//                      - image.
// [in    ] pdstpalette - Pointer to a pallete_t structure, the destination
//                        palette of 256 colors or null.
// [in    ] pdstrect    - Pointer to rect_t structure. Specifies the
//                        destination rectangle. Set this parameter to null to
//                        specify the entire image.
// [in    ] psrc        - Pointer to the file in memory from which to load the
//                        image.
// [in    ] srcsize     - Size of the file in memory, in bytes.
// [in    ] format      - Member of the pixel_t enumerated type describing the,
//                        requested pixel format for the image.
// [in    ] psrcrect    - Pointer to rect_t structure. Specifies the
//                        source rectangle. Set this parameter to null to
//                        specify the entire image.
// [in    ] filter      - Combination of one or more flags controlling how the
//                        image is filtered.
// [in,out] colorkey    - Color value to replace with transparent black, or
//                        null to disable the colorkey.
// [in,out] psrcinfo    - Pointer to a image_info_t structure to be filled with
//                        a description of the data in the source image file or
//                        null.
//-----------------------------------------------------------------------------
bool
LoadImageFromMemory(image_t* pdstimage, palette_t* pdstpalette,
    rect_t *pdstrect, uint8_t* psrc, uint32_t srcsize, pixel_t format,
    rect_t *psrcrect, uint32_t filter, rgb_quad_t colorkey,
    image_info_t* psrcinfo);

//-----------------------------------------------------------------------------
// LoadImage
//
// Loads an image from a file.
//
// [in    ] pdstimage   - Pointer to image structure. Specifies the destination
//                      - image.
// [in    ] pdstpalette - Pointer to a pallete_t structure, the destination
//                        palette of 256 colors or null.
// [in    ] pdstrect    - Pointer to rect_t structure. Specifies the
//                        destination rectangle. Set this parameter to null to
//                        specify the entire image.
// [in    ] psrcfile    - Pointer to a string that specifies the filename.
// [in    ] format      - Member of the pixel_t enumerated type describing the,
//                        requested pixel format for the image.
// [in    ] psrcrect    - Pointer to rect_t structure. Specifies the
//                        source rectangle. Set this parameter to null to
//                        specify the entire image.
// [in    ] filter      - Combination of one or more flags controlling how the
//                        image is filtered.
// [in,out] colorkey    - Color value to replace with transparent black, or
//                        null to disable the colorkey.
// [in,out] psrcinfo    - Pointer to a image_info_t structure to be filled with
//                        a description of the data in the source image file or
//                        null.
//-----------------------------------------------------------------------------
bool
LoadImage(image_t* pdstimage, palette_t* pdstpalette,
    rect_t* pdstrect, const char* psrcfile, pixel_t format,
    rect_t* psrcrect, uint32_t filter, rgb_quad_t colorkey,
    image_info_t* psrcinfo);

#endif // #ifndef _G_D3DTEX_H_

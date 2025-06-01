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


#ifndef _G_TEX_H_
#define _G_TEX_H_


//-----------------------------------------------------------------------------
//
// Texture functions.
//
// Support:
// 8bit formats only (pcx, bmp, tga, png)
//
// Requirements:
// zlib for png compression
//
//-----------------------------------------------------------------------------


#include "g_math.h"
#include "g_geul.h"

// Format
#define GEUL_COLOUR_INDEX       0x01
#define GEUL_RGB                0x02
#define GEUL_RGBA               0x03
#define GEUL_BGR                0x04
#define GEUL_BGRA               0x05
#define GEUL_LUMINANCE          0x06
#define GEUL_LUMINANCE_ALPHA    0x07

// Type
#define GEUL_UNSIGNED_BYTE      0x08
#define GEUL_BITMAP             0x09

// Encoding
#define GEUL_RLE                0x11

// File Type
#define GEUL_PNG                0x12
#define GEUL_BMP                0x13
#define GEUL_PCX                0x14
#define GEUL_TGA                0x15

// rgba
typedef struct _rgba
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
} rgba_t;

// palette
typedef struct _palette
{
    rgba_t data[256];
    uint32_t size;
    uint32_t bits;
} palette_t;

// image
typedef struct _image
{
    uint8_t* pixels;
    uint32_t depth;
    uint32_t width;
    uint32_t height;
    uint32_t format;
    uint32_t type;
} image_t;

//-----------------------------------------------------------------------------
// SaveImageToMemory
//
// Saves an image to a file in memory.
//
// [out   ] ppdst       - Pointer to the file in memory from which to save the
//                        surface.
// [in    ] ppdstsize   - Size of the file in memory, in bytes.
// [in    ] fileformat  - Describes the requested file format for the image.
// [in    ] codec       - Describes the requested encoding for the image.
// [in    ] psrcimage   - Pointer to a image_t structure containing the image
//                        to be saved.
// [in    ] psrcpalette - Pointer to a pallete_t structure, the source palette
//                        of 256 colors or null.
//-----------------------------------------------------------------------------
GEUL_DECLSPEC bool
SaveImageToMemory(uint8_t** ppdst, uint32_t* ppdstsize, uint32_t fileformat,
    uint32_t codec, image_t* psrcimage, palette_t* psrcpalette);

//-----------------------------------------------------------------------------
// SaveImageToFile
//
// Saves an image to a file.
//
// [in    ] pdstfile    - Pointer to a string that specifies the filename.
// [in    ] fileformat  - Describes the requested file format for the image.
// [in    ] codec       - Describes the requested encoding for the image.
// [in    ] psrcimage   - Pointer to a image_t structure containing the image
//                        to be saved.
// [in    ] psrcpalette - Pointer to a pallete_t structure, the source palette
//                        of 256 colors or null.
//-----------------------------------------------------------------------------
GEUL_DECLSPEC bool
SaveImageToFile(const char* pdstfile, uint32_t fileformat, uint32_t codec,
    image_t* psrcimage, palette_t* psrcpalette);

//-----------------------------------------------------------------------------
// GetImageInfoFromMemory
//
// Retrieves information about a given image file in memory.
//
// [out] psrcinfo   - Pointer to a image_t structure to be filled with the
//                    description of the data in the source file.
// [in ] psrc       - Pointer to source file in memory.
// [in ] psrcsize   - Size of file in memory, in bytes.
//-----------------------------------------------------------------------------
GEUL_DECLSPEC bool
GetImageInfoFromMemory(image_t* psrcinfo, uint8_t* psrc, uint32_t psrcsize);

//-----------------------------------------------------------------------------
// GetImageInfo
//
// Retrieves information about a given image file.
//
// [out   ] psrcinfo    - Pointer to a image_t structure to be filled with the
//                        description of the data in the source file.
// [in    ] psrcfile    - Pointer to a string that specifies the filename.
//-----------------------------------------------------------------------------
GEUL_DECLSPEC bool
GetImageInfoFromFile(image_t* psrcinfo, const char* psrcfile);

//-----------------------------------------------------------------------------
// LoadImageFromMemory
//
// Loads an image from a file in memory.
//
// [in    ] pdstimage   - Pointer to image structure. Specifies the destination
//                      - image.
// [in    ] pdstpalette - Pointer to a pallete_t structure, the destination
//                        palette of 256 colors or null.
// [in    ] colorkey    - Color value to replace with transparent black.
// [in    ] psrc        - Pointer to the file in memory from which to load the
//                        image.
// [in    ] psrcsize    - Size of file in memory, in bytes.
//-----------------------------------------------------------------------------
GEUL_DECLSPEC bool
LoadImageFromMemory(image_t* pdstimage, palette_t* pdstpalette, rgba_t colorkey,
    uint8_t* psrc, uint32_t psrcsize);

//-----------------------------------------------------------------------------
// LoadImageFromFile
//
// Loads an image from a file.
//
// [in    ] pdstimage   - Pointer to image structure. Specifies the destination
//                      - image.
// [in    ] pdstpalette - Pointer to a pallete_t structure, the destination
//                        palette of 256 colors or null.
// [in    ] colorkey    - Color value to replace with transparent black.
// [in    ] psrcfile    - Pointer to a string that specifies the filename.
//-----------------------------------------------------------------------------
GEUL_DECLSPEC bool
LoadImageFromFile(image_t* pdstimage, palette_t* pdstpalette, rgba_t colorkey,
    const char* psrcfile);

#endif // #ifndef _G_TEX_H_

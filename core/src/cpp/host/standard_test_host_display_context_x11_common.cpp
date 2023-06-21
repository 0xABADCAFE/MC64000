/**
 *   888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
 *   8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
 *   88888b.d88888 888    888 888          d8P 888  888  d8P
 *   888Y88888P888 888        888d888b.   d8P  888  888d88K
 *   888 Y888P 888 888        888P "Y88b d88   888  8888888b
 *   888  Y8P  888 888    888 888    888 8888888888 888  Y88b
 *   888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
 *   888       888  "Y8888P"   "Y8888P"        888  888    Y88b
 *
 *    - 64-bit 680x0-inspired Virtual Machine and assembler -
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <host/standard_test_host_mem.hpp>
#include <host/standard_test_host_display.hpp>
#include <host/display/context.hpp>
#include <host/display/x11/device.hpp>

#include <host/display/x11/filth/simple.hpp>
#include <host/display/x11/filth/generic.hpp>
#include <host/display/x11/filth/script.hpp>

#include <host/display/x11/filth/conversion.hpp>

namespace MC64K::StandardTestHost::Display::x11 {

typedef uint8  LUT8;
typedef uint16 RGB555;
typedef uint32 ARGB32;

/**
 * Word size of pixels, by format
 */
uint8 const aPixelSize[] = {
    sizeof(LUT8),       // PXL_LUT_8
    sizeof(LUT8),       // PXL_HAM_555
    sizeof(RGB555),     // PXL_RGB_555
    sizeof(ARGB32),     // PXL_ARGB_32
};

void Context::allocateBuffer() {

    // Note that the buffer and view widths have been pre-aligned to 32 pixels

    uNumBufferPixels = uBufferWidth * uBufferHeight;
    uNumViewPixels   = uViewWidth   * uViewHeight;

    std::fprintf(
        stderr,
        "Buffer: %d x %d [%u pixels]\nView: %d x %d [%u pixels]\n",
        (int)uBufferWidth, (int)uBufferHeight, uNumBufferPixels,
        (int)uViewWidth,   (int)uViewHeight,   uNumViewPixels
    );

    uint32 uPixelSize = aPixelSize[uPixelFormat];

    switch (uPixelFormat) {

        case PXL_LUT_8: {
            uNumBufferBytes = uNumBufferPixels;

            // Calculate the total allocation size including the viewport sized transfer buffer and palette.
            size_t uTotalAlloc = uNumBufferBytes + (uNumViewPixels + 256) * sizeof(ARGB32);

            oDisplayBuffer.puByte =
            puData                = new uint8[uTotalAlloc];
            oPaletteData.puLong   = (ARGB32*)(puData + uNumBufferBytes);
            puImageBuffer         = (LUT8*)(oPaletteData.puLong + 256);
            break;
        }

        case PXL_HAM_555: {
            uNumBufferBytes = uNumBufferPixels;

            // Calculate the total allocation size including the viewport sized transfer buffer and palette.
            size_t uTotalAlloc = uNumBufferBytes + (uNumViewPixels + 256) * sizeof(RGB555);

            oDisplayBuffer.puByte =
            puData                = new uint8[uTotalAlloc];
            oPaletteData.puWord   = (RGB555*)(puData + uNumBufferBytes);
            puImageBuffer         = (LUT8*)(oPaletteData.puWord + 256);
            break;
        }

        // Basic symmetric formats have the same allocation semantics
		case PXL_RGB_555:
        case PXL_ARGB_32: {
            // Calculate the VM buffer size
            uNumBufferBytes = uNumBufferPixels * uPixelSize;

            // Calculate the total allocation size,
            // including the viewport sized transfer
            // buffer
            size_t uTotalAlloc = uNumBufferBytes + uNumViewPixels * uPixelSize;

            oPaletteData.puAny    = nullptr;
            oDisplayBuffer.puByte = puData = new uint8[uTotalAlloc];
            puImageBuffer         = puData + uNumBufferBytes;
            break;
		}

        default: {
            throw Error();
            break;
        }
    }
    std::fprintf(
        stderr,
        "X11Context RAII: Allocated Pixel Buffer %d x %d x %d at %p\n",
        (int)uBufferWidth, (int)uBufferHeight, (int)aPixelSize[uPixelFormat],
        oDisplayBuffer.puByte
    );
}

typedef void* (*UpdateFunction)(Context& roContext);

template<>
RGB555 PaletteToHAM555<RGB555>::uPrevRGB = 0;

/**
 * Index: (width|height|offset diff) << 2 | format
 */
UpdateFunction aUpdateFunctions[] = {
    updateLUT8<ARGB32, PaletteLookup<ARGB32>>,
    updateLUT8<RGB555, PaletteToHAM555<RGB555>>,
    update<RGB555>,
    update<ARGB32>,
    updateLUT8ViewPort<ARGB32, PaletteLookup<ARGB32>>,
    updateLUT8ViewPort<RGB555, PaletteToHAM555<RGB555>>,
    updateViewPort<RGB555>,
    updateViewPort<ARGB32>,
};

/**
 * Index: format
 */
UpdateFunction aComplexUpdateFunctions[] = {
    updateLUT8Scripted<ARGB32, PaletteLookup<ARGB32>>,   // ARGB32 is the the palette format here
    updateLUT8Scripted<RGB555, PaletteToHAM555<RGB555>>, // RGB555 is the the palette format here
    updateScripted<RGB555>,
    updateScripted<ARGB32>,
};

void* Context::updateBuffers() {

    // If there is a FILTH script, it takes precedence as it can change any of the other attributes
    if (puFilthScript) {
        return aComplexUpdateFunctions[uPixelFormat](*this);
    }

    // Otherwise choose between simple and generic based on other factors
    unsigned uSelect = uPixelFormat | (
        (
            uViewXOffset | uViewYOffset | // Nonzero View offsets ?
            (uViewWidth ^ uBufferWidth) | // Buffer/View width difference ?
            (uViewHeight ^ uBufferHeight) // Buffer/View height difference ?
        ) ? 4 : 0);

    return aUpdateFunctions[uSelect](*this);
}

} // namespace


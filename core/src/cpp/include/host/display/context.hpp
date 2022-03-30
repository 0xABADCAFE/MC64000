#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_CONTEXT_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_CONTEXT_HPP

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

#include <host/standard_test_host_display.hpp>

namespace MC64K::StandardTestHost::Display {

/**
 * PixelPointer. Simple union to avoid casts.
 */
union PixelPointer {
    void*   puAny;
    uint8*  puByte;
    uint16* puWord;
    uint32* puLong;
    PixelPointer(): puAny(nullptr) {}
};

class Device;

class Error {

};

enum {
    WIDTH_ALIGN      = 32,
    WIDTH_ALIGN_MASK = WIDTH_ALIGN - 1
};

/**
 * Context Handle
 *
 * This is the structure that is returned to the MC64 code on successful opening of a display.
 */
struct Context {
    /**
     * VM frame entry callback. Populated by the VM
     */
    Interpreter::VMCodeEntryPoint apVMCall[CALL_MAX];

    /**
     * Address of direct accessible pixel buffer
     */
    PixelPointer oDisplayBuffer;
    uint32*      puPalette;
    uint32       uNumBufferPixels;
    uint32       uNumBufferBytes;
    uint16       uBufferWidth;
    uint16       uBufferHeight;

    uint32       uNumViewPixels;
    uint16       uViewWidth;
    uint16       uViewHeight;
    uint16       uViewXOffset;
    uint16       uViewYOffset;
    uint8 const* puFilthScript;

    uint16       uFlags;
    uint8        uPixelFormat;
    uint8        uRateHz;

    /**
     * Read by VM code
     * For keyboard events, contains the enumeration of the key pressed or released.
     * For mouse events, contains the enumeration of the button pressed or released.
     */
    uint16 uEventRawCode;

    /**
     * Read by VM code
     * For all events, contains a mask of the currently active mouse buttons.
     */
    uint16 uEventRawMask;

    /**
     * Read by VM code
     * For all events, contains the current mouse position.
     */
    uint16 uPositionX;
    uint16 uPositionY;

    /**
     * Points to the device of this context.
     */
    Device* poDevice;

};

/**
 * Parameters required for display opening
 */
struct OpenParams {
    uint16 uViewWidth;
    uint16 uViewHeight;
    uint16 uBufferWidth;
    uint16 uBufferHeight;
    uint16 uViewXOffset;
    uint16 uViewYOffset;
    uint16 uFlags;
    uint8  uPixelFormat;
    uint8  uRateHz;
};

/**
 * Root interface for different display device. Successful instantiation of a derived class is expected
 * to create a viable display instance.
 *
 * To close the display, destroy the instance.
 */
class Device {
    public:
        virtual ~Device() {};
        virtual Context* getContext()    = 0;
        virtual void     runEventLoop()  = 0;
        virtual void     updateDisplay() = 0;
};


Device* createDevice(OpenParams const& roOpenParams);

}

#endif

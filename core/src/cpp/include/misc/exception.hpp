#ifndef MC64K_EXCEPTION_HPP
    #define MC64K_EXCEPTION_HPP

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

namespace MC64K {

/**
 * Root exception class for all MC64K runtime error
 */
class Exception {
    private:
        char const* sMessage;
    public:
        Exception(char const* sMessage = "Exception") : sMessage(sMessage) {}

        char const* getMessage() const;
};

/**
 * Sundry top level exceptions.
 */
class OutOfRangeException  : public Exception {
    public:
        OutOfRangeException(char const* sMessage = "Value out of range") : Exception(sMessage) {}
};

class OutOfMemoryException : public Exception {
    public:
        OutOfMemoryException(char const* sMessage = "Insufficent memory") : Exception(sMessage) {}
};

inline char const* Exception::getMessage() const {
    return sMessage;
}

}// namespace

#endif

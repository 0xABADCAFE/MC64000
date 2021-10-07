#ifndef __MC64K_STANDARD_TEST_HOST_IO_HPP__
    #define __MC64K_STANDARD_TEST_HOST_IO_HPP__

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

#include "standard_test_host.hpp"

namespace MC64K {
namespace StandardTestHost {

/**
 * IO namespace. Gathers together all parts of the Standard Test Host IO functionality.
 *
 */
namespace IO {

/**
 * Call
 *
 * Enumeration of calls in the IO namespace
 */
typedef enum {
    INIT = 0,
    DONE,

    /**
     * func io_print_string(r8/a0 [char const*] string) => void
     *
     * Print the string pointed to by (r8/a0)
     */
    PRINT_STRING,

    /**
     * func io_print_<byte:word:long:quad>(r0/d0 <type> value) => void
     *
     * Print the the integer type in r0/d0 using the current format template for the integer type.
     */
    PRINT_BYTE,
    PRINT_WORD,
    PRINT_LONG,
    PRINT_QUAD,

    /**
     * func io_print_<single:double>(fp0 <type> value) => void
     *
     * Print the floating point type in fp0 using the current template for the float type.
     */
    PRINT_SINGLE,
    PRINT_DOUBLE,

    /**
     * func io_set_fmt_<byte:word:long:quad:single:double>(r8/a0 char const* format) => void
     *
     * Set the format template for the type to the string pointed to by (r8/a0)
     */
    SET_FMT_BYTE,
    SET_FMT_WORD,
    SET_FMT_LONG,
    SET_FMT_QUAD,
    SET_FMT_SINGLE,
    SET_FMT_DOUBLE,

    /**
     * func io_clr_fmt_<byte:word:long:quad:single:double>() => void
     *
     * Revert the format template for the type to the host-specified default.
     */
    CLR_FMT_BYTE,
    CLR_FMT_WORD,
    CLR_FMT_LONG,
    CLR_FMT_QUAD,
    CLR_FMT_SINGLE,
    CLR_FMT_DOUBLE,

    /**
     * funct io_file_open(r8/a0 char const* name, r0/d0 uint8 mode) => r8/a0 FILE* file, r0/d0 uint64 error
     *
     * Attempts to open the file pointed to by (r8/a0) in the mode indicated by the value in r0/d0.
     * On success, returns the address of the file handle in r8/a0 and a success code in r0/d0.
     * On failure, returns null in r8/a0 on failure and an error code in r0/d0.
     */
    FILE_OPEN,

    /**
     * func io_file_seek(r8/a0 FILE* file, r0/d0 int64 offset, r1/d1 uint8 mode) => r8/a0 FILE* file, r0/d0 uint64 error
     *
     * Attempts to seek withing the file pointed to by (r8/a0) by the offset indicated by the value in r0/d0 using the
     * mode indicated by the value in r1/d1.
     *
     * Returns success or an error code in r0/d0
     */
    FILE_SEEK,

    /**
     * func io_file_tell(r8/a0 FILE* file) => r0/d0 int64 pos, r1/d1 uint64 error
     *
     * Attempts to read the current cursor position within the file pointed to by (r8/a0).
     * On success, returns the cursor position in r0/d0 and a success code in r1/d1
     * On failure, returns -1 in r0/d0 and an error code in r1/d1
     */
    FILE_TELL,

    /**
     * func io_file_read(r8/a0 FILE* file, r9/a1 void* buffer, r0/d0 uint64 size) => r0/d0 uint64 size, r1/d1 uint64 error
     *
     * Attempts to read the number of bytes indicated by the value in r0/d0 of data from the file pointed to by (r8/a0)
     * into the buffer pointed to by (r9/a1).
     *
     * Returns the number of bytes read in r0/d0 and a success or error code in r1/d1. Only when the number of bytes
     * read matches the requeted number of bytes is the return code success.
     */
    FILE_READ,

    /**
     * func io_file_write(r8/a0 FILE* file, r9/a1 void* buffer, r0/d0 uint64 size) => r0/d0 uint64 size, r1/d1 uint64 error
     *
     * Attempts to write the number of bytes indicated by the value in r0/d0 of data to the file pointed to by (r8/a0)
     * from the buffer pointed to by (r9/a1).
     *
     * Returns the number of bytes written in r0/d0 and a success or error code in r1/d1. Only when the number of bytes
     * written matches the requeted number of bytes is the return code success.
     */
    FILE_WRITE,

    /**
     * func io_file_close(r8/a0 FILE* file) => r0/d0 uint64 error
     *
     * Attempts to close the file pointed to by (r8/a0).
     *
     * On success, sets r8/a0 to null and a success code in r0/d0.
     * On failure, returns the error code in r0/d0.
     */
    FILE_CLOSE,
} Call;

/**
 * Allowed modes for file opening
 */
typedef enum {
    OPEN_READ = 0,
    OPEN_WRITE,
    OPEN_APPEND,
    OPEN_READ_UPDATE,
    OPEN_WRITE_UPDATE,
    OPEN_APPEND_UPDATE
} OpenMode;

/**
 * Allowed modes for file opening
 */
typedef enum {
    FROM_START = 0,
    FROM_CURRENT,
    FROM_END,
} SeekMode;


/**
 * Error return values
 */
typedef enum {
    ERR_FILE = 200,
    ERR_OPEN,
    ERR_READ,
    ERR_WRITE,
} Result;

Interpreter::Status hostVector();

}}} // namespace

#endif
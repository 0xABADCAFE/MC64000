#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_2X2_MACROS_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_2X2_MACROS_HPP__

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

#include "./offsets.hpp"
#include "./templates.hpp"

namespace MC64K::StandardTestHost::VectorMath {

/**
 * These macros exist to select the appropriate register union member to invoke the given template operation on.
 */
#define m2x2_identity(T, UNION_NAME) mat_identity<T, 2>(aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME)

#define m2x2_copy(T, UNION_NAME) \
mat_copy<T, 2>( \
    aoGPR[ABI::PTR_REG_1].p ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].p ## UNION_NAME \
)

#define m2x2_scale_assign(T, UNION_NAME) \
mat_scale_assign<T, 2>( \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME \
)

#define m2x2_scale(T, UNION_NAME) \
mat_scale<T, 2>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME \
)

#define m2x2_add_assign(T, UNION_NAME) \
mat_add_assign<T, 2>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m2x2_add(T, UNION_NAME) \
mat_add<T, 2>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m2x2_sub_assign(T, UNION_NAME) \
mat_sub_assign<T, 2>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m2x2_sub(T, UNION_NAME) \
mat_sub<T, 2>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m2x2_multiply(T, UNION_NAME) \
mat2x2_multiply<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m2x2_multiply_assign(T, UNION_NAME) { \
    T* pfDst  = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T  pfTmp[2*2]; \
    mat_copy<T, 2>(pfTmp, pfDst); \
    mat2x2_multiply<T>( \
        pfDst, \
        pfTmp, \
        aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
    ); \
}

#define m2x2_transpose(T, UNION_NAME) \
mat2x2_transpose<T>( \
    aoGPR[ABI::PTR_REG_1].p ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].p ## UNION_NAME \
)


#define m2x2_determinant(T, UNION_NAME) \
Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME = mat2x2_determinant<T>(aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME)

/**
 * Inversion of 2x2
 *
 * | a b |   =>     1     |  d -b |
 * | c d |      (ad - bc) | -c  a |
 */
#define m2x2_inverse(T, UNION_NAME) \
aoGPR[ABI::INT_REG_0].uQuad = mat2x2_inverse<T>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)


}

#endif

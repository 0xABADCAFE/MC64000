
;  888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
;  8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
;  88888b.d88888 888    888 888          d8P 888  888  d8P
;  888Y88888P888 888        888d888b.   d8P  888  888d88K
;  888 Y888P 888 888        888P "Y88b d88   888  8888888b
;  888  Y8P  888 888    888 888    888 8888888888 888  Y88b
;  888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
;  888       888  "Y8888P"   "Y8888P"        888  888    Y88b
;
;   - 64-bit 680x0-inspired Virtual Machine and assembler -
;
;

    @align 0, 8
calculate:

; calculate ranges
    ; fp15 fMinReal | fMinImaginary
    ; fp14 fStep
    ; fp13 fBailout
    ; fp12 fMaxImaginary

    fmove.d     #MAX_ORDINATE, fp14
    fmove.d     #BAILOUT,      fp13
    fmove.d     fp14,   fp12        ; fCurrImaginary = fMaxImaginary
    fneg.d      fp14,   fp15        ; fMinImaginary  = -fMaxImaginary
    fadd.d      fp14,   fp14        ; fRange         = 2*fMaxImaginary
    move.l      #1,     d6
    lsl.l       #IMAGE_SIZE_DIM, d6 ; iHeight = iWidth = (1 << IMAGE_SIZE_DIM)
    fmovel.d    d6,     fp11        ; fWidth  = (float) iWidth
    fdiv.d      fp11,   fp14        ; fStep   = fRange / fWidth
    move.l      d6,     d5          ; iY      = iHeight
    move.l      #MAX_ITERATION, d3  ;
    fadd.d      #REAL_OFFSET, fp15  ; adjust the real coordinate to better centre the set

    ; fp12 fCurrImaginary
    ; d7   num pixels
    ; d6   edge size (width/height)
    ; d5   height countdown
    ; d3   max iteration
.yloop:

    ; fp11 fCurrReal

    fmove.d     fp15,   fp11        ; fCurrReal = fMinR
    move.l      d6,     d4          ; iX = iWidth

    ; fp10 fZImaginary
    ; fp9  fZReal
    ; fp8  fZImaginarySquared | fZRealSquared + fZImaginarySquared (arg length squared)
    ; fp7  fZRealSquared
    ; fp6  fNewZReal

    ; d4   width countdown
.xloop:
    fmove.d     fp12,   fp10        ; fZImaginary = fCurrImaginary
    fmove.d     fp11,   fp9         ; fZReal      = fCurrReal
    move.l      d3,     d2

    ; d2   iteration countdown
.iteration:

    fmove.d     fp10,   fp8
    fmul.d      fp8,    fp8         ; fZImaginarySqared = fZImaginary*fZImaginary
    fmove.d     fp9,    fp7
    fmul.d      fp7,    fp7         ; fZRealSquares = fZReal * fZReal
    fmove.d     fp11,   fp6         ; fNewZReal     = fCurrReal
    fadd.d      fp7,    fp6         ; fNewZReal    += fZRSquared
    fsub.d      fp8,    fp6         ; fNewZReal    -= fZISquared
    fadd.d      fp7,    fp8         ; fZRealSquared + fZImaginarySquared (for bailout test)

    ; fp5 fTemp
    fmove.d     fp9,    fp5         ; fTemp = fZReal
    fmul.d      fp10,   fp5         ; fTemp = fZReal * fZImaginary
    fadd.d      fp5,    fp5         ; fTemp = 2 * fZReal * fZImaginary
    fadd.d      fp12,   fp5
    fmove.d     fp5,   fp10         ; fZImaginary = 2 * fZReal * fZImaginay + fCurrImaginary
    fmove.d     fp6,    fp9         ; fZReal      = fNewZReal
    fbgt.d      fp8,    fp13, .bailout
    dbnz        d2,     .iteration

.bailout:
    fadd.d      fp14,   fp11        ; fCurrReal += fStep

    ; Calculate the iteration count and square for better gradients
    neg.l       d2,     d2
    add.l       d3,     d2          ; iteration count = max iterations - iteration countdown
    mulu.w      d2,     d2
    move.b      d2,     (a0)+

    dbnz        d4,     .xloop      ; while --iX
    fsub.d      fp14,   fp12        ; fCurrImaginary -= fStep
    dbnz        d5,     .yloop      ; while --iY

    rts

  ; Offset 0 : @render

  ; r0 = pixel data address
  ; r1 = width in pixels
  ; r2 = height in pixels
  ; r3 = cY (float pos, starting at yMin)
  ; r4 = 16.0 (bailout)
  ; r5 = xMin (float)
  ; r6 = cX (float pos, starting at xMin)
  ; r7 = fStep
  ; r8 = x (int) pixel
  ; r9 = iStep (1)

  _save         _mr0|_mr2                ; 2 : save pixel base address
    fmove.s     #16.0, _r4               ; 3 : bailout = 16.0
    move.l      #256,  _r9                ; 2 : max iters

.y_loop:
    move.q      #0,    _r8                 ; 1 : x = 0
    fmove.s     _r5,  _r6                 ; 1 : cX = xMin

.x_loop:
    fmove.s     _r6,  _r10                ; 1 : zx = cX
    fmove.s     _r3,  _r11                ; 1 : zy = cY
    move.q      #0,    _r12                ; 1 : n = 0

.iteration:
    fmove.s     _r10, _r13                ; 1
    fmul.s      _r10, _r13                ; 1 : zx2 = zx*zx
    fmove.s     _r11, _r14                ; 1
    fmul.s      _r11, _r14                ; 1 : zy2   = zy*zy

    fmove.s     _r6,  _r15                ; 1 : new_zx = cX
    fadd.s      _r13, _r15                ; 1 : new_zx += zx2
    fsub.s      _r14, _r15                ; 1 : new_zx -= zy2

    fadd.s      _r14, _r13                ; 1 : r14 = zx*zx + zy*zy (for loop test)

    fmove.s     _r10, _r14                ; 1 : tmp = zx
    fmul.s      _r11, _r14                ; 1 : tmp *= zy
    fadd.s      _r14, _r14                ; 1 : tmp += tmp2
    fadd.s      _r3,  _r14                ; 1 : tmp += cY (tmp = 2*zx*zy+cY)

    fmove.s     _r14, _r11                ; 1 : zy = tmp
    fmove.s     _r15, _r10                ; 1 : zx = new_zx
    add.w       #1,    _r12                ; 2 : n++

    fbgt.s      _r13, _r4, 2             ; 2 : bailout
    blt.w       _r12, _r9, .iteration           ; 2 : iteration loop

.bailout:
    _mul_u16    _r12, _r12                ; 1 : out *= out - improve gradient
    _st_ripi_8  _r12, _r0                 ; 1 : out = n
    fadd.s      _r7,  _r6                 ; 1 : cX += fStep
    add.w       #1,    _r8                 ; 2 : x += iStep

    blt.w     _r8, _r1, .x_loop         ; 2 : x loop

    fadd.s      _r7, _r3                 ; 1 : cY += fStep
    sub.w       #1,   _r2                 ; 2 : y += iStep
    _bnz_32     _r2, .y_loop              ; 2 : y loop

    _restore    _mr0|_mr2)                ; 1
    _ret,

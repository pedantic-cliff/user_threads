.global _setup_context
_setup_context:
  mov     x4,#0
  mov     x5,#0
  mov     x2,x0
  stp x4, x5, [x0, #-16]!
  stp x4, x5, [x0, #-16]!
  stp x4, x5, [x0, #-16]!
  stp x4, x5, [x0, #-16]!
  stp x4, x5, [x0, #-16]!
  stp x2, x1, [x0, #-16]!
  str x0, [x0]
  ret

.global _swap_context
.global _enter_context
_swap_context:
  stp x19, x20, [sp, #-16]!
  stp x21, x22, [sp, #-16]!
  stp x23, x24, [sp, #-16]!
  stp x25, x26, [sp, #-16]!
  stp x27, x28, [sp, #-16]!
  stp x29, x30, [sp, #-16]!
  mov x3, sp
  str x3, [sp]
  
_enter_context:
  ldr x1, [x0]
  mov sp, x1
  ldp x29, x30,  [sp], #16
  ldp x27, x28,  [sp], #16
  ldp x25, x26,  [sp], #16
  ldp x23, x24,  [sp], #16
  ldp x21, x22,  [sp], #16
  ldp x19, x20,  [sp], #16
  mov x0, x3
  ret

  


!------------------------------------------------------------------------------
! assembler code for unsigned 32 bit multiplication
! taken from 
!
!            "The SPARC Architecture Manual"  (Appendix E.2)
!
!------------------------------------------------------------------------------


!------------------------------------------------------------------------------
! extern "C" word Multiply_Words(word a, word b, word* high);
! (returns lower 32 bit of product, stores higher 32 bit in *high)
!------------------------------------------------------------------------------

.seg "text"

.global _Multiply_Words
_Multiply_Words:
or      %o0, %o1, %o4   !logical or of multiplier and miltiplicand
mov     %o0, %y         !multiplier to Y 
andncc  %o4, 0xfff, %o5 !mask out lower 12 bits
be      mul_shortway    !can do it the short way
andcc   %g0, %g0, %o4   !zero the partial product; clear N and V cond.

! long  multiply
mulscc  %o4, %o1, %o4   !first of 33 iterations
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4  ! 32nd iteration;
mulscc  %o4, %g0, %o4  ! last iteration only shits
tst     %o1
bge     1f
nop
add     %o4, %o0, %o4
1:
rd      %y, %o0 
addcc   %o4, %g0, %o1  ! set zero bit appropriately
st      %o1, [%o2]     ! return high bits
retl                   ! leaf-routine return
nop

! short multiply
mul_shortway:
mulscc  %o4, %o1, %o4 ! first iteration of 13
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4
mulscc  %o4, %o1, %o4  ! 12nd iteration
mulscc  %o4, %g0, %o4  ! last iteration only shits
rd      %y, %o5
sll     %o4, 12, %o4   ! left shift partial product by 12 bits
srl     %o5, 20, %o5   ! right shift product by 12 bits
or      %o5, %o4, %o0  ! merge for true product
addcc   %g0, %g0, %o1
st      %o1, [%o2]     ! return high bits
retl                   ! leaf routine return
nop


!------------------------------------------------------------------------------
! extern "C" word Mult_Inner_Loop(word *p, word *a, word* a_stop, word B)
!------------------------------------------------------------------------------
! %o0 p
! %o1 a
! %o2 a_stop
! %o3 B
!
.global    _Mult_Inner_Loop
_Mult_Inner_Loop:
cmp        %o1,%o2       ! if a >= a_stop return 
bge        IMULT_L1
mov        0, %g5        ! carry = 0
IMULT_L0:                 ! Loop: while (a < a_stop)
mov        %o3, %y       ! load B to Y register
andcc      %g0, %g0, %g3
ld         [%o1], %g2    ! A = *a
ld         [%o0], %g4    ! P = *p
!
! compute 64 bit product A*B = (high,low)
!
mulscc     %g3, %g2, %g3 
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g2, %g3
mulscc     %g3, %g0, %g3
tst        %g2            ! sign correction
bge        IMULT_L2
rd         %y, %g2        ! low
add        %g3, %o3, %g3  ! sign correction
IMULT_L2:
addcc      %g5, %g4, %g4  ! P += carry
addx       %g0, %g0, %g5  ! new carry from addition
addcc      %g2, %g4, %g2  ! low += P
addx       %g3, %g5, %g5  ! carry += high
st         %g2, [%o0]     ! *p = low
inc        4,   %o1       ! a++
cmp        %o1, %o2       ! if (a < a_stop) goto IMULT_L0
bl         IMULT_L0       ! loop
inc        4,   %o0       ! p++
IMULT_L1:
st         %g5, [%o0]     ! *p = carry
retl
mov        %g5, %o0       ! return carry



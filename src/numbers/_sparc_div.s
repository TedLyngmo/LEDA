
!------------------------------------------------------------------------------
! assembler code for unsigned 32 bit multiplication
! taken from 
!
!            "The SPARC Architecture Manual"  (Appendix E.2)
!
!------------------------------------------------------------------------------


!------------------------------------------------------------------------------
! extern "C" word Div_Inner_Loop(word *p, word *a, word* a_stop, word B)
!------------------------------------------------------------------------------
! %o0 p
! %o1 a
! %o2 a_stop
! %o3 B
!

.global    _Div_Inner_Loop
_Div_Inner_Loop:
cmp        %o1,%o2       ! if a >= a_stop return 
bge        IDIV_L1
mov        0, %g5        ! carry = 0
IDIV_L0:                 ! Loop: while (a < a_stop)
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
bge        IDIV_L2
rd         %y, %g2        ! low
add        %g3, %o3, %g3  ! sign correction
IDIV_L2:
subcc      %g4, %g5, %g4  ! P -= carry
addx       %g0, %g0, %g5  ! new carry from subtraction
subcc      %g4, %g2, %g4  ! P -= low
addx       %g3, %g5, %g5  ! carry += high
st         %g4, [%o0]     ! *p = P
inc        4,   %o1       ! a++
cmp        %o1, %o2       ! if (a < a_stop) goto IDIV_L0
bl         IDIV_L0        ! loop
inc        4,   %o0       ! p++
IDIV_L1:
ld         [%o0], %g4     ! P = *p
subcc      %g4, %g5, %g4  ! P -= carry
addx       %g0, %g0, %g5  ! new carry 
retl
mov        %g5, %o0       ! return carry


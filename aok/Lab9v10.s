;  [((2*T[i+7]) + T[i+1]+T[i+2])*(T[i+3])+T[i+4]) ] /
[T[i+5] ]
.data

licznikT:	.word	130
licznikTB:	.word	110

suma:	.space	8

ulamek:	.double	0.97
skladnik:	.double	201
nr:	.double	19
one:	.double	1.0
stala:	.double	2.0

T:	.space	1040
TB:	.space	880

.text

ld f2,nr
ld f4,skladnik
ld f6,ulamek
ld f8,one
ld f30,suma


addi r1,r0,T
addi r2,r0,TB

lw r5,licznikT
lw r6,licznikTB


addd f14,f2,f6
addd f12,f14,f4

wypelnijT:

sd 0(r1),f12
addi r1,r1,#8
subi r5,r5,#1
addd f12,f12,f8
bnez r5,wypelnijT

addi r1, r0, T

wypelnijTB:

; [((2*T[i+7]) + T[i+1]+T[i+2])*(T[i+3])+T[i+4])]/[T[i+5]]

ld f2,56(r1)	;T[i+7]
ld f4,8(r1)	;T[i+1]
ld f6,16(r1)	;T[i+2]
ld f8,24(r1)	;T[i+3]
ld f10,32(r1)	;T[i+4]
ld f12,40(r1)	;T[i+5]
ld f14,stala

multd f16,f14,f2	;stala * [i+7]
addd f18,f8,f10	;[i+3] + [i+4]
addd f20,f4,f6	;[i+1] + [i+2]
addd f22,f16,f20

multd f24,f22,f18

divd f26,f24,f12

addd f30,f30,f26
sd 0(r2),f26
addi r2, r2, #8
addi r1, r1, #8

subi r6,r6,#1
bnez r6,wypelnijTB

addi r3,r0,suma
sd 0(r3),f30

trap 0 

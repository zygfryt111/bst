[(5.2*T[i] * T[i+1])- (T[i+2]+ T[i+3]+T[i+4]) ] /
[T[i+5] ]
.data


lT: .word 130
lTB: .word 105

suma: .space 8

ulamek: .double 0.55
skladnik: .double 155.0
numer: .double 11.0
jeden: .double 1.0
stala: .double 5.2


T: .space 1040
TB: .space 840




.text

ld f2, ulamek
ld f6, skladnik
ld f8, numer
ld f10, jeden
ld f16, stala
ld f30, suma



addi r1, r0, T ;wskaznik T[0] -> r1




lw r5, lT ;licznik petli T
lw r6, lTB ;licznik petli TB




;liczenie pierwszego elementu T (skladnik+numer+ulamek)

addi r3, r0, #1 
addd f12, f6, f8
addd f12, f12, f2
nop


wypelnijT:
sd 0(r1), f12
addd f12, f12, f10 ;zwiekszenie ulamek+skladnik+numer o 1
addi r1, r1, #8

subi r5, r5, #1 ;dekrementacja licznika
bnez r5, wypelnijT



addi r1, r0, T
addi r3, r0, suma
;petla do uzupelniania tablicy TB

addi r2, r0, TB ;wskaznik TB[0] -> r2

wypelnijTB:
ld f2, 0(r1)
ld f4, 8(r1)
ld f6, 16(r1)
ld f8, 24(r1)
ld f10, 32(r1)


multd f14, f2, f4	; t[i] * t[i+1]

addd f20, f6, f8


multd f18, f14, f16	; t[i]*t[i+1]*5.1
addd f22, f20, f10	; t[i+2]+t[i+3]+t[i+4]

ld f12, 40(r1)
subd f24, f18, f22	; odejmowanie (licznik)
divd f26, f24, f12	; licznik/t[i+5]

addd	f30, f30, f26
sd	0(r2), f26
addi	r2, r2, #8
addi	r1, r1, #8

subi	r6, r6, #1
bnez r6, wypelnijTB
addi r3, r0, suma	
sd 0(r3), f30

trap 0 
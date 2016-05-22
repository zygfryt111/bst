[7.7* (T[i] + T[i+3]+T[i+5]+ T[i+7]+T[i+9]) ] /
[T[i+2] ]
.data


lT: .word 130
lTB: .word 107

suma: .space 8

ulamek: .double 0.77
adnik: .double 177
numer: .double 3
jedynka: .double 1.0
stala: .double 7.7

T: .space 1040
TB: .space 856




.text

ld f2, ulamek
ld f6, adnik
ld f8, numer
ld f10, jedynka
ld f16, stala
ld f30, suma

addi r3, r0, #0 ;taki zerowy rejestr pomocniczy
addi r1, r0, T ;zaladowanie pierwszego el. T do r1
addi r2, r0, TB ;zaladowanie pierwszego el. TB do r2



lw r5, lT ;licznik petli T
lw r6, lTB ;licznik petli TB



;do zrobienia pierwszego skladnika tabeli T
ulamkowa:
addi r3, r0, #1 ;zmiana rejestru r3, zeby nie wejsc wiecej w petle 'ulamkowa';
addd f12, f6, f8
addd f12, f12, f2


skocz:
beqz r3, ulamkowa
sd 0(r1), f12
addd f12, f12, f10 ;zwiekszenie ulamek+skladnik+numer o 1
addi r1, r1, #8


subi r5, r5, #1 ;dekrementacja licznika
bnez r5, skocz



addi r1, r0, T

;petla do uzupelniania tablicy TB



skocz1:
ld f2, 0(r1)
ld f4, 24(r1)
ld f6, 40(r1)
ld f8, 56(r1)
ld f10, 72(r1)
ld f12, 16(r1)





;[7.7* (T[i] + T[i+3]+T[i+5]+ T[i+7]+T[i+9]) ] / [T[i+2] ]

addd f14, f2, f4         ;t[0]+t[3]
addd f18, f14, f6     ;          + t[5]
addd f20, f18, f8     ;                 + t[7]
addd f22, f20, f10     ;                +t[9]

multd f24, f22, f16
divd f26, f24, f12

addd f30, f30, f26
sd 0(r2), f26
addi r2, r2, #8
addi r1, r1, #8

subi r6, r6, #1
bnez r6, skocz1

addi r3, r0, suma
sd 0(r3), f30


trap 0 
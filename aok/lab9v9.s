[9.1*(T[i+7] - T[i+1])+ (T[i+2]* T[i+4]*T[i+6]) ] /
[T[i+4] ]
.data


lT: .word 130
lTB: .word 109

suma: .space 8

ulamek: .double 0.91
skladnik: .double 99
numer: .double 11
jedynka: .double 1.0
stala: .double 9.1


T: .space 1040
TB: .space 872




.text

ld f2, ulamek
ld f6, skladnik
ld f8, numer
ld f10, jedynka
ld f16, stala
ld f30, suma


addi r3, r0, #0 ;taki zerowy rejestr pomocniczy
addi r1, r0, T ;zaladowanie adresu T[0] do r1
addi r2, r0, TB ;zaladowanie adresu TB[0] do r2



lw r5, lT ;licznik petli T
lw r6, lTB ;licznik petli TB




;liczenie pierwszego elemtu T (skladnik+numer+ulamek)
ulamkowa:
addi r3, r0, #1 ;zmiana rejestru r3, zeby nie wejsc wiecej w petle 'ulamkowa';
addd f12, f6, f8
addd f12, f12, f2
beqz r3, ulamkowa

skocz:

sd 0(r1), f12
addd f12, f12, f10 ;zwiekszenie ulamek+skladnik+numer o 1
addi r1, r1, #8

subi r5, r5, #1 ;dekrementacja licznika
bnez r5, skocz



addi r1, r0, T
addi r3, r0, suma
;petla do uzupelniania tablicy TB



skocz1:


ld f2, 56(r1)  ;T[i+7]->f2
ld f4, 8(r1)   ;i+1->f4
subd f14, f2, f4         ;t[7]-t[1]->f14

ld f6, 16(r1)  ;i+2 ->f6
ld f8, 32(r1)  ;i+4->f8
multd f18, f6, f8        ;[2]*[4]->f18

ld f10, 48(r1) ;i+6->f10
ld f12, 32(r1) ;i+4 jeszcze raz->f12
multd f20, f18, f10      ;[2]*[4]*[6]->f20
addd f12,f0,f0

multd f12,f14,f16         ;9.1 * (t[7]-t[1])


addd f22,f20,f12          ;caly licznik

divd f26, f22, f8        ;cale dzialanie
addi r1, r1, #8          ;licznik tablicy T++
subi r6, r6, #1          ;petla--

addd f30, f30, f26       ;dodaj do sumy
sd 0(r2), f26            ;zapisz w TB
addi r2, r2, #8          ;licznik tablicy TB++



bnez r6, skocz1


sd 0(r3), f30


trap 0 
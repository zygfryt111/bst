; [(4.2*T[i+1] * T[i+6]*T[i+2])-  (T[i+5]*T[i+4]) ] /
[T[i+4] ]
.data
		.global T
T:		.space 130*8
		.global TB
TB:		.space 104*8
suma:		.double 0
ulamek:		.double 0.44
numer:		.double 14
skladnik:	.double 144
jeden:		.double 1
mnoznik:	.double 4.2

.text
	addi 	r1, r0, 0
	ld	f2, numer
	addd 	f0, f0, f2
	ld	f2, skladnik
	addd	f0, f0, f2
	ld	f2, ulamek
	addd	f0, f0, f2
	ld	f4, jeden

Ta:
	sd 	T(r1), f0
	addd	f0, f0, f4
	addi	r1, r1, 8
	seqi	r2, r1, 1040
	beqz	r2, Ta

	addi	r1, r0, 0
	ld	f4, mnoznik
TBa:
	addi	r10, r1, 8
	ld	f0, T(r10)
	multd	f0, f0, f4
	addi	r10, r1, 16
	ld	f2, T(r10)
	multd	f0, f0, f2
	addi	r10, r1, 48
	ld	f2, T(r10)
	multd	f0, f0, f2
addi	r10, r1, 40
	ld	f2, T(r10)
	addi	r10, r1, 32
	ld	f6, T(r10)
	multd	f2, f2, f6

	subd	f0, f0, f2

	divd	f0, f0, f6
	nop
	sd	TB(r1), f0

	addd	f10, f10, f0

	addi	r1, r1, 8
	seqi	r2, r1, 832
	beqz	r2, TBa

	sd 	suma, f10

	trap	0
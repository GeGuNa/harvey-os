/*
 *	mpvecdigmulsub(mpdigit *b, int n, mpdigit m, mpdigit *p)
 *
 *	p -= b*m
 *
 *	each step looks like:
 *		hi,lo = m*b[i]
 *		lo += oldhi + carry
 *		hi += carry
 *		p[i] += lo
 *		oldhi = hi
 *
 *	the registers are:
 *		b = R11
 *		carry = R12
 *		n = R14
 *		m = R15
 *		p = R16
 *		i = R17
 *		hi = R18
 *		lo = R19
 *		oldhi = R20
 *		tmp = R21
 */
TEXT	mpvecdigmulsub(SB),$0
	MOV	R8, R11		/* first arg */
	MOVW	n+XLEN(FP),R14
	MOVWU	m+(XLEN+4)(FP),R15
	MOV	p+(2*XLEN)(FP),R16

	MOV	R0, R20		/* oldhi = 0 */
_mulsubloop:
	MOVWU	0(R11), R19	/* lo = b[i] */
	ADD	$4, R11
	MOVWU	0(R16), R21	/* tmp = p[i] */
	MUL	R19, R15
	SRL	$32, R15, R18	/* hi = (b[i] * m)>>32 */
	MOV	$((1ull<<32)-1), R22
	AND	R22, R15, R19	/* lo = b[i] * m */
	ADDW	R20, R19	/* lo += oldhi */
	SLTU	R20, R19, R12
	ADDW	R12, R18	/* hi += carry */
	SUBW	R19, R21, R13	/* tmp -= lo */
	SLTU	R13, R21, R12
	ADDW	R12, R18	/* hi += carry */
	MOVW	R13, 0(R16)	/* p[i] = tmp */
	ADDW	$4, R16
	MOV	R18, R20	/* oldhi = hi */
	SUB	$1, R14
	BNE	R14, _mulsubloop

	MOVWU	0(R16), R21	/* tmp = p[i] */
	SUBW	R20, R21, R13	/* tmp -= oldhi */
	MOVW	R13, 0(R16)	/* p[i] = tmp */
	SLTU	R13, R21, R11
	BNE	R11, _mulsub2
	MOV	$1, R8		/* return +1 for positive result */
	RET

_mulsub2:
	MOV	$-1, R8		/* return -1 for negative result */
	RET

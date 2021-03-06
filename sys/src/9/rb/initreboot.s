/*
 * mips 24k machine assist for routerboard rb450g (minimal for reboot)
 */
#include "mem.h"
#include "mips.s"

	NOSCHED

TEXT	_main(SB), $0
	MOVW	$setR30(SB), R30
	JMP	main(SB)
	  NOP

/* target for JALRHB in BARRIERS */
TEXT ret(SB), $-4
	JMP	(R22)
	  NOP

TEXT	setsp(SB), $-4
	MOVW	R1, SP
	RETURN

TEXT	coherence(SB), $-4
	BARRIERS(7, R7, cohhb)
	SYNC
	EHB
	RETURN

/*
 * next_kernel(entry, r4, r5, r6, r7)
 *	set R4-R7 for next kernel, then jump to it.
 */
TEXT next_kernel(SB), $-4
	MOVW	r4+4(FP), R4
	MOVW	r5+8(FP), R5
	MOVW	r6+12(FP), R6
	MOVW	r7+16(FP), R7
	JMP	(R1)
	  NOP

/*
 *  cache manipulation
 */

/* the i and d caches may be different sizes, so clean them separately */
TEXT	cleancache(SB), $-4
	DI(10)				/* intrs off, old status -> R10 */
	SYNC
	EHB

	MOVW	R0, R1			/* index, not address */
	MOVW	$ICACHESIZE, R9
iccache:
	CACHE	PI+IWBI, (R1)		/* flush & invalidate I by index */
	SUBU	$CACHELINESZ, R9
	ADDU	$CACHELINESZ, R1
	BGTZ	R9, iccache
	  NOP

	MOVW	R0, R1			/* index, not address */
	MOVW	$DCACHESIZE, R9
dccache:
	CACHE	PD+IWBI, (R1)		/* flush & invalidate D by index */
	SUBU	$CACHELINESZ, R9
	ADDU	$CACHELINESZ, R1
	BGTZ	R9, dccache
	  NOP

	SYNC
	MOVW	R10, M(STATUS)		/* restore processor level */
	JRHB(31)			/* return and clear all hazards */

	SCHED

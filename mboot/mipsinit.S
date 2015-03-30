
#include "regs_mips.h"
#include "ls2h.h"

/************************************************************/

	.text
	.set	noreorder

_text_start:
	nop
	nop
	nop
	nop

	bal		_get_pc
	nop
_get_pc:
	la		$s0, _get_pc
	subu	$s0, $ra, $s0
	beqz	$s0, _in_ddr
	nop

/************************************************************/

#define CLOCK_CTRL0 0x220
#define CLOCK_CTRL2 0x228

_sys_pll_cfg:
	lui		$v0, 0xbfd0
	li		$v1, 0x08f1
	sw		$v1, CLOCK_CTRL2($v0)
	li		$v1, 0x08f0
	sw		$v1, CLOCK_CTRL2($v0)

	li		$v1, 3
1:	bnez	$v1, 1b
	addiu	$v1, -1

	li		$v1, 0x00f1
	sw		$v1, CLOCK_CTRL2($v0)

	li		$v1, 0x40
1:	bnez	$v1, 1b
	addiu	$v1, -1

	li		$v1, 0x10f1
	sw		$v1, CLOCK_CTRL2($v0)
	li		$v1, 0x10f0
	sw		$v1, CLOCK_CTRL2($v0)

_cpu_pll_cfg:
	li		$v1, 0x1a540980
	sw		$v1, CLOCK_CTRL0($v0)
	li		$v1, 0x1a550981
	sw		$v1, CLOCK_CTRL0($v0)

	li		$v1, 0x10
1:	bnez	$v1, 1b
	addiu	$v1, -1

	li		$v1, 0x1a510181
	sw		$v1, CLOCK_CTRL0($v0)

	li		$v1, 0x400
1:	bnez	$v1, 1b
	addiu	$v1, -1

	li		$v1, 0x1a531181
	sw		$v1, CLOCK_CTRL0($v0)
	li		$v1, 0x1a521180
	sw		$v1, CLOCK_CTRL0($v0)

/************************************************************/


_dcache_init:
	li		$v0, 0x22
	mtc0	$v0, CP0_ECC
	mtc0	$0 , CP0_TAGHI
	mtc0	$0 , CP0_TAGLO
	lui		$v0, 0x8000
	ori		$v1, $v0, 0x4000  # 64k/4way
1:
	cache	Index_Store_Tag_D, 0($v0)
	cache	Index_Store_Tag_D, 1($v0)
	cache	Index_Store_Tag_D, 2($v0)
	cache	Index_Store_Tag_D, 3($v0)
	bltu	$v0, $v1, 1b
	addiu	$v0, 0x20

_icache_init:
	mtc0	$0 , CP0_ECC
	mtc0	$0 , CP0_TAGHI
	mtc0	$0 , CP0_TAGLO
	lui		$v0, 0x8000
	ori		$v1, $v0, 0x4000  # 64k/4way
1:
	cache	Index_Store_Tag_I, 0($v0)
	cache	Index_Store_Tag_I, 1($v0)
	cache	Index_Store_Tag_I, 2($v0)
	cache	Index_Store_Tag_I, 3($v0)
	bltu	$v0, $v1, 1b
	addiu	$v0, 0x20


_scache_init:
	li		$v0, 0x22
	mtc0	$v0, CP0_ECC
	mtc0	$0 , CP0_TAGHI
	mtc0	$0 , CP0_TAGLO
	lui		$v0, 0x8000
	lui		$v1, 0x8002  # 512k/4way
1:
	cache	Index_Store_Tag_S, 0($v0)
	cache	Index_Store_Tag_S, 1($v0)
	cache	Index_Store_Tag_S, 2($v0)
	cache	Index_Store_Tag_S, 3($v0)
	bltu	$v0, $v1, 1b
	addiu	$v0, 0x20

/************************************************************/


_jump_to_kseg0:
	li      $v0, 0x644000e0
	mtc0    $v0, CP0_STATUS
	li      $v0, 0
	mtc0    $v0, CP0_CAUSE
	nop

	mfc0	$v0, CP0_CONFIG
	ori		$v0, 7
	xori	$v0, 4
	mtc0	$v0, CP0_CONFIG


	la		$v0, _in_kseg0
	li		$v1, 0x0fffffff
	and		$v0, $v1
	li		$v1, 0x90000000
	or		$v0, $v1
	jr		$v0
	nop
_in_kseg0:
	la		$s0, _in_kseg0
	subu	$s0, $v0, $s0

/************************************************************/


_ddr3_init:
	li		$v0, 0xbfd00000
	lw		$t0, 0x0200($v0)
	li		$t1, 0xffffdfff
	and		$t0, $t1
	sw		$t0, 0x0200($v0)

	la		$a0, ddr3_param_list
	addu	$a0, $s0
	addiu	$t0, $a0, 180*8
	li		$v1, 0xaff00000
1:
	ld		$a1, 0($a0)
	sd		$a1, 0($v1)
	addiu	$a0, 8
	bne		$a0, $t0, 1b
	addiu	$v1, 16

	li		$v1, 0xaff00000
	ld		$a0, 0x30($v1)
	dli		$a1, 0x0000010000000000
	or		$a0, $a1
	sd		$a0, 0x30($v1)

	li		$v0, 0xbfd00000
	lw		$t0, 0x0200($v0)
	ori		$t0, 0x2000
	sw		$t0, 0x0200($v0)

/************************************************************/

_uart_init:
	bal		uart_init
	nop

	la		$a0, _str_boot
	bal		puts
	addu	$a0, $s0

	la		$a0, _str_config
	bal		puts
	addu	$a0, $s0
	mfc0	$a0, CP0_CONFIG
	bal		puthex
	li		$a1, 28
	bal		put_return
	nop
	la		$a0, _str_status
	bal		puts
	addu	$a0, $s0
	mfc0	$a0, CP0_STATUS
	bal		puthex
	li		$a1, 28
	bal		put_return
	nop
	
/************************************************************/

_reloc_code:
	la		$v1, _text_start
	addu	$v0, $v1, $s0
	la		$a0, _data_end
2:
	lw		$a1, 0($v0)
	sw		$a1, 0($v1)
	addiu	$v1, 4
	bne		$v1, $a0, 2b
	addiu	$v0, 4

	la		$v0, _in_ddr
	jr		$v0
	nop

_in_ddr:
	li		$s0, 0


/************************************************************/


_call_main:
	la		$sp, _stack_end
	subu	$sp, 16
	la		$a0, _text_start
	la		$v0, main
	jr		$v0
	nop

/************************************************************/


FUNC_ENTRY(uart_init)
	li		$v0, UART0_BASE
	li		$a0, (SYS_CLK/16/UART_BAUD)
	li		$v1, 0x83
	sb		$v1, 3($v0) # LCR
	sb		$a0, 0($v0) # DLL
	srl		$a0, 8
	sb		$a0, 1($v0) # DLH
	li		$v1, 0x03
	sb		$v1, 3($v0) # LCR
	li		$v1, 0x87
	sb		$v1, 2($v0) # FCR
	li		$v1, 0x01
	sb		$v1, 1($v0) # IER

	jr		$ra
	nop
FUNC_END(uart_init)

FUNC_ENTRY(putc)
	li		$v0, UART0_BASE
	sb		$a0, 0($v0)

1:
	lbu		$v1, 5($v0)
	and		$v1, 0x40
	beqz	$v1, 1b
	nop

	jr		$ra
	nop
FUNC_END(putc)

FUNC_ENTRY(puthex)
	move	$a2, $ra
	move	$a3, $a0
1:
	srlv	$a0, $a3, $a1
	andi	$a0, 0x0f
	addiu	$a0, 0x30
	blt		$a0, 0x3a, 3f
	nop
	addiu	$a0, 0x07
3:
	bal		putc
	subu	$a1, 4

	bgez	$a1, 1b
	move	$ra, $a2

	jr		$ra
	nop
FUNC_END(puthex)

FUNC_ENTRY(puts)
	move	$a2, $ra
	move	$a1, $a0
1:
	lbu		$a0, 0($a1)
	beqz	$a0, 2f
	move	$ra, $a2

	bal		putc
	nop
	b		1b
	addiu	$a1, 1
2:
	jr		$ra
	nop
FUNC_END(puts)

FUNC_ENTRY(put_return)
	move	$a2, $ra
	bal		putc
	li		$a0, 0x0d
	bal		putc
	li		$a0, 0x0a
	move	$ra, $a2
	jr		$ra
	nop
FUNC_END(put_return)

/************************************************************/

_str_boot:
	.asciz "\r\nBoot From MIPS!\r\n"
_str_config:
	.asciz "CP0_CONFIG: "
_str_status:
	.asciz "CP0_STATUS: "

/************************************************************/

FUNC_ENTRY(getc)
	li		$v0, 400000
	multu	$a0, $v0
	mtc0	$0, CP0_COUNT
	li		$v0, UART0_BASE
	mflo	$a0

2:
	lbu		$v1, 5($v0)
	and		$v1, 0x01
	beqz	$v1, 1f
	nop

	jr		$ra
	lbu		$v0, 0($v0)
1:
	mfc0	$v1, CP0_COUNT
	nop
	bltu	$v1, $a0, 2b
	nop

	jr		$ra
	lui		$v0, 0xffff
FUNC_END(getc)

/************************************************************/

FUNC_ENTRY(get_clock_tick_rate)
	li      $v0, 400000000
	jr      $ra
	nop
FUNC_END(get_clock_tick_rate)

FUNC_ENTRY(get_timer)
	jr      $ra
	mfc0    $v0, CP0_COUNT
FUNC_END(get_timer)
	    
FUNC_ENTRY(reset_timer)
	jr      $ra
	mtc0    $0, CP0_COUNT
FUNC_END(reset_timer) 

FUNC_ENTRY(udelay)
	li		$v0, 400
	multu	$a0, $v0
	mflo	$v0
	mtc0	$0, CP0_COUNT
1:
	mfc0	$v1, CP0_COUNT
	bltu	$v1, $v0, 1b
	nop

	jr		$ra
	nop
FUNC_END(udelay)


FUNC_ENTRY(get_cp0_status)
	mfc0    $v0, CP0_STATUS
	nop     
	jr      $ra
	nop
FUNC_END(get_cp0_status)

FUNC_ENTRY(set_cp0_status)
	mtc0    $a0, CP0_STATUS
	nop
	jr      $ra
	nop
FUNC_END(set_cp0_status)

FUNC_ENTRY(tlb_init)
	li		$a0, TLB_TABLE_NUM
	li		$a1, 0
	li		$v0, PAGE16K_MASK

	mtc0	$0, CP0_ENTRYHI
	mtc0	$0, CP0_ENTRYLO0
	mtc0	$0, CP0_ENTRYLO1
	mtc0	$v0, CP0_PAGEMASK
1:
	mtc0	$a1, CP0_INDEX
	addiu	$a1, $a1, 1
	bne		$a1, $a0, 1b
	tlbwi
	jr		$ra	
	nop
FUNC_END(tlb_init)

FUNC_ENTRY(fpu_init)
	li      $v0, 0
	ctc1    $v0, $31

	li		$t1, -1
	dmtc1	$t1, $f0
	dmtc1	$t1, $f1
	dmtc1	$t1, $f2
	dmtc1	$t1, $f3
	dmtc1	$t1, $f4
	dmtc1	$t1, $f5
	dmtc1	$t1, $f6
	dmtc1	$t1, $f7
	dmtc1	$t1, $f8
	dmtc1	$t1, $f9
	dmtc1	$t1, $f10
	dmtc1	$t1, $f11
	dmtc1	$t1, $f12
	dmtc1	$t1, $f13
	dmtc1	$t1, $f14
	dmtc1	$t1, $f15
	dmtc1	$t1, $f16
	dmtc1	$t1, $f17
	dmtc1	$t1, $f18
	dmtc1	$t1, $f19
	dmtc1	$t1, $f20
	dmtc1	$t1, $f21
	dmtc1	$t1, $f22
	dmtc1	$t1, $f23
	dmtc1	$t1, $f24
	dmtc1	$t1, $f25
	dmtc1	$t1, $f26
	dmtc1	$t1, $f27
	dmtc1	$t1, $f28
	dmtc1	$t1, $f29
	dmtc1	$t1, $f30
	dmtc1	$t1, $f31

	jr		$ra
	nop

FUNC_END(fpu_init)

/************************************************************/


#include "ddr3_param.S"

/************************************************************/


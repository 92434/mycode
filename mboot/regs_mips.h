
#ifndef _REGS_MIPS_H_
#define _REGS_MIPS_H_


#define VA_TO_PA(x)    (((u32)(x))&0x1fffffff)
#define PA_TO_UVA(x)   (((u32)(x))|0xa0000000)



#define Index_Store_Tag_I   0x08
#define Index_Store_Tag_D   0x09
#define Index_Store_Tag_S   0x0B


#define CP0_INDEX      $0
#define CP0_RANDOM     $1
#define CP0_ENTRYLO0   $2
#define CP0_ENTRYLO1   $3
#define CP0_CONF       $3
#define CP0_CONTEXT    $4
#define CP0_PAGEMASK   $5
#define CP0_WIRED      $6
#define CP0_INFO       $7
#define CP0_BADVADDR   $8
#define CP0_COUNT      $9
#define CP0_ENTRYHI    $10
#define CP0_COMPARE    $11
#define CP0_STATUS     $12
#define CP0_CAUSE      $13
#define CP0_EPC        $14
#define CP0_PRID       $15
#define CP0_CONFIG     $16
#define CP0_LLADDR     $17
#define CP0_WATCHLO    $18
#define CP0_WATCHHI    $19
#define CP0_XCONTEXT   $20
#define CP0_FRAMEMASK  $21
#define CP0_DIAGNOSTIC $22
#define CP0_DEBUG      $23
#define CP0_DEPC       $24
#define CP0_PERFORMANCE $25
#define CP0_ECC        $26
#define CP0_CACHEERR   $27
#define CP0_TAGLO      $28
#define CP0_TAGHI      $29
#define CP0_ERROREPC   $30
#define CP0_DESAVE     $31


#define TLB_TABLE_NUM 64

#define PAGE4K_MASK  (0x00 << 13)
#define PAGE16K_MASK (0x03 << 13)



#define FUNC_ENTRY(x) \
	.global x; \
	.align 2; \
	.type x, @function; \
	.ent x, 0; \
x :

#define FUNC_END(x) \
	.end x; \
	.size x, .-x;







#endif


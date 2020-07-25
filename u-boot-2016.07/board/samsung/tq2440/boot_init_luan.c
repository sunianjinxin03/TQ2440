//#include <common.h>
//#include <asm/arch/s3c24x0_cpu.h>
//#include <asm/sections.h>
#define NAND_SECTOR_SIZE	512
#define NAND_BLOCK_MASK	(NAND_SECTOR_SIZE - 1)

#define NAND_SECTOR_SIZE_LP	2048
#define NAND_BLOCK_MASK_LP	(NAND_SECTOR_SIZE_LP - 1)

#define NFCONF (*((volatile unsigned long *)0x4E000000))
#define NFCONT (*((volatile unsigned long *)0x4E000004))
#define NFCMMD (*((volatile unsigned char *)0x4E000008))
#define NFADDR (*((volatile unsigned char *)0x4E00000C))
#define NFDATA (*((volatile unsigned char *)0x4E000010))
#define NFSTAT (*((volatile unsigned char *)0x4E000020))
#define UPLLCON (*((volatile unsigned long *)0x4C000008))

void nand_init_ll(void);
int nand_read_ll(unsigned char *buf, unsigned long start_addr, int size);
int nand_read_ll_lp(unsigned char *buf, unsigned long start_addr, int size);

/* NAND Flash操作的总入口, 它们将调用S3C2410或S3C2440的相应函数 */
static void nand_reset(void);
static void wait_idle(void);
static void nand_select_chip(void);
static void nand_deselect_chip(void);
static void write_cmd(int cmd);
static void write_addr(unsigned int addr);
static void write_addr_lp(unsigned int addr);
static unsigned char read_data(void);
int NF_ReadID(void);				//HJ_add 20090807


static void nand_reset(void)
{
	nand_select_chip();
	write_cmd(0xff);  // 复位命令
	wait_idle();
	nand_deselect_chip();
}

static void wait_idle(void)
{
	int i;
    while (!(NFSTAT & 1))
        for(i=0; i<10; i++);

}

static void nand_select_chip(void)
{
	int i;
	NFCONT &= ~(1<<1);
	for(i=0; i<10; i++); 
}

static void nand_deselect_chip(void)
{
	NFCONT |= (1<<1); 
}

static void write_cmd(int cmd)
{
	volatile int i;	
	NFCMMD = cmd;	
	for (i = 0; i < 10; i++);
}
static void write_addr(unsigned int addr)
{
	int i;

	NFADDR = addr & 0xff;
	for(i=0; i<10; i++);
	NFADDR = (addr >> 9) & 0xff;
	for(i=0; i<10; i++);
	NFADDR = (addr >> 17) & 0xff;
	for(i=0; i<10; i++);
	NFADDR = (addr >> 25) & 0xff;
	for(i=0; i<10; i++);	
}

static void write_addr_lp(unsigned int addr)
{	
	volatile unsigned int i;
	unsigned int col, page;

	col = addr & NAND_BLOCK_MASK_LP;
	page = addr / NAND_SECTOR_SIZE_LP;
	
	NFADDR = col & 0xff;			/* Column Address A0~A7 */
	for(i=0; i<10; i++);		
	NFADDR = (col >> 8) & 0x0f;		/* Column Address A8~A11 */
	for(i=0; i<10; i++);

	NFADDR = page & 0xff;			/* Row Address A12~A19 */
	for(i=0; i<10; i++);
	NFADDR = (page >> 8) & 0xff;	/* Row Address A20~A27 */
	for(i=0; i<10; i++);

	NFADDR = (page >> 16) & 0x03;	/* Row Address A28~A29 */
	for(i=0; i<10; i++);

}

static unsigned char read_data(void)
{
	return NFDATA;
}

/* 初始化NAND Flash */
void nand_init_ll(void)
{
	#define TACLS   2
	#define TWRPH0  1
	#define TWRPH1  0

	/* ����ʱ�� */	
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);	
	/* ʹ��NAND Flash������, ��ʼ��ECC, ��ֹƬѡ */	
	NFCONT =(1<<4)|(1<<1)|(1<<0);

	/* 复位NAND Flash */
	nand_reset();
}
#if 1
int NF_ReadID(void)
{
	char pMID;
	char pDID;
	int  nBuff;
	char   n4thcycle;
	int i;
	
	b128MB = 1;
	n4thcycle = nBuff = 0;

	nand_init_ll();
	nand_select_chip();
	write_cmd(0x90);	// read id command
	NFADDR = 0x00 & 0xff;
	for ( i = 0; i < 100; i++ );

	pMID = read_data();
	pDID =  read_data();
	nBuff =  read_data();
	n4thcycle = read_data();

	nand_deselect_chip();
	
	if (pDID >= 0xA0)
	{
		b128MB = 0;
	}

	return (pDID);
}
#endif

/* 读函数 */
int nand_read_ll(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;
	char dat;
    
	if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK))
	{
		return -1;    /* 地址或长度不对齐 */
	}

	/* 选中芯片 */
	nand_select_chip();

	for(i=start_addr; i < (start_addr + size);)
	{
     /* Check Bad Block */
     if(1){
		/* 发出READ0命令 */
		write_cmd(0x50);

		NFADDR = 5;
		for(j=0; j<10; j++);
		NFADDR = (i >> 9) & 0xff;
		for(j=0; j<10; j++);
		NFADDR = (i >> 17) & 0xff;
		for(j=0; j<10; j++);
		NFADDR = (i >> 25) & 0xff;
		for(j=0; j<10; j++);
		wait_idle();
		dat = read_data();
		write_cmd(0);
		
		/* 取消片选信号 */
		nand_deselect_chip();
		if(dat != 0xff)
			i += 16384;		// 1 Block = 512*32= 16384
        /* Read Page */
		/* 选中芯片 */
		nand_select_chip();
      }
		/* 发出READ0命令 */
		write_cmd(0);

		/* Write Address */
		write_addr(i);
		wait_idle();

		for(j=0; j < NAND_SECTOR_SIZE; j++, i++)
		{
			*buf = read_data();
			buf++;
		}
	}

	/* 取消片选信号 */
	nand_deselect_chip();

	return 0;
}

/* 读函数 
  * Large Page
  */
int nand_read_ll_lp(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;
	char dat;
	
	if ((start_addr & NAND_BLOCK_MASK_LP) || (size & NAND_BLOCK_MASK_LP))
	{
		return -1;    /* 地址或长度不对齐 */
	}

	/* 选中芯片 */
	nand_select_chip();

	for(i=start_addr; i < (start_addr + size);)
	{
      /* Check Bad Block */
      if(1){
		int col, page;

		col = i & NAND_BLOCK_MASK_LP;
		page = i / NAND_SECTOR_SIZE_LP;
		/* 发出READ0命令 */
		write_cmd(0x00);

		NFADDR = 5;
		for(j=0; j<10; j++);
		NFADDR = 8;
		for(j=0; j<10; j++);
		NFADDR = page & 0xff;		/* Row Address A12~A19 */
		for(j=0; j<10; j++);
		NFADDR = (page >> 8) & 0xff;		/* Row Address A20~A27 */
		for(j=0; j<10; j++);
        if (b128MB == 0)
		    NFADDR = (page >> 16) & 0x03;		/* Row Address A28~A29 */
		for(j=0; j<10; j++);

		write_cmd(0x30);
		wait_idle();
		dat = read_data();
		
		/* 取消片选信号 */
		nand_deselect_chip();
		if(dat != 0xff)
			i += 131072;		// 1 Block = 2048*64= 131072
        /* Read Page */
		/* 选中芯片 */
		nand_select_chip();
        }
		/* 发出READ0命令 */
		write_cmd(0);

		/* Write Address */
		write_addr_lp(i);
		write_cmd(0x30);
		wait_idle();

		for(j=0; j < NAND_SECTOR_SIZE_LP; j++, i++)
		{
			*buf = read_data();
			buf++;
		}
	}

	/* 取消片选信号 */
	nand_deselect_chip();

	return 0;
}

int bBootFrmNORFlash(void)
{
	volatile unsigned int *pdw = (volatile unsigned int *)0;
	unsigned int dwVal;

	/*
	 * 无论是从NOR Flash还是从NAND Flash启动，
	 * 地址0处为指令"b	Reset", 机器码为0xEA00000B，
	 * 对于从NAND Flash启动的情况，其开始4KB的代码会复制到CPU内部4K内存中，
	 * 对于从NOR Flash启动的情况，NOR Flash的开始地址即为0。
	 * 对于NOR Flash，必须通过一定的命令序列才能写数据，
	 * 所以可以根据这点差别来分辨是从NAND Flash还是NOR Flash启动:
	 * 向地址0写入一个数据，然后读出来，如果没有改变的话就是NOR Flash
	 */

	dwVal = *pdw;       
	*pdw = 0x12345678;
	if (*pdw != 0x12345678)
	{
		return 1;
	}
	else
	{
		*pdw = dwVal;
		return 0;
	}
}

int CopyCode2Ram(unsigned long start_addr, unsigned char *buf, int size)
{
	unsigned int *pdwDest;
	unsigned int *pdwSrc;
	int i;

	if (bBootFrmNORFlash())
	{
		pdwDest = (unsigned int *)buf;
		pdwSrc  = (unsigned int *)start_addr;
		/* 从 NOR Flash启动 */
		for (i = 0; i < size / 4; i++)
		{
			pdwDest[i] = pdwSrc[i];
		}
		puts("in flash");
		return 0;
	}
	else
	{
		/* 初始化NAND Flash */
		nand_init_ll();

		/* 从 NAND Flash启动 */
		if (NF_ReadID() == 0x76 )
			nand_read_ll(buf, start_addr, (size + NAND_BLOCK_MASK)&~(NAND_BLOCK_MASK));
		else
			nand_read_ll_lp(buf, start_addr, (size + NAND_BLOCK_MASK_LP)&~(NAND_BLOCK_MASK_LP));
		puts("in nand");
		return 0;
	}
}

#if 1
void clear_bss(void)
{
	int *p = (int *)&__bss_start;
	while(p < (int *)&__bss_end) {
		*p++ = 0;
	}
}
#endif


static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n"
			"subs %0, %1, #1\n"
			"bne 1b":"=r" (loops):"0" (loops));
}


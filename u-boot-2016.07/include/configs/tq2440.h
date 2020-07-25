/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <garyj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * Configuation settings for the SAMSUNG SMDK2410 board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_S3C24X0		/* This is a SAMSUNG S3C24x0-type SoC */
#define CONFIG_S3C2440		/* specifically a SAMSUNG S3C2410 SoC */
/* #define CONFIG_SMDK2410 */		/* on a SAMSUNG SMDK2410 Board */
/* #define CONFIG_SKIP_LOWLEVEL_INIT */ 
/* #define CONFIG_SYS_TEXT_BASE	0x0 */
/* #define CONFIG_SYS_TEXT_BASE		0x33000000 */
#define CONFIG_SYS_TEXT_BASE	0x33D00000 /*编译地址*/
#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH

/* #define DEBUG */ 	
/* enable 128MB SDRAM */
/*#define CONFIG_128MB_SDRAM				0*/

/* input clock of PLL (the SMDK2410 has 12MHz input clock) */
#define CONFIG_SYS_CLK_FREQ	12000000

#define CONFIG_CMDLINE_TAG	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

/*
 * Hardware drivers
 */
#if defined(CONFIG_S3C2440)
#define    CONFIG_CMD_NET
#define    CONFIG_DRIVER_DM9000
#define	   CONFIG_DM9000_NO_SROM   1
#define    CONFIG_DM9000_BASE     0x20000300 /*tq2440开发板的网卡dm9000接在S3C2440的bank4 */
#define    DM9000_IO     CONFIG_DM9000_BASE
#define    DM9000_DATA (CONFIG_DM9000_BASE+4) /*tq2440开发板的网卡dm9000的cmd引脚接在S3C2440的ADDR2 打开之前第一步暂时注释掉的宏 */
#define    CONFIG_CMD_PING 
#define    CONFIG_DM9000_USE_16BIT 1
#define    CONFIG_NET_MULTI
#else
#define CONFIG_CS8900		/* we have a CS8900 on-board */
#define CONFIG_CS8900_BASE	0x19000300
#define CONFIG_CS8900_BUS16	/* the Linux driver does accesses as shorts */
#endif
/*
 * select serial console configuration
 */
#define CONFIG_S3C24X0_SERIAL
#define CONFIG_SERIAL1		1	/* we use SERIAL 1 on SMDK2410 */

/************************************************************
 * USB support (currently only works with D-cache off)
 ************************************************************/
#if 0
#define CONFIG_USB_OHCI
#define CONFIG_USB_OHCI_S3C24XX
/* #define CONFIG_USB_KEYBOARD */
/* #define CONFIG_USB_STORAGE */
/*#define CONFIG_DOS_PARTITION*/
#endif
/************************************************************
 * RTC
 ************************************************************/
#define CONFIG_RTC_S3C24X0

#define CONFIG_BAUDRATE		115200

/*
 * BOOTP options
 */
#if 0
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#endif

/*
 * Command line configuration.
 */
#define CONFIG_CMD_BSP
/*#define CONFIG_CMD_DATE*/
#define CONFIG_CMD_CACHE
#define CONFIG_CMD_NAND
#define CONFIG_CMD_REGINFO

#define CONFIG_CMDLINE_EDITING

/* autoboot */
#define CONFIG_BOOT_RETRY_TIME	-1
#define CONFIG_RESET_TO_RETRY

#define CONFIG_NETMASK		255.255.255.0
#define CONFIG_IPADDR		192.168.1.15
#define CONFIG_SERVERIP		192.168.1.7
#define CONFIG_ETHADDR      00:1a:2b:3c:4d:5e
#define CONFIG_GATEWAYIP    192.168.1.1


#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200	/* speed to run kgdb serial port */
#endif

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_CBSIZE	256
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE + \
				sizeof(CONFIG_SYS_PROMPT)+16)
#define CONFIG_SYS_MAXARGS	16
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE

#define CONFIG_DISPLAY_CPUINFO				/* Display cpu info */

#define CONFIG_SYS_MEMTEST_START	0x30000000	/* memtest works on */

#ifdef	CONFIG_128MB_SDRAM
#define	CONFIG_SYS_MEMTEST_END 	    0x37F00000	/* 127 MB in DRAM	*/
#else
#define	CONFIG_SYS_MEMTEST_END	    0x33F00000	/* 63 MB in DRAM	*/
#endif

#ifdef CONFIG_128MB_SDRAM
#define CONFIG_SYS_LOAD_ADDR		0x37000000
#else
#define	CONFIG_SYS_LOAD_ADDR	    0x33000000	
/*sdram加载缓冲地址*/
#endif

/* support additional compression methods */

#define CONFIG_BZIP2
#define CONFIG_LZO
#define CONFIG_LZMA


/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1          /* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		0x30000000 /* SDRAM Bank #1 */
#if CONFIG_128MB_SDRAM
#define PHYS_SDRAM_1_SIZE				0x08000000 /* 128 MB */
#else
#define PHYS_SDRAM_1_SIZE				0x04000000 /* 64 MB */
#endif

#define PHYS_FLASH_1		0x00000000 /* Flash Bank #0 */

#define CONFIG_SYS_FLASH_BASE	PHYS_FLASH_1
#define CONFIG_SYS_FLASH_LEGACY_1024Kx16
/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_FLASH_CFI_LEGACY
/* #define CONFIG_SYS_FLASH_LEGACY_512Kx16 */
#define CONFIG_FLASH_SHOW_PROGRESS	45

#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_FLASH_BANKS_LIST     { CONFIG_SYS_FLASH_BASE }
#define CONFIG_SYS_MAX_FLASH_SECT	(35)
#if 0
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + 0x070000)
#define CONFIG_ENV_IS_IN_FLASH
#define CONFIG_ENV_SIZE			0x10000
#endif

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OVERWRITE
#define CONFIG_ENV_OFFSET 0x40000 // 256K for u-boot
#define CONFIG_ENV_SIZE 0x20000 // 128K for environmen

/*
 * Size of malloc() pool
 * BZIP2 / LZO / LZMA need a lot of RAM
 */
#define CONFIG_SYS_MALLOC_LEN	(2 * 1024 * 1024)
#define CONFIG_SYS_MONITOR_LEN	(448 * 1024)
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_FLASH_BASE

/*
 * NAND configuration
 */
#ifdef CONFIG_CMD_NAND
/* #define CONFIG_NAND_S3C2410 */
#define CONFIG_SYS_S3C2440_NAND_HWECC
#define CONFIG_NAND_S3C2440
#define CONFIG_S3C2440_NAND_HWECC
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x4E000000
#define CONFIG_SYS_NAND_ECCSIZE           2048     /* 单次校验字节长度 */
#define CONFIG_SYS_NAND_ECCBYTES             4  /* ECC字节长度     */
#define CONFIG_CMD_NAND_YAFFS
#endif

/*
 * File system
 */
 /*
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_RBTREE
#define CONFIG_YAFFS2
*/

#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define MTDIDS_DEFAULT "nand0=s3c_nand"
/*#define MTDPARTS_DEFAULT "mtdparts=s3c_nand:256k@0(uboot),128k(env),6M(kernel),-(rootfs)"*/
#define  MTDPARTS_DEFAULT "mtdparts=s3c_nand:256k@0(bios)," \ 
                          		 		 "128k(params)," \
                         		  		 "128k(toc)," \
                         		  		 "512k(eboot)," \
                        	 	  		 "1024k(logo)," \
                         		  		 "3m(kernel)," \
                         		  		 "-(root)"	
#if CONFIG_128MB_SDRAM
#define CONFIG_BOOTARGS	"noinitrd root=/dev/mtdblock2 rootfstype=yaffs2 init=/linuxrc console=ttySAC0 mem=128M"
#else
#define CONFIG_BOOTARGS	"noinitrd root=/dev/mtdblock2 rootfstype=yaffs2 init=/linuxrc console=ttySAC0,115200"
#endif
										 
/*#define CONFIG_BOOTARGS	"root=/dev/mtdblock2 rootfstype=yaffs2 init=/linuxrc console=ttySAC0,115200"*/
#define CONFIG_BOOTCOMMAND	"nand read 30008000 kernel 0x00300000;bootm 30008000"
								 
/* additions for new relocation code, must be added to all boards */
#define CONFIG_SYS_SDRAM_BASE	PHYS_SDRAM_1
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_SDRAM_BASE + 0x1000 - \
				GENERATED_GBL_DATA_SIZE)

#define CONFIG_BOARD_EARLY_INIT_F

#endif /* __CONFIG_H */

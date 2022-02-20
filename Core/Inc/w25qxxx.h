/*
 * w25qxxx.h
 *
 *  Created on: Feb 13, 2022
 *      Author: Maxiufeng
 */

/*
 * 			W25Q64	 64M-bit ->  8M-Byte
 * 			W25Q128	128M-bit -> 16M-Byte
 * 			W25Q256	256M-bit -> 32M-Byte
 */

/**
  * Use polling to transmit SPI data
  */

/**
  * @brief Implement w25qxxx low-level operations using standard spi and HAL libraries
  *		Provides a set of interface functions for diskio.c through a function pointer structure
  *		 If using FATFs, the following #define is set to 1
  * 	   #define EN_SPI_FLASH_DISK_IO 	0	//1:enable spi_flash_disk_io 0:disable
  */

#ifndef INC_W25QXXX_H_
#define INC_W25QXXX_H_

#include "stm32f4xx_hal.h"


/* Extern variables ---------------------------------------------------------*/
extern 	SPI_HandleTypeDef 	hspi5;
#define hspi_flash			hspi5

#define	SPI_CS_PORT		GPIOF
#define SPI_CS_PIN		GPIO_PIN_6


/**
  * @brief Indicate the equipment used
  */
typedef enum
{
	W25Q10 = 1,
	W25Q20,
	W25Q40,
	W25Q80,
	W25Q16,
	W25Q32,
	W25Q64,
	W25Q128,
	W25Q256,
	W25Q512,

} W25QXXX_ID_t;


/**
  * @brief W25Qxxx spi Command type define [unsigned char]
  * 	Follow the dataSheet
  */
typedef enum{
	CMD_DUMMY					=	(unsigned char)(0x00),

	CMD_Write_Enable 			=	(unsigned char)(0x06),
	CMD_Write_Enable_SR			=	(unsigned char)(0x50),
	CMD_Write_Disable			=	(unsigned char)(0x04),
	CMD_Release_Power_Down 		=   (unsigned char)(0xAB),
	CMD_Device_ID				=	(unsigned char)(0xAB),
	CMD_Manufacture_ID			=	(unsigned char)(0x90),
	CMD_JEDEC_ID				=	(unsigned char)(0x9F),
	CMD_Unique_ID				=	(unsigned char)(0x4B),

	CMD_Read_Data				=	(unsigned char)(0x03),
	CMD_Fast_Read				=	(unsigned char)(0x0B),

	CMD_Page_Program			=	(unsigned char)(0x02),

	CMD_Erase_Sector			=	(unsigned char)(0x20),	//  4KB
	CMD_Erase_Block_32K			=	(unsigned char)(0x52),	// 32KB
	CMD_Erase_Block_64K			=	(unsigned char)(0xD8),	// 64KB
	CMD_Erase_Chip				=	(unsigned char)(0xC7),	// chip

	CMD_Reg_1_Read				=	(unsigned char)(0x05),
	CMD_Reg_1_Write				=	(unsigned char)(0x01),
	CMD_Reg_2_Read				=	(unsigned char)(0x35),
	CMD_Reg_2_Write				=	(unsigned char)(0x31),
	CMD_Reg_3_Read				=	(unsigned char)(0x15),
	CMD_Reg_3_Write				=	(unsigned char)(0x11),

	CMD_Read_SFDP_Reg			=	(unsigned char)(0x5A),
	CMD_Erase_Security_Reg  	=	(unsigned char)(0x44),
	CMD_Program_Security_Reg	=	(unsigned char)(0x42),
	CMD_Read_Security_Reg		=	(unsigned char)(0x48),

	CMD_Global_Block_Lock		=	(unsigned char)(0x7E),
	CMD_Global_Block_Unlock 	=	(unsigned char)(0x98),
	CMD_Read_Block_Lock			=	(unsigned char)(0x3D),
	CMD_Individual_Block_Lock 	=	(unsigned char)(0x36),
	CMD_Individual_Block_Unlock	=	(unsigned char)(0x39),

	CMD_Erase_Program_Suspend	=	(unsigned char)(0x75),
	CMD_Erase_Program_Resume	=	(unsigned char)(0x7A),
	CMD_Power_Down				=	(unsigned char)(0xB9),

	CMD_Enable_Reset			=	(unsigned char)(0x66),
	CMD_Reset_Device			=	(unsigned char)(0x99),

	/* 4 Byte Address Mode Instructions <high capacity> */
	CMD_Read_Data_4_Byte_Addr		=	(unsigned char)(0x13),
	CMD_Fast_Read_4_Byte_Addr		=	(unsigned char)(0x0C),

	CMD_Page_Program_4_Byte_Addr	=	(unsigned char)(0x12),

	CMD_Erase_Sector_4_Byte_Addr	=	(unsigned char)(0x21),	// 4KB
	CMD_Erase_Block_64K_4_Byte_Addr	=	(unsigned char)(0xDC),	// 64KB

	CMD_Read_Ex_Addr_Reg			=	(unsigned char)(0xC8),
	CMD_Write_Ex_Addr_Reg			=	(unsigned char)(0xC5),

	CMD_Enter_4_Byte_Addr_Mode		=	(unsigned char)(0xB7),
	CMD_Exit_4_Byte_Addr_Mode		=	(unsigned char)(0xE9),
}W25QxxxCMD_TypeDef;


/**
  * @brief W25Qxxx Status Register bit mask
  */
typedef enum {
	SR1_S0_BUSY		=	(unsigned char)(1<<0),	// Erase/Write In Progress (BUSY) – Status Only
	SR1_S1_WEL		=	(unsigned char)(1<<1),	// Write Enable Latch (WEL) – Status Only

	SR2_S9_QE		=	(unsigned char)(1<<1),	// Quad Enable (QE) – Volatile/Non-Volatile Writable
	SR2_S15_SUS		=	(unsigned char)(1<<7),	// Erase/Program Suspend Status (SUS) – Status Only

	SR3_S18_WPS		=	(unsigned char)(1<<3),	// Write Protect Selection (WPS) – Volatile/Non-Volatile Writable

	/* 4 Byte Address Mode Instructions <high capacity> */
	SR3_S16_ADS		=	(unsigned char)(1<<0),	// Current Address Mode (ADS) – Status Only
	SR3_S17_ADP		=	(unsigned char)(1<<1),	// Power-Up Address Mode (ADP) – Non-Volatile Writable

	/* high capacity flash  Extended Address Register */
	SR_ADDR_EX_A24_EA0		=	(unsigned char)(1<<0),
	SR_ADDR_EX_A25_EA1		=	(unsigned char)(1<<1),
}W25QxxxSRBitMask_TypeDef;


uint8_t W25Qxxx_Read_REG_x(uint8_t reg_x);
void W25Qxxx_Write_REG_x(uint8_t reg_x, uint8_t data);

uint8_t W25Qxxx_Init(void);
uint8_t W25Qxxx_EraseChip(void);
uint8_t W25Qxxx_EraseSector(uint32_t SectorAddr);
uint8_t W25Qxxx_EraseBlock(uint32_t BlockAddr);
uint8_t W25Qxxx_WriteByte(const uint8_t pBuffer, uint32_t WriteAddr_inBytes);
uint8_t W25Qxxx_WritePage(const uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize);
uint8_t W25Qxxx_WriteSector(const uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize);
uint8_t W25Qxxx_WriteBlock(const uint8_t *pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_BlockSize);
uint8_t W25Qxxx_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address);
uint8_t W25Qxxx_ReadBytes(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint8_t W25Qxxx_ReadPage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_PageSize);
uint8_t W25Qxxx_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize);
uint8_t W25Qxxx_ReadBlock(uint8_t *pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_BlockSize);


// #########################################################################################
/* ---------------- FATFs disk io functions --------------------------------------------- */
#define EN_SPI_FLASH_DISK_IO 	1	//1:enable spi_flash_disk_io 0:disable

#if 	EN_SPI_FLASH_DISK_IO
#include "diskio.h"
/**
  * @brief  Disk IO Driver structure definition
  */
typedef struct
{
  DSTATUS (*disk_initialize) (void);                     /*!< Initialize Disk Drive                     */
  DSTATUS (*disk_status)     (void);                     /*!< Get Disk Status                           */
  DRESULT (*disk_read)       (BYTE*, DWORD, UINT);       /*!< Read Sector(s)                            */
  DRESULT (*disk_write)      (const BYTE*, DWORD, UINT); /*!< Write Sector(s) when _USE_WRITE = 0       */
  DRESULT (*disk_ioctl)      (BYTE, void*);              /*!< I/O control operation when _USE_IOCTL = 1 */

}SPIFLASHDiskio_drvTypeDef;


/* Externally declare spi-flash io driver structure variables *******************************************/
extern const SPIFLASHDiskio_drvTypeDef  SPI_Flash_Driver;

/**
  * @brief  spi-flash status structure definition
  */
#define   FLASH_OK                        ((uint8_t)0x00)
#define   FLASH_ERROR                     ((uint8_t)0x01)

/**
  * @brief  spi-flash transfer state definition
  */
#define   SD_TRANSFER_OK                ((uint8_t)0x00)
#define   SD_TRANSFER_BUSY              ((uint8_t)0x01)

#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)
#define SD_DATATIMEOUT           ((uint32_t)100000000)
#endif /* EN_SPI_FLASH_DISK_IO */
/* -------------------------- END -------------------------- */


#endif /* INC_W25QXXX_H_ */

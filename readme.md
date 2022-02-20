# 1.Functional description [W25Qxxx.c/h]

W25Qxxx spi flash HAL library driver, including basic information acquisition and function implementation required by FATFs file system diskio.c.      **use stm32f429igt6**

**Applicable to the following models of flash memory.**

	W25Q10,
	W25Q20,
	W25Q40,
	W25Q80,
	W25Q16,
	W25Q32,
	W25Q64,
	W25Q128,
	W25Q256,
	W25Q512,
## 1.1 Implementation of basic operations.  [W25Qxxx.h]

```c
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
```

## 1.2 diskio.c interface program part  [W25Qxxx.h]

**Related to FATFs.**

```c
#define EN_SPI_FLASH_DISK_IO 	1	//1:enable spi_flash_disk_io 0:disable
```

```c
typedef struct
{
  DSTATUS (*disk_initialize) (void);    				 /*!< Initialize Disk Drive */
  DSTATUS (*disk_status)     (void);                     /*!< Get Disk Status */
  DRESULT (*disk_read)       (BYTE*, DWORD, UINT);       /*!< Read Sector(s) */
  DRESULT (*disk_write)      (const BYTE*, DWORD, UINT); /*!< Write Sector(s) when _USE_WRITE = 0 */
  DRESULT (*disk_ioctl)      (BYTE, void*); /*!< I/O control operation when _USE_IOCTL = 1 */
}SPIFLASHDiskio_drvTypeDef;
```

# 2. [diskio.c] settings

```c
#include "w25qxxx.h"
```

**FATFs add low-level driver functions.** 

```c
/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
  DSTATUS stat;

  stat = SPI_Flash_Driver.disk_status();

  return stat;
}

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  DSTATUS stat = RES_OK;

  stat = SPI_Flash_Driver.disk_initialize();

  return stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
  DRESULT res;

  res = SPI_Flash_Driver.disk_read(buff, sector, count);
  return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT disk_write (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count        	/* Number of sectors to write */
)
{
  DRESULT res;

  res = SPI_Flash_Driver.disk_write(buff, sector, count);

  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  DRESULT res;

  res = SPI_Flash_Driver.disk_ioctl(cmd, buff);
  return res;
}
#endif /* _USE_IOCTL == 1 */
```

# 3. [file_handling.c/h]

It encapsulates disk file scanning and capacity information acquisition and provides function methods.

```c
FRESULT ff_getCapacity(const TCHAR* path, DWORD* free_byte, DWORD* total_byte);
FRESULT ff_scanDisk (TCHAR* path);
```

# 4.[main.c] provides basic test examples

```c
  /* USER CODE BEGIN 2 */

  HAL_Delay(2000);

  fres = f_mount(&fs, "0:", 1);
  if (fres == FR_OK)
  {
	  printf("mount w25qxxx flash passed...\r\n");

	  //fres = f_opendir(&dp, "0:/dataInfo");

	  fres = f_open(&fp, "0:/dataInfo/info.txt", FA_OPEN_ALWAYS|FA_WRITE);
	  if (fres == FR_OK)
	  {
		  printf("open file ok...\r\n");
		  static uint8_t buff[] = "1234567890\r\nabcdefghijklmnopqrstuvwxyz\r\n!@#$%^&*()...";
		  UINT bw[1];
		  fres = f_write(&fp, buff, sizeof(buff), bw);
		  if (fres == FR_OK)
		  {
			  printf("write file passed...\r\n");

			  fres = f_close(&fp);
			  if (fres == FR_OK)
				  printf("close file passed...\r\n");
			  else
				  printf("close file failed...\r\n");

//			  f_closedir(&dp);
		  }
		  else
			  printf("write file failed...\r\n");
	  }
	  else
		  printf("open file failed...\r\n");


	  DWORD free_byte[1], total_byte[1];
	  fres = ff_getCapacity("0:", free_byte, total_byte);
	  if (fres == FR_OK)
	  {
		  printf("##################################\r\n");
		  printf(" total capacity : %06u KiByte\r\n", total_byte[0]);
		  printf(" free  capacity : %06u KiByte\r\n",  free_byte[0]);
		  printf("##################################\r\n");

		  TCHAR path[50] = "0:";
		  fres = ff_scanDisk(path);
		  if (fres == FR_OK)
			  printf("scan flash disk end...\r\n");
	  }

	  fres = f_mount(NULL, "0:", 1);
	  if (fres == FR_OK)
		  printf("unmount passed...\r\n");
	  else {
		printf("unmount failed...\r\n");
	}
  }
  else
	  printf("mount w25qxxx flash failed...\r\n");

  /* USER CODE END 2 */
```

# 5.some necessary configuration

**Using STM32CubeMx configuration Peripherals and Parameters**

|      | Polling                                   |
| ---- | ----------------------------------------- |
| SPI  | Mode: Full-Duplex Master     NSS: Disable |

**Change the following definitions according to the actual spix peripherals and CS pins used [you need to confirm by yourself]**     [W25Qxxx.h]

```c
extern 	SPI_HandleTypeDef 	hspi5;
#define hspi_flash			hspi5

#define	SPI_CS_PORT		GPIOF
#define SPI_CS_PIN		GPIO_PIN_6
```


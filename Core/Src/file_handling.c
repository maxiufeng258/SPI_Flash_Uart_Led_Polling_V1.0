/*
 * file_handling.c
 *
 *  Created on: Feb 12, 2022
 *      Author: Maxiufeng
 */

#include "file_handling.h"
#include "string.h"
#include "stdio.h"
/**
  * @brief  Get Capacity of Disk Device
  * @param	*path      :[IN]  Logical drive number (0..9)
  * @param	*free_byte :[OUT] kByte of free Capacity
  * @param  *total_byte:[OUT] kByte of total Capacity
  * @retval DRESULT: Operation result
  */
FRESULT ff_getCapacity (const TCHAR* path, DWORD* free_byte, DWORD* total_byte)
{
	FRESULT fRes = FR_OK;
	DWORD fre_clust;
	DWORD sect_size = _MIN_SS;
	double fre_sect, tot_sect, coef;
	FATFS *fs;

    fRes = f_getfree(path, &fre_clust, &fs);
    if (fRes != FR_OK)
    {
    	return fRes;
    }

#if _MAX_SS != _MIN_SS
    sect_size = fs->ssize;
#endif

    /* Get total sectors and free sectors */
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;

    coef = ((double)sect_size)/(1024.0);

    *total_byte = tot_sect * coef;
    *free_byte  = fre_sect * coef;

    return FR_OK;
}

/**
  * @brief  Scan directories and files under the specified path
  *		// TCHAR path[100] = "0:/";
  *		// ff_scanDisk(path)
  * @param	*path  :[IN]  Start node to be scanned
  * @retval DRESULT: Operation result
  */
FRESULT ff_scanDisk (TCHAR* path)
{
	FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       	   /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = ff_scanDisk(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}


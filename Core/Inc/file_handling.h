/*
 * file_handling.h
 *
 *  Created on: Feb 12, 2022
 *      Author: Maxiufeng
 */

#ifndef INC_FILE_HANDLING_H_
#define INC_FILE_HANDLING_H_

/**
 * @note
 * 	Include this file into programs that require disk/file operations
 */

#include "ff.h"

FRESULT ff_getCapacity(const TCHAR* path, DWORD* free_byte, DWORD* total_byte);
FRESULT ff_scanDisk (TCHAR* path);

#endif /* INC_FILE_HANDLING_H_ */

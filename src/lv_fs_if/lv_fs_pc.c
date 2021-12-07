/**
 * @file lv_fs_pc.c
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include "lv_fs_if.h"
#if LV_USE_FS_IF
#if LV_FS_IF_PC != '\0'

#include <stdio.h>
#include <errno.h>
#include "dirent.h"
#ifdef WIN32
#include <windows.h>
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_fs_res_t fs_open (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos);
static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p);
static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);
static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path);
static lv_fs_res_t fs_rename (lv_fs_drv_t * drv, const char * oldname, const char * newname);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Register a driver for the File system interface
 */
void lv_fs_if_pc_init(void)
{
	/*---------------------------------------------------
	 * Register the file system interface  in LittlevGL
	 *--------------------------------------------------*/

	/* Add a simple drive to open images */
	static lv_fs_drv_t fs_drv;                         /*A driver descriptor*/
	lv_fs_drv_init(&fs_drv);

	/*Set up fields...*/
	fs_drv.letter = LV_FS_IF_PC;
	fs_drv.open_cb = fs_open;
	fs_drv.close_cb = fs_close;
	fs_drv.read_cb = fs_read;
	fs_drv.write_cb = fs_write;
	fs_drv.seek_cb = fs_seek;
	fs_drv.size_cb = fs_size;
	fs_drv.tell_cb = fs_tell;
	fs_drv.remove_cb = fs_remove;
    fs_drv.rename_cb = fs_rename;

	lv_fs_drv_register(&fs_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Open a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_open(lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode)
{
    FILE* f = NULL;
    char *flags = "";

    if(mode == LV_FS_MODE_WR) flags = "wb";
    else if(mode == LV_FS_MODE_RD) flags = "rb";
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) flags = "rb+";
    
    f = fopen(path, flags);

    if (f == NULL) {
        (void)file_p;
        return LV_FS_RES_NOT_EX;
    } else {
        *((FILE *)file_p) = *f;
        return LV_FS_RES_OK;
    }
}


/**
 * Close an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p)
{
   (void)drv;
    FILE* fp = (FILE *)file_p;
    if (fp == NULL) {
        return LV_FS_RES_INV_PARAM;
    }
    fclose(fp);
    return LV_FS_RES_OK;
}

/**
 * Read data from an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    (void)drv;
    FILE* fp = (FILE *)file_p;
    *br = fread(buf, 1, btr, fp);
    return LV_FS_RES_OK;
}

/**
 * Write into a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
	(void)drv;
    FILE* fp = (FILE *)file_p;
    *bw = fwrite(buf, 1, btw, fp);
    return LV_FS_RES_OK;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos)
{
    (void)drv;
    FILE* fp = (FILE *)file_p;
    fseek(fp, pos, SEEK_SET);
    return LV_FS_RES_OK;
}

/**
 * Give the size of a file bytes
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param size pointer to a variable to store the size
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p)
{
	(void)drv;
    FILE* fp = (FILE *)file_p;
    uint32_t pos_bak = ftell(fp);
    // bakcup position
    fseek(fp, 0, SEEK_END);
    *size_p = ftell(fp);
    // recover position
    fseek(fp, pos_bak, SEEK_SET);
    return LV_FS_RES_OK;
}

/**
 * Give the position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
	(void)drv;
    FILE* fp = (FILE *)file_p;
    *pos_p = ftell(fp);
    return LV_FS_RES_OK;
}

/**
 * Delete a file
 * @param drv pointer to a driver where this function belongs
 * @param path path of the file to delete
 * @return  LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path)
{
    (void)drv;
    if (remove(path) == 0) {
        return LV_FS_RES_OK;
    } else {
        return LV_FS_RES_UNKNOWN;
    }
}

/**
 * Rename a file
 * @param drv pointer to a driver where this function belongs
 * @param oldname path to the file
 * @param newname path with the new name
 * @return LV_FS_RES_OK or any error from 'fs_res_t'
 */
static lv_fs_res_t fs_rename (lv_fs_drv_t * drv, const char * oldname, const char * newname)
{
    (void)drv;
    if (rename(oldname, newname) == 0) {
        return LV_FS_RES_OK;
    } else {
        return LV_FS_RES_NOT_EX;
    }
}

#endif	/*LV_USE_FS_IF*/
#endif  /*LV_FS_IF_PC*/

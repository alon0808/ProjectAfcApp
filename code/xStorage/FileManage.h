#ifndef _INCLUDED_FILE_MANAGE_H
#define _INCLUDED_FILE_MANAGE_H

// file type
typedef enum {
	fnt_invalid = -1,
	fnt_pboc_cakey,
	fnt_max_value,
}emFileNameType;

// file access mode
typedef enum {
	fam_write = 1,
	fam_read = 2,
	fam_create = 4,
}emFileAccessMode;


#ifdef __cplusplus
extern "C"
{
#endif
	/**
	* @Description open the file handle, close last handle when it is opening
	* @Param fileNameType(int) - type of file
	* @Param accessMode(int) - access mode(struct emFileAccessMode)
	* @Return int - return code(struct ERet_Code)
	*/
	extern int FmOpen(int fileNameType, int accessMode);

	extern int FmWriteOnce(int fileNameType, void *pData, int datLen);

	extern int FmWrite(int fileNameType, int sIndex, void *pData, int dLen);

	extern int FmRead(int fileNameType, int sIndex, void *pData, int dLen);
	/**
	 * @Description close the file handle
	 * @Param fileNameType(int) - type of file
	 * @Return int - return code(struct ERet_Code)
	 */
	extern int FmClose(int fileNameType);


#ifdef __cplusplus
}
#endif

#endif

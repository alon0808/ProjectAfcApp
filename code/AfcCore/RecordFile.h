

extern void initRecordFilePara(void);
extern unsigned int Get_Record_point(unsigned char *headp, unsigned char mode);
//指示头指针或尾指针加rLen. mode=0为尾指针加rlen, mode=1为头指针加rLen
extern int ADD_Record_point(unsigned char mode, unsigned int rLen);
extern void writeBackRec(unsigned char*dat, int len, unsigned char mode);
extern int FR_flashwrite(unsigned int addr, unsigned char *writebuf, unsigned int length);
extern int FR_flashread(unsigned int addr,unsigned char *rec_data,unsigned int length);
//检测有没有需要采集的记录
// =0 有信息错误，不操作
// 1 要处理信息
extern int Check_Send_FAT_Rcord(stFAT_hisRec *hisinfor);
extern int SaveFAT_hisRecInfor(unsigned char num, stFAT_hisRec *hisinfor);

//读取文件的内容
extern int GetFileDatac(char *filename, int offset, int len, unsigned char *obuf);
//打开一个文件，如果不存在则新建。
extern int file_open_creat(char *filename);

extern stFileRecordPoint recordPoint;//指向当前在记录内的写入偏移的指针0-2M
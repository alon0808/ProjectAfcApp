

extern void initRecordFilePara(void);
extern unsigned int Get_Record_point(unsigned char *headp, unsigned char mode);
//ָʾͷָ���βָ���rLen. mode=0Ϊβָ���rlen, mode=1Ϊͷָ���rLen
extern int ADD_Record_point(unsigned char mode, unsigned int rLen);
extern void writeBackRec(unsigned char*dat, int len, unsigned char mode);
extern int FR_flashwrite(unsigned int addr, unsigned char *writebuf, unsigned int length);
extern int FR_flashread(unsigned int addr,unsigned char *rec_data,unsigned int length);
//�����û����Ҫ�ɼ��ļ�¼
// =0 ����Ϣ���󣬲�����
// 1 Ҫ������Ϣ
extern int Check_Send_FAT_Rcord(stFAT_hisRec *hisinfor);
extern int SaveFAT_hisRecInfor(unsigned char num, stFAT_hisRec *hisinfor);

//��ȡ�ļ�������
extern int GetFileDatac(char *filename, int offset, int len, unsigned char *obuf);
//��һ���ļ���������������½���
extern int file_open_creat(char *filename);

extern stFileRecordPoint recordPoint;//ָ��ǰ�ڼ�¼�ڵ�д��ƫ�Ƶ�ָ��0-2M
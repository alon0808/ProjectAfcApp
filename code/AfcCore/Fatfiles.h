
//�����ļ�ϵͳ������ֻ��Ҫ����һ��
extern void checkFlashDisFAT(void);

//���ļ���д��¼���ļ���¼λ��0:/curdatĿ¼�¡�д��¼�ļ���ʱ�䣺172ms��������265ms
//ֻ��Ҫд����ʷ�ļ���31���ļ�(�ļ���Ϊhis01-his31)���µ�һ���¸��Ǿɵġ���ǰ�ļ�ģʽ����.��һ��д���ļ�����������ʱ�䣬������ǵ�����򸲸ǣ��ǵ�������д��
//buffer:Ҫд��ļ�¼ָ��
//mode:д�뷽ʽ��0=д��ʷ�ļ���1=д��ǰ�ļ�,����curdat�ļ����¡�
extern int WriteRecordFAT(unsigned char *wbuf, unsigned char mode);
//extern unsigned char WriteRecordFAT(unsigned char *buffer, unsigned char mode);
//��ʽ��FLASH FAT�ļ�ϵͳ��������curdatĿ¼
extern unsigned char format_disk_RW(void);





#ifndef _SQD_H_
#define _SQD_H_
typedef struct
{
	unsigned char ack_flge[2];
	unsigned char ack_code;
	unsigned char ack_time[7];
	unsigned char ack_NS[3];
	unsigned char res[19];
}oda_ack;//32?¨¢??¡§2

extern int Send_WaitRecvData(unsigned char SQmode, int msecends);
extern void set_flag_aut(unsigned char data);
extern unsigned write_linux_re(unsigned char mode);
extern unsigned char write_linux_re_build(unsigned char transResult);
extern oda_ack ODAACK;
extern void ODA_ACK_INIT(void);
#endif

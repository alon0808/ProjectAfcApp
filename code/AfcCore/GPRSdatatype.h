


//���ػ������־
#define MISS_IF1 0x10//��Ҫ��½��IF1
#define MISS_M_SINGIN 0x11		//0x01 ��½�ƶ�,ǩ��.
#define MISS_M_Settlement 0x12	//0x02 ������
#define MISS_M_TradeBatch 0x13	//0x03 �����ͽ���
#define MISS_IF2 0x20//��Ҫ��½��IF2
#define MISS_M_DPARAM 0x21		//0x01 ���ز���
#define MISS_M_DPGRAM 0x22		//0x02 ������������
#define MISS_M_DPGRAMOK 0x23	//0x03 �������ؽ��֪ͨ
#define MISS_M_KEK 0x24			//0x04 KEK����
#define MISS_M_ActiveTest 0x25	//0x05 ����

#define MISS_GJ 0x40	//��Ҫ��½������
#define MISS_G_LOGINGJ 0x40		//0xF0 ��½������̨
#define MISS_G_DBLKD 0x41		//0xF1 ���غ����� ��  ��������������һ���������Ҫ����
#define MISS_G_UREC 0x42		//2 �ϴ�����
#define MISS_G_FILES 0x43		//3 �����ļ� ����,���е��ļ����ض�ʹ�ô�����
#define MISS_G_DLine	0x44		//3 �����ļ� ��·����
#define MISS_G_DSound	0x45		//3 �����ļ� ��������
#define MISS_G_DPROOK	0x46	//���س�����
#define MISS_G_DBLKI 0x47		//0xF1 ���غ����� ��
#define MISS_G_DLineOK	0x48	//���س�����
#define MISS_G_DSoundOK	0x49	//���س�����
#define MISS_G_HART 0x4A		//������
#define MISS_G_GPS 0x4B			//GPS massage
#define MISS_G_TOO 0x4C			//�ն˻���ָ֪�� -2002: �ն˻����������ն˻�������Ϣ�����ջ���
#define MISS_G_ALAM 0x4D		//���ͱ�����GPS��Ϣ
#define MISS_G_PRICE 0x4E		//����Ʊ����·��Ϣ
#define MISS_G_FREE 0x4F		//���ӵ����������ǿ�����

#define MISS_G_TREC 0x80		//ʵʱ����Ķ�ά������

#define MISS_G_MSG1 0x60		//��Ҫ���Ͷ���
#define MISS_G_MSG2 0x61		//��Ҫ��������

#define MISS_HTTP 0x80			//80���ϵ�������HTTP���ص�
#define MISS_HTTP_BLK 0x81		//ͨ��LINUXģ�����غ������ļ� 
#define MISS_HTTP_EC20 0x82		//ͨ��LINUXģ������LInuxģ����� 
#define MISS_HTTP_PRO 0x83		//ͨ��LINUXģ�����س��ػ����� 

#define MISS_G_SINGOUT 0x70		//�ն�ǩ��

#define MISS_COM_PARA 0x80		//�������س�����
//ǩ��״̬��0λ��KEK�����أ�1λ�����������أ�2λ������������
#define LoadStyle_KEK (0x01<<0)
#define LoadStyle_para (0x01<<1)
#define LoadStyle_program (0x01<<2)


//GPRS����ͨѶ״̬
#define GPRS_LINK_CMD	0xF0//ģ��ر���
#define GPRS_TCPCLOSE_CMD 0xF1//TCPIP���ӹر���

//����MC55i,��Ҫ������ʹ��ƽΪ��
#define GPRS_LINK_UP 0xF3
//GPRS���ڵ�ģ��ע��
#define GPRS_AT_WAIT 0xF4
//����ر�ģ�飬AT^SMSO
#define GPRS_MC55_CLOSE 0xF5
//ģ����Ҫ����
#define GPRS_MODE_NEED_CLOSE 0xF6
//ģ���µ�
#define GPRS_MODE_POWER_DOWN 0xF7
//ģ���ϵ�
#define GPRS_MODE_POWER_ON 0xF8
//AT�����λָ��
#define GPRS_AT_RESET 0xFA
//ģ���Դ�ر�Ȼ��򿪣����¿�ʼGPRS����
#define GPRS_HWPOWER_RESET 0xFE


//ģ����Ҫ�ر�TCP/IP
#define GPRS_NEED_CLOSEIP 0x2F

#define GPRS_Out_CMD 0xF2//GPRS����
#define GPRS_SENDING_CMD 0xA0//���ڷ������ݣ�����Ӧ����
#define GPRS_RING_CMD 0xA1//�е绰����
#define GPRS_CStatues 0xA2	//�ڹ����в�ѯWIFI�����Ƿ����������ڼ�������Ƿ��뿪(��û��ʹ��)

#define TCPSTARTSTAT	22	//7TCP/IP��ʼֵ��֮ǰ��ֵ��ģ���ʼ������


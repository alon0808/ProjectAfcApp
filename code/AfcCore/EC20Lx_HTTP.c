#include "Macro_Proj.h"
#include "EC20Lx_HTTP.h"
#include <stdio.h>//printf
#include <string.h>//字符串处理
#include <sys/socket.h>//套接字
#include <arpa/inet.h>//ip地址处理
#include <fcntl.h>//open系统调用
#include <unistd.h>//write系统调用
#include <netdb.h>//查询DNS
#include <stdlib.h>//exit函数
#include <dirent.h>
#include <sys/stat.h>//stat系统调用获取文件大小
#include <sys/time.h>//获取下载时间
#include <errno.h>
#include <pthread.h>
#include <errno.h>

#include "GprsSocket.h"
#include "szct.h"


#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif


#define DownDir	"/mnt/appbak/"

struct HTTP_RES_HEADER//保持相应头信息
{
	int status_code;//HTTP/1.1 '200' OK
	char content_type[128];//Content-Type: application/gzip
	long content_length;//Content-Length: 11683079
};

typedef struct
{
	int Dtype;	//下载类型
	long hasrecieve;		//已经接收到的文件大小
	long allLen;			//文件总大小
	char FFlag[3];			//文件标识
	char FVer[2];			//文件版本
	char Filename[32];		//文件名
	char HttpAddr[2048];	//url
}stHttpDownInfo;


typedef struct
{
	unsigned char dflag;		//如果此值为EA则需要把版本信息发送给主机
	unsigned char busProName[16];//主机（卡机）程序版本
	unsigned char CSN_BUSName[16];//票价版本
	unsigned char busLineName[16];//站点限速信息版本2B
	unsigned char busVoiceName[16];//公钥信息版本2B   如果有，需要转成16进制数

	unsigned char White_Name[16];//白名单信息版本  低位在后
	unsigned char White_JTB_Name[16];//白名单信息版本  低位在后
	unsigned char BLKName[16];//黑名单版本
	unsigned char LineName[16];//线路信息版本
	unsigned char EC20ProName[16];//主机（卡机）程序版本
}stEc20VerInfo;

stHttpDownInfo gHttpDinfo;
stEc20VerInfo gEc20VerInfo;	//


							//传入文件标识，取出文件名
void getFilesName(char *FFlag, char *obuf)
{
	if (memcmp(FFlag, SL8583FileFLAG_BLK, 3) == 0)
		strcpy(obuf, gEc20VerInfo.BLKName);
	else if (memcmp(FFlag, SL8583FileFLAG_PRO, 3) == 0)
		strcpy(obuf, gEc20VerInfo.busProName);
	else if (memcmp(FFlag, SL8583FileFLAG_GPS, 3) == 0)
		strcpy(obuf, gEc20VerInfo.busLineName);
	else if (memcmp(FFlag, SL8583FileFLAG_CSN, 3) == 0)
		strcpy(obuf, gEc20VerInfo.CSN_BUSName);
	else if (memcmp(FFlag, SL8583FileFLAG_WHT, 3) == 0)
		strcpy(obuf, gEc20VerInfo.White_JTB_Name);
	else if (memcmp(FFlag, SL8583FileFLAG_EC20, 3) == 0)
		strcpy(obuf, gEc20VerInfo.EC20ProName);
	else
		obuf[0] = 0;

}

unsigned long FindIndexFiles(char *fullName, char *obuf)
{
	int fd, ret;
	int l = 0;

#ifdef _debug_
	printf("Black index FileName:%s\r\n", fullName);
#endif

	if (access(fullName, F_OK) != -1) {
		fd = open(fullName, O_RDONLY);
		if (fd >= 0) {
			ret = read(fd, obuf, 16);
			if (ret >= 0) {
				printf("read file %s data%s:\n", fullName, obuf);
				l = 99;
			}
			else {
				printf("read %s fail!\n", fullName);
				perror("read");
			}
		}
		else
		{
			printf("open %s fail!\n", fullName);
			perror("open");
		}

	}
	// 	else{
	// 		printf("NO file:%s??\r\n", fullName);
	// 	}
	return l;
}
void getEc20FilesVer(void)
{
	unsigned long len = 0;
	char fileName[256];

	memset((unsigned char*)&gEc20VerInfo, 0, sizeof(gEc20VerInfo));

	//查找黑名单文件信息
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, DownDir);
	strcat(fileName, "/BLKindex");
	FindIndexFiles(fileName, gEc20VerInfo.BLKName);

	//查找车载机程序
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, DownDir);
	strcat(fileName, "/PROindex");
	len = FindIndexFiles(fileName, gEc20VerInfo.busProName);

	//查找EC20程序
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, DownDir);
	strcat(fileName, "/EC2index");
	len = FindIndexFiles(fileName, gEc20VerInfo.EC20ProName);

	gEc20VerInfo.dflag = 0xEA;
}


void parse_url(const char *url, char *host, int *port, char *file_name)
{
	/*通过url解析出域名, 端口, 以及文件名*/
	int j = 0;
	int start = 0;
	*port = 80;
	char *patterns[] = { "http://", "https://", NULL };
	int i;
	for (i = 0; patterns[i]; i++)//分离下载地址中的http协议
		if (strncmp(url, patterns[i], strlen(patterns[i])) == 0)
			start = strlen(patterns[i]);

	//解析域名, 这里处理时域名后面的端口号会保留
	for (i = start; url[i] != '/' && url[i] != '\0'; i++, j++)
		host[j] = url[i];
	host[j] = '\0';

	//解析端口号, 如果没有, 那么设置端口为80
	char *pos = strstr(host, ":");
	if (pos)
		sscanf(pos, ":%d", port);

	//删除域名端口号
	for (i = 0; i < (int)strlen(host); i++)
	{
		if (host[i] == ':')
		{
			host[i] = '\0';
			break;
		}
	}

	//获取下载文件名
	j = 0;
	for (i = start; url[i] != '\0'; i++)
	{
		if (url[i] == '/')
		{
			if (i != strlen(url) - 1)
				j = 0;
			continue;
		}
		else
			file_name[j++] = url[i];
	}
	file_name[j] = '\0';
}

struct HTTP_RES_HEADER parse_header(const char *response)
{
	/*获取响应头的信息*/
	struct HTTP_RES_HEADER resp;

	char *pos = strstr(response, "HTTP/");
	if (pos)//获取返回代码
		sscanf(pos, "%*s %d", &resp.status_code);

	pos = strstr(response, "Content-Type:");
	if (pos)//获取返回文档类型
		sscanf(pos, "%*s %s", resp.content_type);

	pos = strstr(response, "Content-Length:");
	if (pos)//获取返回文档长度
		sscanf(pos, "%*s %ld", &resp.content_length);

	return resp;
}

void get_ip_addr(char *host_name, char *ip_addr)
{
	/*通过域名得到相应的ip地址*/
	struct hostent *host = gethostbyname(host_name);//此函数将会访问DNS服务器
	if (!host)
	{
		ip_addr = NULL;
		return;
	}
	int i;
	for (i = 0; host->h_addr_list[i]; i++)
	{
		strcpy(ip_addr, inet_ntoa(*(struct in_addr*) host->h_addr_list[i]));
		break;
	}
}


void progress_bar(long cur_size, long total_size, double speed)
{
	/*用于显示下载进度条*/
	float percent = (float)cur_size / total_size;
	const int numTotal = 50;
	int numShow = (int)(numTotal * percent);

	if (numShow == 0)
		numShow = 1;

	if (numShow > numTotal)
		numShow = numTotal;

	char sign[51] = { 0 };
	memset(sign, '=', numTotal);

	printf("\r%.2f%%[%-*.*s] %.2f/%.2fMB %4.0fkb/s", percent * 100, numTotal, numShow, sign, cur_size / 1024.0 / 1024.0, total_size / 1024.0 / 1024.0, speed);
	fflush(stdout);


	if (numShow == numTotal)
		printf("\n");
}

unsigned long get_file_size(const char *filename)
{
	//通过系统调用直接得到文件的大小
	struct stat buf;
	if (stat(filename, &buf) < 0)
		return 0;
	return (unsigned long)buf.st_size;
}

//extern void com_send_data_OTHER_CMD(unsigned char socketindex, unsigned char cmd, unsigned int len, unsigned char *dat);
void download(int client_socket, char *file_name, long content_length, char *shortFilename)
{
	/*下载文件函数*/
	long hasrecieve = 0;//记录已经下载的长度
	struct timeval t_start, t_end;//记录一次读取的时间起点和终点, 计算速度
	int mem_size = 8192;//缓冲区大小8K
	int buf_len = mem_size;//理想状态每次读取8K大小的字节流
	int len;

	//创建文件描述符
	int fd = open(file_name, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
	if (fd < 0)
	{
		printf("文件创建失败!\n");
		return;
	}

	char *buf = (char *)malloc(mem_size * sizeof(char));

	//从套接字流中读取文件流
	long diff = 0;
	int prelen = 0;
	double speed;

	int pos = 0;
	unsigned char sndbuf[128];

	while (hasrecieve < content_length)
	{
		gettimeofday(&t_start, NULL); //获取开始时间
		len = read(client_socket, buf, buf_len);
		write(fd, buf, len);
		gettimeofday(&t_end, NULL); //获取结束时间

		hasrecieve += len;//更新已经下载的长度

						  //计算速度
		if (t_end.tv_usec - t_start.tv_usec >= 0 && t_end.tv_sec - t_start.tv_sec >= 0)
			diff += 1000000 * (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_usec - t_start.tv_usec);//us

		if (diff >= 1000000)//当一个时间段大于1s=1000000us时, 计算一次速度
		{
			speed = (double)(hasrecieve - prelen) / (double)diff * (1000000.0 / 1024.0);
			prelen = hasrecieve;//清零下载量
			diff = 0;//清零时间段长度

			memset(sndbuf, 0, sizeof(sndbuf));	//发送进度到主机,速度太快时，小于1秒钟下载完成则进不来。
			sndbuf[pos++] = 2;
			strcpy((char *)sndbuf + pos, shortFilename);
			pos += 20;
			memcpy(sndbuf + pos, (unsigned char*)&content_length, 4);
			pos += 4;
			memcpy(sndbuf + pos, (unsigned char*)&hasrecieve, 4);
			pos += 4;
			//com_send_data_OTHER_CMD(0, 0x09, pos, sndbuf);//CMD发09，主机不应答，如果要应答需要发29
		}



		//progress_bar(hasrecieve, content_length, speed);

		if (hasrecieve == content_length) {
			memset(sndbuf, 0, sizeof(sndbuf));	//发送进度到主机,完成了，要发。
			sndbuf[pos++] = 2;
			strcpy((char *)sndbuf + pos, shortFilename);
			pos += 20;
			memcpy(sndbuf + pos, (unsigned char*)&content_length, 4);
			pos += 4;
			memcpy(sndbuf + pos, (unsigned char*)&hasrecieve, 4);
			pos += 4;
			//com_send_data_OTHER_CMD(0, 0x09, pos, sndbuf);//CMD发09，主机不应答，如果要应答需要发29
			break;
		}
	}
}

void CheckDownloadFile(const char * file_name, char *findName)
{
	int mem_size = 8192;//缓冲区大小8K
	int buf_len = mem_size;//理想状态每次读取8K大小的字节流
	int len;
	int i, t;
	char name[100], name2[100];
	char newName[64];//最新的黑名单文件
	int newVer = 0;
	char strtokbuffer[100];
	char *token, *token2;
	char *buf = (char *)malloc(mem_size * sizeof(char));


	memset(newName, 0, sizeof(newName));

	printf("检测文件%s\n", file_name);
	int fd = open(file_name, O_CREAT | O_RDONLY, S_IRWXG | S_IRWXO | S_IRWXU);
	if (fd < 0)
	{
		printf("文件创建失败!\n");
		return;
	}
	len = read(fd, buf, buf_len);
	if (len < 0)
	{
		printf("读取文件失败\r\n");
		return;
	}
	//	 for(i=0;i<len;i++)
	//	 {
	//		printf("%X",buf[i]);
	//	 }
	printf("\n下载的内容%d->\n%s\r\n***************************\r\n", len, buf);

	//	token=strtok(buf,"\n");
	//	while(token!=NULL)
	//	{
	//		printf("%s\n",token);
	//		token=strtok(NULL,"\n");
	//	}

	i = 0;
	token = strstr(buf, "a href=");
	while (token != NULL)
	{
		//	token2=strstr(token,"</a>");
		token2 = strstr(token, "\"");
		token2 += 1;
		token = strstr(token2, "\"");
		memcpy(name, token2, (token - token2));

		printf("firs name:%s\n", name);
		memset(strtokbuffer, 0, 100);
		token2 = strstr(token, "</a>");
		token += 1;
		memcpy(name2, token + 1, (token2 - token - 1));
		printf("last name:%s\n", strtokbuffer);

		if (strncmp(name, name2, strlen(name)) == 0)
		{
			printf("是下载链接链接名称-》:%s\n", name);
			if ((memcmp(name, "BLK_", 4) == 0) && (strlen(name) >= 8)) {//黑名单文件
				memset(name2, 0, sizeof(name2));
				memcpy(name2, name + strlen(name) - 4, 4);	//取版本号,只取最后4字节
				t = atol(name2);
				printf("Blk Ver=%d\r\n", t);
				if (t > newVer) {//找到更新的版本
					newVer = t;
					strcpy(newName, name);
				}
			}
		}
		else
		{
			printf("不是下载链接:%s->%s\n", name, name2);
		}
		memset(name, 0, 100);
		memset(name2, 0, 100);
		memset(strtokbuffer, 0, 100);
		token += i;
		token = strstr(token2, "a href=");
	}

	printf("Find new ver File:%s\r\n", newName);
	strcpy(findName, newName);//传出最新的版本文件

}
#define MODE (S_IRWXU | S_IRWXG | S_IRWXO)  
int mk_dir(char *dir)
{
	DIR *mydir = NULL;
	if ((mydir = opendir(dir)) == NULL)//判断目录   
	{
		int ret = mkdir(dir, MODE);//创建目录  
		if (ret != 0)
		{
			return -1;
		}
		printf("%s created sucess!/n", dir);
	}
	else
	{
		printf("%s exist!/n", dir);
	}

	return 0;
}


int processHTTPDown(int mode, char *url, char *DFileName)
{
	/* 命令行参数: 接收两个参数, 第一个是下载地址, 第二个是文件的保存位置和名字, 下载地址是必须的, 默认下载到当前目录
	* 示例: ./download http://www.baidu.com baidu.html
	*/
	//    char url[2048] = "http://120.77.173.234/download/BLK.tar";//设置默认地址为本机,
	char host[64] = { 0 };//远程主机地址
	char ip_addr[16] = { 0 };//远程主机IP地址
	int port = 80;//远程主机端口, http默认80端口
	char file_name[256] = { 0 };//下载文件名
	char downfiletemp[256];

	printf("************************down start****************\n");
	printf("argv[0] =%s\n", url);

	//  strcpy(url, (char*)desAddr);


	if (mk_dir(DownDir) < 0) {
		printf("Creat QR Dir Error\r\n");
		return -1;
	}

	printf("1: 正在解析下载地址...%s\n", url);
	parse_url(url, host, &port, file_name);//从url中分析出主机名, 端口号, 文件名

	puts("2: 正在获取远程服务器IP地址...");
	get_ip_addr(host, ip_addr);//调用函数同访问DNS服务器获取远程主机的IP
	if (strlen(ip_addr) == 0)
	{
		printf("错误: 无法获取到远程服务器的IP地址, 请检查下载地址的有效性\n");
		return -1;
	}

	strcpy(file_name, DownDir);
	strcat(file_name, "/");
	strcat(file_name, DFileName);	//使用传进来的文件名
	puts("\n>>>>下载地址解析成功<<<<");
	printf("\t下载地址: %s\n", url);
	printf("\t远程主机: %s\n", host);
	printf("\tIP 地 址: %s\n", ip_addr);
	printf("\t主机PORT: %d\n", port);
	printf("\t 文件名 : %s\n\n", file_name);


	strcpy(downfiletemp, DownDir);
	strcat(downfiletemp, "/downtemp.tar");

	//设置http请求头信息
	char header[2048] = { 0 };
	sprintf(header, \
		"GET %s HTTP/1.1\r\n"\
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"\
		"User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537(KHTML, like Gecko) Chrome/47.0.2526Safari/537.36\r\n"\
		"Host: %s\r\n"\
		"Connection: keep-alive\r\n"\
		"\r\n"\
		, url, host);

	puts("3: 创建网络套接字...");
	int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket < 0)
	{
		printf("套接字创建失败: %d\n", client_socket);
		return -2;//exit(-1);
	}

	//创建IP地址结构体
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_addr);
	addr.sin_port = htons(port);

	//连接远程主机
	puts("4: 正在连接远程主机...");//如果主机没有服务，则会有1分多钟的超时才退出
	int res = connect(client_socket, (struct sockaddr *) &addr, sizeof(addr));
	if (res == -1)
	{
		printf("连接远程主机失败, error: %d\n", res);
		return -3;//exit(-1);
	}

	puts("5: 正在发送http下载请求...");
	write(client_socket, header, strlen(header));//write系统调用, 将请求header发送给服务器


	int mem_size = 4096;
	int length = 0;
	int len;
	char *buf = (char *)malloc(mem_size * sizeof(char));
	char *response = (char *)malloc(mem_size * sizeof(char));

	//每次单个字符读取响应头信息
	puts("6: 正在解析http响应头...");
	while ((len = read(client_socket, buf, 1)) != 0)
	{
		if (length + len > mem_size)
		{
			//动态内存申请, 因为无法确定响应头内容长度
			mem_size *= 2;
			char * temp = (char *)realloc(response, sizeof(char) * mem_size);
			if (temp == NULL)
			{
				printf("动态内存申请失败\n");
				return -4;//exit(-1);
			}
			response = temp;
		}

		buf[len] = '\0';
		strcat(response, buf);

		//找到响应头的头部信息
		int flag = 0;
		int i;
		for (i = strlen(response) - 1; response[i] == '\n' || response[i] == '\r'; i--, flag++);
		if (flag == 4)//连续两个换行和回车表示已经到达响应头的头尾, 即将出现的就是需要下载的内容
			break;

		length += len;
	}

	struct HTTP_RES_HEADER resp = parse_header(response);

	printf("\n>>>>http响应头解析成功:<<<<\n");

	printf("\tHTTP响应代码: %d\n", resp.status_code);
	if (resp.status_code != 200)
	{
		printf("文件无法下载, 远程主机返回: %d\n", resp.status_code);
		return -5;
	}
	printf("\tHTTP文档类型: %s\n", resp.content_type);
	printf("\tHTTP主体长度: %ld字节\n\n", resp.content_length);



	printf("7: 开始文件下载...\n");

	if (access(downfiletemp, F_OK) != -1)
	{

		printf("%s文件存在，删除\n", downfiletemp);
		remove(downfiletemp);
	}
	else
		printf("没有缓冲文件\n");
	download(client_socket, downfiletemp, resp.content_length, DFileName);
	printf("8: 关闭套接字\n");

	if (resp.content_length == get_file_size(downfiletemp))
	{

		if (rename(downfiletemp, file_name) == 0)
			printf("\n文件%s下载成功! ^_^\n\n", file_name);
		else
		{
			remove(downfiletemp);
			remove(file_name);
			perror("rename");
		}
	}
	else
	{
		remove(file_name);
		printf("\n文件下载中有字节缺失, 下载失败, 请重试!\n\n");
	}

	shutdown(client_socket, 2);//关闭套接字的接收和发送

	if (mode == 0)//下载的目录内容
		CheckDownloadFile(file_name, DFileName);
	return 0;
}



void* main_HTTPDataDown(void *arg)
{
	int n, ret;
	boolean f = FALSE;
	char fullName[256];
	char IndexName[256];
	char tarName[256];
	//	char HttpDes[] = "http://120.77.173.234/download/";		//这里还是使用的固定目录,正式使用时要和公交的通讯服务器地址一样
	//	char HttpDes[] = "http://139.199.161.164:12020/File/Down?merchant=00000000&file=BLKBUS&ver=0012";

	while (1) {
		if (gGprsinfo.isNetOK[LINK_GJ] == TRUE) {
			if (gHttpDinfo.Dtype == HTTP_NEED_DOWN) {
#ifdef _debug_
				printf("HTTPData gHttpDinfo.Filename:%s\r\n", gHttpDinfo.Filename);
#endif
				if (strlen(gHttpDinfo.Filename) != 0) {	//文件名根据应用生成

					strcpy(fullName, DownDir);
					strcat(fullName, "/");
					strcat(fullName, gHttpDinfo.Filename);

					getFilesName(gHttpDinfo.FFlag, IndexName);
#ifdef _debug_
					printf("main HTTP DOWN fullName:%s\r\n", gHttpDinfo.Filename);
					printf("main HTTP DOWN addr:%s\r\n", gHttpDinfo.HttpAddr);
					printf("main HTTP DOWN Afile:%s\r\n", IndexName);
#endif

					if (strcmp(gHttpDinfo.Filename, IndexName) == 0)
					{	//文件存在就不再下载了
						printf("%s File exist!\n", fullName);
						memset(gHttpDinfo.Filename, 0, sizeof(gHttpDinfo.Filename));
						gHttpDinfo.Dtype = HTTP_FINISH_DOWN;
					}
					else {
						ret = processHTTPDown(1, gHttpDinfo.HttpAddr, gHttpDinfo.Filename);
						if (ret == 0)
						{//下载成功
							if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_BLK, 3) == 0)
								strcpy(gEc20VerInfo.BLKName, gHttpDinfo.Filename);
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_PRO, 3) == 0)
								strcpy(gEc20VerInfo.busProName, gHttpDinfo.Filename);
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_EC20, 3) == 0)
								strcpy(gEc20VerInfo.EC20ProName, gHttpDinfo.Filename);


							memset(IndexName, 0, sizeof(IndexName));
							strcpy(IndexName, DownDir);
							strcat(IndexName, "/");
							memcpy(IndexName + strlen(IndexName), gHttpDinfo.FFlag, 3);
							strcat(IndexName, "index");
							if (access(IndexName, F_OK) != -1)
							{
								printf("%s file exist,delete\n", IndexName);
								remove(IndexName);
							}

							int fd = open(IndexName, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);	//写黑名单索引文件
							if (fd >= 0)
							{
								ret = write(fd, gHttpDinfo.Filename, strlen(gHttpDinfo.Filename));//写入原始文件名，文件名=3字节标识_4字节版本号
							}
							close(fd);

							memset(tarName, 0, sizeof(tarName));
							if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_BLK, 3) == 0)
								strcpy(tarName, BLACK_LIST_FILE_NAME);
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_PRO, 3) == 0)
								strcpy(tarName, BUSPRO_LIST_FILE_NAME);
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_EC20, 3) == 0)
								strcpy(tarName, EC20PRO_LIST_FILE_NAME);

							remove(tarName);
							if (rename(fullName, tarName) == 0) {
								printf("\n%s complete! ^_^\n\n", tarName);
							}
							else
							{
								remove(fullName);
								perror("rename");
							}

							if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_EC20, 3) == 0) {
								//把原有程序备份
								if (access("/usrdata/qr/wxhj_bak/", F_OK) == -1) {
									mkdir("/usrdata/qr/wxhj_bak", 0777);
								}

								system("mv -f /usrdata/qr/ec20net /usrdata/qr/wxhj_bak/");

								strcpy(tarName, "tar -xvf ");
								strcat(tarName, EC20PRO_LIST_FILE_NAME);
								strcat(tarName, " -C /usrdata/qr/");

								ret = system(tarName);

								remove(IndexName);
								MSG_LOG("SYStem:ret:%d\r\n", ret);
								sleep(1);
								system("reboot");

							}
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_PRO, 3) == 0) {	//下载了车载机程序，下载完成后需要把程序传给车载机
																								//								SendPROtoBUS();	//在 OtherMission 定时任务内下发
							}
							else {
								getEc20FilesVer();	//重新获取文件版本，把版本信息给主机
							}

							memset(gHttpDinfo.Filename, 0, sizeof(gHttpDinfo.Filename));//清掉文件名，再去找是否有新的文件
							gHttpDinfo.Dtype = HTTP_FINISH_DOWN;
						}
						else {//下载错误，还要下载？
							printf("main_HTTPDown 1 error:%d \n", ret);

							n = 0;
							memset(IndexName, 0, sizeof(IndexName));	//发送进度到主机,完成了，要发。
							IndexName[n++] = 2;
							strcpy((char *)IndexName + n, gHttpDinfo.Filename);
							n += 20;
							memcpy(IndexName + n, "\xFF\xFF\xFF\xFF", 4);
							n += 4;
							memcpy(IndexName + n, "\xFF\xFF\xFF\xFF", 4);
							n += 4;
							//com_send_data_OTHER_CMD(0, 0x09, n, IndexName);//CMD发09，主机不应答，如果要应答需要发29

							memset(gHttpDinfo.Filename, 0, sizeof(gHttpDinfo.Filename));//不下载下载了
							gHttpDinfo.Dtype = HTTP_FINISH_DOWN;
						}
					}
				}
				else {
					gHttpDinfo.Dtype = HTTP_FINISH_DOWN;
				}
			}
		}
		sleep(1);
	}
	pthread_exit((void *)2);  // Explicitly exit the thread, and return (void *)2

}

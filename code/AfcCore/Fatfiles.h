
//挂载文件系统。开机只需要调用一次
extern void checkFlashDisFAT(void);

//向文件内写记录，文件记录位于0:/curdat目录下。写记录文件的时间：172ms，跨扇区265ms
//只需要写入历史文件，31个文件(文件名为his01-his31)，新的一个月覆盖旧的。当前文件模式不变.第一行写入文件建立的日期时间，如果不是当天的则覆盖，是当天的向后写。
//buffer:要写入的记录指针
//mode:写入方式，0=写历史文件；1=写当前文件,都在curdat文件夹下。
extern int WriteRecordFAT(unsigned char *wbuf, unsigned char mode);
//extern unsigned char WriteRecordFAT(unsigned char *buffer, unsigned char mode);
//格式化FLASH FAT文件系统。并建立curdat目录
extern unsigned char format_disk_RW(void);





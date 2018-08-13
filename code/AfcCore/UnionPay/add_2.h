
#ifndef _ADD_2_H_
#define _ADD_2_H_
#include "../include/include.h"



// typedef struct
// {
// 	
// 	unsigned char card_no[20];			//谷?辰?㊣那?“o?
// 	unsigned char c_serial_qpoc[4];  //??角赤?“o?
// 	unsigned char NS[4];				//谷?辰?㊣那芍¯??o?
// 	unsigned char time_last[7];      //谷?辰?㊣那那㊣??
//     unsigned char flge; //㊣那??
// 	unsigned char RES[20];
// 
// }qpboc_pos_infor;

extern int save_infor_add(unsigned char mode, unsigned char *re_infor);
extern unsigned int cal_space_add(void);
extern void lib_clr_Record_point_add(void);
extern void showGPRStR_add_2(void);
extern void write_re_flash(void);
extern void add_2_init_sum(void);
extern void add_2_init_sum_frist(void);
#endif



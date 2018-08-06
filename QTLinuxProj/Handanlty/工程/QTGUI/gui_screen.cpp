
/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_hifb.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/7/1
  Description   : 
  History       :
  1.Date        : 2013/7/1
    Author      : 
    Modification: Created file

******************************************************************************/
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

//#include "sample_comm.h"

#include <linux/fb.h>
#include "hifb.h"
//#include "loadbmp.h"
#include "hi_tde_api.h"
#include "hi_tde_type.h"
#include "hi_tde_errcode.h"


static VO_DEV VoDev = SAMPLE_VO_DEV_DHD0;
static HI_CHAR gs_cExitFlag = 0;

static struct fb_bitfield s_r16 = {10, 5, 0};
static struct fb_bitfield s_g16 = {5, 5, 0};
static struct fb_bitfield s_b16 = {0, 5, 0};
static struct fb_bitfield s_a16 = {15, 1, 0};

static struct fb_bitfield s_a32 = {24,8,0};
static struct fb_bitfield s_r32 = {16,8,0};
static struct fb_bitfield s_g32 = {8,8,0};
static struct fb_bitfield s_b32 = {0,8,0};

#define WIDTH                  1024 //1280
#define HEIGHT                 768  //600//720
#define SAMPLE_IMAGE_WIDTH     300
#define SAMPLE_IMAGE_HEIGHT    150
#define SAMPLE_IMAGE_SIZE      (300*150*2)
#define SAMPLE_IMAGE_NUM       20
#define HIFB_RED_1555   0xFC00

#define GRAPHICS_LAYER_HC0 3
#define GRAPHICS_LAYER_G0  0
#define GRAPHICS_LAYER_G1  1


#define SAMPLE_IMAGE1_PATH		"./res/%d.bmp"
#define SAMPLE_IMAGE2_PATH		"./res/1280_720.bits"
#define SAMPLE_CURSOR_PATH		"./res/cursor.bmp"

typedef struct hiPTHREAD_HIFB_SAMPLE
{
    HI_S32 fd;
    HI_S32 layer;
    HI_S32 ctrlkey;
}PTHREAD_HIFB_SAMPLE_INFO;


HI_VOID *SAMPLE_HIFB_COMPRESS(void *pData);

HI_S32 SAMPLE_HIFB_Compression(HI_VOID)
{
	HI_S32 s32Ret = HI_SUCCESS;


    PTHREAD_HIFB_SAMPLE_INFO stInfo0;
    HI_U32 u32PicWidth         = WIDTH;
    HI_U32 u32PicHeight        = HEIGHT;
    SIZE_S  stSize;

	VO_LAYER VoLayer = 0;
    VO_PUB_ATTR_S stPubAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    HI_U32 u32VoFrmRate;

    VB_CONF_S       stVbConf;
	HI_U32 u32BlkSize;

    /******************************************
     step  1: init variable 
    ******************************************/
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = CEILING_2_POWER(u32PicWidth,SAMPLE_SYS_ALIGN_WIDTH)\
        * CEILING_2_POWER(u32PicHeight,SAMPLE_SYS_ALIGN_WIDTH) *2;

    stVbConf.u32MaxPoolCnt = 128;
    
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt =  6;

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }

    /******************************************
     step 3:  start vo hd0. 
    *****************************************/
    s32Ret = HI_MPI_VO_UnBindGraphicLayer(GRAPHICS_LAYER_HC0, VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("UnBindGraphicLayer failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }

    s32Ret = HI_MPI_VO_BindGraphicLayer(GRAPHICS_LAYER_HC0, VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("BindGraphicLayer failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }
    stPubAttr.enIntfSync = VO_OUTPUT_1024x768_60;//VO_OUTPUT_720P50;
    stPubAttr.enIntfType = VO_INTF_VGA;
	stPubAttr.u32BgColor = 0x00ff00;

    stLayerAttr.bClusterMode = HI_FALSE;
	stLayerAttr.bDoubleFrame = HI_FALSE;
	stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    s32Ret = SAMPLE_COMM_VO_GetWH(stPubAttr.enIntfSync,&stSize.u32Width,\
        &stSize.u32Height,&u32VoFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get vo wh failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }
    memcpy(&stLayerAttr.stImageSize,&stSize,sizeof(stSize)); 

    stLayerAttr.u32DispFrmRt = 30 ;
	stLayerAttr.stDispRect.s32X = 0;
	stLayerAttr.stDispRect.s32Y = 0;
	stLayerAttr.stDispRect.u32Width = stSize.u32Width;
	stLayerAttr.stDispRect.u32Height = stSize.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vo dev failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
	}

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vo layer failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_1;
	}
    
    if (stPubAttr.enIntfType & VO_INTF_HDMI)
    {
        s32Ret = SAMPLE_COMM_VO_HdmiStart(stPubAttr.enIntfSync);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("start HDMI failed with %d!\n", s32Ret);
            goto SAMPLE_HIFB_NoneBufMode_2;
    	}
    }

    /******************************************
     step 4:  start hifb. 
    *****************************************/

    stInfo0.layer   =  0;
    stInfo0.fd      = -1;
    stInfo0.ctrlkey =  3;

    printf("SAMPLE_HIFB_Compression  start.\n");
    SAMPLE_HIFB_COMPRESS((void *)(&stInfo0));

    printf("SAMPLE_HIFB_Compression end.\n");
    while(1)
    {
        HI_CHAR ch;

        printf("press 'q' to exit this sample.\n");
        ch = getchar();
        if (10 == ch)
        {
            continue;
        }
        getchar();
        if ('q' == ch)
        {
            gs_cExitFlag = ch;
            break;
        }
        else
        {
            printf("input invaild! please try again.\n");
        }
    }

    
SAMPLE_HIFB_NoneBufMode_2:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
SAMPLE_HIFB_NoneBufMode_1:
    SAMPLE_COMM_VO_StopDev(VoDev);
SAMPLE_HIFB_NoneBufMode_0:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}



HI_VOID *SAMPLE_HIFB_COMPRESS(void *pData)
{
    HI_S32 s32Ret = HI_SUCCESS;

    struct fb_var_screeninfo stVarInfo;
	HI_CHAR file[12] = "/dev/fb0";
    PTHREAD_HIFB_SAMPLE_INFO *pstInfo;
    FILE *fp;
    HI_BOOL bEnable;
    HIFB_DDRZONE_S stDDRZonePara;
    HIFB_LAYER_INFO_S stLayerinfo;
    HIFB_BUFFER_S stCanvasBuf;
    HI_CHAR *pcBuf;
    HI_S32 i = 0;

    pstInfo = (PTHREAD_HIFB_SAMPLE_INFO *)pData;
    switch (pstInfo->layer)
	{
		case GRAPHICS_LAYER_G0:
			strcpy(file, "/dev/fb0");
			break;
        case GRAPHICS_LAYER_G1:
            strcpy(file, "/dev/fb1");
			break;
		default:
			strcpy(file, "/dev/fb0");
			break;
	}

    /* 1. open framebuffer device overlay 0 */
	pstInfo->fd = open(file, O_RDWR, 0);
	if(pstInfo->fd < 0)
	{
		SAMPLE_PRT("open %s failed!\n",file);
		return HI_NULL;
	}

    s32Ret = ioctl (pstInfo->fd, FBIOGET_VSCREENINFO, &stVarInfo);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOGET_VSCREENINFO failed!\n");
        close(pstInfo->fd);
        return HI_NULL;
    }

    stVarInfo.red = s_r32;
	stVarInfo.green = s_g32;
	stVarInfo.blue = s_b32;
	stVarInfo.transp = s_a32;
	stVarInfo.bits_per_pixel = 32;
    stVarInfo.xres=WIDTH;
    stVarInfo.yres=HEIGHT;
    stVarInfo.activate=FB_ACTIVATE_NOW;
    stVarInfo.xres_virtual=WIDTH;
    stVarInfo.yres_virtual = HEIGHT;
    stVarInfo.xoffset=0;
    stVarInfo.yoffset=0;

    s32Ret = ioctl (pstInfo->fd, FBIOPUT_VSCREENINFO, &stVarInfo);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_VSCREENINFO failed! s32Ret:%d\n",s32Ret);
        close(pstInfo->fd);
        return HI_NULL;
    }

    s32Ret = ioctl(pstInfo->fd, FBIOGET_LAYER_INFO, &stLayerinfo);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOGET_LAYER_INFO failed!\n");
        close(pstInfo->fd);
        return HI_NULL;
    }
    
   	stLayerinfo.u32Mask = 0;
    stLayerinfo.BufMode = HIFB_LAYER_BUF_NONE;
    stLayerinfo.u32Mask |= HIFB_LAYERMASK_BUFMODE;   
    s32Ret = ioctl(pstInfo->fd, FBIOPUT_LAYER_INFO, &stLayerinfo);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_LAYER_INFO failed!\n");
        close(pstInfo->fd);
        return HI_NULL;
    }
    stDDRZonePara.u32StartSection = 0;
    stDDRZonePara.u32ZoneNums = 15;
    s32Ret = ioctl(pstInfo->fd, FBIOPUT_MDDRDETECT_HIFB, &stDDRZonePara);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_MDDRDETECT_HIFB failed!\n");
        close(pstInfo->fd);
        return HI_NULL;
    }
    
    /* 2. open compress */
    bEnable = HI_FALSE;//HI_FALSE;//HI_TRUE;
    s32Ret = ioctl(pstInfo->fd, FBIOPUT_COMPRESSION_HIFB, &bEnable);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_COMPRESSION_HIFB failed!\n");
        close(

		pstInfo->fd);
        return HI_NULL;
    }
    SAMPLE_PRT("SAMPLE_HIFB_COMPRESS sleep start\n");
#if 1
    while(1)
    {
        sleep(1000);
    }
#endif
     SAMPLE_PRT("SAMPLE_HIFB_COMPRESS sleep end\n");

    if (HI_FAILURE == HI_MPI_SYS_MmzAlloc(&(stCanvasBuf.stCanvas.u32PhyAddr), ((void**)&pcBuf), 
            NULL, NULL, WIDTH*HEIGHT*4))
    {
        SAMPLE_PRT("allocate memory failed\n");
        close(pstInfo->fd);
        return HI_NULL;
    }
    stCanvasBuf.stCanvas.u32Height = HEIGHT;
    stCanvasBuf.stCanvas.u32Width = WIDTH;
    stCanvasBuf.stCanvas.u32Pitch = WIDTH * 4;
    stCanvasBuf.stCanvas.enFmt = HIFB_FMT_ARGB8888;
    stCanvasBuf.UpdateRect.x = 0;
    stCanvasBuf.UpdateRect.y = 0;
    stCanvasBuf.UpdateRect.w = WIDTH;
    stCanvasBuf.UpdateRect.h = HEIGHT;

    fp = fopen(SAMPLE_IMAGE2_PATH, "rb");
    if (HI_NULL == fp)
    {
        SAMPLE_PRT("fopen file failed!\n");
        HI_MPI_SYS_MmzFree(stCanvasBuf.stCanvas.u32PhyAddr, pcBuf);
        close(pstInfo->fd);
        return HI_NULL;
    }
    memset(pcBuf, 0, stCanvasBuf.stCanvas.u32Pitch * stCanvasBuf.stCanvas.u32Height);
    fread(pcBuf, 1, stCanvasBuf.stCanvas.u32Pitch * stCanvasBuf.stCanvas.u32Height, fp);
    fclose(fp);

    s32Ret = ioctl(pstInfo->fd, FBIO_REFRESH, &stCanvasBuf);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIO_REFRESH failed\n");
        HI_MPI_SYS_MmzFree(stCanvasBuf.stCanvas.u32PhyAddr, pcBuf);
        close(pstInfo->fd);
        return HI_NULL;
    }
    while(i < 5)
    {
        if ('q' == gs_cExitFlag)
        {
            printf("process exit...\n");
            break;
        }
        
        i++;
        stCanvasBuf.UpdateRect.x = WIDTH / 4;
        stCanvasBuf.UpdateRect.y = HEIGHT / 4;
        stCanvasBuf.UpdateRect.w = WIDTH / 2;
        stCanvasBuf.UpdateRect.h = HEIGHT / 2;
        DrawBox(stCanvasBuf.UpdateRect.x, stCanvasBuf.UpdateRect.y,\
            stCanvasBuf.UpdateRect.w, stCanvasBuf.UpdateRect.h, \
            stCanvasBuf.stCanvas.u32Pitch, pcBuf, 0xffff0000);
        sleep(2);
        stCanvasBuf.UpdateRect.x = WIDTH * 3/8;
        stCanvasBuf.UpdateRect.y = HEIGHT * 3/8;
        stCanvasBuf.UpdateRect.w = WIDTH / 4;
        stCanvasBuf.UpdateRect.h = HEIGHT / 4;
        DrawBox(stCanvasBuf.UpdateRect.x, stCanvasBuf.UpdateRect.y,\
            stCanvasBuf.UpdateRect.w, stCanvasBuf.UpdateRect.h, \
            stCanvasBuf.stCanvas.u32Pitch, pcBuf, 0xff00ff00);
        sleep(2);
    
    }
    HI_MPI_SYS_MmzFree(stCanvasBuf.stCanvas.u32PhyAddr, pcBuf);
    close(pstInfo->fd);
    return HI_NULL;
}
#endif

#ifeq ($(wildcard ../../config.mk) ,../../config.mk)
#$(warning  config.mk found) 
#include ../../config.mk
#else
#$(warning  config.mk not found)
#endif

APP_START	= 0
APP_SIZE	= 9
APPBAK_SIZE	= 9
FONT_SIZE	= 12
APPBAK_START	= $(APP_START)+$(APP_SIZE)
FONT_START 	= $(APPBAK_START)+$(APPBAK_SIZE)
CROSS ?= arm-linux-gnueabi-
CC    = $(CROSS)g++
AR    = $(CROSS)ar
STRIP = $(CROSS)strip


				

CFLAGS = -Dhi3536 -Wall -std=c++0x -D_THREAD_SAFE -MMD  -Wno-strict-aliasing
ifeq ($(VER), debug)
CFLAGS+= -g3
else
CFLAGS += -O2
endif

CFLAGS+= -DPRODUCT_P2

TARGET = ltyappcomm/ltyapp
PRODUCT_TARGET =;
 
INCFLAGS = -I./ -I./include -I./SlDemo 	
	#STATIC += -lUart
	PRODUCT_TARGET = ltyp2/
	LNKFLAGS += -lpthread -L./ltyp2/lib 
	


SRC_INPUT_MISC = inputmisc/Uart.c  inputmisc/PsamCard.c  inputmisc/GPIOCtrl.c  inputmisc/IcCardCtrlApi.c  SlDemo/GPRS.c  SlDemo/LightColor.c  SlDemo/DemoMain.c  SlDemo/ProcCharacter.c 
#SRC_INPUT_MISC =   inputmisc/PsamCard.cpp inputmisc/IcCardCtrlApi.cpp	


			

SOURCE = $(SRC_INPUT_MISC)\
		ICCardLib.c PSAMLib.c CPUCard.c CPUCARJTB.c MYDES.c main.c libsm2.a RecordFile.c SL8583_4GLinux.c GprsSocket.c

OBJECT = $(SOURCE:.c=.o) 
DEPEND = $(SOURCE:.c=.d)

.PHONY : all
all : upVer $(TARGET)

upVer:
	@if [ $(GIT_REV) ] ;then \
	cp version.h.template ./include/version.h; \
	sed -i "s/\\$$//g" ./include/version.h; \
	sed -i "s/MJREV/$(MJ_REV)/g"  ./include/version.h; \
	sed -i "s/MNREV/$(MN_REV)/g"  ./include/version.h; \
	sed -i "s/WCREV/$(BD_REV)/g"  ./include/version.h; \
	sed -i "s/WCDATE/$(REV_DATE)/g" ./include/version.h; \
	fi
uptag:
	@MJ_REV=`echo $(GIT_REV) | awk -F"." '{print $$1}'`; \

	
$(TARGET) : $(OBJECT)
	$(CC) $(OBJECT) $(INCFLAGS) $(STATIC) $(LNKFLAGS) -o $(TARGET)
	cp ltyappcomm/* ltyapp/ -rf
	cp $(PRODUCT_TARGET)* ltyapp/ -rf

#	$(STRIP) $(TARGET)
%.o : %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F)
		
clean :
#	@for x in `ls `; do if [ -d $$x ]; then cd $$x; make clean; cd ../..; fi; done
	#rm $(TARGET) $(OBJECT) $(DEPEND) -f
	find . -name "*.o"  | xargs rm -f
	# find . -name "*.d"  | xargs rm -f
	rm -rf ltyappcomm/ltyapp
	rm -rf *.tar
	rm -rf bin/app/*
	rm -rf bin/font/*
	rm -rf bin/*.bin
	rm -rf ltyapp/*


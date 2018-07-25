// @file: GPIOCtrl.h
// @date: 2018.05.18

#include "LtyCommon.h"
#include "LtyTimer.h"
#include "SlzrError.h"
#include "SlzrTypeDef.h"

#include "LightColor.h"

extern SLZR_VOID CGPIOCtrl(void);
extern SLZR_VOID _CGPIOCtrl(void);
extern SLZR_RESULT Init_3Gmode(void);
extern SLZR_VOID CGPIOCtrl_Release(void);
extern SLZR_RESULT Init_QR(struct ev_loop *loop);
extern SLZR_RESULT Init_SCREEN(struct ev_loop *loop);
extern SLZR_RESULT CGPIOCtrl_Open();
extern SLZR_RESULT GPIOSetVal(SLZR_U32 pin , gpio_pin_e enPin);
extern SLZR_RESULT SetGPIODir(SLZR_U32 pin, gpio_dir_e enDir);
extern SLZR_RESULT GPIOGenDir(SLZR_U32 pin);
extern SLZR_RESULT ClrGPIODir(SLZR_U32 pin);
extern SLZR_RESULT SetGPIOPin(SLZR_U32 pin, gpio_pin_e enPin,gpio_dir_e enDir);
extern SLZR_RESULT InitGPIOPin(SLZR_U32 pin, gpio_pin_e enPin,gpio_dir_e enDir);
extern SLZR_RESULT InitGPIOPinInt(SLZR_U32 pin, gpio_pin_e enPin,gpio_dir_e enDir);
extern SLZR_RESULT GetGPIOPin(SLZR_U32 pin, gpio_pin_e *penPin);
extern SLZR_RESULT gpio_edge(SLZR_U32 pin, SLZR_U32 edge);
extern SLZR_RESULT gpio_init(SLZR_U32 gpiopin,SLZR_U32 *fd);
extern SLZR_U32 gpio_read(SLZR_U32 pin);
extern SLZR_RESULT set_ic_r(int value);
extern SLZR_RESULT set_ic_g(int value);
extern SLZR_RESULT set_ic_b(int value);	
extern SLZR_RESULT set_beer_b(int value);


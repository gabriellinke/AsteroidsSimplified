/* generated thread header file - do not edit */
#ifndef SPACESHIP_CONTROL_THREAD_H_
#define SPACESHIP_CONTROL_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void spaceship_control_thread_entry(void);
                #else
extern void spaceship_control_thread_entry(void);
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern TX_QUEUE button_queue;
extern TX_QUEUE touch_queue;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SPACESHIP_CONTROL_THREAD_H_ */

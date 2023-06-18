/* generated thread header file - do not edit */
#ifndef GAME_ENGINE_THREAD_H_
#define GAME_ENGINE_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void game_engine_thread_entry(void);
                #else
extern void game_engine_thread_entry(void);
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern TX_QUEUE control_queue;
extern TX_QUEUE graphic_queue;
extern TX_EVENT_FLAGS_GROUP event_flags;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GAME_ENGINE_THREAD_H_ */

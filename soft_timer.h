#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H

#include "stdint.h"
#include "stdbool.h"

enum
{
	TIMER_ID_0 = 0,
	TIMER_ID_END,
};

enum
{
	TON_ID_0 = 0,
	TON_ID_END,
};

void timer_check(uint8_t b_id, uint32_t dw_now);
void timer_start(uint8_t b_id);
void timer_stop(uint8_t b_id);
void timer_set(uint8_t b_id, uint32_t dw_interval, void(*cb)(void));
bool TON(uint8_t b_id, bool o_in, uint32_t dw_now, uint32_t dw_preset_time);

#endif /* SOFT_TIMER_H */

/*
 * manchester_transmit.c
 *
 *  Created on:
 *      Author:
 */

#include "manchester_transmit.h"
#include "macro.h"
#include "main.h"

#define MAN_TX_GET_STATE(x) READ_BIT_POS(s_man_tx_obj[x].aw_tx_buff[s_man_tx_obj[x].b_tx_idx], s_man_tx_obj[x].b_bit_cnt++)

typedef struct
{
	uint8_t b_bit_cnt;
	uint8_t b_state;
	uint8_t b_size;
	uint8_t b_tx_idx;
	uint16_t *aw_tx_buff;
	void(*fp)(void);
}man_tx_t;

man_tx_t s_man_tx_obj[MAN_TX_OBJ_SIZE];

static void man_0(void);
static void man_1(void);

void manchester_tx_set_callback(uint8_t b_id, void (*fp)(void))
{
	s_man_tx_obj[b_id].fp = fp;
}

void manchester_tx_start_it(uint8_t b_id, const uint16_t *aw_data, uint8_t size)
{
	s_man_tx_obj[b_id].b_state = MAN_TX_BUSY;
	s_man_tx_obj[b_id].aw_tx_buff = aw_data;
	s_man_tx_obj[b_id].b_size = size;
	s_man_tx_obj[b_id].b_tx_idx = 0;
	s_man_tx_obj[b_id].b_bit_cnt = 0;
}

void manchester_tx_isr(uint8_t b_id)
{
	if (s_man_tx_obj[b_id].b_tx_idx != s_man_tx_obj[b_id].b_size)
	{
		MAN_TX_GET_STATE(b_id) ? man_1() : man_0();

		if (s_man_tx_obj[b_id].b_bit_cnt == 16)
		{
			++s_man_tx_obj[b_id].b_tx_idx;
			s_man_tx_obj[b_id].b_bit_cnt = 0;
		}
	}
	else
	{
		s_man_tx_obj[b_id].b_state = MAN_TX_READY;
		man_0();
		s_man_tx_obj[b_id].fp();
	}
}

uint16_t nrz_to_manchester(uint8_t b_data)
{
	uint16_t w_tmp = 0;

	for (uint8_t j = 0; j < 8; ++j)
	{
		if(READ_BIT_POS(b_data, j))
		{
			SET_BIT_POS(w_tmp, j*2);
			RESET_BIT_POS(w_tmp, j*2+1);
		}
		else
		{
			RESET_BIT_POS(w_tmp, j*2);
			SET_BIT_POS(w_tmp, j*2+1);
		}
	}
	return w_tmp;
}

static void man_0(void)
{
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
}

static void man_1(void)
{
	TIM1->CCR1 = 16;
	TIM1->CCR2 = 16;
}

/*
 * manchester_transmit.h
 *
 *  Created on:
 *      Author:
 */

#ifndef MANCHESTER_TRANSMIT_H
#define MANCHESTER_TRANSMIT_H

#include "stdint.h"
#include "stdbool.h"

#define MAN_TX_OBJ_SIZE		1

typedef enum
{
	MAN_TX_READY,
	MAN_TX_BUSY,
	MAN_TX_ERROR,
}man_tx_state_t;

void manchester_tx_set_callback(uint8_t b_id, void(*fp)(void));
void manchester_tx_start_it(uint8_t b_id, const uint16_t *aw_data, uint8_t size);
void manchester_tx_isr(uint8_t b_id);
uint16_t nrz_to_manchester(uint8_t b_data);

#endif /* MANCHESTER_TRANSMIT_H */

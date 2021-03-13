
/* sample_buffer.h */

#ifndef __SAMPLE_BUFFER_H
#define __SAMPLE_BUFFER_H

#include <stdint.h>

typedef struct SAMPLE {
  uint8_t left, right;
} sample_t;

void buffer_q_init(void);
uint8_t buffer_q_almostfull(void);
uint8_t buffer_q_empty(void);
uint32_t buffer_q_size(void);

const sample_t * buffer_q_get(void);

void buffer_q_push_p( const sample_t * const );
void buffer_q_push( const uint8_t left, const uint8_t right );
void buffer_q_pop(void);

#endif

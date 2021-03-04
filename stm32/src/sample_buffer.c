
/* sample_buffer.c */

#include "sample_buffer.h"

#define BUFFER_MAX_SIZE ( 1UL << 20 )
#define BUFFER_FULL_PADDING 100

sample_t BUFFER_Q[ BUFFER_MAX_SIZE ];
uint32_t BUFFER_Q_head;
uint32_t BUFFER_Q_tail;
uint32_t BUFFER_Q_size;
uint8_t BUFFER_Q_full, BUFFER_Q_empty;

uint32_t format_sample_p( const sample_t * const s ) {
  return ( (uint32_t) s->left )|( (uint32_t) s->right << 16 );
}

void buffer_q_init() {
  BUFFER_Q_head   = 0;
  BUFFER_Q_tail   = 0;
  BUFFER_Q_size   = 0;
  BUFFER_Q_full   = 0;
  BUFFER_Q_empty  = 1;
}

uint8_t buffer_q_almostfull() {
  return ( BUFFER_MAX_SIZE - BUFFER_Q_size ) < BUFFER_FULL_PADDING;
}
uint8_t buffer_q_empty() {
  return ( BUFFER_Q_empty != 0 );
}
uint32_t buffer_q_size() {
  return BUFFER_Q_size;
}


const sample_t * buffer_q_get() {
  if ( BUFFER_Q_empty )
    return 0;
  return &BUFFER_Q[ BUFFER_Q_head ];
}

void buffer_q_push_p( const sample_t * const in ) {
  buffer_q_push( in->left, in->right );
}

void buffer_q_push( const uint16_t left, const uint16_t right ) {
  if ( BUFFER_Q_full )
    return;
  
  BUFFER_Q_tail = ( BUFFER_Q_tail + 1 ) % BUFFER_MAX_SIZE;

  BUFFER_Q[ BUFFER_Q_tail ].left = left;
  BUFFER_Q[ BUFFER_Q_tail ].right = right;

  BUFFER_Q_empty = 0;
  BUFFER_Q_size++;

  if ( BUFFER_Q_head == BUFFER_Q_tail )
    BUFFER_Q_full = 1;
}


void buffer_q_pop() {
  if ( BUFFER_Q_empty )
    return;

  BUFFER_Q_head = ( BUFFER_Q_head + 1 ) % BUFFER_MAX_SIZE;

  BUFFER_Q_full = 0;
  BUFFER_Q_size--;

  if ( BUFFER_Q_head == BUFFER_Q_tail )
    BUFFER_Q_empty = 1;
}

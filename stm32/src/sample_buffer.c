
/* sample_buffer.c */

#include "sample_buffer.h"

#define BUFFER_MAX_SIZE ( 1UL << 15 )
#define BUFFER_FULL_PADDING ( BUFFER_MAX_SIZE >> 1 )

// #define BUFFER_MAX_SIZE 256
// #define BUFFER_FULL_PADDING 10

sample_t BUFFER_Q[ BUFFER_MAX_SIZE ];
uint32_t BUFFER_Q_head;
uint32_t BUFFER_Q_tail;
uint32_t BUFFER_Q_size;
uint8_t BUFFER_Q_full, BUFFER_Q_empty;


void buffer_q_init() {
  __disable_irq();
  BUFFER_Q_head   = 0;
  BUFFER_Q_tail   = 0;
  BUFFER_Q_size   = 0;
  BUFFER_Q_full   = 0;
  BUFFER_Q_empty  = 1;
  __enable_irq();
}

uint8_t buffer_q_almostfull() {
  __disable_irq();
  uint8_t out = ( BUFFER_Q_full || ( BUFFER_MAX_SIZE - BUFFER_Q_size ) < BUFFER_FULL_PADDING );
  __enable_irq();
  return out;
}
uint8_t buffer_q_empty() {
  __disable_irq();
  uint8_t out = ( BUFFER_Q_empty != 0 );
  __enable_irq();
  return out;
}
uint32_t buffer_q_size() {
  __disable_irq();
  uint32_t out = BUFFER_Q_size;
  __enable_irq();
  return out;
}


const sample_t * buffer_q_get() {
  __disable_irq();
  sample_t * out;
  if ( BUFFER_Q_empty )
    out = 0;
  else
    out = &BUFFER_Q[ BUFFER_Q_head ];
  __enable_irq();
  return out;
}

void buffer_q_push_p( const sample_t * const in ) {
  buffer_q_push( in->left, in->right );
}

void buffer_q_push( const uint8_t left, const uint8_t right ) {
  __disable_irq();
  if ( !BUFFER_Q_full ) {

    BUFFER_Q[ BUFFER_Q_tail ].left = left;
    BUFFER_Q[ BUFFER_Q_tail ].right = right;

    BUFFER_Q_tail = ( BUFFER_Q_tail + 1 ) % BUFFER_MAX_SIZE;

    BUFFER_Q_empty = 0;
    BUFFER_Q_size++;

    if ( BUFFER_Q_head == BUFFER_Q_tail )
      BUFFER_Q_full = 1;
  }
  __enable_irq();
}


void buffer_q_pop() {
  __disable_irq();
  if ( !BUFFER_Q_empty ) {

    BUFFER_Q_head = ( BUFFER_Q_head + 1 ) % BUFFER_MAX_SIZE;

    BUFFER_Q_full = 0;
    BUFFER_Q_size--;

    if ( BUFFER_Q_head == BUFFER_Q_tail )
      BUFFER_Q_empty = 1;
  }
  __enable_irq();
}

#ifndef BYTE_QUEUE_H
#define BYTE_QUEUE_H

#include <linux/types.h>

struct byte_queue;
typedef struct byte_queue byte_queue_t;

byte_queue_t *byte_queue_create(void);
void byte_queue_push(byte_queue_t *queue, char ch);
unsigned char byte_queue_pop(byte_queue_t *queue);
unsigned char byte_queue_first(byte_queue_t *queue);
unsigned char byte_queue_last(const byte_queue_t *queue);
ssize_t byte_queue_size(const byte_queue_t *queue);
void byte_queue_destroy(byte_queue_t *queue);

void byte_queue_print(const byte_queue_t *queue);

#endif // BYTE_QUEUE_H
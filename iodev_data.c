#ifndef IODEV_DATA_H
#define IODEV_DATA_H

#include "byte_queue.h"
#include "iodev.h"
#include <asm/uaccess.h>
#include <linux/slab.h>

typedef struct {
  byte_queue_t *queue;
  bool odd; // read flag
} iodev_data_t;

iodev_data_t *iodev_data_create(void) {
  iodev_data_t *ret = kmalloc(sizeof(*ret), GFP_KERNEL);
  if (ret != NULL) {
    ret->queue = byte_queue_create();
    ret->odd = false;
  }
  return ret;
}

ssize_t iodev_data_read(iodev_data_t *data, void *buf, ssize_t size) {
  if (data->odd == false) {
    unsigned char ch = 0;
    ssize_t i = 0;
    while (byte_queue_size(data->queue) > 0 && i < size) {
      ch = byte_queue_pop(data->queue);
      // copy_to_user((void *)(buf + i), (void *)&ch, sizeof(ch));
      put_user(ch, (char *)buf + i);
      ++i;
    }
    data->odd = true;
    // put_user('\n', buf + strlen(data->buf));
    return i;
  }
  data->odd = false;
  return 0;
}

ssize_t iodev_data_write(iodev_data_t *data, const void *buf, ssize_t size) {
  const int len = size < IODEV_BUF_LEN ? size : IODEV_BUF_LEN;
  ssize_t i = 0;
  unsigned char ch;
  while (i < len && byte_queue_size(data->queue) < IODEV_BUF_LEN) {
    // copy_from_user(&ch, (void *)(buf + i), sizeof(ch));
    get_user(ch, (char *)buf + i);
    byte_queue_push(data->queue, ch);
    ++i;
  }
  // byte_queue_push(data->queue, '\0');
  return i;
}

void iodev_data_destroy(iodev_data_t *data) {
  byte_queue_destroy(data->queue);
  kfree(data);
  data = NULL;
}

#endif // IODEV_DATA_H
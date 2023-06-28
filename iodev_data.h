#ifndef IODEV_DATA_H
#define IODEV_DATA_H

#include <linux/types.h>

struct iodev_data;
typedef struct iodev_data iodev_data_t;

iodev_data_t *iodev_data_create(void);
size_t iodev_data_read(iodev_data_t *data, void *buf, ssize_t size);
ssize_t iodev_data_write(iodev_data_t *data, const void *buf, ssize_t size);
void iodev_data_destroy(iodev_data_t *data);

#endif // IODEV_DATA_H
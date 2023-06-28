#include "iodev.h"
#include "iodev_data.h"
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/slab.h>
// #include <linux/stdarg.h>

// --- Initialize module

typedef enum {
  IODEV_MODE_WITHOUT_CONTROL,
  IODEV_MODE_SINGLE,
  IODEV_MODE_MULTI
} IODEV_MODE;

static int mode = IODEV_MODE_WITHOUT_CONTROL;
static int debug = true;

static int __init iodev_init(void);
static void __exit iodev_exit(void);
static int iodev_open(struct inode *n, struct file *f);
static int iodev_release(struct inode *n, struct file *f);
static ssize_t iodev_read(struct file *f, char *buf, size_t count, loff_t *pos);
static ssize_t iodev_write(struct file *f, const char *buf, size_t count,
                           loff_t *pos);

static int iodev_num_open = 0;
iodev_data_t *iodev_data_instance = NULL;

static const struct file_operations iodev_fops = {
    .owner = THIS_MODULE,
    .open = iodev_open,
    .release = iodev_release,
    .read = iodev_read,
    .write = iodev_write,
};
static struct miscdevice iodev_dev = {MISC_DYNAMIC_MINOR, IODEV_NAME,
                                      &iodev_fops};

module_param(mode, int, S_IRUGO);
module_param(debug, int, S_IRUGO);
module_init(iodev_init);
module_exit(iodev_exit);

// --- Module functions

// static inline void logging(const char *fmt, ...) {
//   if (debug == false)
//     return;
//   va_list args;
//   va_start(args, fmt);
//   printk(fmt, args);
//   va_end(args);
// }

#define logging(...)                                                           \
  if (debug != false)                                                          \
  printk(KERN_INFO "! "__VA_ARGS__)

static inline iodev_data_t *iodev_get_data(struct file *f) {
  return iodev_data_instance == NULL ? (iodev_data_t *)f->private_data
                                     : iodev_data_instance;
}

int __init iodev_init(void) {
  const int ret = misc_register(&iodev_dev);
  if (ret > 0) {
    printk(KERN_ERR "unable to register %s misc device", IODEV_NAME);
  } else {
    logging("installed device /dev/%s in mode %d", IODEV_NAME, mode);
    if (iodev_data_instance == NULL && mode < 2)
      iodev_data_instance = iodev_data_create();
  }

  return ret;
}

void __exit iodev_exit(void) {
  logging("released device /dev/%s\n", IODEV_NAME);
  if (iodev_data_instance != NULL)
    iodev_data_destroy(iodev_data_instance);
  misc_deregister(&iodev_dev);
}

int iodev_open(struct inode *n, struct file *f) {
  logging("open - node: %p, file: %p, refcount: %d", n, f,
          module_refcount(THIS_MODULE));
  if (iodev_num_open > 0 && mode == 1) {
    logging("device /dev/%s is busy", IODEV_NAME);
    return -EBUSY;
  }

  if (mode == 2) {
    f->private_data = (void *)iodev_data_create();
    if (f->private_data == NULL) {
      logging("memory allocation error");
      return -ENOMEM;
    }
  } else {
    ++iodev_num_open;
  }
  return 0;
}

int iodev_release(struct inode *n, struct file *f) {
  logging("close - node: %p, file: %p, refcount: %d", n, f,
          module_refcount(THIS_MODULE));
  if (mode == 2) {
    iodev_data_destroy((iodev_data_t *)(f->private_data));
  } else {
    --iodev_num_open;
  }
  return 0;
}

ssize_t iodev_read(struct file *f, char *buf, size_t count, loff_t *pos) {
  ssize_t ret = 0;
  iodev_data_t *data = iodev_get_data(f);
  logging("read - file: %p, read from %p bytes %ld; refcount: %d", f, data,
          count, module_refcount(THIS_MODULE));
  ret = iodev_data_read(data, buf, count);
  if (ret > 0) {
    logging("return bytes : %ld", ret);
  } else {
    logging("return : EOF");
  }
  return ret;
}

ssize_t iodev_write(struct file *f, const char *buf, size_t count,
                    loff_t *pos) {
  ssize_t ret = 0;
  iodev_data_t *data = iodev_get_data(f);
  logging("write - file: %p, write to %p bytes %ld; refcount: %d", f, data,
          count, module_refcount(THIS_MODULE));
  ret = iodev_data_write(data, buf, count);
  printk("put bytes : %ld", ret);
  return ret;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mihail Ezhov <ezhoh93@gmail.com>");
MODULE_VERSION("1.0");
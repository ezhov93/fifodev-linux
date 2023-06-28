#include "byte_queue.h"
#include <linux/list.h>
#include <linux/slab.h>

// --- private
struct byte_node {
  struct list_head list;
  unsigned char ch;
};

struct byte_queue {
  struct list_head head;
  int size;
};

static struct byte_node *byte_node_create(const char ch) {
  struct byte_node *node =
      (struct byte_node *)kmalloc(sizeof(*node), GFP_KERNEL);
  node->ch = ch;
  return node;
}

static inline void byte_node_destroy(struct byte_node *node) { kfree(node); }

// --- public
byte_queue_t *byte_queue_create() {
  byte_queue_t *ret = (byte_queue_t *)kmalloc(sizeof(*ret), GFP_KERNEL);
  INIT_LIST_HEAD(&ret->head);
  ret->size = 0;
  return ret;
}

void byte_queue_push(byte_queue_t *queue, char ch) {
  struct byte_node *node = byte_node_create(ch);
  list_add_tail(&node->list, &queue->head);
  queue->size += 1;
}

unsigned char byte_queue_pop(byte_queue_t *queue) {
  struct byte_node *node =
      list_first_entry_or_null(&queue->head, struct byte_node, list);
  char ret = 0;
  if (node == NULL)
    return ret;
  ret = node->ch;
  list_del(&node->list);
  byte_node_destroy(node);
  queue->size -= 1;
  return ret;
}

unsigned char byte_queue_first(byte_queue_t *queue) {
  struct byte_node *node =
      list_first_entry_or_null(&queue->head, struct byte_node, list);
  if (node == NULL)
    return 0;
  return node->ch;
}

unsigned char byte_queue_last(const byte_queue_t *queue) {
  if (list_empty(&queue->head))
    return 0;
  return list_last_entry(&queue->head, struct byte_node, list)->ch;
}

ssize_t byte_queue_size(const byte_queue_t *queue) { return queue->size; }

void byte_queue_destroy(byte_queue_t *queue) {
  struct byte_node *node;
  struct byte_node *tmp;

  list_for_each_entry_safe(node, tmp, &queue->head, list) {
    list_del(&node->list);
    byte_node_destroy(node);
  }
  kfree(queue);
  queue = NULL;
}

void byte_queue_print(const byte_queue_t *queue) {
  struct byte_node *node;
  printk("{ ");
  list_for_each_entry(node, &queue->head, list) printk("%c, ", node->ch);
  printk(" }\n");
}
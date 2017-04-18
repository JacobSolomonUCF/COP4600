#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#define  DEVICE_NAME "copinput"
#define  CLASS_NAME  "cop"
#define BUFFER_SIZE 1024

extern char shared_buffer[BUFFER_SIZE + 1] = {0}

MODULE_LICENSE("GPL");

static int    majorNumber;
static int    bytesUsed = 0;
static struct class*  ebbcharClass  = NULL;
static struct device* ebbcharDevice = NULL;

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release
};
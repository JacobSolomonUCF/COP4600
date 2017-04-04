/**
 * File: copchar.c
 * Author: Tyler Petresky, Jacob Solomon, David Akridge
 *
 * NOTE: The framwork for this code was found at: http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#define  DEVICE_NAME "copchar"    
#define  CLASS_NAME  "cop"
#define BUFFER_SIZE 10

MODULE_LICENSE("GPL");

static int    majorNumber;
static char   message[BUFFER_SIZE + 1] = {0};
static int    bytesUsed = 0;
static struct class*  ebbcharClass  = NULL;
static struct device* ebbcharDevice = NULL;

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release
};

static int __init ebbchar_init(void){
   printk(KERN_INFO "Initializing the Driver\n");

   // Get a major numbers
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0)
   {
      printk(KERN_ALERT "Failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "Registered correctly with major number %d\n", majorNumber);

   // Register the device class
   ebbcharClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(ebbcharClass))
   {
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(ebbcharClass);
   }
   printk(KERN_INFO "Device class registered correctly\n");

   // Register device driver
   ebbcharDevice = device_create(ebbcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(ebbcharDevice))
   {
      class_destroy(ebbcharClass);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device class\n");
      return PTR_ERR(ebbcharDevice);
   }
   printk(KERN_INFO "Device class created correctly\n");
   return 0;
}

static int dev_open(struct inode *inoded, struct file *filep)
{
   printk(KERN_INFO "-------------------- Opened Driver --------------------\n");
   return 0;
}

static void __exit ebbchar_exit(void){
   device_destroy(ebbcharClass, MKDEV(majorNumber, 0));
   class_unregister(ebbcharClass);
   class_destroy(ebbcharClass);
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk(KERN_INFO "Closing the driver\n");
}

// Read bytes from the driver into the user space
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   int error_count = 0;
   int bytesSent = 0;
   if (len > bytesUsed)
   {
      error_count = copy_to_user(buffer, message, bytesUsed);
      buffer[bytesUsed] = '\0';
      message[0] = '\0';
      bytesSent = bytesUsed;
      bytesUsed = 0;
   }
   else
   {
      error_count = copy_to_user(buffer, message, len);

      int i = 0;
      for (i = len; i < bytesUsed; i++)
         message[i - len] = message[i];
      message[i - len] = '\0';
      bytesUsed -= len;
      bytesSent = len;
   }

   if (error_count == 0)
   {
      printk(KERN_INFO "Sent %d characters to the user\n", bytesSent);
      return 0;
   }
   else {
      printk(KERN_INFO "Failed to send %d characters to the user\n", error_count);
      return -EFAULT;
   }
}

// Write from the user into the driver.
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
   if (len + bytesUsed >= BUFFER_SIZE)
   {
      int bytesAvailable = BUFFER_SIZE - bytesUsed;
      int i = 0;

      for (; i < bytesAvailable; i++)
         message[bytesUsed + i] = buffer[i];
      bytesUsed += bytesAvailable;

      message[bytesUsed] = '\0';
   }
   else
   {
      sprintf(message + bytesUsed, "%s", buffer);
      bytesUsed += len;
   }

   printk(KERN_INFO "Wrote \'%s\' to buffer", buffer);

   return len;
}

// Release the driver
static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "Device successfully closed\n");
   return 0;
}

module_init(ebbchar_init);
module_exit(ebbchar_exit);

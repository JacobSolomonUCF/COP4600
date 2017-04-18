#include <linux/mutex.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#define  DEVICE_NAME "copoutput"
#define  CLASS_NAME  "copo"
#define BUFFER_SIZE 1024

extern char shared_buffer[BUFFER_SIZE + 1];

MODULE_LICENSE("GPL");
static DEFINE_MUTEX(output_mutex);

static int    majorNumber;
static struct class*  ebbcharClass  = NULL;
static struct device* ebbcharDevice = NULL;

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
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
   mutex_init(&output_mutex);
   printk(KERN_INFO "Device class created correctly\n");
   return 0;
}

static int dev_open(struct inode *inoded, struct file *filep)
{
	if(!mutex_trylock(&output_mutex)){    /// Try to acquire the mutex (i.e., put the lock on/down)
                                          /// returns 1 if successful and 0 if there is contention
      printk(KERN_ALERT "Device in use by another process");
      return -EBUSY;
   }
   	printk(KERN_INFO "-------------------- Opened Driver --------------------\n");
   	return 0;
}

static void __exit ebbchar_exit(void){
   mutex_destroy(&output_mutex);
   device_destroy(ebbcharClass, MKDEV(majorNumber, 0));
   class_unregister(ebbcharClass);
   class_destroy(ebbcharClass);
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk(KERN_INFO "Closing the driver\n");
}
// Read bytes from the driver into the user space
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   int bytesUsed = strlen(shared_buffer);
   printk(KERN_INFO "MESS %d: \n", bytesUsed);
   int error_count = 0;
   int bytesSent = 0;

   bytesUsed = strlen(shared_buffer);
   if (len > bytesUsed)
   {
      error_count = copy_to_user(buffer, shared_buffer, bytesUsed);
      buffer[bytesUsed] = '\0';
      shared_buffer[0] = '\0';
      bytesSent = bytesUsed;
      bytesUsed = 0;
      printk(KERN_INFO "Buffer1 %s: \n", buffer);
   }
   else
   {
      error_count = copy_to_user(buffer, shared_buffer, len);
      printk(KERN_INFO "Buffer %s: \n", buffer);

      int i = 0;
      for (i = len; i < bytesUsed; i++)
         shared_buffer[i - len] = shared_buffer[i];
      shared_buffer[i - len] = '\0';
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
static int dev_release(struct inode *inodep, struct file *filep){
   mutex_unlock(&output_mutex); 
   printk(KERN_INFO "Device successfully closed\n");
   return 0;
}

module_init(ebbchar_init);
module_exit(ebbchar_exit);

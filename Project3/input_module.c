#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/mutex.h>
#define  DEVICE_NAME "copinput"
#define  CLASS_NAME  "cop"
#define BUFFER_SIZE 1024

char shared_buffer[BUFFER_SIZE + 1] = {0}
EXPORT_SYMBOL(shared_buffer);

MODULE_LICENSE("GPL");

static DEFINE_MUTEX(ebbchar_mutex);

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

static int __init ebbchar_init(void)
{
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

   	mutex_init(&ebbchar_mutex);
   	return 0;
}

static int dev_open(struct inode *inoded, struct file *filep)
{
   	if (!mutex_trylock(&ebbchar_mutex))
   	{
   		printk(KERN_ALERT "COPInput: Device in use by another process");
   		return -EBUSY;
   	}
   	printk(KERN_INFO "-------------------- Opened Driver --------------------\n");
   	return 0;
}

static void __exit ebbchar_exit(void)
{
   	device_destroy(ebbcharClass, MKDEV(majorNumber, 0));
   	class_unregister(ebbcharClass);
   	class_destroy(ebbcharClass);
   	unregister_chrdev(majorNumber, DEVICE_NAME);
   	mutex_unlock(&ebbchar_mutex);
   	printk(KERN_INFO "Closing the driver\n");
}

// Write from the user into the driver.
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
   	if (len + bytesUsed >= BUFFER_SIZE)
   	{
      	int bytesAvailable = BUFFER_SIZE - bytesUsed;
      	int i = 0;

      	for (; i < bytesAvailable; i++)
         	shared_buffer[bytesUsed + i] = buffer[i];
      	bytesUsed += bytesAvailable;

      	shared_buffer[bytesUsed] = '\0';
   	}
   	else
   	{
    	sprintf(shared_buffer + bytesUsed, "%s", buffer);
      	bytesUsed += len;
   	}

   	printk(KERN_INFO "Wrote \'%s\' to buffer", buffer);

   	return len;
}

// Release the driver
static int dev_release(struct inode *inodep, struct file *filep)
{
   	printk(KERN_INFO "Device successfully closed\n");
   	return 0;
}

module_init(ebbchar_init);
module_exit(ebbchar_exit);
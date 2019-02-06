#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

static dev_t first; 
static char* dev_names[] = {"_x", "_y", "_z"};
static struct cdev c_dev;
static struct class *cls;
unsigned char x[10];

//STEP 4 : Driver callback functions
static int my_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "mychar: open()\n");
	return 0;
}

static int my_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "mychar: close()\n");
	return 0;
}

static ssize_t my_read(struct file *f,char __user *buf, size_t len, loff_t *off)
{
	unsigned int i;
	int r;
	printk(KERN_INFO "mychar: read()\n");
	for(r=0;r<10;r++)
	{	get_random_bytes(&i, sizeof i); //generate random no of bytes in i
		i= i%10; 
		x[r]=i; //sending every one digit of i to an char array of x
		printk("%u",i);
	}
	copy_to_user(buf,x,len);  //copying char x array to buf
	return 0;
}

static struct file_operations fops =
				{
					.owner  = THIS_MODULE,
					.open   = my_open,
					.release= my_close,
					.read   = my_read,
				};

static int __init mychar_init(void)
{
	int i = 0;
	int t=0;
	printk(KERN_INFO "Char driver registered");

	//STEP 1: reserve <major, minor>
	if (alloc_chrdev_region(&first, 0, 3, "BITS-PILANI") < 0)  //allocating memory to files for three minor no
	{
		return -1;
	}

	//STEP 2 creation of device file
	if((cls=class_create(THIS_MODULE, "chardrv"))==NULL)
	{
		unregister_chrdev_region(first,1);
		return -1;
	}
	for (i= 0;i< 3;i++) //creating three device files for same majorno and different minor no
	{
	   if(device_create(cls, NULL,MKDEV(MAJOR(first),MINOR(first)+i), NULL, "adxl%s",dev_names[i])==NULL) 
	   {
		class_destroy(cls);
		unregister_chrdev_region(first,1);
		return -1;
	   }
	}
	//STEP 3 link fops and cdev to the device node
	for(t=1;t<4;t++) //linking fops to all the device files
        {	cdev_init(&c_dev, &fops);
		if(cdev_add(&c_dev, first, t)==-1)
	    {
		device_destroy(cls, first);
		class_destroy(cls);
		unregister_chrdev_region(first,t);
		return -1;
	     }
	}
	return 0;
}
static void __exit mychar_exit(void)
{
	int i=0;
	cdev_del(&c_dev);
	for(i=0;i<3;i++)   //for loop used for destroying all device files
	    device_destroy(cls, MKDEV(first,i));
	class_destroy(cls);
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "Bye : mychar driver unregistered\n\n");
}

module_init(mychar_init);
module_exit(mychar_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Megha Agarwal/20180H1400132G");
MODULE_DESCRIPTION("Assignment_1");

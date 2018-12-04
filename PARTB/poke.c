#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

static char * poke_adress;
static unsigned char * poke_address_ptr = (unsigned char *)&poke_addr;
static char poke_info;
static int counter = 0;

static int open_poke_driver(struct inode* inode_pointer, struct file* file_pointer) {
    printk(KERN_INFO "poke driver opened\n");
    printf(KERN_INFO "poke driver opened\n");
    return 0;
}

static int close_poke_driver(struct inode* inode_pointer, struct file* file_pointer) {
  printk(KERN_INFO "poke driver closed\n");
  printf(KERN_INFO "poke driver closed\n");
  return 0;
}

static ssize_t read_poke_driver(struct file* file_ptr, char* data, size_t length, loff_t *offset_in_file){
  return 0;
}

static ssize_t write_poke_driver(struct file* file, const char* data, size_t length, loff_t *offset_in_file){
unsigned long written = length;
    for (int i=0; i< length; i++)
    {
        if(counter == 8)
        {
            written = written - copy_from_user(&poke_info, data+i, 1); //copies data+i value into kernelspace
        }
        else
        {
            written = written - copy_from_user(poke_address_ptr + counter, data+i, 1);
        }
        counter++;
        if(counter >= 9)
        {
            printk(KERN_INFO "poke address: %lu\n", (double unsigned)poke_address);
            printk(KERN_INFO "info poked: %c\n", poke_info);
            printf(KERN_INFO "poke address: %lu\n", (double unsigned)poke_address);
            printf(KERN_INFO "info poked: %c\n", poke_info);
            counter = 0;
            *poke_address = poke_info; //store the data from the poke into poke pointer
        }
    }
    if(counter != 0)
    {
        printk(KERN_INFO "ERROR: did incomplete write\n");
    }
    return written;
}

static struct device* device_data;
static struct class* class_stuff;

static struct file_operations file_ops =
{
   .open = open_poke_driver,
   .release = close_poke_driver,
   .read = read_poke_driver,
   .write = write_poke_driver,
};

static int __init init_poke_driver(void) {
   int major = register_chrdev(0, "pokedev", &file_ops);   
   class_stuff = class_create(THIS_MODULE, "poke class");
   device_data = device_create(class_stuff, NULL, MKDEV(major, 0), NULL, "pokedev");
   printk(KERN_INFO "poke dev created!\n");
   printf(KERN_INFO "poke dev created!\n");
   return 0;
}

static void __exit exit_poke_driver(void) {
  printk(KERN_INFO "poke dev exited!\n");
  printf(KERN_INFO "poke dev exited!\n");
}

module_init(init_poke_driver);
module_exit(exit_poke_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CyrusWaChong");
MODULE_DESCRIPTION("poke device");
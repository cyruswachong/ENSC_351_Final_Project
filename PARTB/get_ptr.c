#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

static char getptr_loc[1024];  //piece of 1024 memory
static char *getptr_loc_ptr = getptr_loc //Pointer to chunk of memory

static int open_getptr_driver(struct inode* inode_pointer, struct file* file_pointer) {
  printk(KERN_INFO "get_ptr driver opened\n");
  printf(KERN_INFO "get_ptr driver opened\n");
  return 0;
  }
  
static int close_getptr_driver(struct inode* inode_pointer, struct file* file_pointer) {
  printk(KERN_INFO "get_ptr driver closed\n");
  printf(KERN_INFO "get_ptr driver closed\n");
  return 0;
}

static ssize_t read_getptr_driver(struct file* file_ptr, char* data, size_t length, loff_t *offset_in_file){
    
    if(length == 8){
        printk(KERN_INFO "Getptr's address: %lu\n", (long unsigned)getptr_loc_ptr);
        printf(KERN_INFO "Getptr's address: %lu\n", (long unsigned)getptr_loc_ptr);
        unsigned long value = copy_to_user(data, &getptr_loc_ptr, 8);               //Send address of me to user
        return 8 - value;                                                           // returns value of succesfully sent bytes
    }
    else{
        printk(KERN_INFO "ERROR: incomplete getptr read.    Not executing read.\n");
        printf(KERN_INFO "ERROR: incomplete getptr read.    Not executing read.\n");  
        return 0;
    }

}

static ssize_t write_getptr_driver(struct file* file_ptr, const char* data, size_t length, loff_t *offset_in_file){
  return 0;
}

static struct device* device_data;
static struct class* class_stuff;

static struct file_operations file_ops =
{
   .open = open_getptr_driver,
   .release = close_getptr_driver,
   .read = read_getptr_driver,
   .write = write_getptr_driver,
};

static int __init init_getptr_driver(void) {

   int major = register_chrdev(0, "getptr", &file_ops);   
   class_stuff = class_create(THIS_MODULE, "getptr class");
   device_data = device_create(class_stuff, NULL, MKDEV(major, 0), NULL, "getptrdev");
   printk(KERN_INFO "getptr dev created!\n");
   printf(KERN_INFO "getptr dev created!\n");
   return 0;
}

static void __exit exit_getptr_driver(void) {
  printk(KERN_INFO "getptr dev exited!\n");
  printf(KERN_INFO "getptr dev exited!\n");
}

module_init(init_getptr_driver);
module_exit(exit_getptr_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CyrusWaChong");
MODULE_DESCRIPTION("getprt device");

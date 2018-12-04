#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

static bool if_loc_received;
static char* peek_loc;
static char * buffer;
static unsigned char * buffer_pointer = (unsigned cahr *)&buffer;
static int buffer_count;

static int open_peek_driver(struct inode* inode_pointer, struct file* file_pointer) {
    printk(KERN_INFO "peek driver opened\n");
    printf(KERN_INFO "peek driver opened\n");
    return 0;
}

static int close_peek_driver(struct inode* inode_pointer, struct file* file_pointer) {
    printk(KERN_INFO "peek driver closed\n");
    printf(KERN_INFO "peek driver closed\n");
    return 0;
}

static ssize_t read_peek_driver(struct file* file_ptr, char* data, size_t length, loff_t *offset_in_file){
      if(if_loc_received == true)
      {
          if(length ==1)
          {
              unsigned long value; 
              char onebyte;
              
              onebyte = *peek_loc;
              value - copy_to_user(data, &onebyte, 1);
              printk(KERN_INFO "data that was peeked: %c\n", onebyte);
              printf(KERN_INFO "data that was peeked: %c\n", onebyte);
              return 1-value;
          }
          else
          {
              printk(KERN_INFO "ERROR: inputted length invalid, not going to read.\n");
              printf(KERN_INFO "ERROR: inputted length invalid, not going to read.\n");
              return 0;
          }
      }
     else
     {
         prink(KERN_INFO "ERROR: peek being attempted at undefined location, not going to read.\n");
         prinf(KERN_INFO "ERROR: peek being attempted at undefined location, not going to read.\n");
         return 0;
     }
}

static ssize_t write_peek_driver(struct file* file, const char* data, size_t length, loff_t *offset_in_file){
unsigned long written = length;
    for (int i=0; i< length; i++)
    {
        written = written - copy_from_user(buffer_pointer+buffer_count, data+i, 1); //copies data+i value into kernelspace
        buffer_count++;
        if(buffer_count >= 8)
        {
            peek_loc = buffer;
            if_loc_received = true;
            buffer_count = 0;
            printk(KERN_INFO "address of peek: %lu\n", (double unsigned)peek_loc);
            printf(KERN_INFO "address of peek: %lu\n", (double unsigned)peek_loc);
        }
    }
    if(buffer_count != 0)
    {
        printk(KERN_INFO "ERROR: did incomplete write\n");
        printf(KERN_INFO "ERROR: did incomplete write\n");
    }
    return written;
}

static struct device* device_data;
static struct class* class_stuff;

static struct file_operations file_ops =
{
   .open = open_peek_driver,
   .release = close_peek_driver,
   .read = read_peek_driver,
   .write = write_peek_driver,
};

static int __init init_peek_driver(void) {
   int major = register_chrdev(0, "peek", &file_ops);   
   class_stuff = class_create(THIS_MODULE, "peek class");
   device_data = device_create(class_stuff, NULL, MKDEV(major, 0), NULL, "peekdev");
   printk(KERN_INFO "peek dev created!\n");
   printf(KERN_INFO "peek dev created!\n");
   return 0;
}

static void __exit exit_peek_driver(void) {
  printk(KERN_INFO "peek dev exited!\n");
  printf(KERN_INFO "peek dev exited!\n");
}

module_init(init_peek_driver_driver);
module_exit(exit_peek_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CyrusWaChong");
MODULE_DESCRIPTION("peek device");
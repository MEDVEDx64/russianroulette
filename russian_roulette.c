#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/random.h>
#include <linux/fs.h>

#define DEV_NAME "rr"

MODULE_LICENSE("WTFPL");
MODULE_AUTHOR("Andrey Bobkov <medvedx64@gmail.com>");
MODULE_DESCRIPTION("Russian Roulette");

static int cass = 0;

static void roll(void) {
	cass <<= 1;
	cass |= ((cass & 64) ? 1 : 0);
	cass &= 63;
}

static void handle_input(char c) {
	if(c == 'r') {
		char c = 0;
		get_random_bytes(&c, 1);
		c &= 31;

		for(; c; --c)
			roll();
	}

	else if(c == 's') {
		if(cass & 1) {
			cass &= 62;
			panic("Congratulations! You are dead.");
		}

		roll();
	}

	else if(c == '0') {
		cass = 0;
	}

	else if(c > '0' && c < '7') {
		cass |= (1 << (c - '1'));
	}
}

static int dev_number = 0;

static int dev_open(struct inode *i, struct file *f) {
	return 0;
}

static int dev_release(struct inode *i, struct file *f) {
	return 0;
}

static ssize_t dev_read(struct file *f, char *buffer, size_t length, loff_t *offset) {
	return -EINVAL;
}

static ssize_t dev_write(struct file *f, const char *buffer, size_t length, loff_t *offset) {
	register size_t r = 0;

	if(!buffer)
		return 0;

	while(length && *buffer) {
		handle_input(*buffer);
		++buffer;
		--length;
		++r;
	}

	return r;
}

static struct file_operations fops = {
	.open = dev_open,
	.release = dev_release,
	.read = dev_read,
	.write = dev_write,
};

static int __init rr_init(void) {
	dev_number = register_chrdev(0, DEV_NAME, &fops);
	if(dev_number < 0) {
		printk("Failed to register a device: %d\n", dev_number);
		return dev_number;
	}

	printk("Russian Roulette has loaded with major number %d!\n", dev_number);
	return 0;
}

static void __exit rr_exit(void) {
	unregister_chrdev(dev_number, DEV_NAME);
	printk("Russian Roulette has unloaded!\n");
}

module_init(rr_init);
module_exit(rr_exit);

#include <linux/module.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/delay.h>

struct tp_sys_info{
        struct class *tp_class;
        int index;
        struct device *dev;
};
static atomic_t tp_device_count;
//int gesture_dubbleclick_en = 0;
static int double_gesture_switch;

extern void fts_gesture_en(int enable);
extern void synaptics_gesture_en(int enable);

int fts_get_driver_load(void);
int fts_get_finger_match(void);
int synaptics_get_finger_match(void);

static ssize_t ft_gesture_wakeup_show(struct device *dev,
                struct device_attribute *attr, char *buf)
{
        return snprintf(buf, PAGE_SIZE, "%d\n",double_gesture_switch);
}

static ssize_t ft_gesture_wakeup_store(struct device *dev,
                struct device_attribute *attr, const char *buf, size_t count)
{
       int val;

       sscanf(buf, "%d", &val);
	if(val)
		double_gesture_switch = 1;
	else
		double_gesture_switch = 0;

	fts_gesture_en(double_gesture_switch);
	synaptics_gesture_en(double_gesture_switch);

       return count;

}

static ssize_t ft_finger_match_show(struct device *dev,
                struct device_attribute *attr, char *buf)
{
	int ft_finger_match = 0;

	if (fts_get_driver_load() == 1)
		msleep(20);
	else
		msleep(35);

	if (fts_get_driver_load() == 1)
		ft_finger_match = fts_get_finger_match();
	else
		ft_finger_match = synaptics_get_finger_match();

        return snprintf(buf, PAGE_SIZE, "%d\n",ft_finger_match);
}

static ssize_t ft_finger_match_store(struct device *dev,
                struct device_attribute *attr, const char *buf, size_t count)
{
       int val;

       sscanf(buf, "%d", &val);

       return count;
}

static struct device_attribute attrs[] = {
        __ATTR(gesture_on, 0664,
                        ft_gesture_wakeup_show,
                        ft_gesture_wakeup_store),
        __ATTR(finger_match, 0664,
                        ft_finger_match_show,
                        ft_finger_match_store),
};

static int tp_gesture_ctl_class(void)
{
       int attr_count = 0;
	int err;
 	struct tp_sys_info *ts;

       ts = kzalloc(sizeof(*ts), GFP_KERNEL);
       memset(ts, 0, sizeof(*ts));
       ts->tp_class = class_create(THIS_MODULE, "touch");
       if (IS_ERR(ts->tp_class))
       {
       	printk("create tp class err!");
	}
	else
     	       atomic_set(&tp_device_count, 0);

	ts->index = atomic_inc_return(&tp_device_count);
       ts->dev = device_create(ts->tp_class, NULL,
       MKDEV(0, ts->index), NULL, "tp_dev");
	if (IS_ERR(ts->dev))
	{
              printk("create device err!");
	}
       for (attr_count = 0; attr_count < ARRAY_SIZE(attrs); attr_count++) {
              err = sysfs_create_file(&ts->dev->kobj,
                              &attrs[attr_count].attr);
              if (err < 0) {
                   pr_err("%s: Failed to create sysfs attributes\n", __func__);
                   return err;
             }
        }
        dev_set_drvdata(ts->dev,ts);
        //end tp class to show tp info

	return 0;
}

static int __init tp_gesture_ctl_init(void)
{
	return tp_gesture_ctl_class();
}

static void __exit tp_gesture_ctl_exit(void)
{
	//if (ts & ts->tp_class)
	//	class_destroy(ts->tp_class);
}

module_init(tp_gesture_ctl_init);
module_exit(tp_gesture_ctl_exit);

MODULE_AUTHOR("xxx <xxx@android.com>");
MODULE_DESCRIPTION("tp gesture control class driver");
MODULE_LICENSE("GPL");

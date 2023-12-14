/**
 * Top and bottom half interrupt handling
 */
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>

/* Macro DECLARE_TASKLET_OLD exists for compatibility */
#ifndef DECLARE_TASKLET_OLD
#define DECLARE_TASKLET_OLD(arg1, arg2) DECLARE_TASKLET(arg1, arg2, 0L)
#endif

static int button_irqs[] = {
        -1,
        -1,
};

/* Define GPIOs for LEDs */
static struct gpio leds[] = {
        {
                4,
                GPIOF_OUT_INIT_LOW,
                "LED 1",
        },
};

/* Define GPIOs for BUTTONs */
static struct gpio buttons[] = {
        {
                17,
                GPIOF_IN,
                "LED 1 ON BUTTON",
        },
        {
                18,
                GPIOF_IN,
                "LED 1 OFF BUTTON",
        },
};

/* Tasklet containing some non-trivial amount of processing */
static void button_tasklet_fn(unsigned long data)
{
        pr_info("tasklet_fn started\n");

        /* do something which takes awhile */
        mdelay(500);

        pr_info("tasklet_fn exiting\n");
}

static DECLARE_TASKLET_OLD(buttontask, button_tasklet_fn);

/* interrupt function triggered when a button is pressed */
static irqreturn_t button_isr(int irq, void *data)
{
        /* do something quickly right now */
        if (irq == button_irqs[0] && !gpio_get_value(leds[0].gpio))
                gpio_set_value(leds[0].gpio, 1);
        if (irq == button_irqs[1] && gpio_get_value(leds[0].gpio))
                gpio_set_value(leds[0].gpio, 0);

        /* do the rest at leisure via the scheduler */
        tasklet_schedule(&buttontask);

        return IRQ_HANDLED;
}

static int __init bottomhalf_example_init(void)
{
        int ret = 0;

        pr_info("bottomhalf_example_init started\n");

        /* register LED gpios */
        ret = gpio_request_array(leds, ARRAY_SIZE(leds));
        if (ret) {
                pr_err("unable to request GPIOs for LEDs: %d\n", ret);
                return ret;
        }

        /* register BUTTON gpios */
        ret = gpio_request_array(buttons, ARRAY_SIZE(buttons));
        if (ret) {
                pr_err("unable to request GPIOs for BUTTONs: %d\n", ret);
                goto free_led_gpio;
        }

        ret = gpio_to_irq(buttons[0].gpio);
        if (ret < 0) {
                pr_err("unable to request IRQ: %d\n", ret);
                goto free_button_gpio;
        }
        button_irqs[0] = ret;

        ret = request_irq(button_irqs[0],
                          button_isr,
                          IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                          "gpiomod#button1",
                          NULL);
        if (ret) {
                pr_err("unable to request IRQ: %d\n", ret);
                goto free_button_gpio;
        }

        ret = gpio_to_irq(buttons[1].gpio);
        if (ret < 0) {
                pr_err("unable to request IRQ: %d\n", ret);
                goto free_irq;
        }
        button_irqs[1] = ret;

        ret = request_irq(button_irqs[1],
                          button_isr,
                          IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                          "gpiomod#button2",
                          NULL);
        if (ret) {
                pr_err("unable to request IRQ: %d\n", ret);
                goto free_irq;
        }

        return 0;
free_irq:
        free_irq(button_irqs[0], NULL);
free_button_gpio:
        gpio_free_array(buttons, ARRAY_SIZE(buttons));
free_led_gpio:
        gpio_free_array(leds, ARRAY_SIZE(leds));
        return ret;
}

static void __exit bottomhalf_example_exit(void)
{
        int i;

        pr_info("bottomhalf_example_exit started\n");

        free_irq(button_irqs[0], NULL);
        free_irq(button_irqs[1], NULL);

        for (i = 0; i < ARRAY_SIZE(leds); i++)
                gpio_set_value(leds[i].gpio, 0);

        gpio_free_array(buttons, ARRAY_SIZE(buttons));
        gpio_free_array(leds, ARRAY_SIZE(leds));
}

module_init(bottomhalf_example_init);
module_exit(bottomhalf_example_exit);

MODULE_DESCRIPTION("tasklet example");
MODULE_LICENSE("GPL");

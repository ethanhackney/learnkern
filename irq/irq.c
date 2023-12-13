/**
 * Handling GPIO with interrupts
 */
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>

static int button_irqs[] = {
        -1,
        -1,
};

/* define GPIOs for LEDs */
static struct gpio leds[] = {
        {
                4,
                GPIOF_OUT_INIT_LOW,
                "LED 1",
        },
};

/* define GPIOs for BUTTONs */
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

/* interrupt function triggered when a button is pressed */
static irqreturn_t button_isr(int irq, void *data)
{
        /* first button */
        if (irq == button_irqs[0] && !gpio_get_value(leds[0].gpio))
                gpio_set_value(leds[0].gpio, 1);
        /* second button */
        else if (irq == button_irqs[1] && gpio_get_value(leds[0].gpio))
                gpio_set_value(leds[0].gpio, 0);

        return IRQ_HANDLED;
}

static int __init example_irq_init(void)
{
        int ret = 0;

        /* register LED gpios */
        ret = gpio_request_array(leds, ARRAY_SIZE(leds));
        if (ret) {
                pr_err("unable to request GPIOs for LEDS: %d\n", ret);
                return ret;
        }
        pr_info("LEDs registered\n");

        /* register BUTTON gpios */
        ret = gpio_request_array(buttons, ARRAY_SIZE(buttons));
        if (ret) {
                pr_err("unable to request GPIOs for BUTTONs: %d\n", ret);
                goto free_leds;
        }
        pr_info("BUTTONs registered\n");

        ret = gpio_to_irq(buttons[0].gpio);
        if (ret < 0) {
                pr_err("unable to request IRQ: %d\n", ret);
                goto free_buttons;
        }
        button_irqs[0] = ret;

        ret = request_irq(button_irqs[0],
                          button_isr,
                          IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                          "gpiomod#button1",
                          NULL);
        if (ret) {
                pr_err("unable to request IRQ: %d\n", ret);
                goto free_buttons;
        }
        pr_info("button[0] irq registered\n");

        ret = gpio_to_irq(buttons[1].gpio);
        if (ret < 0) {
                pr_err("unable to request IRQ: %d\n", ret);
                goto free_buttons;
        }
        button_irqs[1] = ret;

        ret = request_irq(button_irqs[1],
                          button_isr,
                          IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                          "gpiomod#button2",
                          NULL);
        if (ret) {
                pr_err("unable to request IRQ: %d\n", ret);
                goto free_button0_irq;
        }
        pr_info("button[1] irq registered\n");

        return 0;
free_button0_irq:
        free_irq(button_irqs[0], NULL);
free_buttons:
        gpio_free_array(buttons, ARRAY_SIZE(buttons));
free_leds:
        gpio_free_array(leds, ARRAY_SIZE(leds));
        return ret;
}

static void __exit example_irq_exit(void)
{
        int i;

        free_irq(button_irqs[1], NULL);
        free_irq(button_irqs[0], NULL);

        for (i = 0; i < ARRAY_SIZE(leds); i++)
                gpio_set_value(leds[i].gpio, 0);

        gpio_free_array(buttons, ARRAY_SIZE(buttons));
        gpio_free_array(leds, ARRAY_SIZE(leds));
}

module_init(example_irq_init);
module_exit(example_irq_exit);

MODULE_DESCRIPTION("irq example");
MODULE_LICENSE("GPL");

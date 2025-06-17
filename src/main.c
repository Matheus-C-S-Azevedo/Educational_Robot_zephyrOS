#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

#define STACKSIZE 2048
#define PRIORITY 7
#define STEP PWM_USEC(50)

#define SERVO1 DT_ALIAS(servo1)
#define SERVO2 DT_ALIAS(servo2)

#if !DT_NODE_HAS_STATUS_OKAY(SERVO1)
#error "Unsupported board: SERVO1 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS_OKAY(SERVO2)
#error "Unsupported board: SERVO2 devicetree alias is not defined"
#endif

enum direction {
	UP,
	DOWN,
};

struct servo_config {
	struct pwm_dt_spec pwm;
	uint32_t pulse_width;
	enum direction dir;
	uint32_t min_pulse;
	uint32_t max_pulse;
};

static struct servo_config servo1 = {
	.pwm = PWM_DT_SPEC_GET(SERVO1),
	.pulse_width = DT_PROP(SERVO1, min_pulse),
	.dir = UP,
	.min_pulse = DT_PROP(SERVO1, min_pulse),
	.max_pulse = DT_PROP(SERVO1, max_pulse),
};

static struct servo_config servo2 = {
	.pwm = PWM_DT_SPEC_GET(SERVO2),
	.pulse_width = DT_PROP(SERVO2, max_pulse),
	.dir = DOWN,
	.min_pulse = DT_PROP(SERVO2, min_pulse),
	.max_pulse = DT_PROP(SERVO2, max_pulse),
};

void servo_motor(void *pctx, void *unused1, void *unused2)
{
	struct servo_config *ctx = pctx;

	while (1) {
		pwm_set_pulse_dt(&ctx->pwm, ctx->pulse_width);

		if (ctx->dir == UP) {
			if (ctx->pulse_width + STEP >= ctx->max_pulse) {
				ctx->pulse_width = ctx->max_pulse;
				ctx->dir = DOWN;
			} else {
				ctx->pulse_width += STEP;
			}
		} else {
			if (ctx->pulse_width <= ctx->min_pulse + STEP) {
				ctx->pulse_width = ctx->min_pulse;
				ctx->dir = UP;
			} else {
				ctx->pulse_width -= STEP;
			}
		}

		k_sleep(K_MSEC(200)); 
	}
}

int main(void)
{
	printk("Controle de dois servos iniciado\n");
	
	
	return 0;
}

K_THREAD_DEFINE(servo1_id, STACKSIZE, servo_motor, &servo1, NULL, NULL,	PRIORITY, 0, 0);
K_THREAD_DEFINE(servo2_id, STACKSIZE, servo_motor, &servo2, NULL, NULL, PRIORITY, 0, 0);

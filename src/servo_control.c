#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(servo_control, LOG_LEVEL_INF);

#define STACKSIZE 1024
#define PRIORITY 5
#define STEP PWM_USEC(50)

#define NUM_SERVOS 4

#define SERVO_ALIAS(n) DT_ALIAS(servo##n)

#if !DT_NODE_HAS_STATUS_OKAY(SERVO_ALIAS(1))
#error "Alias servo1 não definido"
#endif
#if !DT_NODE_HAS_STATUS_OKAY(SERVO_ALIAS(2))
#error "Alias servo2 não definido"
#endif
#if !DT_NODE_HAS_STATUS_OKAY(SERVO_ALIAS(3))
#error "Alias servo3 não definido"
#endif
#if !DT_NODE_HAS_STATUS_OKAY(SERVO_ALIAS(4))
#error "Alias servo4 não definido"
#endif

uint32_t servo1_posicao = 0;


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

static struct servo_config servos[NUM_SERVOS] = {
    {
        .pwm = PWM_DT_SPEC_GET(SERVO_ALIAS(1)),
        .pulse_width = DT_PROP(SERVO_ALIAS(1), min_pulse),
        .dir = UP,
        .min_pulse = DT_PROP(SERVO_ALIAS(1), min_pulse),
        .max_pulse = DT_PROP(SERVO_ALIAS(1), max_pulse),
    },
    {
        .pwm = PWM_DT_SPEC_GET(SERVO_ALIAS(2)),
        .pulse_width = DT_PROP(SERVO_ALIAS(2), max_pulse),
        .dir = DOWN,
        .min_pulse = DT_PROP(SERVO_ALIAS(2), min_pulse),
        .max_pulse = DT_PROP(SERVO_ALIAS(2), max_pulse),
    },
    {
        .pwm = PWM_DT_SPEC_GET(SERVO_ALIAS(3)),
        .pulse_width = DT_PROP(SERVO_ALIAS(3), min_pulse),
        .dir = UP,
        .min_pulse = DT_PROP(SERVO_ALIAS(3), min_pulse),
        .max_pulse = DT_PROP(SERVO_ALIAS(3), max_pulse),
    },
    {
        .pwm = PWM_DT_SPEC_GET(SERVO_ALIAS(4)),
        .pulse_width = DT_PROP(SERVO_ALIAS(4), max_pulse),
        .dir = DOWN,
        .min_pulse = DT_PROP(SERVO_ALIAS(4), min_pulse),
        .max_pulse = DT_PROP(SERVO_ALIAS(4), max_pulse),
    },
};

static void servo_motor(void *p1, void *p2, void *p3)
{
    struct servo_config *ctx = p1;

    while (1) {
        pwm_set_pulse_dt(&ctx->pwm, ctx->pulse_width);
        if (ctx == &servos[0]) {
            servo1_posicao = ctx->pulse_width;
        }

        LOG_INF("Servo ajustado para %d us", ctx->pulse_width);

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

K_THREAD_DEFINE(servo1_thread_id, STACKSIZE, servo_motor, &servos[0], NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(servo2_thread_id, STACKSIZE, servo_motor, &servos[1], NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(servo3_thread_id, STACKSIZE, servo_motor, &servos[2], NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(servo4_thread_id, STACKSIZE, servo_motor, &servos[3], NULL, NULL, PRIORITY, 0, 0);

void servo_control_init(void) {
    for (int i = 0; i < NUM_SERVOS; i++) {
        if (!device_is_ready(servos[i].pwm.dev)) {
            LOG_ERR("PWM %d não está pronto!", i);
        } else {
            pwm_set_pulse_dt(&servos[i].pwm, servos[i].pulse_width);
        }
    }
}
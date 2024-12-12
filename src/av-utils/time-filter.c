#include "time-filter.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#define FMAX(a,b) ((a) > (b) ? (a) : (b))

struct time_filter_s {
    double cycle_time;
    double feedback2_factor;
    double feedback3_factor;
    double clock_period;
    int count;
};

static double qexpneg(double x) {
    return 1 - 1 / (1 + x * (1 + x / 2 * (1 + x / 3)));
}

time_filter_t* time_filter_create(double time_base, double period, double bandwidth) {
    time_filter_t* self = malloc(sizeof(time_filter_t));
    if (!self) {
        return NULL;
    }
    double o = 2 * M_PI * bandwidth * period * time_base;

    self->clock_period = time_base;
    self->feedback2_factor = qexpneg(M_SQRT2 * o);
    self->feedback3_factor = qexpneg(o * o) / period;
    return self;
}

void time_filter_destroy(time_filter_t* self) {
    if (self) {
        free(self);
        self = NULL;
    }
}

void time_filter_reset(time_filter_t* self) {
    self->count = 0;
}

double time_filter_update(time_filter_t* self, double system_time, double period) {
    self->count++;
    if (self->count == 1) {
        self->cycle_time = system_time;
    }
    else {
        double loop_error;
        self->cycle_time += self->clock_period * period;
        loop_error = system_time - self->cycle_time;

        self->cycle_time += FMAX(self->feedback2_factor, 1.0 / self->count) * loop_error;
        self->clock_period += self->feedback3_factor * loop_error;
    }
    return self->cycle_time;
}

double time_filter_eval(time_filter_t* self, double delta) {
    return self->cycle_time + self->clock_period * delta;
}

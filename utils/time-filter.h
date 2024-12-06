_Pragma("once")

typedef struct time_filter_s time_filter_t;

time_filter_t* time_filter_create(double time_base, double period, double bandwidth);
double time_filter_update(time_filter_t* self, double system_time, double period);
double time_filter_eval(time_filter_t* self, double delta);
void time_filter_reset(time_filter_t*);
void time_filter_destroy(time_filter_t*);


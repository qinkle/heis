//Timer code

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

clock_t init_time;
unsigned int diff = 3; //Time in seconds (må kanskje endre til millisek her)
unsigned int timer_is_on = 0; //set to 1 if timer i active

void timer_start() {
    timer_is_on = 1;
    init_time = clock();
    
}

int timer_is_out() {
    
    clock_t current_time = clock();
    
    if ((current_time - init_time) / CLOCKS_PER_SEC >= diff && timer_is_on) {
        timer_is_on = 0;
        return 1;
    }
    
    return 0;
}



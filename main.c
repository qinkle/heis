#include "queue.h"
#include "elev.h"
#include "FSM.h"
#include "timer.h"
#include "io.h"
#include "channels.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    
    fsm_init();
	
	//Go to defined state without heeding buttons
elev_set_motor_direction(DIRN_UP);
	while (elev_get_floor_sensor_signal() == -1){}
	elev_set_motor_direction(DIRN_STOP);
	fsm_floor_sensor();

	
	//Initialize state machine and queue

	queue_init();
    
    while (1) {
		if (elev_get_stop_signal()) {
			fsm_stop_button_pressed();
		}
        
		if (elev_get_floor_sensor_signal()) {
			fsm_floor_sensor();
		}
        
		if (elev_any_button_pressed()) {
			fsm_button_pressed();
		}
        
        if (timer_is_out()){
            fsm_timer_is_out();
        }
        
        if (!elev_get_stop_signal()){
            fsm_stop_button_released();
        }
        
        if (!queue_is_empty()){
            fsm_queue_not_empty();
        }
        
        if (queue_stop_here()) {
            fsm_floor_is_ordered();
        }
        
    }

    return 0;
}
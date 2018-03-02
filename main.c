#include "elev.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
	
	//Go to defined state without heeding buttons
    elev_set_motor_direction(DIRN_UP);
	while (elev_get_floor_sensor_signal == -1){}
	elev_set_motor_direction(DIRN_STOP);
	fsm_floor_sensor();
	
	
	//Initialize state machine and queue
	fsm_init();
	queue_init();
	
    while (1) {
		if (elev_get_stop_signal() != stop){
			fsm_stop_signal(); //Implementer!
		}
		if (elev_get_floor_sensor_signal() != -1){
			fsm_floor_sensor(); //Implementer!
		}
		if (elev_order_button_pressed() /*Implementer! */){
			fsm_button_pressed(); //Implementer!
		}
	
    }

    return 0;
}

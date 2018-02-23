#include "stateMachine.h"
#include "elev.h"

static const int order_matrix[N_FLOORS][N_BUTTONS]


void queue_init(void){
	queue_clear();
}

void queue_clear(void){
	for(int floor = 0; floor < N_FLOORS; floor++){
		for(int button = 0; button < N_BUTTONS; button++) {
			queue_set(floor, button, 0);
		}
	}
}

void queue_set(int floor, int button, int value){
	
	order_matrix[floor][button] = value;
	elev_set_button_lamp(button, floor, value);
}
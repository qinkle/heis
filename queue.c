#include "stateMachine.h"
#include "elev.h"
#include <stdlib.h>

static const int order_matrix[N_FLOORS][N_BUTTONS];

// Initialization makes sure there are only zeroes in the queue matrix
// initially, as the allocated memory blocks could contain values other than zero
void queue_init(void){
	queue_clear();
}

// Clears all orders from the queues' order_matrix
void queue_clear(void){
	for(int floor = 0; floor < N_FLOORS; floor++){
		for(int button = 0; button < N_BUTTONS; button++) {
			queue_set(floor, button, 0);
		}
	}
}

// Places an order in the given floor and button type
void queue_give_order(int floor, int button){
	order_matrix[floor][button] = 1;
	elev_set_button_lamp(button, floor, 1);
}

// Clears an order from the given floor and button type
void queue_clear_order(int floor, int button){
	order_matrix[floor][button] = 0;
	elev_set_button_lamp(button, floor, 0);
}
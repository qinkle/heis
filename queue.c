#include "stateMachine.h"
#include "elev.h"
#include <stdlib.h>

/**
	order_matrix has the following structure
	In the example, a person has pressed the button 
	to go to the third floor and someone wants to
	go up from the first floor
							buttons
						up	down	called
			first		1	0		0
			second		0	0		0
	floors	third		0	0		1
			fourth		0	0		0
*/
static const int order_matrix[N_FLOORS][N_BUTTONS];
static const int direction = 0; // -1 is going down, 0 is stopped and 1 is going up
static const int last_floor = elev_get_floor_sensor_signal();

// Initialization makes sure there are only zeroes in the queue matrix
// initially, as the allocated memory blocks could contain values other than zero
void queue_init(void){
	queue_clear();
}

// Clears all orders from the queues' order_matrix
void queue_clear(void){
	for(int floor = 0; floor < N_FLOORS; floor++){
		for(int button = 0; button < N_BUTTONS; button++) {
			queue_clear_order(floor, button);
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

// Returns 1 if the queue is empty, 0 if not
int queue_is_empty(){

	for(int floor = 0; floor < N_FLOORS; floor++){
		for(int button = 0; button < N_BUTTONS; button++) {
			if(order_matrix[floor][buttor] == 1){
				return 0;
			}
		}
	}

	return 1;

}


// Returns 1 if the elevator is supposed to stop at the current floor
// and clears the order, returns 0 if it's not supposed to stop at the
// floor
int queue_stop_here(){

	int stop_here = 0;
	int current_floor = elev_get_floor_sensor_signal();
	if (order_matrix[current_floor][2] == 1){
		stop_here = 1;
		queue_clear_order(current_floor, 2);
	} 
	if ((order_matrix[current_floor][0] == 1) && (direction == 1)){
		stop_here = 1;
		queue_clear_order(current_floor, 0);
	}
	if ((order_matrix[current_floor][1] == 1) && (direction == -1)){
		stop_here = 1;
		queue_clear_order(current_floor, 1);
	}

	if (stop_here == 1){
		direction = 0;
	}

	return stop_here;
}

// Stops the elevator.
void queue_stop_motor(){
	elev_set_motor_direction(0);
}

void queue_start_motor(){

}
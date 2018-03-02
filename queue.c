#include "stateMachine.h"
#include "elev.h"
#include <stdlib.h>
#include <stdio.h>


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
static const int direction = 0; // -1 is going down, 0 is stopped at a floor without orders and 1 is going up
static const int last_floor = -1;

// Initialization makes sure there are only zeroes in the queue matrix
// initially, as the allocated memory blocks could contain values other than zero
void queue_init(void){
	queue_clear();
	last_floor = elev_get_floor_sensor_signal();
	direction = 0;
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
void queue_give_order(int floor, elev_button_type_t button){
	order_matrix[floor][button] = 1;
	elev_set_button_lamp(button, floor, 1);
}

// Clears an order from the given floor and button type
void queue_clear_order(int floor, elev_button_type_t button){
	order_matrix[floor][button] = 0;
	elev_set_button_lamp(button, floor, 0);
}


// Checks every button if they're pressed and places orders if
// they're been given
void queue_any_button_pressed(){
	for(int floor = 0; floor < N_FLOORS; floor++){
		for(int button = 0; button < N_BUTTONS; button++){
			if (elev_get_button_signal(button, floor) == 1){
				queue_give_order(floor, button);
			}
		}
	}

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


	if (order_matrix[current_floor][BUTTON_COMMAND] == 1){
		stop_here = 1;
		queue_clear_order(current_floor, 2);
	} 
	if ((order_matrix[current_floor][BUTTON_CALL_UP] == 1) && ( (direction == 1) || (direction == 0) )) {
		stop_here = 1;
		queue_clear_order(current_floor, 0);
	}
	if ((order_matrix[current_floor][BUTTON_CALL_DOWN] == 1) && ( (direction == -1) || (direction == 0) )) {
		stop_here = 1;
		queue_clear_order(current_floor, 1);
	}

	if (queue_is_last_stop()){
		stop_here = 1;
		if (queue_is_empty()){
			direction = 0;
		}

		for(int button = 0; button < N_BUTTONS; button++){
			queue_clear_order(current_floor, button);
		}
	}

	return stop_here;
}

// Stops the elevator.
void queue_stop_motor(){
	elev_set_motor_direction(0);
}



// Checks where the elevator is at the moment and decides where to move the elevator
// It only changes the direction of the elevator if there are no new orders in the direction
// the elevator is already moving to avoid people waiting for an elevator that never
// arrives
void queue_start_motor(){
	int current_floor = elev_get_floor_sensor_signal();

	if(queue_is_empty()){ // Does not start the motor unless there is an order
		return;
	}

	if ((current_floor == -1) ){ // If the elevator is between floors
		for(int floor = 0; floor < N_FLOORS; floor++){
			for(int button = 0; button < N_BUTTONS; button++){

				if (order_matrix[floor][button] == 1){
					if (floor < last_floor){
						direction = -1;
					}
					if (floor > last_floor){
						direction = 1;
					}
					if (floor == last_floor){
						direction = -direction;
					}
					elev_set_motor_direction(direction);

					return;
				}

			}
		}
	}

	if (direction == 0) { // The elevator is standing still on a floor, it therefore calculates the closest order and starts moving in that direction
		int distance = 99; // The distance a given order is from the elevators' current position

		for(int floor = 0; floor < N_FLOORS; floor++){
			for(int button = 0; button < N_BUTTONS; button++){

				if (order_matrix[floor][button] == 1){
					int distance_evaluation = floor - current_floor;

					if (abs(distance_evaluation) < abs(distance)){
						distance = distance_evaluation;
					}

				}
			}
		}
		if (distance != 99){
			direction = distance/ abs(distance);
			elev_set_motor_direction(direction);
		}
		return;
	}

	// If there are no more orders beyond the current floor, the elevator switches direction and starts the motor
	if (queue_is_last_stop()){
		direction = -direction;
		elev_set_motor_direction(direction);
		return;
	}
	
	// If none of the statements above apply, the elevator moves in the same direction it was going in
	elev_set_motor_direction(direction);
	return;


}

int queue_is_last_stop(){

	int current_floor = elev_get_floor_sensor_signal();
	int is_last_stop = 1;

	if (current_floor == 0 || current_floor == N_BUTTONS -1){ // If the elevator is in the top or bottom floor
		return 1;
	}

	for(int floor = current_floor + direction; floor < N_FLOORS; floor = floor + direction){
		for(int button = 0; button < N_BUTTONS; button++){
			if (order_matrix[floor][button] == 1){
				is_last_stop == 0;
			}
		}
	}

	return is_last_stop;
}
//Header for queue.c


/**
	Initialization makes sure there are only zeroes in the queue matrix
	initially, as the allocated memory blocks could contain values other than zero
*/
void queue_init(void);

/**
	Clears all orders from the queues' order_matrix
*/
void queue_clear(void);

/**
	Places an order in the given floor and button type
*/
void queue_give_order(int floor, int button);

/**
	Clears an order from the given floor and button type
*/
void queue_clear_order(int floor, int button);

// Checks every button if they're pressed and places orders if
// they're been given
void queue_any_button_pressed(void);

// Returns 1 if the queue is empty, 0 if not
int queue_is_empty(void);

// Returns 1 if the elevator is supposed to stop at the current floor
// and clears the order, returns 0 if it's not supposed to stop at the
// floor
int queue_stop_here();

// Stops the elevator.
void queue_stop_motor();

// Checks where the elevator is at the moment and decides where to move the elevator
// It only changes the direction of the elevator if there are no new orders in the direction
// the elevator is already moving to avoid people waiting for an elevator that never
// arrives
void queue_start_motor();


// Checks if there are any orders beyond the current position in the direction the elevator is moving
// Returns 1 if there are none and 0 if there are.
int queue_is_last_stop();
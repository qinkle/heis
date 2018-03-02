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
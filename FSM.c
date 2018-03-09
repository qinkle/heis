#include "elev.h"
#include "queue.h"
#include "timer.h"

enum FSM_STATE {
    ELEVATOR_MOVES,
    ELEVATOR_STOPPED,
    EMERGENCY_STOP,
    DOORS_OPEN
} STATE; // Holds curent state

void fsm_init(void) {
    if (elev_get_floor_sensor_signal()) {
        STATE = ELEVATOR_STOPPED;
    }
    else {
        STATE = ELEVATOR_MOVES
    }
}

void fsm_stop_button_pressed(){
    
    queue_clear();
    
    switch (STATE) {
            
        case ELEVATOR_MOVES:
            queue_stop_motor();
            break;
            
        case DOORS_OPEN:
            elev_set_door_open_lamp(0); //Closes doors
            break;
            
        default:
            break;
    }
    
    STATE = EMERGENCY_STOP;
    
}

void fsm_stop_button_released() {
    
    switch (STATE) {
            
        case EMERGENCY_STOP:
            
            if (!elev_get_floor_sensor_signal()){
                STATE = ELEVATOR_STOPPED;
            }
            
            else if (elev_get_floor_sensor_signal()) {
                elev_set_door_open_lamp(1);
                timer_start();
                STATE = DOORS_OPEN;
            }
            
            break;
            
        default:
            break;
    }
    
}

void fsm_floor_sensor() {
    
    switch (STATE) {
            
        case ELEVATOR_MOVES:
            
            if (queue_stop_here()) {
                queue_stop_motor();
                STATE = ELEVATOR_STOPPED;
            }
            
            break;
            
        default:
            break;
    }
}

void fsm_button_pressed() {
    
    switch (STATE) {
            
        case EMERGENCY_STOP:
            //Ignore orders when in EMERGENCY_STOP state
            break;
            
        default:
            //Tells queue module that an order has been made
            queue_any_button_pressed();
            break;
    }
}

void fsm_timer_is_out() {
    
    switch (STATE) {
            
        case DOORS_OPEN:
            elev_set_door_open_lamp(0);
            STATE = ELEVATOR_STOPPED;
            break;
            
        default:
            break;
    }
    
}

void fsm_queue_not_empty() {
    
    switch (STATE) {
            
        case ELEVATOR_STOPPED:
            queue_start_motor();
            STATE = ELEVATOR_MOVES;
            break;
            
        default:
            break;
    }
    
}

void fsm_floor_is_ordered() {
    
    switch (STATE) {
            
        case ELEVATOR_STOPPED:
            elev_set_door_open_lamp(0);
            timer_start();
            STATE = DOORS_OPEN;
            break;
            
        default:
            break;
    }
}





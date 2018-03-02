#include "elev.h"
#include "queue.h"
#include "timer.h"

enum FSM_STATE {
    ELEVATOR_MOVES,
    ELEVATOR_STOPPED,
    EMERGENCY_STOP,
    DOORS_OPEN
};

FSM_STATE STATE;

void fsm_stop_signal(){
    
    queue_clear(); //Kan denne ligger her?
    
    switch (STATE) {
            
        case ELEVATOR_MOVES:
            queue_stop_motor();
            break;
            
        case ELEVATOR_STOPPED:
            break;
            
        case DOORS_OPEN:
            elev_set_door_open_lamp(0);
            break;
            
        default:
            break;
    }
    
    STATE = EMERGENCY_STOP; //Kan denne ligger her?
    
}

void fsm_stop_button_released() {
    
    switch (STATE) {
            
        case EMERGENCY_STOP:
            
            if (!elev_get_floor_sensor_signal()){
                STATE = ELEVATOR_STOPPED;
            }
            
            else if (elev_get_floor_sensor_signal()) {
                elev_set_door_open_lamp(1);
                timer_start_timer();
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
                queue_stop_motor();;
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
            // Ignorerer bestillinger i EMERG_STOP
            break;
            
        default:
            queue_any_button_pressed();
            break;
    }
}

void fsm_timer_is_out() {
    
    switch (STATE) {
            
        case DOORS_OPEN:
            timer_stop_timer();
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
            timer_start_timer();
            STATE = DOORS_OPEN;
            break;
            
        default:
            break;
    }
}





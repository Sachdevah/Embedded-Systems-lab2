/* ------------------------------------------
       ECS642/ECS714 Lab2

   The aim of this lab exercise is to understand how to react
   to button presses.
  -------------------------------------------- */
  
#include <stdbool.h>
#include <stdint.h>
#include <MKL25Z4.h>

#include "..\inc\SysTick.h"
#include "..\inc\led.h"
#include "..\inc\button.h"

/* --------------------------------------
     Documentation of the Given Code
     ==============================

     Behaviour
     ---------     
     Two buttons on the shield are used:
        * Button B1 is polled. Each press toggles the state of the LED1 
        * Button B5 has an interrupt handler. Each press toggles state of LED5
     
     Files
     -----
     The following file may need to be changed to complete the exercise
     
     main.c  Contains the main loop with 3 tasks and an interrupt handler
     
     The following files should not be changed:
       mask.h   Definition of the MASK macro
       button.h  Definitions and API for shield buttons
       button.c  Code to configure buttons and detect button position
       led.h   Definitions and API for shield LEDs
       led.c   Code to configure and switch shield LEDs
       SysTick.h  Definitions and API for SysTick timer
       SySTick.c  Code for SySTick timer
 -------------------------------------- */

/*----------------------------------------------------------------------------
  Variables for communication
*----------------------------------------------------------------------------*/
bool pressedB1_ev ;  // set by task1 (polling) and cleared by task 2
volatile uint32_t pressedB5_ev ;  // set by interrupt handler
                                  //   Cleared by the task3
                                  
/*----------------------------------------------------------------------------
  task1pollB1
  
  This task polls button B1
*----------------------------------------------------------------------------*/
int b1State ;        // Current state - corresponds to position
int b1BounceCount ;

void initTask1() {
    b1State = BOPEN ;
    pressedB1_ev = false ; 
    b1BounceCount = 0 ;
}

void task1pollB1() {
    if (b1BounceCount > 0) b1BounceCount -- ;
    switch (b1State) {
        case BOPEN:
            if (isPressed(B1MASK)) {
                b1State = BCLOSED ;
                pressedB1_ev = true ; 
            }
          break ;

        case BCLOSED:
            if (!isPressed(B1MASK)) {
                b1State = BBOUNCE ;
                b1BounceCount = BOUNCEDELAY ;
            }
            break ;

        case BBOUNCE:
            if (isPressed(B1MASK)) {
                b1State = BCLOSED ;
            }
            else if (b1BounceCount == 0) {
                b1State = BOPEN ;
            }
            break ;
    }                
}


/*----------------------------------------------------------------------------
  task2LED1
  
  This task toggles the state of LED1 when the 'B1 pressed' event occurs
*----------------------------------------------------------------------------*/
int stateLED1 ;  // this variable holds the current state

void initTask2() {
    stateLED1 = MoveLit1to5;        // initialise the state 
    ledOnOff (LED1, LED_ON) ;  // turn the LED off
}
int TIMER=timer;
int i=0;
void LEDSon(){  //turns all LED's ON
		ledOnOff(LED1,LED_ON);
		ledOnOff(LED2,LED_ON);
		ledOnOff(LED3,LED_ON);
		ledOnOff(LED4,LED_ON);
		ledOnOff(LED5,LED_ON);
}
void LEDSoff(){		//turns all LED's OFF
		ledOnOff(LED1,LED_OFF);
		ledOnOff(LED2,LED_OFF);
		ledOnOff(LED3,LED_OFF);
		ledOnOff(LED4,LED_OFF);
		ledOnOff(LED5,LED_OFF);
}

void LabWork() {			//all the code that makes B1 and B5 work
	if(TIMER>0){
		TIMER--;
	}

	int LEDS[]={LED1, LED2, LED3, LED4,LED5};
	

  switch (stateLED1) {
			
  case MoveLit1to5:  // case for making LED lit from LED1 to LED5 when B1 is pressed
           
			if(TIMER==0 ){
						TIMER=timer;
						LEDSoff();
						ledOnOff (LEDS[i], LED_OFF) ;
						i++;
						if(i>=5){
								i=0;
						}
						ledOnOff (LEDS[i], LED_ON) ;
			}

			if (pressedB1_ev) {            // B1 has been pressed
            pressedB1_ev = false ;     // acknowledge the event
            stateLED1 = MoveLit5to1 ;       //  ... the new state
      }
			if(pressedB5_ev){  // B5 has been pressed
				stateLED1=MoveUnlit1to5;
				pressedB5_ev = false ;   
			}
      break ;

  case MoveLit5to1:  //case for making LED lit from LED5 to LED1 when B1 is pressed
          
			if(TIMER==0 ){
						TIMER=timer;
						LEDSoff();
						ledOnOff (LEDS[i], LED_OFF) ;
						i--;
						if(i<0){
								i=4;
						}
						ledOnOff (LEDS[i], LED_ON) ;
			}

			if (pressedB1_ev) {            // B1 has been pressed
            pressedB1_ev = false ;     // acknowledge the event
            stateLED1 = MoveLit1to5 ;      // turn LED off
      }
			if(pressedB5_ev){	// B5 has been pressed
				stateLED1=MoveUnlit5to1;				
				pressedB5_ev = false ;  
				
			}
      break ;
			
			

	 case MoveUnlit5to1: //case for making LED unlit from LED5 to LED1 when B5 is pressed
		 
			
			
			if(TIMER==0 ){
						TIMER=timer;
						LEDSon();
						ledOnOff (LEDS[i], LED_ON) ;
						i--;
						if(i<0){
								i=4;
						}
						ledOnOff (LEDS[i], LED_OFF) ;
			}
			
			if (pressedB1_ev) {            // B1 has been pressed
            pressedB1_ev = false ;     // acknowledge the event
            stateLED1 = MoveUnlit1to5 ;       //  ... the new state
      }
			if (pressedB5_ev) {            // B5 has been pressed
            pressedB5_ev = false ;     // acknowledge the event
            stateLED1 = MoveLit5to1 ;       //  ... the new state
      }
      break ;

   case MoveUnlit1to5:  //case for making LED unlit from LED5 to LED1 when B5 is pressed
		 
	 			if(TIMER==0 ){
						TIMER=timer;
						LEDSon();
						ledOnOff (LEDS[i], LED_ON) ;
						i++;
						if(i>=5){
								i=0;
						}
						ledOnOff (LEDS[i], LED_OFF) ;
			}
            
				if (pressedB5_ev) {            // B5 has been pressed
                pressedB5_ev = false ;     // acknowledge the event
                stateLED1 = MoveLit1to5 ;     //  ... the new state
            }
				
				if (pressedB1_ev) {            // B1 has been pressed
            pressedB1_ev = false ;     // acknowledge the event
            stateLED1 = MoveUnlit5to1 ;       //  ... the new state
      }
            break ;
    }
    
	
}

/*----------------------------------------------------------------------------
 * Interrupt Handler GPIO A
 *    - Clear the pending request
 *    - Test the bit to see if it generated the interrupt 
  ---------------------------------------------------------------------------- */
void PORTA_IRQHandler(void) {  
    NVIC_ClearPendingIRQ(PORTA_IRQn);
    if ((PORTA->ISFR & B5MASK)) {
        // Add code to respond to interupt here
        pressedB5_ev = 1 ;
    }
    // Clear status flags 
    PORTA->ISFR = B5MASK ; 
}

/*----------------------------------------------------------------------------
  task3LED5
  
  This task toggles the state of LED5 when the 'B5 pressed' event occurs
*----------------------------------------------------------------------------*/
int stateLED5 ;  // this variable holds the current state

void initTask3() {
    stateLED5 = LED_OFF;        // initialise the state 
    ledOnOff (LED5, LED_OFF) ;  // turn the LED off
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
    configureLEDs() ;                         // configure LED1-5 using GPIO
    configureButtons(B1MASK | B5MASK, true) ; // configures buttons B1 and B5 (with an interrupt)
    initTask1() ;  // initialise task1 state
    initTask2() ;  // initialise task2 state
    initTask3() ;  // initialise task3 state
    Init_SysTick(1000) ; // initialse SysTick every 1ms
    waitSysTickCounter(10) ;
    while (1) {      // this runs for ever
        task1pollB1() ;
        LabWork() ;		//all the code that makes B1 and B5 work

			waitSysTickCounter(10) ;  // cycle every 10 ms - not explained until week 4
    }
}

/*
 * Created: 29.08.2018 
 * Author : Grzegorz Brotoń & Michał Żurowski
 */
#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

#define row1 (1<<PC5)             
#define row2 (1<<PC4)
#define row3 (1<<PB5)
#define row4 (1<<PB4)
 
#define row1Down PORTC &= ~ row1     
#define row2Down PORTC &= ~ row2
#define row3Down PORTB &= ~ row3
#define row4Down PORTB &= ~ row4
 
#define row1Up PORTC |= row1        
#define row2Up PORTC |= row2
#define row3Up PORTB |= row3
#define row4Up PORTB |= row4

#define column1 (1<<PB3)         
#define column2 (1<<PB2)
#define column3 (1<<PB1)
#define column4 (1<<PB0)
 
#define Readcolumn1 !(PINB & column1)   
#define Readcolumn2 !(PINB & column2)   
#define Readcolumn3 !(PINB & column3)  
#define Readcolumn4 !(PINB & column4)

#define high PORTD                   
#define highDDR DDRD                 

#define an1 (1<<PC0)             
#define an2 (1<<PC1)
#define an3 (1<<PC2)
#define an4 (1<<PC3)
 
#define an1_on PORTC &= ~ an1      
#define an2_on PORTC &= ~ an2   
#define an3_on PORTC &= ~ an3
#define an4_on PORTC &= ~ an4
 
#define an1_off PORTC |= an1        
#define an2_off PORTC |= an2
#define an3_off PORTC |= an3
#define an4_off PORTC |= an4

void setSegments(uint8_t number);
void setButton(uint8_t button);
void checkButtons();
void setAnodes();
void selectedButton(uint8_t button);
void reset_counter();
void reset();
void check();
void print_marks(uint8_t seg_nr);

uint8_t toDisplay[] = {0x00, 0x01, 0x1F, 0x77, 0x3D, 0x5E ,0x1D}; 
// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, null, -, b, A, d, g, o

uint8_t timerButtons[16]; 
uint8_t segments[4] = {0,0,0,0}; 
uint8_t lock_pin[4] = {1,2,3,6}; // it is 1 2 3 5
uint8_t entered_pin[4] = {13,13,13,13};

uint8_t stage = 0; 
bool test = false; 
bool counter_start = false;
int counter = 0;
uint8_t processButton = 0;
uint8_t anodes = 0; 

 int main(void){

    UCSR0B = 0;                                         

    TCCR0A |= (1 << WGM01);   
    OCR0A =  0xF9;
    TIMSK0 |= (1<<OCIE0A);
    TCCR0B|= (1<< CS02);                             
    sei();                                                 
    
    DDRC |= an1 | an2 | an3 | an4;                      
    highDDR |= 0xFF;                                     
  
    DDRC |= row1 | row2;    DDRB |= row3| row4;         
    PORTC |= row1 | row2;   PORTB|= row3| row4;        
     
    DDRB &= ~(column1 | column2 | column3 | column4);   
    PORTB |= (column1 | column2 | column3 | column4);   

    while (1) {
        
    }
}

ISR (TIMER0_COMPA_vect) 
{
    checkButtons();
    setAnodes();
    reset_counter();
}

void reset_counter(){
    if( counter_start == true){
      counter++;
    }
    if( counter == 300){
      reset();
      counter = 0;
      counter_start = false;
    }
}

void reset(){
  
    for (int i = 0; i < 4; i++){
      segments[i] = 10;
      entered_pin[i] = 13;
    }
    stage = 0;
    test = false;  
}

void check(){

      if((entered_pin[0] == lock_pin[0]) && (entered_pin[1] == lock_pin[1]) && (entered_pin[2] == lock_pin[2]) && (entered_pin[3] == lock_pin[3]))
        test = true;
        stage++;
}

void setAnodes(){

    if (anodes==0){
        an1_on;     an2_off;    an3_off;    an4_off;
        setSegments(segments[0]);
    }
    else if(anodes == 1){
        an1_off;    an2_on;     an3_off;    an4_off;
        setSegments(segments[1]);
    }
    else if(anodes == 2){
        an1_off;    an2_off;    an3_on;     an4_off;
        setSegments(segments[2]);
    }
    else if(anodes == 3){
        an1_off;    an2_off;    an3_off;    an4_on;
        setSegments(segments[3]);
    }
    else{
        anodes = 0;
        return;
    }
    anodes ++;
}
 
void setSegments(uint8_t number){

    high = ~toDisplay[number];
}

void checkButtons(){   

    if (processButton == 0){
        row1Down;   row2Up;     row3Up;     row4Up;
        processButton = 1;
    }
    else if (processButton == 1){
        if (Readcolumn1) setButton(1);
        if (Readcolumn2) setButton(2);
        if (Readcolumn3) setButton(3);
        if (Readcolumn4) setButton(4);
        processButton = 2;
    }
    else if (processButton == 2){
        row1Up;     row2Down;       row3Up;     row4Up;
        processButton = 3;
    }
    else if (processButton == 3){
        if (Readcolumn1) setButton(5);
        if (Readcolumn2) setButton(6);
        if (Readcolumn3) setButton(7);
        if (Readcolumn4) setButton(8);
        processButton = 4;
    }
    else if (processButton == 4){
        row1Up;     row2Up;     row3Down;       row4Up;
        processButton = 5;
    }
    else if (processButton == 5){
        if (Readcolumn1) setButton(9);
        if (Readcolumn2) setButton(10);
        if (Readcolumn3) setButton(11);
        if (Readcolumn4) setButton(12);
        processButton = 6;
    }
    else if (processButton == 6){
        row1Up;     row2Up;     row3Up;     row4Down;
        processButton = 7;
    }
    else if (processButton == 7){
        if (Readcolumn1) setButton(13);
        if (Readcolumn2) setButton(14);
        if (Readcolumn3) setButton(15);
        if (Readcolumn4) setButton(16);
        processButton = 0;
    }
    setButton(0);
}

void setButton(uint8_t button){

    if (button!= 0) timerButtons[button-1] = 20;
    for(uint8_t i = 0; i<16;i++){
        if (timerButtons[i] > 0) timerButtons[i] --;
        if (timerButtons[i] == 1){
            selectedButton(i+1);
        }   
    }
}
 
void print_marks(uint8_t seg_nr){

    for(int i = 0; i < 4; i++){
    segments[i] = 0;
    }
    for(int i = 1; i <= seg_nr; i++){
    segments[4-i] = 1;
    }
}
        
void selectedButton(uint8_t button){
    
    if (button== 12){
        reset();
    }
        
    else if ((button== 1 || button== 2 || button== 3 || button== 5 || button== 6 || button== 7 || button== 9 || button== 10 || button== 11 || button== 14) && stage < 4){

        if (stage == 0) {
          entered_pin[stage] = button;
          stage++;
          print_marks(stage);
        }
        else if (stage == 1) {
          entered_pin[stage] = button;
          stage++;
          print_marks(stage);
        }
        else if (stage == 2) {
          entered_pin[stage] = button;
          stage++;
          print_marks(stage);
        }
        else if (stage == 3) {
          entered_pin[stage] = button;
          stage++;
          print_marks(stage);
        }
    }
    
    else if (button== 13 || button== 15 || button== 16) { 
        // do nothing
    }
    
    else if (button== 8 && stage > 0 && stage < 5){
        stage--;
        print_marks(stage);
    }
    
    else if (button == 4 && stage == 4){
    
        check();
        if(test == true){
          segments[0] = 5;
          segments[1] = 6;
          segments[2] = 6;
          segments[3] = 4;
          counter_start = true;
        }
        if(test == false){
          segments[0] = 0;
          segments[1] = 2;
          segments[2] = 3;
          segments[3] = 4;
          counter_start = true;
        }   
    }
}

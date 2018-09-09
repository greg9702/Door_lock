#Safe lock project


#define row1 (1<<PC5)                - ustawia Port C bit 5 na high
#define row1Down PORTC &= ~ row1     - podciaganie pinow do masy
#define row1Up PORTC |= row1         - podciaganie pinow do zasilania
#define Readcolumn1 !(PINB & column1)- sprawdzanie stanu pinow
                                        zwraca 1 gdy wcisniety - stad 
                                        negacja
#define an1_on PORTC &= ~ an1		  - wlaczanie danego segment
#define an1_off PORTC |= an1          - wylaczanie danego segmentu

## Zmienne

uint8_t toDisplay[] - Przechowuje znaki potrzebne do wyświetlania

uint8_t timerButtons[16] - uzywana przy eliminacji wielokrotnych
                     przecisnięć

uint8_t segments[4] - okresla co ma sie wyswietlac na danym segmencie

uint8_t lock_pin[4] - przechowuje wzorzec kodu

uint8_t entered_pin[4] - przechowuje kod wpisany przez uzytkownika

uint8_t stage - liczba od 0 - 5:
                    0 - nic nie wpisane,
                    1 - jedna liczba wpisana
                    2 - diwe liczby wpisane
                    3 - trzy liczby wpisane
                    4 - cztery liczby wpisane 
                    5 - wcisnieto OK i czekamy na autoreset programu
 
bool test - okresla czy lock_pin[] zgadza sie z entered_pin[]

counter_start = false; - zmieniamy na true, gdy wcisnieto Ok i program 
                    musi uruchomic licznik do autoresetu

int counter - liczy czas po ktorym nastepuje autoreset

uint8_t processButton - sluzy do sprawdzenia w ktorym momencie sprawdzania 
                    klawiatury jestesmy

uint8_t anodes - okresla ktora obecnie anoda jest wlaczona, uzywana w 
                    funkcji setAnodes() - gdzie caly czas jej wartosc sie
                    zmienia w przedziale [0,3]


## Funkcje

void reset_counter() - funkcja do autoresetu programu po wcisnieciu OK

void reset() - resetuje tablice entered_pin, wysweitlacz i stage - cofa 
                    program do poczatku

void check() - porownuje po wcisnieciu OK enetered_pin[] z lock_pin[] i
                    ustawaia zmienna test

void setAnodes() - wywolywane przerwanie, ustawia zmienna anodes - stereuje
                     ktory segment ma byc wyswietlany

void setSegments(uint8_t number) - ustawia PORTD - czyli zmienna high - 
                     okresla co ma byc wyswietlane na tym segmencie

void checkButtons() - sprawdzanie ktory rzad i kolumna wcisniete

void setButton(uint8_t button) - zapobiega wielokrotnemu wcisniciu przycisku

void print_marks(uint8_t seg_nr) - wyswietla tyle "-" ile wpisano liczb

void selectedButton(uint8_t button) - okresla co znaczy konkretny przycisk w
                        danej fazie programu, zapobiega rowniez wciskaniu 
                        przyciskow kiedy sie nie da, np klikanie Ok przy 
                        trzech liczbach


## Klawiatura 

Widziana z poziomu programu: 
- uzywana do ustawiania tablicy lock_pin[]

 |                              |
 |  | 1 |  | 2 |  | 3 |  | 4 |  |
 |                              |
 |  | 5 |  | 6 |  | 7 |  | 8 |  |
 |                              |
 |  | 9 |  | 10|  | 11|  | 12|  |
 |                              |
 |  | 13|  | 14|  | 15|  | 16|  |
 |______________________________|

Widziana przez uzytkowanika:

 |                              |
 |  | 1 |  | 2 |  | 3 |  | OK|  |
 |                              |
 |  | 4 |  | 5 |  | 6 |  | C |  |
 |                              |
 |  | 7 |  | 8 |  | 9 |  |RES|  |
 |                              |
 |  | x |  | 0 |  | x |  | x |  |
 |______________________________|

x - oznacza przycisk nieuzywany 

Oznacza to, że jezeli uzytkownik chce ustawic jako wzorcowy pin np liczbe
1567 musi ustawic tablice lock_pin[] jako 1679 - poniewaz nie ma sensu 
tego tlumaczyc zaraz po wpisaniu i przy sprawdzaniu uzywac drugiego wzorca
skoro mozna uzywac pierwszego bez zmieniania niczego

## int main(void) i ISR (TIMER0_COMPA_vect) 

UCSR0B = 0 - blokuje UART aby uzywac PD0 i PD1
TCCR0A |= (1 << WGM01) - inicjalizuje przerywania

OCR0A =  0xF9;
TIMSK0 |= (1<<OCIE0A);
TCCR0B|= (1<< CS02) - inicjalizuje licznik, prescaler 256

sei() - wlacza przerwania globalne

DDRC |= an1 | an2 | an3 | an4; - ustawia anody jako wyjscia
wysDDR |= 0xFF; - ustawia PORT jako wyjscie 

DDRC |= row1 | row2;	
DDRB |= row3| row4; - rzedy klawiatury jako wyjscia

PORTC |= row1 | row2;	
PORTB|= row3| row4; - rzedy ustawia na stan wysoki

DDRB &= ~(column1 | column2 | column3 | column4); - kolumny jako wejscia
PORTB |= (column1 | column2 | column3 | column4); - posciagniecie do 
                                zasilania, przycisnicie przycisku wymusza
                                stan niski

ISR (TIMER0_COMPA_vect) - funkcja wywolywana przez przetwanie w                               konkretnych odstepach czasu

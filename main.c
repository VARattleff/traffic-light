#define MY_PORT_B *(unsigned char*)0x23
#define MY_PORT_B_DDRB *(unsigned char*)0x24

#define NV_RED     (1 << 0)    // Pin 8
#define NV_YELLOW  (1 << 1)    // Pin 9
#define NV_GREEN   (1 << 2)    // Pin 10

#define EW_RED     (1 << 3)    // Pin 11
#define EW_YELLOW  (1 << 4)    // Pin 12
#define EW_GREEN   (1 << 5)    // Pin 13

#define TURN_ON(LED)    (MY_PORT_B |= LED)   
#define TURN_OFF(LED)   (MY_PORT_B &= ~LED) 

#define custom_delay(delayMs) \
    for (int i = 0; i < delayMs; i++) \ 
    { \
        for (volatile int j = 0; j < 1000; j++) \
        { \
        } \
    }

void setup() 
{
  MY_PORT_B_DDRB |= (NV_RED | NV_YELLOW | NV_GREEN | EW_RED | EW_YELLOW | EW_GREEN);
}

void loop() 
{   
    // NS && EW
    // Rødt 
    TURN_ON(NV_RED);
    TURN_ON(EW_RED);
    TURN_OFF(NV_YELLOW);
    TURN_OFF(NV_GREEN);
    TURN_OFF(EW_YELLOW);
    TURN_OFF(EW_GREEN);
    custom_delay(3000);  

    // NS 
    // Rødt + Gult
    TURN_ON(NV_YELLOW);
    custom_delay(1000);  

    // NS 
    // Grønt
    TURN_OFF(NV_RED);
    TURN_OFF(NV_YELLOW);
    TURN_ON(NV_GREEN);
    custom_delay(5000);  

    // NS 
    // Gult
    TURN_OFF(NV_GREEN);
    TURN_ON(NV_YELLOW);
    custom_delay(1000);  

    // NS 
    // rødt
    TURN_OFF(NV_YELLOW);
    TURN_ON(NV_RED);
    custom_delay(2000);  

    // EW 
    // Rødt + Gult
    TURN_ON(EW_YELLOW);
    custom_delay(1000);  

    // EW
    // Grønt
    TURN_OFF(EW_RED);
    TURN_OFF(EW_YELLOW);
    TURN_ON(EW_GREEN);
    custom_delay(5000);  

    // NS
    // Gult
    TURN_OFF(EW_GREEN);
    TURN_ON(EW_YELLOW);
    custom_delay(1000);  

    // Tilbage til rødt
    TURN_OFF(EW_YELLOW);
    TURN_ON(EW_RED);
    custom_delay(2000);  
}

int main ()
{
  setup();
  while(1) 
  {
    loop();
  }
}
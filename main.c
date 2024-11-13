// Port definitions
#define MY_PORT_B *(unsigned char*)0x23
#define MY_PORT_B_DDRB *(unsigned char*)0x24
#define MY_PORT_D *(unsigned char*)0x2B
#define MY_PORT_D_DDRD *(unsigned char*)0x2A
#define MY_PIN_D *(unsigned char*)0x29

// Traffic light pins (PORTB)
#define NS_RED     (1 << 0)    // Pin 8
#define NS_YELLOW  (1 << 1)    // Pin 9
#define NS_GREEN   (1 << 2)    // Pin 10
#define EW_RED     (1 << 3)    // Pin 11
#define EW_YELLOW  (1 << 4)    // Pin 12
#define EW_GREEN   (1 << 5)    // Pin 13

// Pedestrian pins (PORTD)
#define PEDESTRIAN_NS_RED     (1 << 3)    // Pin 3
#define PEDESTRIAN_NS_GREEN   (1 << 4)    // Pin 4
#define PEDESTRIAN_NS_CONFIRM (1 << 5)    // Pin 5
#define PEDESTRIAN_EW_RED     (1 << 6)    // Pin 6
#define PEDESTRIAN_EW_GREEN   (1 << 7)    // Pin 7
#define PEDESTRIAN_EW_CONFIRM (1 << 2)    // Pin 2

// Button inputs (PORTD)
#define PEDESTRIAN_NS_BTN     (1 << 0)    // Pin 0
#define PEDESTRIAN_EW_BTN     (1 << 1)    // Pin 1

// Control macros
#define TURN_ON(LED, PORT)    (PORT |= LED)   
#define TURN_OFF(LED, PORT)   (PORT &= ~LED)
#define READ_PIN(PIN)         (MY_PIN_D & PIN)

// Global state
volatile unsigned char ns_btn_pressed = 0;
volatile unsigned char ew_btn_pressed = 0;

#define custom_delay(delayMs) \
    for (int i = 0; i < delayMs; i++) { \
        check_pedestrian_buttons(); \
        for (volatile int j = 0; j < 1000; j++) { \
        } \
    }

void check_pedestrian_buttons() 
  {
    if (READ_PIN(PEDESTRIAN_NS_BTN) && !ns_btn_pressed) 
      {
        if (MY_PORT_B & NS_RED) 
        {  
            ns_btn_pressed = 1;
            TURN_ON(PEDESTRIAN_NS_CONFIRM, MY_PORT_D);
        }
    }
    
    if (READ_PIN(PEDESTRIAN_EW_BTN) && !ew_btn_pressed) 
      {
        if (MY_PORT_B & EW_RED) 
        { 
            ew_btn_pressed = 1;
            TURN_ON(PEDESTRIAN_EW_CONFIRM, MY_PORT_D);
        }
    }
}

void setup() 
{
    // Configure car traffic lights as output
    MY_PORT_B_DDRB |= (NS_RED | NS_YELLOW | NS_GREEN | EW_RED | EW_YELLOW | EW_GREEN);
    
    // Configure pedestrian lights and confirm LEDs as output
    MY_PORT_D_DDRD |= (PEDESTRIAN_NS_RED | PEDESTRIAN_NS_GREEN | PEDESTRIAN_EW_RED | PEDESTRIAN_EW_GREEN |PEDESTRIAN_NS_CONFIRM | PEDESTRIAN_EW_CONFIRM);
    
    // Configure buttons as input
    MY_PORT_D_DDRD &= ~(PEDESTRIAN_NS_BTN | PEDESTRIAN_EW_BTN);
    
    // Enable pull-up resistors for buttons
    MY_PORT_D |= (PEDESTRIAN_NS_BTN | PEDESTRIAN_EW_BTN);
    
    // Initial state: All red
    TURN_ON(NS_RED, MY_PORT_B);
    TURN_ON(EW_RED, MY_PORT_B);
    TURN_ON(PEDESTRIAN_NS_RED, MY_PORT_D);
    TURN_ON(PEDESTRIAN_EW_RED, MY_PORT_D);
}

void handle_ns_cycle() 
{
    int delay_time = ns_btn_pressed ? 1000 : 3000;  
    
    // NS R + Y
    TURN_ON(NS_YELLOW, MY_PORT_B);
    custom_delay(1000);
    
    // NS G
    TURN_OFF(NS_RED, MY_PORT_B);
    TURN_OFF(NS_YELLOW, MY_PORT_B);
    TURN_ON(NS_GREEN, MY_PORT_B);
    
    // Pedestrian G 
    custom_delay(500);
    if (ns_btn_pressed) 
    {
        TURN_OFF(PEDESTRIAN_NS_RED, MY_PORT_D);
        TURN_ON(PEDESTRIAN_NS_GREEN, MY_PORT_D);
        TURN_OFF(PEDESTRIAN_NS_CONFIRM, MY_PORT_D);
        ns_btn_pressed = 0;
    }
    
    custom_delay(4500);
    
    // Turn pedestrian light red before yellow car light
    TURN_OFF(PEDESTRIAN_NS_GREEN, MY_PORT_D);
    TURN_ON(PEDESTRIAN_NS_RED, MY_PORT_D);
    
    // NS G
    TURN_OFF(NS_GREEN, MY_PORT_B);
    TURN_ON(NS_YELLOW, MY_PORT_B);
    custom_delay(1000);
    
    // NS R
    TURN_OFF(NS_YELLOW, MY_PORT_B);
    TURN_ON(NS_RED, MY_PORT_B);
    custom_delay(delay_time);
}

void handle_ew_cycle() 
{
    int delay_time = ew_btn_pressed ? 1000 : 3000;  
    
    // EW R + Y
    TURN_ON(EW_YELLOW, MY_PORT_B);
    custom_delay(1000);
    
    // EW G
    TURN_OFF(EW_RED, MY_PORT_B);
    TURN_OFF(EW_YELLOW, MY_PORT_B);
    TURN_ON(EW_GREEN, MY_PORT_B);
    
    custom_delay(500);
    if (ew_btn_pressed) 
    {
        TURN_OFF(PEDESTRIAN_EW_RED, MY_PORT_D);
        TURN_ON(PEDESTRIAN_EW_GREEN, MY_PORT_D);
        TURN_OFF(PEDESTRIAN_EW_CONFIRM, MY_PORT_D);
        ew_btn_pressed = 0;
    }
    
    custom_delay(4500);
    
    TURN_OFF(PEDESTRIAN_EW_GREEN, MY_PORT_D);
    TURN_ON(PEDESTRIAN_EW_RED, MY_PORT_D);
    
    // EW G
    TURN_OFF(EW_GREEN, MY_PORT_B);
    TURN_ON(EW_YELLOW, MY_PORT_B);
    custom_delay(1000);
    
    // ALL R
    TURN_OFF(EW_YELLOW, MY_PORT_B);
    TURN_ON(EW_RED, MY_PORT_B);
    custom_delay(delay_time);
}

void loop() 
{
    handle_ns_cycle();
    handle_ew_cycle();
}

int main() 
{
    setup();
    while(1) 
    {
        loop();
    }
    return 0;
}
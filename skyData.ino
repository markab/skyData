/*
    skyData - markab
    FrSky telemetry data module for Naza and TBS frame sensors
    
    Created based on ideas from the following projects...
    
    https://code.google.com/p/nazatofrsky/
    https://code.google.com/p/bagaosd/
    https://code.google.com/p/openxvario/
*/


// ************************************************************************************
// FIRMWARE VERSION
// ************************************************************************************

#define         FIRMWARE_VERSION        "002"


// ************************************************************************************
// ENABLE MODULES
// ************************************************************************************

#define         ENABLE_SENSOR           1   // process sensor data, 1 = On, 0 = Off
#define         ENABLE_NAZA             1   // process naza data, 1 = On, 0 = Off
#define         ENABLE_FRSKY            1   // send telem values to frsky, 1 = On, 0 = Off
#define         ENABLE_DEBUG            0   // print debug to terminal, 1 = On, 0 = Off


// ************************************************************************************
// PROGRAM FLOW
// ************************************************************************************

#define         RATE_SENSOR             0       // analoge sensor read rate, 0 as fast as possible
#define         RATE_DEBUG              1000    // debug print output delay


// ************************************************************************************
// HARDWARE CONFIGURATION
// ************************************************************************************

int             pin_led                  = 13;  // teensy onboard led
int             pin_i                    = 0;   // current sensor pin teensy A0
// !!USE A VOLTAGE DIVIDER DO NOT CONNECT A1 DIRECTLY TO BATTERY!!
int             pin_v                    = 1;   // voltage sensor pin teensy A1


// ************************************************************************************
// SERIAL INTERFACES
// ************************************************************************************

#define         FrSkySerial             Serial1
#define         NazaSerial              Serial3
#define         ConsoleSerial           Serial


// ************************************************************************************
// TIMERS
// ************************************************************************************

unsigned long   timer_sensor;
unsigned long   timer_debug;


// ************************************************************************************
// FRSKY DATA VARIABLES
// ************************************************************************************

int32_t         data_gps_fix_type       = 0;    // gps fix type, 0 = no gps, 1 = no fix, 2 = 2d fix, 3 = 3d fix
int32_t         data_gps_sat_count      = 0;    // gps satellite count
int32_t         data_gps_vario          = 0;    // vertical speed, 100 = 1m/s
uint32_t        data_curr               = 0;    // amps from teensy A0, configured for tbs 50a sensor
uint32_t        data_vfas               = 0;    // volts from teensy A1, voltage divider connected to tbs frame voltage
uint32_t        data_gps_long           = 0;    // gps position
uint32_t        data_gps_lati           = 0;    // gps position 
int32_t         data_gps_alt            = 0;    // gps altitude, 1000 = 1m
uint32_t        data_gps_speed          = 0;    // gps speed, 100 = 1m/s
uint32_t        data_gps_cours          = 0;    // gps heading degrees, 10000 = 100 deg modified


// ************************************************************************************
// SETUP
// ************************************************************************************

void setup() {

    if (ENABLE_DEBUG) {
        ConsoleSerial.begin(9600);
        ConsoleSerial.print("markab - skyData Version : ");
        ConsoleSerial.println(FIRMWARE_VERSION);
    }

    if (ENABLE_NAZA) {
        NazaSerial.begin(115200);
    }

    if (ENABLE_FRSKY) {
        FrSkySerial_Init(57600);
    }

    pinMode(pin_led, OUTPUT);
}

// ************************************************************************************
// LOOP
// ************************************************************************************

void loop() {

    // read sensors
    Analogue_ReadSensors();

    // process naza data
    Naza_ProcessData();

    // send to frsky
    FrSkySPort_ProcessData();

    // debug
    Print_Debug();
}

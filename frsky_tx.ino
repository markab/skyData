/*
    skyData - markab
    FrSky telemetry data module for Naza and TBS frame sensors
    
    Created based on ideas from the following projects...
    
    https://code.google.com/p/nazatofrsky/
    https://code.google.com/p/bagaosd/
    https://code.google.com/p/openxvario/
*/

// ************************************************************************************
// FRSKY PORT CONFIG
// ************************************************************************************

#define         _FrSkySPort_C1          UART0_C1
#define         _FrSkySPort_C3          UART0_C3
#define         _FrSkySPort_S2          UART0_S2


// ************************************************************************************
// FRSKY SENSOR IDS
// ************************************************************************************

#define         SENSOR_ID1              0x1B
#define         SENSOR_ID2              0x0D
#define         SENSOR_ID3              0x34
#define         SENSOR_ID4              0x67


// ************************************************************************************
// FRSKY INTERNAL IDS
// ************************************************************************************

#define         START_STOP              0x7e
#define         DATA_FRAME              0x10


// ************************************************************************************
// FRSKY DATA IDS
// ************************************************************************************

#define         RSSI_ID                 0xf101
#define         ADC1_ID                 0xf102
#define         ADC2_ID                 0xf103
#define         BATT_ID                 0xf104
#define         SWR_ID                  0xf105
#define         T1_ID                   0x0400
#define         T2_ID                   0x0410
#define         RPM_ID                  0x0500
#define         FUEL_ID                 0x0600
#define         ALT_ID                  0x0100
#define         VARIO_ID                0x0110
#define         ACCX_ID                 0x0700
#define         ACCY_ID                 0x0710
#define         ACCZ_ID                 0x0720
#define         CURR_ID                 0x0200
#define         VFAS_ID                 0x0210
#define         CELLS_ID                0x0300
#define         GPS_LONG_LATI_ID        0x0800
#define         GPS_ALT_ID              0x0820
#define         GPS_SPEED_ID            0x0830
#define         GPS_COURS_ID            0x0840
#define         GPS_TIME_DATE_ID        0x0850

// ************************************************************************************
// PROGRAM DEFINITIONS
// ************************************************************************************

#define         MAX_ID_COUNT            9


// ************************************************************************************
// PROGRAM VARIABLES
// ************************************************************************************

short           crc;
uint8_t         last_rx;
uint32_t        id_count                = 0;


// ************************************************************************************
// INIT
// ************************************************************************************

void FrSkySerial_Init(long baud)  {

    FrSkySerial.begin(baud);
    _FrSkySPort_C3 = 0x10;  // tx invert
    _FrSkySPort_C1 = 0xA0;  // single wire mode
    _FrSkySPort_S2 = 0x10;  // rx invert
}


// ************************************************************************************
// PROCESS DATA
// ************************************************************************************

void FrSkySPort_ProcessData(void) {

    if(ENABLE_FRSKY)  {

        uint8_t data = 0;

        while (FrSkySerial.available()) {

          data = FrSkySerial.read();

          if (last_rx == START_STOP && ((data == SENSOR_ID1) || (data == SENSOR_ID2) || (data == SENSOR_ID3)  || (data == SENSOR_ID4))) {

              switch(id_count) {

                 case 0:
                   FrSkySPort_SendPackage(T1_ID, data_gps_fix_type);
                   break;

                 case 1:
                   FrSkySPort_SendPackage(T2_ID, data_gps_sat_count);
                   break;

                 case 2:
                   if(data_gps_fix_type == 3) {
                       FrSkySPort_SendPackage(VARIO_ID, data_gps_vario);
                   }
                   break;

                 case 3:
                   FrSkySPort_SendPackage(CURR_ID, data_curr);
                   break;

                 case 4:
                   FrSkySPort_SendPackage(VFAS_ID, data_vfas);
                   break;

                 case 5:
                   if(data_gps_fix_type == 3) {
                       FrSkySPort_SendPackage(GPS_LONG_LATI_ID, data_gps_long);
                   }
                   break;
                   
                 case 6:
                   if(data_gps_fix_type == 3) {
                       FrSkySPort_SendPackage(GPS_LONG_LATI_ID, data_gps_lati);
                   }
                   break;

                 case 7:
                   if(data_gps_fix_type == 3) {
                       FrSkySPort_SendPackage(GPS_ALT_ID, data_gps_alt);
                   }
                   break;

                 case 8:
                   if(data_gps_fix_type == 3) {
                     FrSkySPort_SendPackage(GPS_SPEED_ID, data_gps_speed);
                   }
                   break;

                 case 9:
                   FrSkySPort_SendPackage(GPS_COURS_ID, data_gps_cours);
                   break;
               }

            id_count++;
            if(id_count > MAX_ID_COUNT) id_count = 0;

            }
            last_rx=data;
        }
    }
}


// ************************************************************************************
// SEND BYTE
// ************************************************************************************

void FrSkySPort_SendByte(uint8_t byte) {

    FrSkySerial.write(byte);

    // crc update
	crc += byte;        //0-1FF
	crc += crc >> 8;    //0-100
	crc &= 0x00ff;
	crc += crc >> 8;    //0-0FF
	crc &= 0x00ff;
}


// ************************************************************************************
// SEND CRC
// ************************************************************************************

void FrSkySPort_SendCrc() {

	FrSkySerial.write(0xFF-crc);
    crc = 0; // crc reset
}


// ************************************************************************************
// SEND PACKAGE
// ************************************************************************************

void FrSkySPort_SendPackage(uint16_t id, uint32_t value) {

    digitalWrite(pin_led, HIGH);
    UART0_C3 |= 32; // transmit direction to s.port

    FrSkySPort_SendByte(DATA_FRAME);

    uint8_t *bytes = (uint8_t*)&id;
	FrSkySPort_SendByte(bytes[0]);
	FrSkySPort_SendByte(bytes[1]);

    bytes = (uint8_t*)&value;
	FrSkySPort_SendByte(bytes[0]);
	FrSkySPort_SendByte(bytes[1]);
	FrSkySPort_SendByte(bytes[2]);
	FrSkySPort_SendByte(bytes[3]);

    FrSkySPort_SendCrc();
	FrSkySerial.flush();

    UART0_C3 ^= 32; // transmit direction from s.port
    digitalWrite(pin_led, LOW);
}

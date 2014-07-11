/*
    skyData - markab
    FrSky telemetry data module for Naza and TBS frame sensors
    
    Created based on ideas from the following projects...
    
    https://code.google.com/p/nazatofrsky/
    https://code.google.com/p/bagaosd/
    https://code.google.com/p/openxvario/
*/


// ************************************************************************************
// INCLUDE
// ************************************************************************************

#include "NazaDecoderLib.h"


// ************************************************************************************
// Process Data
// ************************************************************************************

void Naza_ProcessData(void) {

    if((NazaSerial.available()) && ENABLE_NAZA) {

        uint8_t decoded_message = NazaDecoder.decode(NazaSerial.read());
        switch (decoded_message) {

          case NAZA_MESSAGE_GPS:

            // process fix data
            data_gps_fix_type = NazaDecoder.getFixType();
            data_gps_sat_count = NazaDecoder.getNumSat();

            if(data_gps_fix_type == 3) {

               data_gps_alt = (NazaDecoder.getAlt() * 100);
               data_gps_vario = (NazaDecoder.getClimbSpeed() * 100);

               // process longitude
               double longitude = NazaDecoder.getLon();
               int32_t longitude2 = longitude * 10000000;

               if(longitude2 < 0) {
                   data_gps_long = ((abs(longitude2) / 100) * 6)  | 0xC0000000;
               }
               else {
                   data_gps_long = ((abs(longitude2) / 100) * 6)  | 0x80000000;
               }

               // process latitude
               double latitude = NazaDecoder.getLat();
               int32_t latitude2 = latitude * 10000000;

               if(latitude2 < 0 ) {
                   data_gps_lati = ((abs(latitude2) /100 ) * 6) | 0x40000000;
               }
               else {
                   data_gps_lati = ((abs(latitude2) / 100) * 6);
               }

               data_gps_speed = (NazaDecoder.getSpeed() * 100);
            }
            break;

          case NAZA_MESSAGE_COMPASS:
            data_gps_cours = (NazaDecoder.getHeading() * 100);
            break;
        }
    }
}

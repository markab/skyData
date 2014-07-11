/*
    skyData - markab
    FrSky telemetry data module for Naza and TBS frame sensors
    
    Created based on ideas from the following projects...
    
    https://code.google.com/p/nazatofrsky/
    https://code.google.com/p/bagaosd/
    https://code.google.com/p/openxvario/
*/


// ************************************************************************************
// PRINT DEBUG
// ************************************************************************************

void Print_Debug(void) {

    if(((millis() - timer_debug) > RATE_DEBUG) && ENABLE_DEBUG)  {

        timer_debug = millis();

        ConsoleSerial.print("sat fix   : "); ConsoleSerial.println(data_gps_fix_type);
        ConsoleSerial.print("sat count : "); ConsoleSerial.println(data_gps_sat_count);
        ConsoleSerial.print("climb     : "); ConsoleSerial.println(data_gps_vario);
        ConsoleSerial.print("current   : "); ConsoleSerial.println(data_curr);
        ConsoleSerial.print("voltage   : "); ConsoleSerial.println(data_vfas);
        ConsoleSerial.print("lat       : "); ConsoleSerial.println(data_gps_lati);
        ConsoleSerial.print("long      : "); ConsoleSerial.println(data_gps_long);
        ConsoleSerial.print("gps alt   : "); ConsoleSerial.println(data_gps_alt);
        ConsoleSerial.print("gps speed : "); ConsoleSerial.println(data_gps_speed);
        ConsoleSerial.print("gps cours : "); ConsoleSerial.println(data_gps_cours);
    }
}

/*
    skyData - markab
    FrSky telemetry data module for Naza and TBS frame sensors
    
    Created based on ideas from the following projects...
    
    https://code.google.com/p/nazatofrsky/
    https://code.google.com/p/bagaosd/
    https://code.google.com/p/openxvario/
*/


// ************************************************************************************
// READ SENSORS
// ************************************************************************************

void Analogue_ReadSensors(void) {

    if(((millis() - timer_sensor) > RATE_SENSOR) && ENABLE_SENSOR)  {

        timer_sensor = millis();

        Analogue_ReadTBSCurrent();
        Analogue_ReadTBSVoltage();
    }
}


// ************************************************************************************
// READ TBS CURRENT
// ************************************************************************************

void Analogue_ReadTBSCurrent(void) {

    int raw_i = analogRead(pin_i);

    data_curr = raw_i / 2.81; // tbs 50A current sensor conversion
}


// ************************************************************************************
// READ TBS VOLTAGE
// ************************************************************************************

void Analogue_ReadTBSVoltage(void) {

    int raw_v = analogRead(pin_v);
    float pin_voltage = (raw_v * 0.00322);    // reading * volt per adc step
    float in_voltage = (pin_voltage * 10.2);  // measured voltage * voltage divider ratio, 33kOhm and 3.6kOhm resistors

    data_vfas = in_voltage * 100;             // *100 conversion from float
}

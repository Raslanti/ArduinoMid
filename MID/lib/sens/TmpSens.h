//
// Created by Angel Zaprianov on 9/17/2016.
//

#ifndef ARDUINOMID_TMPSENS_H
#define ARDUINOMID_TMPSENS_H


int OUT_TMP = 0;

int getTmpOut() {
    return OUT_TMP;
}

/**
 * Temperature sensor
 */
void sensTmp() {

    int temperatureC;
    if (isSensorReadLow()) {
        int reading = analogRead(TMP_PIN_OUT);

//
//        Serial.print("RealTempSens: ");
//        Serial.printnl(reading);

        // converting that reading to voltage, for 3.3v arduino use 3.3
        float voltage = reading / 2.4;


        // now print out the temperature
        temperatureC = (voltage /*- 0.5*/);  //converting from 10 mv per degree wit 500 mV offset
        //to degrees ((voltage - 500mV) times 100)

        OUT_TMP = temperatureC;

    }

}


#endif //ARDUINOMID_TMPSENS_H
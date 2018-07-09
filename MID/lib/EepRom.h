//
// Created by Angel Zaprianov on 28.6.2016 г..
//
// Since EEPROM have live time will use external-sourcecode ...
// http://www.hobbytronics.co.uk/arduino-external-eeprom

// http://playground.arduino.cc/Main/LibraryForI2CEEPROM
#ifdef EEP_ROM_ADDRESS

#include <Wire.h>

#else

#include <EEPROM.h>

#endif


#include "CarSens.h"
#include "MainFunc.h"


#ifndef ARDUINOMID_EepRom_H
#define ARDUINOMID_EepRom_H

//#define EEP_ROM_ADDRESS 0x50    //Address of 24LC256 eeprom chip
#define DEBUG_EEP_ROM
//#define EEP__INJ_SER true


// AT24C256
// We’re using a 256kbit eeprom which is actually 32kbytes of space.
//      262,144 bits / 8 bits in a byte = 32,768 bytes.
//      That’s 62 times the Arduino’s built-in storage!


// General information for bit bool in integers
// data |=(1<<4) - bit 4 is 1
// data &=~(1<<4) - bit 4 is 0

#ifndef EEP_ROM_WORK_DIV
#define EEP_ROM_WORK_DIV 100
#endif

#define EEP_ROM_INDEXES 18

/**
 *
 */
class EepRom {


    CarSens *car;

    float data[EEP_ROM_INDEXES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

protected:
    SavedData container;

public:
/**
 * EepRom Constructor
 * @param carSens
 */
    EepRom(CarSens &carSens) : car(&carSens) {

    }

    void begin() {
#ifdef EEP_ROM_ADDRESS
        Wire.begin();
#endif

    };

    //
    // Called when zeroed current data
    void saveResetData();

    void saveCurrentData();

    void load();


    float getAverageLitersPer100km() {
        return (getConsumedFuel() * 100) / (container.dist_trv + car->getDst());
    }

/**
 *
 */
    float getConsumedFuel() {

#ifdef SWITCH_FUEL_ON_STATE
        //
        // Switching between LPG / BNZ
        if (car->getFuelState() == 0) { // BNZ [default]
            if (isnan(container.fuel_def)) {
                container.fuel_def = 0;
            }

            return container.fuel_def + car->getDefFuelCns();
        }
        if (car->getFuelState() == 1) { // LPG [additional]
            if (isnan(container.fuel_adt)) {
                container.fuel_adt = 0;
            }
            return container.fuel_adt + car->getAdtFuelCns();
        }
#else
        return container.fuel_def + car->getDefFuelCns();
#endif
        return 0;
    }

/** TODO needs to be changed
 * Saves trip data in order to continue the trip
 */
    inline void saveTripData() {
        float time = millis() / MILLIS_PER_HR;
        container.dist_trp = container.dist_trp + car->getDst();
        container.time_trp = container.time_trp + time;
        saveCurrentData(); // Ordinary save of data
    }

/**
 * Clears trip data
 */
    inline void clearTripData() {
        container.time_trp = 0;
        container.dist_trp = 0;
    }

/**
 * Gets Car's total distance
 * @return
 */
    inline int getWorkDistance(void) {
        return int(container.total_km * EEP_ROM_WORK_DIV);
    }

/**
 * Gets saved travel distance
 * @return
 */
    inline float getTravelDistance(void) {
        return car->getDst() + container.dist_trv;
    }

/**
 * Gets default consumed fuel
 * @return
 */
    inline float getDefFuel(void) {
        return container.fuel_def;
    }

/**
 * Gets additional consumed fuel
 * @return
 */
    inline float getAdtFuel(void) {
        return container.fuel_adt;
    }

    inline float getSensVss(void) {
        return container.sens_vss;
    }

    inline float getSensRpm(void) {
        return container.sens_rpm;
    }

    inline float getSensDst(void) {
        return container.sens_dst;
    }

    inline float getSensEco(void) {
        return container.sens_ecu;
    }

/**
 * Gets SavedData data
 * @return
 */
    inline SavedData getData(void) {
        return container;
    }

/**
 *
 */
    inline void setData(SavedData newer) {
        container = newer;
    }

/**
 *
 * @param value
 * @return
 */
    inline void setWorkDistance(float value) {
        container.total_km = value;
    }

/**
 *
 * @param value
 * @return
 */
    inline void setTravelDistance(float value) {
        container.dist_trv = value;
    }

/**
 *
 * @param value
 * @return
 */
    inline void setDefFuel(float value) {
        container.fuel_def = value;
    }

/**
 *
 * @param value
 * @return
 */
    inline void setAdtFuel(float value) {
        container.fuel_adt = value;
    }

/**
 *
 * @param value
 * @return void
 */
    inline void setSensVss(float value) {
        container.sens_vss = value;
    }

    /*
   * @param value
   * @return void
   */
    inline void setSensRpm(float value) {
        container.sens_rpm = value;
    }

/**
 *
 * @param value
 * @return void
 */
    inline void setSensDst(float value) {
        container.sens_dst = value;
    }

/**
 *
 * @param value
 * @return void
 */
    inline void setSensEcu(float value) {
        container.sens_ecu = value;
    }

    TripData getTrip0() {
        TripData trip;
        trip.fuel = 0;  //
        trip.range = 0; // LCD will pass live data
        return trip;
    }

    TripData getTripA() {
        return container.trip_a;
    }

    TripData getTripB() {
        return container.trip_b;
    }

    TripData getTripC() {
        return container.trip_c;
    }

    void resetTripA() {
        container.trip_a.fuel = 0;
        container.trip_a.range = 0;
    }

    void resetTripB() {
        container.trip_b.fuel = 0;
        container.trip_b.range = 0;
    }

    void resetTripC() {
        container.trip_c.fuel = 0;
        container.trip_c.range = 0;
    }

    void resetWrkOdo() {
        container.total_km = 0;
    }

    float getPec() {
        return container.total_pec + car->getPec();
    }

private:

/**
 * Fixes EepRom trip nan values
 */
    void fixtureTripNan() {
        if (isnan(container.trip_a.fuel) || isnan(container.trip_a.range)) {
            resetTripA();
            Serial.println(F("Fix nan trip A"));
        }
        if (isnan(container.trip_b.fuel) || isnan(container.trip_b.range)) {
            resetTripB();
            Serial.println(F("Fix nan trip B"));
        }
        if (isnan(container.trip_c.fuel) || isnan(container.trip_c.range)) {
            resetTripC();
            Serial.println(F("Fix nan trip C"));
        }
    }

    //
    // Gets target fuel of use
    float getDefaultFuelUse() {
#if (DEFAULT_FUEL_USING == 1)
        return car->getAdtFuelCns();
#else
        return car->getDefFuelCns();
#endif
    }


};





/*************************************************
 *  public
 *              Combined methods
 *
 */
/**
 * Saves data to EepRom
 */
void EepRom::saveCurrentData() {

    data[1] = container.fuel_adt + car->getAdtFuelCns();
    data[2] = container.fuel_def + car->getDefFuelCns();
    data[3] = container.dist_trv + car->getDst();
    data[4] = container.total_km;
    // trip
    data[5] = container.dist_trp;
    data[6] = container.time_trp;
    //
    // Sens editor
    data[7] = container.sens_vss;
    data[8] = container.sens_rpm;
    data[9] = container.sens_dst;
    data[10] = container.sens_ecu;
    //
    // Trip records
    data[11] = container.trip_a.fuel + getDefaultFuelUse();
    data[12] = container.trip_a.range + car->getDst();

    data[13] = container.trip_b.fuel + getDefaultFuelUse();
    data[14] = container.trip_b.range + car->getDst();

    data[15] = container.trip_c.fuel + getDefaultFuelUse();
    data[16] = container.trip_c.range + car->getDst();
    data[17] = container.total_pec + car->getPec();

    //
    // Sow warning message if size of data is bigger then memory chip
#ifdef DEBUG
    int savedDataSize = ARRAY_SIZE(data);
    if (savedDataSize >= 1024) {
        Serial.print(F("[DANGER] EepRom data exceeds memory limit by "));
        Serial.print(savedDataSize - 1024);
        Serial.println(F("bytes"));
    } else if (cmdLive(DBG_SR_EPR)) {
        dump(" EepRom allocate FREE memory bytes: ", 1024 - savedDataSize)
        dump(" EepRom allocate USED memory bytes:  ", savedDataSize)
    }
#endif

    for (int i = 1; i < (EEP_ROM_INDEXES + 1); i++) {
        EEPROM.put(i * sizeof(data[i]), data[i]);
    }
}

/**
 * Saves final data
 */
void EepRom::saveResetData() {
    //
    // Calculate total work distance
    if (container.dist_trv > 10) {
        if (container.total_km < 1) {
            container.total_km = 0;
        }
        container.total_km = container.total_km + (container.dist_trv / EEP_ROM_WORK_DIV);
    }
    //
    // Resenting ...
    data[1] = container.fuel_adt = 0;
    data[2] = container.fuel_def = 0;
    data[3] = container.dist_trv = 0;
    data[17] = container.total_pec = 0;
    //
    // Pass work distance for save
    data[4] = container.total_km;
    // trip
    data[5] = container.dist_trp;
    data[6] = container.time_trp;

    //
    // Sens editor
    data[7] = container.sens_vss;
    data[8] = container.sens_rpm;
    data[9] = container.sens_dst;
    data[10] = container.sens_ecu;

    //
    // Trip record
    data[11] = container.trip_a.fuel + getDefaultFuelUse();
    data[12] = container.trip_a.range + car->getDst();

    data[13] = container.trip_b.fuel + getDefaultFuelUse();
    data[14] = container.trip_b.range + car->getDst();

    data[15] = container.trip_c.fuel + getDefaultFuelUse();
    data[16] = container.trip_c.range + car->getDst();

    for (int i = 1; i < (EEP_ROM_INDEXES + 1); i++) {
        EEPROM.put(i * sizeof(data[i]), data[i]);
        delay(5);
    }
    //
    // In order to fix and clear assumed data
    car->clearBuffer();

}

/**
 * Load data from chip
 */
void EepRom::load() {

    float eGetValue = 0;
    int eLocation = 0;
    volatile uint8_t i;
    for (i = 0; i < (EEP_ROM_INDEXES + 1); i++) {
#ifdef DEBUG
        if (cmdLive(DBG_SR_EPR))
            Serial.println(F("EepRom Restore Value:"));
#endif
        EEPROM.get(eLocation, eGetValue);

        if (isnan(eGetValue))
            eGetValue = 0;

#ifdef DEBUG
        if (cmdLive(DBG_SR_EPR))
            Serial.println(eGetValue, 2);
#endif
        eLocation = eLocation + sizeof(eGetValue);
        data[i] = eGetValue;
        delay(2);
    }

    container.fuel_adt = data[1];
    container.fuel_def = data[2];
    container.dist_trv = data[3];
    container.total_km = data[4];
    // Trip
    container.dist_trp = data[5];
    container.time_trp = data[6];

    //
    // Sens Edit
    container.sens_vss = data[7];
    container.sens_rpm = data[8];
    container.sens_dst = data[9];
    container.sens_ecu = data[10];

    //
    // Trips record
    container.trip_a.fuel = data[11];
    container.trip_a.range = data[12];

    container.trip_b.fuel = data[13];
    container.trip_b.range = data[14];

    container.trip_c.fuel = data[15];
    container.trip_c.range = data[16];

    container.total_pec = data[17];

    this->fixtureTripNan(); // Fix broken not NAN  values

#ifdef DEBUG
    if (cmdLive(DBG_SR_EPR)) {
        Serial.println(data[1], 2);
        Serial.println(data[2], 2);
        Serial.println(data[3], 2);
        Serial.println(data[4], 2);
        Serial.println(data[7]);
        Serial.println(data[8]);
        Serial.println(data[9]);
        Serial.println(data[10]);
        Serial.println(data[17]);

        Serial.println(F("Trip records: "));
        Serial.println(data[11]);
        Serial.println(data[12]);
        Serial.println(data[13]);
        Serial.println(data[14]);
        Serial.println(data[15]);
        Serial.println(data[16]);
    }
#endif
}


#endif
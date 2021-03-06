//
// Created by Angel Zaprianov on 29.11.2016 г..
//

#ifndef ARDUINO_MID_CAR_STATE_H
#define ARDUINO_MID_CAR_STATE_H

#include "AmpTime.h"
#include "CarSens.h"

#ifndef CAR_STT_A1_ALERT
#define CAR_STT_A1_ALERT 14 // a wait minute - default 10 / last 13
#endif

#ifndef CAR_STT_A2_ALERT
#define CAR_STT_A2_ALERT 120
#endif

#ifndef CAR_STT_A3_ALERT
#define CAR_STT_A3_ALERT 125
#endif

#ifndef CAR_STT_TM_BELT
#define CAR_STT_TM_BELT 85000
#endif

#ifndef CAR_STT_AR_FILTER
#define CAR_STT_AR_FILTER 40000
#endif


struct Diagnostic {
    uint8_t oil = 0;    // oil level
    uint8_t cnt = 0;    // Coolant level
    uint8_t win = 0;    // Window washer level
    uint8_t brk = 0;    // Brake ware
    uint8_t vol = 0;   // Car Voltage
    uint8_t blt = 0;   // Belt ware change
    uint8_t la1 = 0;   // 1 Incandescent lamps
    uint8_t la2 = 0;    // 2 incandescent lamps
    uint8_t wnt = 0;   // Winter warning
    uint8_t ovh = 0;   // Overheating warning
    uint8_t blg = 0;   // Bad break light
};

/**
 *
 */
class CarState {

private:
    Diagnostic result;

    AmpTime *amp;
    CarSens *car;

    boolean alertWinter = false;
    boolean alertOverheat = false;
    boolean alertBrkLgBad = false;
    boolean alertState = false;
    boolean initAlertState = false;
    uint8_t code = 0b1000000;
    uint8_t cursorMenu = 0;

    int lastVoltageValue = 0;
    float workDistance;


/**
 *
 * @return
 */
    boolean isBadVoltage(void) {

        int readingVoltage = analogRead(STT_VLT_PIN);
        uint8_t compareVoltage = uint8_t(readingVoltage / 10);
        //
        // Voltage too high
        // are maximum 13.8V-14.2V
        if (lastVoltageValue > 0 && lastVoltageValue == compareVoltage && readingVoltage > 980) {
            return true;
        }
        //
        // Voltage too low 11.2V
        if (lastVoltageValue > 0 && lastVoltageValue == compareVoltage && readingVoltage < BATTERY_LOW_VOLT /*680*/) {
            return true;
        }
        //
        // Save last reading
        if (readingVoltage > 0 && lastVoltageValue != compareVoltage) {
            lastVoltageValue = compareVoltage;
        }


//
// debug info
#ifdef DEBUG
        if (cmd(amp, DBG_SR_STW)) {
            dump("Voltage calculate", readingVoltage / 68)
            dump("Voltage readings", readingVoltage);
        }
#endif

        //
        // Voltage is good
        return false;
    }

/**
 *
 * @param pin
 * @param val
 */
    void sensorDigital(uint8_t pin, uint8_t &val) {
        if ((bool) digitalRead(pin) && val < 254) {
            val++;
            return;
        } else
            val = 0;
    }

/**
 * @param result
 * @param val
 */
    void sensorCustom(boolean result, uint8_t &val) {
        if (result && val < 254) {
            val++;
            return;
        }
        val = 0;
    }

/**
 *
 * @param value
 * @return
 */
    boolean isStateDisplay(uint8_t value) {

#if defined(DEBUG) && defined(DEBUG_STATE)
        if (amp->isSecond()) {
            Serial.print(F("Compare state: "));
            Serial.print(CAR_STT_A2_ALERT);
            Serial.print(F(" > |"));
            Serial.print(value);
            Serial.print(F("| < "));
            Serial.print(CAR_STT_A3_ALERT);
            Serial.println();
        }
#endif

        if (value >= CAR_STT_A2_ALERT && value < CAR_STT_A3_ALERT) {
            if (value == CAR_STT_A2_ALERT) {
//                car->passMelodyClass()->play(7);
            }
            return true;
        }
        return false;
    }

/**
 * Note this method do not change any value .. (unusable correctly)
 * @param value
 */
    void setStateShowed(uint8_t &value) {

#if defined(DEBUG) && defined(DEBUG_STATE)
        if (amp->isMid()) {
            Serial.print(F("Sets show state: "));
            Serial.print(CAR_STT_A2_ALERT);
            Serial.print(F(" < |"));
            Serial.print(value);
            Serial.print(F("| "));
            Serial.println();
        }
#endif

        if (value > CAR_STT_A2_ALERT + 5)
            value = CAR_STT_A3_ALERT;
    }


public:
    static constexpr uint8_t MENU_SERVICE = 101;

    /**
 * Construction Car State class
 * @param amp
 */
    CarState(AmpTime &_amp, CarSens &_car) : amp(&_amp), car(&_car) {}

    void setWorkState(float distance);

    boolean isAlert();

    /**
     * Display warning screen
     */
    void menu(LcdUiInterface *lcd) {


        if (isStateDisplay(result.oil)) {
            lcd->warnMotorOil();
            car->passMelodyClass()->play(7);
            setStateShowed(result.oil);
        } else if (isStateDisplay(result.cnt)) {
            lcd->warnCoolant();
            car->passMelodyClass()->play(7);
            setStateShowed(result.cnt);
        } else if (isStateDisplay(result.win)) {
            lcd->warnWasher();
            car->passMelodyClass()->play(7);
            setStateShowed(result.win);
        } else if (isStateDisplay(result.brk)) {
            lcd->warnBreakWare();
            car->passMelodyClass()->play(7);
            setStateShowed(result.brk);
        } else if (isStateDisplay(result.vol)) {
            lcd->warnBattery(this->getVoltage());
            setStateShowed(result.vol);
        } else if (isStateDisplay(result.la1)) {
            lcd->warnLightsFront();
            car->passMelodyClass()->play(7);
            setStateShowed(result.la1);
        } else if (isStateDisplay(result.la2)) {
            lcd->warnLightsBack();
            car->passMelodyClass()->play(7);
            setStateShowed(result.la2);
        } else if (isStateDisplay(result.blt)) {
            lcd->warnTmBelt();
            car->passMelodyClass()->play(7);
            setStateShowed(result.blt);
        } else if (isStateDisplay(result.wnt)) {
            lcd->warnWinter();
            setStateShowed(result.wnt);
        } else if (isStateDisplay(result.ovh)) {
            lcd->warnOverheat();
            car->passMelodyClass()->play(7);
            setStateShowed(result.ovh);
        } else if (isStateDisplay(result.blg)) {
            lcd->warnLightsBack();
            car->passMelodyClass()->play(7);
            setStateShowed(result.blg);
        } else {
#if defined(DEBUG) && defined(DEBUG_STATE)
            Serial.print(F("Restoring screen from STATE: "));
            Serial.println(cursorMenu);
#endif
            MidCursorMenu = cursorMenu; // return user to last usable screen
        };

    }


    /**
     *
     * @return int Voltage
     */
    float getVoltage(void);

    boolean getLiveOil();

    boolean getLiveCnt();

    boolean getLiveWin();

    boolean getLiveBrk();

    boolean getLiveVol();

/**
 *
 * @param pinO
 * @param pinC
 * @param pinW
 * @param pinB
 * @param pinV
 */
    void begin();

    void listener() {
        if (amp->isSecond() && millis() > 3000) {

            sensorDigital(STT_OIL_PIN, result.oil);
            sensorDigital(STT_CLN_PIN, result.cnt);
            sensorDigital(STT_BRK_PIN, result.brk);
            sensorDigital(STT_WNW_PIN, result.win);


            sensorCustom(isWinter(), result.wnt);
            sensorCustom(isOverhead(), result.ovh);
            sensorCustom(isBadVoltage(), result.vol);
            sensorCustom(isBadBLights(), result.blg);
            sensorCustom(workDistance > CAR_STT_TM_BELT, result.blt);
        }

#if  defined(DEBUG) && defined(DEBUG_STATE)
        if (amp->isSecond()) {
            Serial.print(F("Break state counter: "));
            Serial.println(result.brk);
            Serial.print(F("Oil state counter: "));
            Serial.println(result.oil);
            Serial.print(F("Coolant state counter: "));
            Serial.println(result.cnt);
            Serial.print(F("Washer state counter: "));
            Serial.println(result.win);
        }
#endif
        // Show new warning
        // Reset values after an hour
        if (amp->isHour()) {
            result.oil = 0;
            result.cnt = 0;
            result.win = 0;
            result.brk = 0;
            result.vol = 0;
            result.blt = 0;
            result.la1 = 0;
            result.la2 = 0;
//            result.wnt = 0;
            result.ovh = 0;
            result.blg = 0;
            initAlertState = false;
        }
        //
        // Handle menu cursor
        cursor();


    };


    boolean isStateAlert(uint8_t &container) {
        if (container > CAR_STT_A1_ALERT && container < CAR_STT_A2_ALERT && amp->is4Seconds()) {
            container = CAR_STT_A2_ALERT;
            return true;
        }
        return false;
    }


    /**
     * Changes menu cursor
     */
    void cursor() {

        if (isStateAlert(result.oil) || isStateAlert(result.brk) || isStateAlert(result.cnt) ||
            isStateAlert(result.win) || isStateAlert(result.vol) || isStateAlert(result.wnt) ||
            isStateAlert(result.ovh) || isStateAlert(result.blg)) {
            alertState = 1;
            if (!initAlertState) {
                cursorMenu = MidCursorMenu;
                MidCursorMenu = MENU_SERVICE;
                usedMenu.used = 0;
                usedMenu.back = 0;
                initAlertState = 1;
            }
        }
    }

    /**
     * Checks for winter temperature
     */
    boolean isWinter() {
        if (car->getTmpOut() < -2) {
            alertWinter = true;
            return true;
        }
        if (alertWinter) {
            result.wnt = 0; // reset winter for new check
            alertWinter = false;
        }

        return false;
    }

    /**
     * Checks engine overheating
     */
    boolean isOverhead() {
        if (car->getEngTmp() > ENGINE_OVERHEAT) {
            alertOverheat = true;
            return true;
        }
        if (alertOverheat) {
            result.ovh = 0; // reset overheat for new check
            alertOverheat = false;
        }

        return false;
    }

    boolean isBadBLights() {
        if (alertBrkLgBad) {
            return true;
        }
        if (analogRead(STT_UL2_PIN) < 500 && digitalRead(BRK_LGH_PIN)) {
            alertBrkLgBad = true;
            return true;
        }

        return false;
    }

    Diagnostic getResult();
};


/**
 *
 * @param pinO
 * @param pinC
 * @param pinW
 * @param pinB
 * @param pinV
 * @param pinL
 */
void CarState::begin() {
    //
    // Sets mode
    pinMode(STT_OIL_PIN, INPUT_PULLUP);
    pinMode(STT_CLN_PIN, INPUT_PULLUP);
    pinMode(STT_WNW_PIN, INPUT_PULLUP);
    pinMode(STT_BRK_PIN, INPUT_PULLUP);
    pinMode(STT_UL2_PIN, INPUT_PULLUP);

    pinMode(STT_VLT_PIN, INPUT);


}


/**
 * Sets work distance from EepRom for calculation
 * @param distance
 */
void CarState::setWorkState(float distance) {
    workDistance = distance;
}

/**
 * Gets live Oil level
 * @return boolean
 */
boolean CarState::getLiveOil() {
    return (boolean) digitalRead(STT_OIL_PIN);
}

/**
 * Gets live Window Wash level
 * @return boolean
 */
boolean CarState::getLiveCnt() {
    return (boolean) digitalRead(STT_CLN_PIN);
}

/**
 * Gets live Window Wash level
 * @return boolean
 */
boolean CarState::getLiveWin() {
    return (boolean) digitalRead(STT_WNW_PIN);
}

/**
 * Gets live Brake status
 * @return boolean
 */
boolean CarState::getLiveBrk() {
    return (boolean) digitalRead(STT_BRK_PIN);
}

/**
 * Gets live voltage
 * @return boolean
 */
boolean CarState::getLiveVol() {
    return isBadVoltage();
}

/**
 *  Gets car voltage
 * @return integer
 */
float CarState::getVoltage(void) {
    return float(analogRead(STT_VLT_PIN)) / 68;
}

/**
 * Gets diagnostic result
 * @return Diagnostic
 */
Diagnostic CarState::getResult() {
    return result;
}

/**
 * Is car have some issue with simple verification
 * @return boolean
 */
boolean CarState::isAlert() {
    return alertState;
}


#endif //ARDUINOMID_CARSTAT_H

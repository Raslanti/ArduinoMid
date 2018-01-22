//
// Created by Angel Zaprianov on 23.2.2017 г..
//

#ifndef ARDUINO_MID_MENU_BASE_H
#define ARDUINO_MID_MENU_BASE_H

//#define MENU_DEBUG // Uncomment to get info

#include <Arduino.h>
#include "MenuBackend.h"
#include "../MID.h"
#include "MenuBtn.h"


//
// Define menu tree
#ifndef MENU_ENTRY
#define MENU_ENTRY 0
#endif
/**
 * Structure of menu container
 */
struct UsedMenu {
    const char *used;
    const char *back;
    const char *next;
    const char *last;
    const char *down;
};
/**
* Reserve space in used menu container
*/
static UsedMenu usedMenu = {
        used:""/* new char(512)*/
};

//
// Saves cursor between changes
uint8_t MenuBase_savedCursor = 0;

/**
 *
 */
class MenuBase {
    MenuUiInterface *mci;
    MenuBtn *btn;

public:
    //
    // Constructor
    MenuBase(MenuBtn &_btn, MenuUiInterface &_mci) : btn(&_btn), mci(&_mci) {
        MenuBase_savedCursor = 1;
    }


    /**
     * Used to activate / deactivate navigation
     */
    void startEntry() {
//        resRam.listen();
        btn->setNavigationState(false);
    }

    /**
     * Restore position of menu
     */
    void finishEntry() {

        //
        // Handles initialization
        if (MenuBase_savedCursor == 0) {
#if defined(DEBUG_MENU)

            Serial.println(F(" Makes init move ..."));

#endif
//            resRam.listen();
            //
            // Move menu to first index
            mci->moveUp();
            MenuBase_savedCursor = 1;
//            resRam.listen();
        }

        MidCursorMenu = MenuBase_savedCursor;
        btn->setNavigationState(true);

#if defined(DEBUG_MENU)
        Serial.print(F("Cursor menu: "));
        Serial.println(MidCursorMenu);
        Serial.print(F("Saved Cursor menu: "));
        Serial.println(MenuBase_savedCursor);
        Serial.print(F("Used menu: "));
        Serial.println(usedMenu.used);
#endif
    }

    void triggerMoveUp() {
        mci->moveUp();
    }

    void triggerMoveDw() {
        mci->moveDw();
    }

    /**
     * Perform navigation
     */
    void listener() {

        //
        // Handle navigation
        if (btn->isUp() && !btn->isHl()) {
            mci->moveUp();
#if defined(DEBUG_MENU)
            if (btn->passAmp()->isMid()) {
                Serial.print(F("Up hit \n\r"));
            }
#endif
        }
        if (btn->isDw()) {
            mci->moveDw();
#if defined(DEBUG_MENU)
            if (btn->passAmp()->isMid()) {
                Serial.print(F("Dw hit \n\r"));
            }
#endif
        }


        btn->clearLastButton();


#if defined(DEBUG_MENU)
        if (btn->passAmp()->isSecond()) {
            Serial.print(F("Cursor saved: "));
            Serial.println(MenuBase_savedCursor);
        }
#endif
        //
        //
        if (MenuBase_savedCursor != MidCursorMenu && MidCursorMenu != MENU_ENTRY) {
            //
            // Keep cursor in save place
            MenuBase_savedCursor = MidCursorMenu;
            //
            // Change menu to show screen
            MidCursorMenu = MENU_ENTRY;
        }
#if defined(DEBUG_MENU)
        if (btn->passAmp()->isSecond()) {
            Serial.print(F("Cursor MID: "));
            Serial.println(MidCursorMenu);
        }
#endif
    }


};


#endif //ARDUINOMID_MENUBASE_H

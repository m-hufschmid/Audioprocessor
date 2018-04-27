/*******************************************************************************
 * Project:
 *  "Audioprocessor"
 * 
 * Version / Date:
 *  2.3 / Jan 2018
 * 
 * Authors:
 *  Markus Hufschmid
 * 
 * File Name:
 *  controls.c
 * 
 * Description:
 *  Updates and processes controls (buttons, encoders)
 *******************************************************************************/

#include <xc.h>
#include "controls.h"
#include "GUI.h"
#include "DOGS.h"
#include "globals.h"
#include "GenericTypeDefs.h"
#include "menu.h"
#include "hardware.h"
#include "filterdesign.h"

/* states */
#define ARMED 0
#define PRESSED 1
#define RELEASED 2

enum buttonStates {
    BTN_STATE_ARMED, BTN_STATE_DEBOUNCE, BTN_STATE_PRESSED, BTN_STATE_RELEASED
};

/* time constants for debouncing */
#define DEBOUNCE_5MS 50
#define DEBOUNCE_10MS 100
#define WAIT_250MS 2500


CONTROLSTATUS updateControls(void) {
    static unsigned char REncState = 0, LEncState = 0;

    //update states of buttons
    buttonStateMachine(&topButton, TOPBUTTON);
    buttonStateMachine(&middleButton, MIDDLEBUTTON);
    buttonStateMachine(&bottomButton, BOTTOMBUTTON);
    buttonStateMachine(&leftEncoderButton, LEFTENCODERBUTTON);
    buttonStateMachine(&rightEncoderButton, RIGHTENCODERBUTTON);
    switch (REncState) {
        case ARMED:
            if (RIGHTENCODER_A == 0) {
                REncState = PRESSED;
                if (RIGHTENCODER_B == 1) {
                    REncValue += 1;
                } else {
                    REncValue -= 1;
                }
                controls.flags.REnc = 1;
            }
            break;
        case PRESSED:
            if (RIGHTENCODER_A == 1) {
                REncState = RELEASED;
            }
            REncTimer = DEBOUNCE_5MS;
            break;
        case RELEASED:
            if (RIGHTENCODER_A == 0) {
                REncState = PRESSED;
            } else if (REncTimer == 0) {
                REncState = ARMED;
            }
            break;
    }
    switch (LEncState) {
        case ARMED:
            if (LEFTENCODER_A == 0) {
                LEncState = PRESSED;
                if (LEFTENCODER_B == 1) {
                    LEncValue += 1;
                } else {
                    LEncValue -= 1;
                }
                controls.flags.LEnc = 1;
            }
            break;
        case PRESSED:
            if (LEFTENCODER_A == 1) {
                LEncState = RELEASED;
            }
            LEncTimer = DEBOUNCE_5MS;
            break;
        case RELEASED:
            if (LEFTENCODER_A == 0) {
                LEncState = PRESSED;
            } else if (LEncTimer == 0) {
                LEncState = ARMED;
            }
            break;
    }
    return controls;
}

void processControls(void) {

    if (topButton.counter) {
        filterOn = 1 - filterOn;
        topButton.counter = 0;
    }
    if (middleButton.counter) {
        autonotchOn = 1 - autonotchOn;
        middleButton.counter = 0;
    }
    if (bottomButton.counter) {
        clearPartScreen(1, 0, 4, 66);
        dispFFT = 1 - dispFFT;
        bottomButton.counter = 0;
    }
    if (rightEncoderButton.counter) {
        rightEncoderButton.counter = 0;
        filter_type = 1 - filter_type;
        if (filter_type == BPFILTER) {
            build_filter(BANDPASS);
        } else {
            build_filter(LOWPASS);
            build_filter(HIGHPASS);
        }
    }
    if (leftEncoderButton.counter > 4) { //if left encoder button is pressed longer than 4*250ms
        menu();
        build_menu();
        build_GUI();
        update_GUI();
        leftEncoderButton.counter = 0;
    }

    if (controls.flags.REnc || controls.flags.LEnc) {
        controls.flags.REnc = 0;
        controls.flags.LEnc = 0;
        if (filter_type == BPFILTER) {
            iBPFilter += LEncValue;
            if (iBPFilter < 0) {
                iBPFilter = 0;
            }
            if (iBPFilter > N_BP_FILTERS - 1) {
                iBPFilter = N_BP_FILTERS - 1;
            }
            LEncValue = 0;
            fc += FC_STEP*REncValue;
            if (fc < FC_MIN) {
                fc = FC_MIN;
            }
            if (fc > FC_MAX) {
                fc = FC_MAX;
            }
            REncValue = 0;
            build_filter(BANDPASS);
        } else {
            iHPFilter += LEncValue;
            if (iHPFilter < 0) {
                iHPFilter = 0;
            }
            if (iHPFilter > N_HP_FILTERS - 1) {
                iHPFilter = N_HP_FILTERS - 1;
            }
            LEncValue = 0;
            iLPFilter += REncValue;
            if (iLPFilter < 0) {
                iLPFilter = 0;
            }
            if (iLPFilter > N_LP_FILTERS - 1) {
                iLPFilter = N_LP_FILTERS - 1;
            }
            REncValue = 0;
            build_filter(LOWPASS);
            build_filter(HIGHPASS);
        }
    }
}

void buttonStateMachine(buttonStruct* button, int port) {
    // update state of button, increment counter if necessary

    switch (button->state) {
        case BTN_STATE_ARMED:
            if (port == 0) {
                button->state = BTN_STATE_DEBOUNCE;
                button->counter++;
                controls.flags.anyButton = 1;
                button->timer = DEBOUNCE_10MS;
            }
            break;
        case BTN_STATE_DEBOUNCE:
            if (button->timer == 0) {
                button->state = BTN_STATE_PRESSED;
                button->timer = WAIT_250MS;
            }
            break;
        case BTN_STATE_PRESSED:
            if (port == 1) {
                button->state = BTN_STATE_RELEASED;
                button->timer = DEBOUNCE_10MS;
            } else {
                if (button->timer == 0) {
                    button->timer = WAIT_250MS;
                    button->counter++;
                }
            }
            break;
        case BTN_STATE_RELEASED:
            if (button->timer == 0) {
                button->state = BTN_STATE_ARMED;
            }
            break;
    }

}

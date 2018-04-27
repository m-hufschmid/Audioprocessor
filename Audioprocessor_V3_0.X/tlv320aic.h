#ifndef _TLV320AIC_H
#define	_TLV320AIC_H

#include <stdint.h>
#include "constants.h"


#define TLV320_ADDR 0x1A
//#define TLV320_ADDR 0x1B
#define TLV320_NUM_REGS 10

#define TLV320_LEFT_IN_VOL_REG 0x00
#define TLV320_RIGHT_IN_VOL_REG 0x01
#define TLV320_LEFT_HP_VOL_REG 0x02
#define TLV320_RIGHT_HP_VOL_REG 0x03
#define TLV320_ANALOG_AUDIO_PATH_REG 0x04
#define TLV320_DIGITAL_AUDIO_PATH_REG 0x05
#define TLV320_POWER_DOWN_REG 0x06
#define TLV320_DIGITAL_AUDIO_IF_FORMAT_REG 0x07
#define TLV320_SAMPLE_RATE_CTRL_REG 0x08
#define TLV320_DIGITAL_IF_ACTIVATION_REG 0x09
#define TLV320_RESET_REG 0x0F

typedef enum {
    LINE, 
    MIC,     
} input_t; /**< @typedef input_t
 * @enum LINE   specifies the line input
 * @enum MIC    specifies the microphone input
 */
/**
 * Initialises the codec to its default values.
 */
void codec_init();
/**
 * Resets the codec to its default values.
 */
void codec_reset();
/**
 * Mutes the codec Input if the bool <em>on</em> is true.
 * @param mute  defines whether the coded should be muted or not
 */
void codec_mute(bool mute);
/**
 * Chooses the input channel for the codec.
 * @param in    is an enum, it can be LINE or MIC
 */
void codec_setInput(input_t in);
/**
 * Activates/Deactivates the + 20 dB boost for the mirophone input.
 * @param boost     defines whether the boost is turned on or not
 */
void codec_setMicboost(bool );
/**
 * Sets the line input volume.
 * @param vol   should be a value between 0x1F=12dB and 0x00=-34.5dB (1.5 dB steps)
 */
void codec_setLineInVol(uint8_t);
/**
 * Sets the headphone output volume.
 * @param vol   should be a value between 0x4F=6dB and 0x00=-73dB (79 steps)
 */
void codec_setHeadphoneVol(uint8_t);

#endif	/* _TLV320AIC_H */


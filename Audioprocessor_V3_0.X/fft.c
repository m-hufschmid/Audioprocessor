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
 *  fft.c
 * 
 * Description:
 *  Computing and displaying the Fast Fourier Transform
 *******************************************************************************/

#include <dsp.h>
#include <xc.h>
#include "fft.h"
#include "DOGS.h"
#include "globals.h"
#include "GUI.h"

unsigned int tempDSRPAG;

extern const fractcomplex __attribute__((space(auto_psv), aligned(FFT_BLOCK_LENGTH * 2))) twiddleFactors[FFT_BLOCK_LENGTH / 2];

fractcomplex __attribute__((eds, space(ymemory), aligned(FFT_BLOCK_LENGTH * 2 * 2))) fftVector[FFT_BLOCK_LENGTH];

static INT fftValues[FFT_BLOCK_LENGTH / 2];

INT kaiserWindow[FFT_BLOCK_LENGTH] = {
    12, 23, 38, 59, 85, 120, 162, 215, 280, 357, 449, 557, 683, 829, 996, 1187, 1403, 1646, 1919, 2221, 2556, 2925, 3328, 3768, 4245, 4760, 5314, 5907, 6538, 7208, 7917, 8662, 9444, 10260, 11108, 11987, 12894, 13826, 14779, 15751, 16737, 17733, 18735, 19739, 20739, 21731, 22709, 23670, 24607, 25515, 26390, 27227, 28020, 28766, 29459, 30095, 30671, 31184, 31629, 32004, 32307, 32536, 32690, 32767, 32767, 32690, 32536, 32307, 32004, 31629, 31184, 30671, 30095, 29459, 28766, 28020, 27227, 26390, 25515, 24607, 23670, 22709, 21731, 20739, 19739, 18735, 17733, 16737, 15751, 14779, 13826, 12894, 11987, 11108, 10260, 9444, 8662, 7917, 7208, 6538, 5907, 5314, 4760, 4245, 3768, 3328, 2925, 2556, 2221, 1919, 1646, 1403, 1187, 996, 829, 683, 557, 449, 357, 280, 215, 162, 120, 85, 59, 38, 23, 12
};

INT chebyWindow[FFT_BLOCK_LENGTH] = {
    223, 152, 203, 264, 337, 422, 521, 635, 766, 916, 1084, 1274, 1486, 1722, 1982, 2269, 2583, 2926, 3298, 3701, 4134, 4600, 5097, 5627, 6189, 6784, 7411, 8069, 8758, 9477, 10224, 10998, 11798, 12621, 13465, 14328, 15208, 16100, 17004, 17914, 18829, 19745, 20657, 21563, 22458, 23339, 24201, 25042, 25858, 26643, 27396, 28111, 28787, 29418, 30003, 30539, 31022, 31450, 31821, 32134, 32386, 32576, 32703, 32767, 32767, 32703, 32576, 32386, 32134, 31821, 31450, 31022, 30539, 30003, 29418, 28787, 28111, 27396, 26643, 25858, 25042, 24201, 23339, 22458, 21563, 20657, 19745, 18829, 17914, 17004, 16100, 15208, 14328, 13465, 12621, 11798, 10998, 10224, 9477, 8758, 8069, 7411, 6784, 6189, 5627, 5097, 4600, 4134, 3701, 3298, 2926, 2583, 2269, 1982, 1722, 1486, 1274, 1084, 916, 766, 635, 521, 422, 337, 264, 203, 152, 223
};

INT rectWindow[FFT_BLOCK_LENGTH] = {
    32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767
};

void computeFFT(int *data) {

    INT SquareMagnitudeValues[FFT_BLOCK_LENGTH];
    UINT8 i;
    INT32 SqrdReal, SqrdImag;
    long int realValue, imagValue;
 
    switch (config.fft_window) {
        case 0:
            VectorMultiply(FFT_BLOCK_LENGTH, &data[0], &data[0], &rectWindow[0]);
            break;
        case 1:
            VectorMultiply(FFT_BLOCK_LENGTH, &data[0], &data[0], &kaiserWindow[0]);
            break;
        case 2:
            VectorMultiply(FFT_BLOCK_LENGTH, &data[0], &data[0], &chebyWindow[0]);
            break;
    }
    for (i = 0; i < FFT_BLOCK_LENGTH; i++) {
        fftVector[i].real = data[i]>>1;
        fftVector[i].imag = 0;
    }
    FFTComplexIP(LOG2N, (fractcomplex*) & fftVector[0], (fractcomplex *) 
            __builtin_psvoffset(&twiddleFactors[0]), __builtin_psvpage(&twiddleFactors[0]));
    BitReverseComplex(LOG2N, (fractcomplex*) & fftVector[0]);

    tempDSRPAG = DSRPAG;
    DSRPAG = 0x0001; //without this, access to fftVector[i] does not work 
    for (i = 0; i < FFT_BLOCK_LENGTH / 2; i++) {
        realValue = (long int) fftVector[i].real;
        imagValue = (long int) fftVector[i].imag;
        SqrdReal = realValue*realValue;
        SqrdImag = imagValue*imagValue;
        SqrdReal = SqrdImag + SqrdReal;

        SquareMagnitudeValues[i] = intSqrt(SqrdReal);
    }
    DSRPAG = tempDSRPAG;
    for (i = 0; i < FFT_BLOCK_LENGTH / 2; i++) {

        if (config.fft_detect == 0) {
            if (SquareMagnitudeValues[i] > fftValues[i]) {
                fftValues[i] = SquareMagnitudeValues[i];
            } else {
                fftValues[i] = (INT) (0.852 * (float) fftValues[i]);
            }
        } else {
            fftValues[i] = (INT) (0.852 * (float) fftValues[i] + 0.148 * (float) SquareMagnitudeValues[i]);
        }

    }
}

void plotFFT(void) {
    UINT8 i, j, x, y, dataDOGS;
    UINT16 dBValue;
    const UINT8 gaugeData[8] = {0, 128, 192, 224, 240, 248, 252, 254};

    x = 0;
    y = FFT_TOP_PAGE;
    for (j = 0; j < 3; j++) {
        set_column_DOGS(x);
        set_page_DOGS(y);
        send_data_DOGS(0xFF);
        y--;
    }
    x++;
    for (i = 0; i < FFT_BLOCK_LENGTH / 2; i++) {
        dBValue = dirtyLog((UINT16) fftValues[i]);
        y = FFT_TOP_PAGE;
        for (j = 0; j < 3; j++) {
            set_column_DOGS(x);
            set_page_DOGS(y);
            if (dBValue > 7) {
                dataDOGS = 0xFF;
            } else {
                dataDOGS = gaugeData[dBValue];
            }
            if (j == 2) {
                dataDOGS |= 0x01;
            }
            send_data_DOGS(dataDOGS);
            if (dBValue > 7) {
                dBValue = dBValue - 8;
            } else {
                dBValue = 0;
            }
            y--;
        }
        x++;
    }
    y = FFT_TOP_PAGE;
    for (j = 0; j < 3; j++) {
        set_column_DOGS(x);
        set_page_DOGS(y);
        send_data_DOGS(0xFF);
        y--;
    }
}

UINT16 dirtyLog(UINT16 x) {
    //computes an estimate in dB from linear value x
    UINT16 r = 15;

    while (r > 0) {
        if (x & 32768) {
            break;
        }
        x = x << 1;
        r--;
    }
    r = r << 1;
    if (x > 46340) {
        r++;
    }
    return r;
}

UINT16 intSqrt(UINT32 x) {
    //computes integer value of square root
    UINT16 exponent, i;
    UINT32 xTemp, y;

    if (x == 0) {
        y = 0;
    } else {
        exponent = 31;
        xTemp = x;
        while (exponent > 0) {
            if (xTemp & 0x80000000) {
                break;
            }
            xTemp = xTemp << 1;
            exponent--;
        }
        y = 1U << (exponent >> 1);
        for (i = 0; i < 5; i++) {
            y = (x / y + y) / 2;
        }
    }
    return (UINT16) y;
}

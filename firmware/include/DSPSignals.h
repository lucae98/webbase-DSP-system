/****************************************************************************
 * Filename:    DSPSignals.h
 * Description: DSPSignals.h from webbase-DSP-system
 * Author:      Luca Echterhof
 * Year:        2025
 * License:     GPL-3.0 License
 ****************************************************************************/

#ifndef DSPSIGNALS_H
#define DSPSIGNALS_H

#include <stdint.h> // Bib für Datentypen
#include <Arduino.h>
#include <SPI.h>
#include <math.h>
#include "IO.h" // Include the My IO library to handle gloabl data

#define TWO_POW_24 16777216 // 2^24 = 16777216 für 8.24 Umwandlung

typedef int32_t fixed_8_24; // Datentyp für fixpointwerte

extern SPISettings spiSettings;

//=================
// DSP SetUp

#define SPI_CS_PIN 5    // Chip Select Pin
#define SPI_CLK_PIN 18  // Clock Pin (HSPI)
#define SPI_MISO_PIN 19 // MISO Pin
#define SPI_MOSI_PIN 23 // MOSI Pin
#define DSP_BYTES_ADDR 2
#define DSP_BYTES_DATA 4
#define DSP_ADDR_READ 1
#define DSP_ADDR_WRITE 0
#define DSP_NYQUIST_LIMIT 48000

// DSP Adressen

namespace DSPAddresses
{
    // Safeload
    const uint16_t Safeload_Data0 = 24;  // Addresse für Data im Safeload
    const uint16_t Safeload_Data1 = 25;  // Addresse für Data im Safeload
    const uint16_t Safeload_Data2 = 26;  // Addresse für Data im Safeload
    const uint16_t Safeload_Data3 = 27;  // Addresse für Data im safeload
    const uint16_t Safeload_Data4 = 28;  // Addresse für Data im safeload
    const uint16_t Safeload_Adress = 29; // Adresse für ZielAdresse im Safeload
    const uint16_t Safeload_Number = 30; // Adresse für Number und Trigger register

    // Filter 1 Ch 1
    constexpr uint16_t Ch1_Filter1_B2 = 24660;
    constexpr uint16_t Ch1_Filter1_B1 = 24661;
    constexpr uint16_t Ch1_Filter1_B0 = 24662;
    constexpr uint16_t Ch1_Filter1_A2 = 24663;
    constexpr uint16_t Ch1_Filter1_A1 = 24664;
    constexpr uint16_t Ch1_Filter1_SlewMode = 50;

    // Filter 1 Ch 2
    constexpr uint16_t Ch2_Filter1_B2 = 24665;
    constexpr uint16_t Ch2_Filter1_B1 = 24666;
    constexpr uint16_t Ch2_Filter1_B0 = 24667;
    constexpr uint16_t Ch2_Filter1_A2 = 24668;
    constexpr uint16_t Ch2_Filter1_A1 = 24669;
    constexpr uint16_t Ch2_Filter1_SlewMode = 58;

    // Filter 2 Ch 1
    constexpr uint16_t Ch1_Filter2_B2 = 24670;
    constexpr uint16_t Ch1_Filter2_B1 = 24671;
    constexpr uint16_t Ch1_Filter2_B0 = 24672;
    constexpr uint16_t Ch1_Filter2_A2 = 24673;
    constexpr uint16_t Ch1_Filter2_A1 = 24674;
    constexpr uint16_t Ch1_Filter2_SlewMode = 54;

    // Filter 2 Ch 2
    constexpr uint16_t Ch2_Filter2_B2 = 24675;
    constexpr uint16_t Ch2_Filter2_B1 = 24676;
    constexpr uint16_t Ch2_Filter2_B0 = 24677;
    constexpr uint16_t Ch2_Filter2_A2 = 24678;
    constexpr uint16_t Ch2_Filter2_A1 = 24679;
    constexpr uint16_t Ch2_Filter2_SlewMode = 60;

    // Filter 3 Ch 1
    constexpr uint16_t Ch1_Filter3_B2 = 24680;
    constexpr uint16_t Ch1_Filter3_B1 = 24681;
    constexpr uint16_t Ch1_Filter3_B0 = 24682;
    constexpr uint16_t Ch1_Filter3_A2 = 24683;
    constexpr uint16_t Ch1_Filter3_A1 = 24684;
    constexpr uint16_t Ch1_Filter3_SlewMode = 66;

    // Filter 3 Ch 2
    constexpr uint16_t Ch2_Filter3_B2 = 24685;
    constexpr uint16_t Ch2_Filter3_B1 = 24686;
    constexpr uint16_t Ch2_Filter3_B0 = 24687;
    constexpr uint16_t Ch2_Filter3_A2 = 24688;
    constexpr uint16_t Ch2_Filter3_A1 = 24689;
    constexpr uint16_t Ch2_Filter3_SlewMode = 72;

    // Filter 4 Ch 1
    constexpr uint16_t Ch1_Filter4_B2 = 24690;
    constexpr uint16_t Ch1_Filter4_B1 = 24691;
    constexpr uint16_t Ch1_Filter4_B0 = 24692;
    constexpr uint16_t Ch1_Filter4_A2 = 24693;
    constexpr uint16_t Ch1_Filter4_A1 = 24694;
    constexpr uint16_t Ch1_Filter4_SlewMode = 78;

    // Filter 4 Ch 2
    constexpr uint16_t Ch2_Filter4_B2 = 24695;
    constexpr uint16_t Ch2_Filter4_B1 = 24696;
    constexpr uint16_t Ch2_Filter4_B0 = 24697;
    constexpr uint16_t Ch2_Filter4_A2 = 24698;
    constexpr uint16_t Ch2_Filter4_A1 = 24699;
    constexpr uint16_t Ch2_Filter4_SlewMode = 84;

    // Mute Ch1
    constexpr uint16_t Ch1_Mute = 93;
    constexpr uint16_t Ch1_Mute_SlewMode = 92;
    // Mute Ch2
    constexpr uint16_t Ch2_Mute = 96;
    constexpr uint16_t Ch2_Mute_SlewMode = 95;
    // Pre Gain Ch1
    constexpr uint16_t Ch1_PreGain = 35;
    constexpr uint16_t Ch1_PreGain_SlewMode = 36;
    // Pre Gain Ch2
    constexpr uint16_t Ch2_PreGain = 32;
    constexpr uint16_t Ch2_PreGain_SlewMode = 33;
    // Post Gain Ch1
    constexpr uint16_t Ch1_PostGain = 86;
    constexpr uint16_t Ch1_PostGain_SlewMode = 87;
    // Post Gain Ch2
    constexpr uint16_t Ch2_PostGain = 89;
    constexpr uint16_t Ch2_PostGain_SlewMode = 90;
    // Meter
    constexpr uint16_t Ch1_MeterL = 121;
    constexpr uint16_t Ch2_MeterR = 107;

}

void handleNewDatafromWebsite();
void calculateFilterCoefficients(GlobalData::Filter &filter);
void dspWriteRegister_8_24(uint16_t const address, float const data);
void dspWriteRegisterFixPoint(uint16_t const address, uint32_t const data);
void dspWriteSafeloadAdressRegister(uint16_t addressRegister);
void dspInitForSPI();
float dspReadRegister(uint16_t const address);
fixed_8_24 floatToFixed8_24(float value);
float fixed8_24ToFloat(fixed_8_24 num);
float mapFrequenz(float freq);
float unmapFrequenz(float num);
float dBToFloat(float dB);
float floatTodB(float num);

#endif

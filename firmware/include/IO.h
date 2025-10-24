/****************************************************************************
 * Filename:    IO.h
 * Description: IO.h from webbase-DSP-system
 * Author:      Luca Echterhof
 * Year:        2025
 * License:     GPL-3.0 License
 ****************************************************************************/

#ifndef IO_H
#define IO_H

#include <SPIFFS.h>
#include <memory>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include <stdint.h>
// #include "DSPSignals.h"

#define PATH_CONFIG "/config.json"
#define JSON_KEY_SSID "ssid"
#define JSON_KEY_PW "password"
#define JSON_KEY_WIFICHANNEL "wifi-channel"
#define JSON_KEY_WIFICLIENTS "wifi-clients"
#define JSON_KEY_WIFIHIDDEN "wifi-hidden"
#define JSON_KEY_MDNS "mdns"
#define JSON_KEY_IP "ip"
#define JSON_KEY_IPGATEWAY "ip-gateway"
#define JSON_KEY_SUBNETMASK "subnetmask"
#define PATH_HTML_INDEX "/index.html"
#define PATH_CSS_MAIN "/main.css"
#define PATH_JS_APP "/app.js"
#define DSP_SAMPLINGFREQ 96000
#define DSP_SAMPLING_T 11 // 10,4 µs ist die Periodendauer von der Samplingfrequnez

class GlobalData
{
public:
    // Verschachtelte Klasse für Filterdaten
    class Filter
    {
    private:
        bool hasChanged;
        bool hasChangedBypass;
        uint8_t bypass = 0;
        double a1 = 0.0, a2 = 0.0, b0 = 1.0, b1 = 0.0, b2 = 0.0;
        float freq = 0.0f, gain = 0.0f, q = 0.0f, samplingFreq = DSP_SAMPLINGFREQ;
        uint16_t slewMode = 8330;
        uint8_t parameterCount = 5; // for saveload Write der Coeefizenten

        // Adresses Ch1
        const uint16_t dspAdressB2Ch1;
        const uint16_t dspAdressB1Ch1;
        const uint16_t dspAdressB0Ch1;
        const uint16_t dspAdressA2Ch1;
        const uint16_t dspAdressA1Ch1;
        const uint16_t dspAdressSlewMCh1;
        // Adresses Ch2
        const uint16_t dspAdressB2Ch2;
        const uint16_t dspAdressB1Ch2;
        const uint16_t dspAdressB0Ch2;
        const uint16_t dspAdressA2Ch2;
        const uint16_t dspAdressA1Ch2;
        const uint16_t dspAdressSlewMCh2;

    public:
        // Konstruktor Adressen müssen übergeben werden
        Filter(
            uint16_t b2Ch1, uint16_t b1Ch1, uint16_t b0Ch1,
            uint16_t a2Ch1, uint16_t a1Ch1, uint16_t slewMCh1,
            uint16_t b2Ch2, uint16_t b1Ch2, uint16_t b0Ch2,
            uint16_t a2Ch2, uint16_t a1Ch2, uint16_t slewMCh2)
            : dspAdressB2Ch1(b2Ch1), dspAdressB1Ch1(b1Ch1), dspAdressB0Ch1(b0Ch1),
              dspAdressA2Ch1(a2Ch1), dspAdressA1Ch1(a1Ch1), dspAdressSlewMCh1(slewMCh1),
              dspAdressB2Ch2(b2Ch2), dspAdressB1Ch2(b1Ch2), dspAdressB0Ch2(b0Ch2),
              dspAdressA2Ch2(a2Ch2), dspAdressA1Ch2(a1Ch2), dspAdressSlewMCh2(slewMCh2)
        {
            hasChanged = false;
            hasChangedBypass = false;
            bypass = 0;
            // Optional: weitere Initialisierung
        }

        // Getter und Setter für alle Werte
        bool getHasChanged() const;
        void setHasChanged(bool value);

        bool getHasChangedBypassed() const;
        void setHasChangedBypassed(bool value);

        uint8_t getBypass() const;
        void setBypass(uint8_t value);

        double getA1() const;
        void setA1(double value);

        double getA2() const;
        void setA2(double value);

        double getB0() const;
        void setB0(double value);

        double getB1() const;
        void setB1(double value);

        double getB2() const;
        void setB2(double value);

        float getFreq() const;
        void setFreq(float value);

        float getGain() const;
        void setGain(float value);

        float getQ() const;
        void setQ(float value);

        float getSamplingFreq() const;
        void setSamplingFreq(float value);

        uint16_t getSlewMode() const;
        void setSlewMode(uint16_t value);

        uint16_t getDSPAdressA1Ch1() const;

        uint16_t getDSPAdressA2Ch1() const;

        uint16_t getDSPAdressB0Ch1() const;

        uint16_t getDSPAdressB1Ch1() const;

        uint16_t getDSPAdressB2Ch1() const;

        uint16_t getDSPAdressSlewModeCh1() const;

        uint16_t getDSPAdressA1Ch2() const;

        uint16_t getDSPAdressA2Ch2() const;

        uint16_t getDSPAdressB0Ch2() const;

        uint16_t getDSPAdressB1Ch2() const;

        uint16_t getDSPAdressB2Ch2() const;

        uint16_t getDSPAdressSlewModeCh2() const;

        uint8_t getParameterCount() const;

        void writeSafeloadRegister();

        void writeBypassedSafeloadRegister();
    };

    // Verschachtelte Klasse für Oszillator
    class Meter
    {
    private:
        float MeterLdB = 0.0f;
        float MeterRdB = 0.0f;
        unsigned long lastSend = 0;
        uint16_t sendIntervall_ms = 0;
        // Addressen Channel L
        const uint16_t dspAdressMeterLCh1;
        // Addressen Channel R
        const uint16_t dspAdressMeterRCh2;

    public:
        Meter(
            uint16_t Ch1_MeterL, uint16_t Ch2_MeterR)
            : dspAdressMeterLCh1(Ch1_MeterL),
              dspAdressMeterRCh2(Ch2_MeterR)
        {
            sendIntervall_ms = 400;
        }
        long getLastSend();
        void setLastSend(long values);
        uint16_t getsendIntervall();
        void updateMeterValues();
        float getLdB();
        float getRdB();
        char const *checkInfinityOrNaN(float value);

        String getDataForWebsocket();
    };

    // Verschachtelte Klasse für Mute
    class Mute
    {
    private:
        bool hasChanged = false;
        uint8_t mute = 0;
        uint16_t slewMode = 8330;
        uint8_t parameterCount = 2;
        // Adresses
        const uint16_t dspAdressMuteCh1;
        const uint16_t dspAdressSlewMCh1;
        const uint16_t dspAdressMuteCh2;
        const uint16_t dspAdressSlewMCh2;

    public:
        Mute(
            uint16_t muteCh1, uint16_t slewModeCh1,
            uint16_t muteCh2, uint16_t slewModeCh2)
            : dspAdressMuteCh1(muteCh1),
              dspAdressSlewMCh1(slewModeCh1),
              dspAdressMuteCh2(muteCh2),
              dspAdressSlewMCh2(slewModeCh2)
        {
            hasChanged = false;
        }
        bool getHasChanged() const;
        void setHasChanged(bool value);

        uint8_t getMute() const;
        void setMute(uint8_t value);

        uint16_t getSlewMode() const;
        void setSlewMode(uint16_t value);

        uint16_t getDSPAdressMuteCh1() const;

        uint16_t getDSPAdressSlewMCh1() const;

        uint16_t getDSPAdressMuteCh2() const;

        uint16_t getDSPAdressSlewMCh2() const;

        uint8_t getParameterCount() const;

        void writeRegister();
    };

    class Safeload
    {
    private:
        const uint16_t dspAdressSafeloadData0;  // Addresse für Data im Safeload
        const uint16_t dspAdressSafeloadData1;  // Addresse für Data im Safeload
        const uint16_t dspAdressSafeloadData2;  // Addresse für Data im Safeload
        const uint16_t dspAdressSafeloadData3;  // Addresse für Data im Safeload
        const uint16_t dspAdressSafeloadData4;  // Addresse für Data im Safeload
        const uint16_t dspAdressSafeloadAdress; // Addresse für Adresse des Ziels im Safeload
        const uint16_t dspAdressSafeloadNumber; // Number of Data + Trigger
        const unsigned long minAudioFrameDurationMicros = DSP_SAMPLING_T;
        unsigned long lastSafeloadMicros = 0;

    public:
        Safeload(
            uint16_t dspAdressSafeloadData0, uint16_t dspAdressSafeloadData1,
            uint16_t dspAdressSafeloadData2, uint16_t dspAdressSafeloadData3,
            uint16_t dspAdressSafeloadData4, uint16_t dspAdressSafeloadAdress,
            uint16_t dspAdressSafeloadNumber)
            : dspAdressSafeloadData0(dspAdressSafeloadData0),
              dspAdressSafeloadData1(dspAdressSafeloadData1),
              dspAdressSafeloadData2(dspAdressSafeloadData2),
              dspAdressSafeloadData3(dspAdressSafeloadData3),
              dspAdressSafeloadData4(dspAdressSafeloadData4),
              dspAdressSafeloadAdress(dspAdressSafeloadAdress),
              dspAdressSafeloadNumber(dspAdressSafeloadNumber)
        {
        }

        uint16_t getDSPAdressSafeloadData0() const;

        uint16_t getDSPAdressSafeloadData1() const;

        uint16_t getDSPAdressSafeloadData2() const;

        uint16_t getDSPAdressSafeloadData3() const;

        uint16_t getDSPAdressSafeloadData4() const;

        uint16_t getDSPAdressSafeloadAdress() const;

        uint16_t getDSPAdressSafeloadNumber() const;

        bool checkSafeloadAudioFrameOver();
    };

    class Gain
    {
    private:
        bool hasChanged;
        float gainLinear;
        float gaindB;
        uint16_t slewMode = 8330;
        uint8_t parameterCount = 1;
        // Adresses
        const uint16_t dspAdressGainCh1;
        const uint16_t dspAdressSlewMCh1;
        const uint16_t dspAdressGainCh2;
        const uint16_t dspAdressSlewMCh2;

    public:
        Gain(
            uint16_t gainCh1, uint16_t slewModeCh1,
            uint16_t gainCh2, uint16_t slewModeCh2)
            : dspAdressGainCh1(gainCh1),
              dspAdressSlewMCh1(slewModeCh1),
              dspAdressGainCh2(gainCh2),
              dspAdressSlewMCh2(slewModeCh2)
        {
            hasChanged = false;
            gainLinear = 0;
            gaindB = 0;
        }
        bool getHasChanged() const;
        void setHasChanged(bool value);

        float getGainLinear() const;
        void setGainLinear(float value);

        float getGaindB() const;
        void setGaindB(float value);

        uint16_t getSlewMode() const;
        void setSlewMode(uint16_t value);

        uint16_t getDSPAdressGainCh1() const;

        uint16_t getDSPAdressSlewMCh1() const;

        uint16_t getDSPAdressGainCh2() const;

        uint16_t getDSPAdressSlewMCh2() const;

        uint8_t getParameterCount() const;

        void writeSafeloadRegister();
    };

    // Singleton-Zugriff
    static GlobalData *getInstance();

    // Zugriff auf die Objekte
    Filter *getFilter1Ptr();
    Filter *getFilter2Ptr();
    Filter *getFilter3Ptr();
    Filter *getFilter4Ptr();
    Meter *getMeterPtr();
    Mute *getMute1Ptr();
    Safeload *getSafeloadPtr();
    Gain *getGainPrePtr();
    Gain *getGainPostPtr();

private:
    GlobalData(); // Konstruktor privat

    // Instanzen der verschachtelten Klassen
    Safeload safeload;
    Filter filter1;
    Filter filter2;
    Filter filter3;
    Filter filter4;
    Meter meter;
    Mute mute1;
    Gain gainPost;
    Gain gainPre;
};

extern GlobalData *globalData; // Declare a pointer to the GlobalData class to access global data

void mountSPIFFS(void);

void setupAccesPoint(void);
std::unique_ptr<char[]> fileHandlerReadSPIFFS(const String &path);
String readFromJSON(const String &path, const String &jsonKey);

#endif

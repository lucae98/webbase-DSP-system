#include "DSPSignals.h"
#include "IO.h" // Include the My IO library to handle gloabl data

// Define SPI Settings
SPISettings spiSettings(2000000, MSBFIRST, SPI_MODE3);

// Check for Updates an Write to DSP Register
void handleNewDatafromWebsite()
{

    if (globalData != nullptr)
    {
        // Check all Flags

        // Check Mute
        auto *mute1 = globalData->getMute1Ptr(); // Pointer to the mute class
        if (mute1 != nullptr)
        {
            if (mute1->getHasChanged() == true) // Check if the flag is set for the mute
            {
                Serial.println("Mute 1 has changed New Value: " + String(mute1->getMute())); // Print the new value of the mute
                // dspWriteRegister_8_24(mute1->getDSPAdressMuteCh1(), static_cast<uint32_t>((mute1->getMute())));         // Write the mute value to the DSP register Ch1
                // dspWriteRegisterFixPoint(mute1->getDSPAdressSlewMCh1(), static_cast<uint32_t>((mute1->getSlewMode()))); // Write the SlewMode value to the DSP register Ch1
                // dspWriteRegister_8_24(mute1->getDSPAdressMuteCh2(), static_cast<uint32_t>((mute1->getMute())));         // Write the mute value to the DSP register Ch2
                // dspWriteRegisterFixPoint(mute1->getDSPAdressSlewMCh2(), static_cast<uint32_t>((mute1->getSlewMode()))); // Write the SlewMode value to the DSP register Ch2
                mute1->writeRegister();
                mute1->setHasChanged(false); // Set the flag to false after reading the value
            }
        }
        else
        {
            // Serverbekommt davon nichts mit
            Serial.println("Ptr_mute1 is null from handleNewDatafromWebsite");
        }
        // Check Filter1
        auto *filter1 = globalData->getFilter1Ptr(); // Pointer to filter class
        if (filter1 != nullptr)
        {
            if (filter1->getHasChanged() == true)
            {
                Serial.println("Filter 1 has changed New Values: Frequenz: " + String(filter1->getFreq()) + " Gain: " + String(filter1->getGain()) + " Q: " + String(filter1->getQ()));
                calculateFilterCoefficients(*filter1);
                /*  dspWriteRegisterFixPoint(filter1->getDSPAdressSlewModeCh1(), filter1->getSlewMode());
                 dspWriteRegister_8_24(filter1->getDSPAdressA1Ch1(), filter1->getA1());
                 dspWriteRegister_8_24(filter1->getDSPAdressA2Ch1(), filter1->getA2());
                 dspWriteRegister_8_24(filter1->getDSPAdressB0Ch1(), filter1->getB0());
                 dspWriteRegister_8_24(filter1->getDSPAdressB1Ch1(), filter1->getB1());
                 dspWriteRegister_8_24(filter1->getDSPAdressB2Ch1(), filter1->getB2());
                 dspWriteRegisterFixPoint(filter1->getDSPAdressSlewModeCh2(), filter1->getSlewMode());
                 dspWriteRegister_8_24(filter1->getDSPAdressA1Ch2(), filter1->getA1());
                 dspWriteRegister_8_24(filter1->getDSPAdressA2Ch2(), filter1->getA2());
                 dspWriteRegister_8_24(filter1->getDSPAdressB0Ch2(), filter1->getB0());
                 dspWriteRegister_8_24(filter1->getDSPAdressB1Ch2(), filter1->getB1());
                 dspWriteRegister_8_24(filter1->getDSPAdressB2Ch2(), filter1->getB2()); */
                filter1->writeSafeloadRegister();
                filter1->setHasChanged(false);
            }
            if (filter1->getHasChangedBypassed() == true && filter1->getBypass() == 1)
            {
                Serial.println("Bypass 1 has changed New Value: " + String(filter1->getBypass()));
                // Set all Coeffizents to 0 exepct B0
                filter1->writeBypassedSafeloadRegister();
                filter1->setHasChangedBypassed(false);
            }
            else if (filter1->getHasChangedBypassed() == true && filter1->getBypass() == 0)
            {
                Serial.println("Bypass 1 has changed New Value: " + String(filter1->getBypass()));
                // Write The Old Coeffizents
                filter1->writeSafeloadRegister();
                filter1->setHasChangedBypassed(false);
            }
        }
        else
        {
            // Serverbekommt davon nichts mit
            Serial.println("Filter1 Ptr is null from handleNewDatafromWebsite");
        }

        // Check Filter2
        auto *filter2 = globalData->getFilter2Ptr(); // Pointer to filter class
        if (filter2 != nullptr)
        {
            if (filter2->getHasChanged() == true)
            {
                Serial.println("Filter 2 has changed New Values: Frequenz: " + String(filter2->getFreq()) + " Gain: " + String(filter2->getGain()) + " Q: " + String(filter2->getQ()));
                calculateFilterCoefficients(*filter2);
                /*  dspWriteRegisterFixPoint(filter1->getDSPAdressSlewModeCh1(), filter1->getSlewMode());
                 dspWriteRegister_8_24(filter1->getDSPAdressA1Ch1(), filter1->getA1());
                 dspWriteRegister_8_24(filter1->getDSPAdressA2Ch1(), filter1->getA2());
                 dspWriteRegister_8_24(filter1->getDSPAdressB0Ch1(), filter1->getB0());
                 dspWriteRegister_8_24(filter1->getDSPAdressB1Ch1(), filter1->getB1());
                 dspWriteRegister_8_24(filter1->getDSPAdressB2Ch1(), filter1->getB2());
                 dspWriteRegisterFixPoint(filter1->getDSPAdressSlewModeCh2(), filter1->getSlewMode());
                 dspWriteRegister_8_24(filter1->getDSPAdressA1Ch2(), filter1->getA1());
                 dspWriteRegister_8_24(filter1->getDSPAdressA2Ch2(), filter1->getA2());
                 dspWriteRegister_8_24(filter1->getDSPAdressB0Ch2(), filter1->getB0());
                 dspWriteRegister_8_24(filter1->getDSPAdressB1Ch2(), filter1->getB1());
                 dspWriteRegister_8_24(filter1->getDSPAdressB2Ch2(), filter1->getB2()); */
                filter2->writeSafeloadRegister();
                filter2->setHasChanged(false);
            }
            if (filter2->getHasChangedBypassed() == true && filter2->getBypass() == 1)
            {
                Serial.println("Bypass 2 has changed New Value: " + String(filter2->getBypass()));
                // Set all Coeffizents to 0 exepct B0
                filter2->writeBypassedSafeloadRegister();
                filter2->setHasChangedBypassed(false);
            }
            else if (filter2->getHasChangedBypassed() == true && filter2->getBypass() == 0)
            {
                Serial.println("Bypass 2 has changed New Value: " + String(filter2->getBypass()));
                // Write The Old Coeffizents
                filter2->writeSafeloadRegister();
                filter2->setHasChangedBypassed(false);
            }
        }
        else
        {
            // Serverbekommt davon nichts mit
            Serial.println("Filter2 Ptr is null from handleNewDatafromWebsite");
        }

        // Check Filter3
        auto *filter3 = globalData->getFilter3Ptr(); // Pointer to filter class
        if (filter3 != nullptr)
        {
            if (filter3->getHasChanged() == true)
            {
                Serial.println("Filter 3 has changed New Values: Frequenz: " + String(filter3->getFreq()) + " Gain: " + String(filter3->getGain()) + " Q: " + String(filter3->getQ()));
                calculateFilterCoefficients(*filter3);
                /*  dspWriteRegisterFixPoint(filter1->getDSPAdressSlewModeCh1(), filter1->getSlewMode());
                 dspWriteRegister_8_24(filter1->getDSPAdressA1Ch1(), filter1->getA1());
                 dspWriteRegister_8_24(filter1->getDSPAdressA2Ch1(), filter1->getA2());
                 dspWriteRegister_8_24(filter1->getDSPAdressB0Ch1(), filter1->getB0());
                 dspWriteRegister_8_24(filter1->getDSPAdressB1Ch1(), filter1->getB1());
                 dspWriteRegister_8_24(filter1->getDSPAdressB2Ch1(), filter1->getB2());
                 dspWriteRegisterFixPoint(filter1->getDSPAdressSlewModeCh2(), filter1->getSlewMode());
                 dspWriteRegister_8_24(filter1->getDSPAdressA1Ch2(), filter1->getA1());
                 dspWriteRegister_8_24(filter1->getDSPAdressA2Ch2(), filter1->getA2());
                 dspWriteRegister_8_24(filter1->getDSPAdressB0Ch2(), filter1->getB0());
                 dspWriteRegister_8_24(filter1->getDSPAdressB1Ch2(), filter1->getB1());
                 dspWriteRegister_8_24(filter1->getDSPAdressB2Ch2(), filter1->getB2()); */
                filter3->writeSafeloadRegister();
                filter3->setHasChanged(false);
            }
            if (filter3->getHasChangedBypassed() == true && filter3->getBypass() == 1)
            {
                Serial.println("Bypass 3 has changed New Value: " + String(filter3->getBypass()));
                // Set all Coeffizents to 0 exepct B0
                filter3->writeBypassedSafeloadRegister();
                filter3->setHasChangedBypassed(false);
            }
            else if (filter3->getHasChangedBypassed() == true && filter3->getBypass() == 0)
            {
                Serial.println("Bypass 3 has changed New Value: " + String(filter3->getBypass()));
                // Write The Old Coeffizents
                filter3->writeSafeloadRegister();
                filter3->setHasChangedBypassed(false);
            }
        }
        else
        {
            // Serverbekommt davon nichts mit
            Serial.println("Filter3 Ptr is null from handleNewDatafromWebsite");
        }

        // Check Filter4
        auto *filter4 = globalData->getFilter4Ptr(); // Pointer to filter class
        if (filter4 != nullptr)
        {
            if (filter4->getHasChanged() == true)
            {
                Serial.println("Filter 4 has changed New Values: Frequenz: " + String(filter4->getFreq()) + " Gain: " + String(filter4->getGain()) + " Q: " + String(filter4->getQ()));
                calculateFilterCoefficients(*filter4);
                /*  dspWriteRegisterFixPoint(filter1->getDSPAdressSlewModeCh1(), filter1->getSlewMode());
                 dspWriteRegister_8_24(filter1->getDSPAdressA1Ch1(), filter1->getA1());
                 dspWriteRegister_8_24(filter1->getDSPAdressA2Ch1(), filter1->getA2());
                 dspWriteRegister_8_24(filter1->getDSPAdressB0Ch1(), filter1->getB0());
                 dspWriteRegister_8_24(filter1->getDSPAdressB1Ch1(), filter1->getB1());
                 dspWriteRegister_8_24(filter1->getDSPAdressB2Ch1(), filter1->getB2());
                 dspWriteRegisterFixPoint(filter1->getDSPAdressSlewModeCh2(), filter1->getSlewMode());
                 dspWriteRegister_8_24(filter1->getDSPAdressA1Ch2(), filter1->getA1());
                 dspWriteRegister_8_24(filter1->getDSPAdressA2Ch2(), filter1->getA2());
                 dspWriteRegister_8_24(filter1->getDSPAdressB0Ch2(), filter1->getB0());
                 dspWriteRegister_8_24(filter1->getDSPAdressB1Ch2(), filter1->getB1());
                 dspWriteRegister_8_24(filter1->getDSPAdressB2Ch2(), filter1->getB2()); */
                filter4->writeSafeloadRegister();
                filter4->setHasChanged(false);
            }
            if (filter4->getHasChangedBypassed() == true && filter4->getBypass() == 1)
            {
                Serial.println("Bypass 4 has changed New Value: " + String(filter4->getBypass()));
                // Set all Coeffizents to 0 exepct B0
                filter4->writeBypassedSafeloadRegister();
                filter4->setHasChangedBypassed(false);
            }
            else if (filter4->getHasChangedBypassed() == true && filter4->getBypass() == 0)
            {
                Serial.println("Bypass 4 has changed New Value: " + String(filter4->getBypass()));
                // Write The Old Coeffizents
                filter4->writeSafeloadRegister();
                filter4->setHasChangedBypassed(false);
            }
        }
        else
        {
            // Serverbekommt davon nichts mit
            Serial.println("Filter4 Ptr is null from handleNewDatafromWebsite");
        }

        // Check PreGain
        auto *preGain = globalData->getGainPrePtr(); // Pointer to Gain class
        if (preGain != nullptr)
        {
            if (preGain->getHasChanged() == true)
            {
                Serial.println("PreGain has changed New Value Gain: " + String(preGain->getGaindB()));
                preGain->writeSafeloadRegister();
                preGain->setHasChanged(false);
            }
        }
        else
        {
            // Serverbekommt davon nichts mit
            Serial.println("preGain Ptr is null from handleNewDatafromWebsite");
        }

        // Check PostGain
        auto *postGain = globalData->getGainPostPtr(); // Pointer to Gain class
        if (postGain != nullptr)
        {
            if (postGain->getHasChanged() == true)
            {
                Serial.println("PostGain has changed New Value Gain: " + String(postGain->getGaindB()));
                postGain->writeSafeloadRegister();
                postGain->setHasChanged(false);
            }
        }
        else
        {
            // Serverbekommt davon nichts mit
            Serial.println("PostGain Ptr is null from handleNewDatafromWebsite");
        }
    }
    else
    {
        Serial.println("globalData is null from handleNewDatafromWebsite");
        // Server send fehlt -->
    }
}

void calculateFilterCoefficients(GlobalData::Filter &filter) // Calculate filter coefficients form frequency, gain and q
{
    // Filter Calculations from https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

    // check start time for performance measurement
    // unsigned long starttime = micros(); // Start time for performance measurement

    // get the filter parameters from the filter class
    float freq = filter.getFreq();                 // Frequency of the filter
    float gain = filter.getGain();                 // Gain of the filter
    float q = filter.getQ();                       // Q factor of the filter
    float samplingFreq = filter.getSamplingFreq(); // Sampling frequency of the filter

    // Serial.println("Filter Coefficients Calculation: ");
    // Serial.println("Freq: " + String(freq) + "Hz ");                      // Print the frequency for debugging
    // Serial.println("Gain: " + String(gain) + "dB");                       // Print the gain for debugging
    // Serial.println("Q: " + String(q));                                    // Print the Q factor for debugging
    // Serial.println("Sampling Frequency: " + String(samplingFreq) + "Hz"); // Print the sampling frequency for debugging

    // Set helpvalues for filter coefficients calculation
    double omega = (2 * PI * freq) / samplingFreq; // Normalized frequency
    double A = pow(10, (gain / 40));               // Convert gain from dB to linear scale
    double alpha = (sin(omega)) / (2 * q);         // Calculate alpha for the filter design
    double G = pow(10, (gain / 40));               // linear Gain G ist Sqrt(A)
    // Serial.println("PI: " + String(PI, 6));                 // Print the value of PI for debugging
    // Serial.println("Omega: " + String(omega, 6));           // Print the normalized frequency for debugging
    // Serial.println("A: " + String(A, 6));                   // Print the gain in linear scale for debugging
    // Serial.println("Alpha: " + String(alpha, 6));           // Print the alpha value for debugging
    // Serial.println("Cos(omega): " + String(cos(omega), 6)); // Print the cosine of omega for debugging
    // Serial.println("Sin(omega): " + String(sin(omega), 6)); // Print the sine of omega for debugging

    double a0, a1, a2, b0, b1, b2; // Coefficients for the filter design
    a0 = 1 + alpha / A;
    a1 = -2 * cos(omega);
    a2 = 1 - alpha / A;
    b0 = 1 + alpha * A;
    b1 = -2 * cos(omega);
    b2 = 1 - alpha * A;

    // Serial.println("b0: " + String(b0, 6)); // Print the b0 coefficient for debugging
    // Serial.println("b1: " + String(b1, 6)); // Print the b1 coefficient for debugging
    // Serial.println("b2: " + String(b2, 6)); // Print the b2 coefficient for debugging
    // Serial.println("a0: " + String(a0, 6)); // Print the a0 coefficient for debugging
    // Serial.println("a1: " + String(a1, 6)); // Print the a1 coefficient for debugging
    // Serial.println("a2: " + String(a2, 6)); // Print the a2 coefficient for debugging

    // Normalize the coefficients
    double a1_norm, a2_norm, b0_norm, b1_norm, b2_norm;
    b0_norm = b0 / a0; // Normalize b0
    b1_norm = b1 / a0; // Normalize b1
    b2_norm = b2 / a0; // Normalize b2
                       // a1_norm = a1 / a0; // Normalize a1
                       // a2_norm = a2 / a0; // Normalize a2

    a1_norm = -1 * (a1 / a0); // Normalize a1 and change sign for MAC DSP operation
    a2_norm = -1 * (a2 / a0); // Normalize a2 and change sign for MAC DSP operation

    // Serial.println("b0_norm: " + String(b0_norm, 6)); // Print the normalized b0 coefficient for debugging
    // Serial.println("b1_norm: " + String(b1_norm, 6)); // Print the normalized b1 coefficient for debugging
    // Serial.println("b2_norm: " + String(b2_norm, 6)); // Print the normalized b2 coefficient for debugging
    // Serial.println("a1_norm: " + String(a1_norm, 6)); // Print the normalized a1 coefficient for debugging
    // Serial.println("a2_norm: " + String(a2_norm, 6)); // Print the normalized a2 coefficient for debugging

    // check end time for performance measurement
    unsigned long endtime = micros();
    // calculate the time taken for the calculation and print

    // write the coeefficients to global data class
    filter.setB0(b0_norm); // Set the normalized b0 coefficient in the filter struct
    filter.setB1(b1_norm); // Set the normalized b1 coefficient in the filter struct
    filter.setB2(b2_norm); // Set the normalized b2 coefficient in the filter struct
    filter.setA1(a1_norm); // Set the normalized a1 coefficient in the filter struct
    filter.setA2(a2_norm); // Set the normalized a2 coefficient in the filter struct

    // Serial.println("Filter Coefficients Calculation Time: " + String((endtime - starttime)) + "µs"); // Print the time taken for the calculation
    // Serial.println("A0: 1 ");                                                                        // Print the A0 coefficient for debugging
    // Serial.println("A1: " + String(filter.getA1(), 7));                                             // Print the A1 coefficient for debugging
    // Serial.println("A2: " + String(filter.getA2(), 7));                                             // Print the A2 coefficient for debugging
    // Serial.println("B0: " + String(filter.getB0(), 7));                                             // Print the B0 coefficient for debugging
    // Serial.println("B1: " + String(filter.getB1(), 7));                                             // Print the B1 coefficient for debugging
    // Serial.println("B2: " + String(filter.getB2(), 7));
    // Print the coefficients for debugging
}

void calculateFilterValues() // Calculate filter values from coefficients
{
    // Filter Values Calculation
    // Example: Butterworth filter values calculation
    // This is a placeholder function and should be replaced with actual filter design calculations
    // The values are calculated based on the filter coefficients and stored in the filter struct for further processing
}

// Initialize the SPI bus with the defined settings
void dspInitForSPI()
{
    // 2. SPI-Pins "neutral" DSP bei Sleboot nicht stören
    pinMode(SPI_CLK_PIN, INPUT);
    pinMode(SPI_MISO_PIN, INPUT);
    pinMode(SPI_MOSI_PIN, INPUT);
    pinMode(SPI_CS_PIN, INPUT);

    delay(500); // Warten bis Selfboot fertig

    // laut datenblatt fehlt das hier:
    pinMode(SPI_CS_PIN, OUTPUT);

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(SPI_CS_PIN, LOW);
        delayMicroseconds(1); // Mindestdauer ≥ 1 Master-Takt 1/2000000 sind 0.5 mikrosekunden
        digitalWrite(SPI_CS_PIN, HIGH);
        delayMicroseconds(1);
    }

    SPI.begin(SPI_CLK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CS_PIN); // SPI Pins
    pinMode(SPI_CS_PIN, OUTPUT);                                    // set Chip Select Pin as OUTPUT
                                                                    /*   SPI.beginTransaction(spiSettings);                              // start SPI transaction
                                                                      digitalWrite(SPI_CS_PIN, LOW);                                  // Set Chip Select LOW to start communication
                                                                
                                                                      // Set Adau to SPI Mode --> Data Sheet
                                                                      SPI.transfer(0x00);
                                                                      SPI.transfer(0x00);
                                                                      SPI.transfer(0x00);
                                                                
                                                                      digitalWrite(SPI_CS_PIN, HIGH); // Set Chip Select HIGH
                                                                      SPI.endTransaction();           // end SPI transaction */
}
// Write to DSP register in 8.24 format
void dspWriteRegister_8_24(uint16_t const address, float const data)
{
    // convert float to fixed_8_24 format
    fixed_8_24 data_8_24 = floatToFixed8_24(data); // Convert float to fixed_8_24 format

    // initialize Arrays
    uint8_t devAddressWrite = DSP_ADDR_WRITE; // Write Mode Device Address to DSP for Read or Write Mode
    uint8_t byteArrayAddress[DSP_BYTES_ADDR]; // Address Array for Register Address
    uint8_t byteArrayData[DSP_BYTES_DATA];    // Data Array for Data

    // Write Address in Array MSB First
    byteArrayAddress[0] = (address >> 8) & 0xFF; // Bytes Arrage MSB First in Array [0]
    byteArrayAddress[1] = address & 0xFF;        // Bytes Arrange LSB Second in Array [1]

    // Write Data in Array MSB First
    byteArrayData[3] = data_8_24 & 0xFF;         // LSB to Array [3]
    byteArrayData[2] = (data_8_24 >> 8) & 0xFF;  // Second LSB to Array [2]
    byteArrayData[1] = (data_8_24 >> 16) & 0xFF; // Second MSB to Array [1]
    byteArrayData[0] = (data_8_24 >> 24) & 0xFF; // MSB to Array [0]

    digitalWrite(SPI_CS_PIN, LOW);     // Set Chip Select LOW to start communication
    SPI.beginTransaction(spiSettings); // set SPI settings and start transaction

    // Start SPI Communication in strict order
    SPI.transfer(devAddressWrite); // Send 1 Byte Device Address --> set to Write Mode
    // Serial.printf("8-Bit-Device Adresse: 0x%02X\n", devAddressWrite);

    // Send the Register Address 2 Byte MSB First
    SPI.transfer(byteArrayAddress[0]);
    // Serial.printf("16-Bit-Adresse: 0x%02X", byteArrayAddress[0]);
    SPI.transfer(byteArrayAddress[1]);
    // Serial.printf("%02X\n", byteArrayAddress[1]);

    // Send the Data 4 Byte MSB First
    SPI.transfer(byteArrayData[0]);
    // Serial.printf("32-Bit-Data: 0x%02X", byteArrayData[0]);
    SPI.transfer(byteArrayData[1]);
    // Serial.printf("%02X", byteArrayData[1]);
    SPI.transfer(byteArrayData[2]);
    // Serial.printf("%02X", byteArrayData[2]);
    SPI.transfer(byteArrayData[3]);
    // Serial.printf("%02X\n", byteArrayData[3]);

    digitalWrite(SPI_CS_PIN, HIGH); // Deactivate the Chip Select end communication
    SPI.endTransaction();           // end SPI transaction
}

void dspWriteRegisterFixPoint(uint16_t const address, uint32_t const data)
{
    // initialize Arrays
    uint8_t devAddressWrite = DSP_ADDR_WRITE; // Write Mode Device Address to DSP for Read or Write Mode
    uint8_t byteArrayAddress[DSP_BYTES_ADDR]; // Address Array for Register Address
    uint8_t byteArrayData[DSP_BYTES_DATA];    // Data Array for Data

    // Write Address in Array MSB First
    byteArrayAddress[0] = (address >> 8) & 0xFF; // Bytes Arrage MSB First in Array [0]
    byteArrayAddress[1] = address & 0xFF;        // Bytes Arrange LSB Second in Array [1]

    // Write Data in Array MSB First
    byteArrayData[3] = data & 0xFF;         // LSB to Array [3]
    byteArrayData[2] = (data >> 8) & 0xFF;  // Second LSB to Array [2]
    byteArrayData[1] = (data >> 16) & 0xFF; // Second MSB to Array [1]
    byteArrayData[0] = (data >> 24) & 0xFF; // MSB to Array [0]

    digitalWrite(SPI_CS_PIN, LOW);     // Set Chip Select LOW to start communication
    SPI.beginTransaction(spiSettings); // set SPI settings and start transaction

    // Start SPI Communication in strict order
    SPI.transfer(devAddressWrite); // Send 1 Byte Device Address --> set to Write Mode

    // Send the Register Address 2 Byte MSB First
    SPI.transfer(byteArrayAddress[0]);
    // Serial.printf("16-Bit-Adresse: 0x%02X", byteArrayAddress[0]);
    SPI.transfer(byteArrayAddress[1]);
    // Serial.printf("%02X\n", byteArrayAddress[1]);

    // Send the Data 4 Byte MSB First
    SPI.transfer(byteArrayData[0]);
    // Serial.printf("32-Bit-Data: 0x%02X", byteArrayData[0]);
    SPI.transfer(byteArrayData[1]);
    // Serial.printf("%02X", byteArrayData[1]);
    SPI.transfer(byteArrayData[2]);
    // Serial.printf("%02X", byteArrayData[2]);
    SPI.transfer(byteArrayData[3]);
    //  Serial.printf("%02X\n", byteArrayData[3]);

    digitalWrite(SPI_CS_PIN, HIGH); // Deactivate the Chip Select end communication
    SPI.endTransaction();           // end SPI transaction
}

// write Safeloadregister
void dspWriteSafeloadAdressRegister(uint16_t addressRegister)
{
    auto *safeload = globalData->getSafeloadPtr();

    if (safeload == nullptr)
    {
        Serial.println("safeloadpointer is null! function:dspWriteSafeloadAdressRegister ");
        return;
    }
    // initialize Arrays
    uint8_t devAddressWrite = DSP_ADDR_WRITE;        // Write Mode Device Address to DSP for Read or Write Mode
    uint8_t byteArrayAddress[DSP_BYTES_DATA];        // Address Array for Register Address
    uint8_t byteArraySafeloadAdress[DSP_BYTES_ADDR]; // Address Array for Safeload Register Address

    // Write Address in Array MSB First
    byteArraySafeloadAdress[0] = (safeload->getDSPAdressSafeloadAdress() >> 8) & 0xFF; // Bytes Arrage MSB First in Array [0]
    byteArraySafeloadAdress[1] = (safeload->getDSPAdressSafeloadAdress()) & 0xFF;      // Bytes Arrange LSB Second in Array [1]

    // Write Address in Array MSB First
    byteArrayAddress[0] = 0x00;                          // Write Dummy Bytes for the 4 Byte Register
    byteArrayAddress[1] = 0x00;                          // Write Dummy Bytes
    byteArrayAddress[2] = (addressRegister >> 8) & 0xFF; // Bytes Arrage MSB First in Array [0]
    byteArrayAddress[3] = addressRegister & 0xFF;        // Bytes Arrange LSB Second in Array [1]

    digitalWrite(SPI_CS_PIN, LOW);     // Set Chip Select LOW to start communication
    SPI.beginTransaction(spiSettings); // set SPI settings and start transaction

    // Start SPI Communication in strict order
    SPI.transfer(devAddressWrite); // Send 1 Byte Device Address --> set to Write Mode
    // Serial.printf("8-Bit-Device Adresse: 0x%02X\n", devAddressWrite);

    // Send the Safeload Trigger Address 2 Byte MSB First
    SPI.transfer(byteArraySafeloadAdress[0]);
    // Serial.printf("16-Bit Safeload Adresse: 0x%02X", byteArraySafeloadAdress[0]);
    SPI.transfer(byteArraySafeloadAdress[1]);
    // Serial.printf("%02X\n", byteArraySafeloadAdress[1]);

    // Send the Register Address 4 Byte MSB First
    SPI.transfer(byteArrayAddress[0]);
    //  Serial.printf("32-Bit-Adress with Dummy Bytes DSP RAM: 0x%02X", byteArrayAddress[0]);
    SPI.transfer(byteArrayAddress[1]);
    // Serial.printf("%02X", byteArrayAddress[1]);
    SPI.transfer(byteArrayAddress[2]);
    // Serial.printf("%02X", byteArrayAddress[2]);
    SPI.transfer(byteArrayAddress[3]);
    // Serial.printf("%02X\n", byteArrayAddress[3]);

    digitalWrite(SPI_CS_PIN, HIGH); // Deactivate the Chip Select end communication
    SPI.endTransaction();           // end SPI transaction
}

// Read from DSP register
float dspReadRegister(uint16_t const address)
{
    // initialize Arrays
    uint8_t devAddressRead = DSP_ADDR_READ;   // Write Mode Device Address to DSP for Read or Write Mode
    uint8_t byteArrayAddress[DSP_BYTES_ADDR]; // Address Array for Register Address
    uint8_t byteArrayData[DSP_BYTES_DATA];    // Data Array for Data

    // Write Address in Array MSB First
    byteArrayAddress[0] = (address >> 8) & 0xFF; // Bytes Arrage MSB First in Array [0]
    byteArrayAddress[1] = address & 0xFF;        // Bytes Arrange LSB Second in Array [1]

    digitalWrite(SPI_CS_PIN, LOW);     // Set Chip Select LOW to start communication
    SPI.beginTransaction(spiSettings); // set SPI settings and start transaction

    // Start SPI Communication in strict order

    SPI.transfer(devAddressRead); // Send 1 Byte Device Address --> set to Read Mode

    // Send the Register Address 2 Byte MSB First
    SPI.transfer(byteArrayAddress[0]);
    SPI.transfer(byteArrayAddress[1]);

    // Read data 4 Byte MSB First

    // revceive data from DSP
    byteArrayData[0] = SPI.transfer(0x00); // Send Dummy Byte to receive data MSB First to Array [0]
    byteArrayData[1] = SPI.transfer(0x00); // Send Dummy Byte to receive data MSB Second to Array [1]
    byteArrayData[2] = SPI.transfer(0x00); // Send Dummy Byte to receive data LSB Second to Array [2]
    byteArrayData[3] = SPI.transfer(0x00); // Send Dummy Byte to receive data LSB First to Array [3]

    digitalWrite(SPI_CS_PIN, HIGH); // Deactivate the Chip Select end communication
    SPI.endTransaction();           // end SPI transaction

    /* byteArrayData[0] = 0x00;
    byteArrayData[1] = 0x00;
    byteArrayData[2] = 0x80;
    byteArrayData[3] = 0x00; */

    /* Serial.println("");
    for (int i = 0; i < 4; i++)
    {
        Serial.printf("%02X", byteArrayData[i]);
    }
    Serial.println(""); */

    fixed_8_24 data_8_24 = 0; // Initialize the fixed_8_24 variable to store the converted value

    // Convert the byte array to fixed_8_24 format with OR operation no Overwriting from LSB to MSB
    data_8_24 = data_8_24 | byteArrayData[3];         // LSB First Byte not shifted
    data_8_24 = data_8_24 | (byteArrayData[2] << 8);  // Second LSB Byte shifted 8 Bit to the left
    data_8_24 = data_8_24 | (byteArrayData[1] << 16); // Second MSB Byte shifted 16 Bit to the left
    data_8_24 = data_8_24 | (byteArrayData[0] << 24); // MSB Byte shifted 24 Bit to the left

    // Serial.printf("%08X\n", data_8_24);

    // Serial.println(fixed_8_24_to_float(data_8_24));

    return (fixed8_24ToFloat(data_8_24)); // Convert to Float and return
}
// Convert float to 8.24 Format
fixed_8_24 floatToFixed8_24(float value)
{
    // Check for overflow and underflow for the 8.24 format
    if (value < -128.0f)
        value = -128.0f;
    if (value > 127.999984f)
        value = 127.999984f;

    // Convert the float value to fixed_8_24 format with 2^24 scaling
    // The value is multiplied by 2^24 to shift the decimal point 24 bits to the right
    int32_t fixed_value = (int32_t)(value * TWO_POW_24);

    return fixed_value;
}
// Convert fixed_8_24 to float
float fixed8_24ToFloat(fixed_8_24 value)
{
    // Extract the integer part (upper 8 bits) and fractional part (lower 24 bits)
    int32_t integer_part = (value >> 24) & 0xFF; // Upper 8 bits
    if (integer_part & 0x80)                     // and operation with 0x80 (10000000) to check for negative value sign bit
    {                                            // if the sign bit is set, it indicates a negative value
        integer_part = integer_part - 0x100;     // Subtract 256 (0x100) to get the correct negative value
    }

    // Extract the fractional part (lower 24 bits)
    uint32_t fractional_part = value & 0xFFFFFF; // Lower 24 bits with and operation with 0xFFFFFF to get the fractional part

    // Use type casting to convert the fractional part to float and divide by 2^24 to get the correct value revers to value * TWO_POW_24
    float fractional_float = (float)fractional_part / (float)(1 << 24);

    // Combine the integer and fractional parts to get the final float value
    return (float)integer_part + fractional_float;
}
// map frequenz to Nyquist
float mapFrequenz(float freq)
{
    return freq / DSP_NYQUIST_LIMIT;
}
// unmap frequenz to Nyquist
float unmapFrequenz(float value)
{

    return value * DSP_NYQUIST_LIMIT;
}
// Convert dB to float
float dBToFloat(float dB)
{
    return pow(10, dB / 20);
}
// Convert float to dB
float floatTodB(float value)
{
    return 20 * log10(value);
}
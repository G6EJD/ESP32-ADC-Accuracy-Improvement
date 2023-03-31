#include "esp_adc_cal.h"

#define ADC_PIN     36

void setup() {
  Serial.begin(115200);
}

void loop() {
  float unadjusted_voltage = (analogRead(ADC_PIN) / 4095.0 * 3.3);
  float adjusted_voltage   = ReadVoltage(ADC_PIN);
  
  Serial.println("      Input Voltage = 3.000 v");
  Serial.println("------------------------------");
  Serial.println("   Adjusted Voltage = " + String(adjusted_voltage, 3) + "v  " + String(adjusted_voltage / 3.00 * 100 - 100) + "% error");
  Serial.println("Un-adjusted Voltage = " + String(unadjusted_voltage, 3) + "v  " + String(unadjusted_voltage / 3.00 * 100 - 100) + "% error");
  Serial.println();
  delay(5000);
}

float ReadVoltage(byte ADC_Pin) {
  float calibration  = 1.000; // Adjust for ultimate accuracy when input is measured using an accurate DVM, if reading too high then use e.g. 0.99, too low use 1.01
  float vref = 1100;
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  vref = adc_chars.vref; // Obtain the device ADC reference voltage
  return (analogRead(ADC_Pin) / 4095.0) * 3.3 * (1100 / vref) * calibration;  // ESP by design reference voltage in mV
}

// The esp_adc_cal/include/esp_adc_cal.h API provides functions to correct for differences
// in measured voltages caused by variation of ADC reference voltages (Vref) between chips.
// Per design the ADC reference voltage is 1100 mV, however the true reference voltage can
// range from 1000 mV to 1200 mV amongst different ESP32's

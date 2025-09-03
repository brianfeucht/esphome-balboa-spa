## Component for Balboa Spa

This project is based on the UART reader from [Dakoriki/ESPHome-Balboa-Spa](https://github.com/Dakoriki/ESPHome-Balboa-Spa)

There are a ton of these implementations on Github.  None of the ones I could find implemented the external component pattern as prescribed by EspHome.  So I create this one.  

Climate, binary sensors, sensors, and switches are all optional.  So you only need to import what you want with your implementation.

TODO:
I am seeing a ton of CRC errors when reading data on my spa.  This might be invalid UART config (baud, buffer, etc). Or it might just be due to the noisy nature of running next two heaters and pumps.

```
esphome:
  name: hottub
  friendly_name: hottub

esp32:
  board: lolin_s2_mini
  framework: 
    type: arduino

external_components:
  - source:
     type: git
     url: https://github.com/brianfeucht/esphome-balboa-spa
     ref: main

# API and Time required for Sync Spa Time Button. 
api:

time:
  - platform: homeassistant

uart:
  id: spa_uart_bus
  tx_pin: GPIO37
  rx_pin: GPIO39
  data_bits: 8
  parity: NONE
  stop_bits: 1
  baud_rate: 115200
  rx_buffer_size: 128

balboa_spa:
  id: spa
  # Set this to C or F based on the units your spa is configured for
  spa_temp_scale: F

switch:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet1:
      name: Jet1
    jet2:
      name: Jet2
    jet3:
      name: Jet3
    jet4:
      name: Jet4
    light:
      name: Lights
    blower:
      name: Blower

climate:
  - platform: balboa_spa
    balboa_spa_id: spa
    name: "Spa Thermostat"
    visual:
      min_temperature: 62 °F    # min: 7 C
      max_temperature: 105 °F    # max: 30 C
      temperature_step: 0.5 °F  # min: 0.5 C

sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    blower:
      name: Blower
    highrange:
      name: High Range
    circulation:
      name: Circulation
    restmode:
      name: Rest Mode
    heatstate:
      name: Heat State

binary_sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    blower:
      name: Blower
    highrange:
      name: High Range
    circulation:
      name: Circulation Pump
    restmode:
      name: Rest Mode
    heatstate:
      name: Heat State
    connected:
      name: Connected

text:
  - platform: balboa_spa
    balboa_spa_id: spa
    spa_time:
      name: "Set Spa Time"
      mode: TEXT
    filter1_config:
      name: "Set Filter 1 Config"
      mode: TEXT
    filter2_config:
      name: "Set Filter 2 Config"
      mode: TEXT

text_sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    spa_time:
      name: "Spa Time"
    filter1_config:
      name: "Filter 1 Config"
    filter2_config:
      name: "Filter 2 Config"

button:
  - platform: spa
    balboa_spa_id: test_spa
    sync_time:
      name: "Sync Spa Time"
```

## Text Components (Writable)

The text components allow you to set spa time and filter configurations:

- **spa_time**: Set the spa time in HH:MM format (24-hour format, e.g., "14:30")
- **filter1_config**: Set filter 1 configuration using JSON format: `{"start":"HH:MM","duration":"HH:MM"}`
- **filter2_config**: Set filter 2 configuration using JSON format or "disabled" to disable filter 2

### Examples:
- Set spa time to 2:30 PM: `14:30`
- Set filter 1 to start at 8:00 AM and run for 4 hours 30 minutes: `{"start":"08:00","duration":"04:30"}`
- Disable filter 2: `disabled`

All inputs are validated. Invalid formats will be rejected with error messages in the logs.

## Text Sensors (Read-only)

The text sensors display current spa status:

- **spa_time**: Current spa time in HH:MM format
- **filter1_config**: Current filter 1 configuration in JSON format
- **filter2_config**: Current filter 2 configuration in JSON format (or "disabled")

### ESP WebUI
![image](https://github.com/user-attachments/assets/af602be2-da9e-4880-8fb8-e7f7f9122977)

### Home Assistant UI
![image](https://github.com/user-attachments/assets/a37a7e08-94b2-4231-83ca-0ffc4646fbfa)

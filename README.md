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
    filter1_start_time:
      name: "Set Filter 1 Start Time"
      mode: TEXT
    filter1_duration:
      name: "Set Filter 1 Duration"
      mode: TEXT
    filter2_start_time:
      name: "Set Filter 2 Start Time"
      mode: TEXT
    filter2_duration:
      name: "Set Filter 2 Duration"
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
  - platform: balboa_spa
    balboa_spa_id: test_spa
    sync_time:
      name: "Sync Spa Time"
    disable_filter2:
      name: "Disable Filter 2"
```

## Text Components (Writable)

The text components allow you to set spa time and filter configurations using simple time formats. These components automatically display current values from the spa and update when changes are detected from the spa panel.

- **spa_time**: Set and view the spa time in H:MM or HH:MM format (24-hour format, e.g., "8:30" or "14:30")
- **filter1_start_time**: Set and view filter 1 start time in H:MM or HH:MM format
- **filter1_duration**: Set and view filter 1 duration in H:MM or HH:MM format  
- **filter2_start_time**: Set and view filter 2 start time in H:MM or HH:MM format
- **filter2_duration**: Set and view filter 2 duration in H:MM or HH:MM format

### Auto-Population from Spa
Text components automatically populate with current spa values:
- On startup, components display current spa time and filter settings
- When settings are changed from the spa panel, text components update automatically
- Values stay synchronized between ESPHome and the spa control panel

### Button Components

- **sync_time**: Synchronizes spa time with ESPHome system time
- **disable_filter2**: Disables the filter 2 schedule

### Examples:
- Set spa time to 2:30 PM: `14:30` or `2:30`
- Set filter 1 to start at 8:00 AM: `08:00` or `8:00`
- Set filter 1 to run for 4 hours 30 minutes: `04:30` or `4:30`
- Set filter 2 to start at 6:00 PM: `18:00`
- Set filter 2 to run for 2 hours: `02:00` or `2:00`

All inputs are validated for proper time format (H:MM or HH:MM with valid hours 0-23 and minutes 0-59). Invalid formats will be rejected with error messages in the logs.

## Text Sensors (Read-only)

The text sensors display current spa status:

- **spa_time**: Current spa time in HH:MM format
- **filter1_config**: Current filter 1 configuration in JSON format
- **filter2_config**: Current filter 2 configuration in JSON format (or "disabled")

### ESP WebUI
![image](https://github.com/user-attachments/assets/af602be2-da9e-4880-8fb8-e7f7f9122977)

### Home Assistant UI
![image](https://github.com/user-attachments/assets/a37a7e08-94b2-4231-83ca-0ffc4646fbfa)

## Component for Balboa Spa

This project is based on the UART reader from [Dakoriki/ESPHome-Balboa-Spa](https://github.com/Dakoriki/ESPHome-Balboa-Spa)

There are a ton of these implementations on Github.  None of the ones I could find implemented the external component pattern as prescribed by EspHome.  So I create this one.  

Climate, sensors, and switches are all optional.  So you only need to import what you want with your implementation.

TODO:
There is still a bit of clean up and tweaking to be done.  I often see sensor values jump around, often to invalid state.  I suspect this is either due to the polling frequency or uart config.  Or it might just be due to the noisy nature of running next two heaters and pumps.

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

switch:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet1:
      name: Jet1
    jet2:
      name: Jet2
    jet3:
      name: Jet3
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
```
ESP WebUI
![image](https://github.com/user-attachments/assets/af602be2-da9e-4880-8fb8-e7f7f9122977)

Home Assistant UI
![image](https://github.com/user-attachments/assets/a37a7e08-94b2-4231-83ca-0ffc4646fbfa)

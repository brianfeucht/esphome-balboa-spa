## Component for Balboa Spa

This project is based on the UART reader from [Dakoriki/ESPHome-Balboa-Spa](https://github.com/Dakoriki/ESPHome-Balboa-Spa)

There are a ton of these implementations on Github.  None of the ones I could find implemented the external component pattern as prescribed by EspHome.  So I create this one.  

Climate, binary sensors, sensors, switches, and fans are all optional.  So you only need to import what you want with your implementation.

### Features

- **Climate Control**: Full thermostat functionality with temperature control
- **Jet Control**: Two options for controlling jets:
  - **Switch Platform**: Traditional on/off control (backward compatible)
  - **Fan Platform**: 3-state control (off/low/high) for jets that support multiple speeds
- **Light & Blower Control**: On/off switches for lights and blower
- **Status Monitoring**: Sensors and binary sensors for spa state monitoring
- **Flexible Configuration**: Import only the platforms you need

### Jet Control Options

This component provides two ways to control jets, depending on your needs:

#### Option 1: Switch Platform (Traditional On/Off)
Use the switch platform for simple on/off control of jets. This is backward compatible with previous versions:

```yaml
switch:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet1:
      name: "Jet 1"
    jet2:
      name: "Jet 2"
    # ... additional jets
```

#### Option 2: Fan Platform (3-State Control)
Use the fan platform for full 3-state control (off/low/high) of jets:

```yaml
fan:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet1:
      name: "Jet 1 Fan"
    jet2:
      name: "Jet 2 Fan" 
    # ... additional jets
```

The fan platform maps spa jet states to ESPHome fan speeds:
- **Off**: Fan speed 0, jet state 0
- **Low**: Fan speed 1, jet state 1  
- **High**: Fan speed 3, jet state 2

**Note**: You can use both switch and fan platforms simultaneously for the same jets. The switch will show "on" for any non-zero jet state (low or high), while the fan will show the specific speed level.

### Basic Configuration Example

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
```
### ESP WebUI
![image](https://github.com/user-attachments/assets/af602be2-da9e-4880-8fb8-e7f7f9122977)

### Home Assistant UI
![image](https://github.com/user-attachments/assets/a37a7e08-94b2-4231-83ca-0ffc4646fbfa)

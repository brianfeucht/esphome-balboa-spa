## Component for Balboa Spa
This project is based on the UART reader from [Dakoriki/ESPHome-Balboa-Spa](https://github.com/Dakoriki/ESPHome-Balboa-Spa)

There are a ton of these implementations on Github.  None of the ones I could find implemented the external component pattern as prescribed by EspHome.  So I create this one.  

All components are optional (climate, switch, text_sensor, etc).  So you only need to import what you want with your implementation.

### CRC Errors
I and multiple other users see a ton of CRC errors.  I've spent some time investigating the serial bit stream and all the cases I've identified have been bit flipping.  This might be invalid UART config (baud, buffer, etc) or a bad hardware design. However, I'm assuming this is just due to the noisy nature of running next heaters and pumps.
**Note: CRC errors can be silenced specifically - see Troubleshooting section below.**

## Sample Config
```yaml
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
  rx_buffer_size: 512

balboa_spa:
  id: spa
  # Set this to C or F based on the units your spa is configured for
  spa_temp_scale: F
  # Optional: Override the automatically assigned client ID
  # client_id: 10

switch:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet1:
      name: Jet1
      max_toggle_attempts: 5  # Optional: max attempts to reach desired state (default: 5)      
      discard_updates: 5      # Optional: state updates to ignore after each toggle (default: 20)
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

# Fan platform for multi-speed jet control (recommended for jets with speed support)
fan:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet_1:
      name: "Jet 1"
      id: jet1_fan
      max_toggle_attempts: 5  # Optional: max attempts to reach desired state (default: 5)
      discard_updates: 20     # Optional: state updates to ignore after each toggle (default: 20)
    jet_2:
      name: "Jet 2"
      id: jet2_fan
    jet_3:
      name: "Jet 3"
      id: jet3_fan
    jet_4:
      name: "Jet 4"
      id: jet4_fan

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

## Jet Control: Switch vs Fan Components

This component provides two ways to control your spa jets, each with different capabilities:

### Fan Components

The **fan** platform provides full control over multi-speed jets with three distinct states:
- **OFF** - Jet is completely off
- **LOW** - Low speed (speed 1)
- **HIGH** - High speed (speed 2)

**Configuration:**
```yaml
fan:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet_1:
      name: "Jet 1"
      max_toggle_attempts: 5  # Optional, default: 5
      discard_updates: 20     # Optional, default: 20
```

### Switch Components (Simple ON/OFF Control)

The **switch** platform provides simple boolean control:
- **OFF** - Jet is off
- **ON** - Jet is on (typically LOW speed)

**When to use switches:**
- Your spa only supports simple ON/OFF jets (no multi-speed)
- You prefer simple toggle behavior
- You need backwards compatibility with existing automations

**Configuration:**
```yaml
switch:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet1:
      name: Jet1
      max_toggle_attempts: 5  # Optional, default: 5
      discard_updates: 20      # Optional, default: 20
```

### MAX_TOGGLE_ATTEMPTS Behavior

Both switch and fan components support two configurable parameters:

**`max_toggle_attempts`** (default: 5) - Maximum retry attempts when spa blocks state changes
**`discard_updates`** (default: 20) - Number of state updates to ignore after each toggle command

These work together to handle cases where the spa temporarily blocks state changes:

- **Why it's needed:** During heating or filter cycles, the spa may prevent jets from turning off
- **How it works:** If a state change is requested but not achieved, the component will retry on each spa state update
- **Max attempts:** After reaching the maximum number of attempts, the component will sync with the actual spa state and stop retrying
- **Typical value:** 5 attempts is usually sufficient (covers about 5-10 seconds)
- **Discard updates:** After each toggle command, the component ignores the next 20 state updates to allow the spa to process the change

**Example scenario:** If you try to turn off a jet during a heating cycle:
1. Component sends toggle command
2. Spa ignores the command (heating in progress)
3. Component retries on next state update
4. After heating completes, spa accepts the command
5. Jet turns off successfully

## Troubleshooting

### UART RX Buffer Size

**Important**: The `rx_buffer_size` parameter in the UART configuration must be set appropriately for your ESP framework:

- **ESP-IDF framework (esp32)**: The RX buffer size **must be greater than 128 bytes** (the hardware FIFO length). Using exactly 128 will cause boot loops with errors like:
  - `uart rx buffer length error`
  - `uart_driver_install failed: ESP_FAIL`
  - `uart is marked FAILED: unspecified`

- **Recommended value**: **512 bytes or higher** (512-1024 is a good balance between memory usage and reliability)
- **Minimum value for ESP-IDF**: 256 bytes (but 512 is strongly recommended)

```yaml
uart:
  id: spa_uart_bus
  tx_pin: GPIO37
  rx_pin: GPIO39
  baud_rate: 115200
  rx_buffer_size: 512  # Recommended: 512 or higher, minimum 256 for ESP-IDF
```

If you experience boot loops when using ESP-IDF framework, increase your `rx_buffer_size` to 512 or higher.

### CRC Errors

CRC errors are very common with Balboa spa controllers due to electrical interference from heaters and pumps. If you're seeing frequent CRC error messages in your logs, you can silence them specifically while keeping other DEBUG level logging:

```yaml
logger:
  level: DEBUG
  logs:
    BalboaSpa.CRC: NONE  # Silence CRC error messages
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

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
    type: esp-idf

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
    filter2:
      name: "Filter 2"

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
    # Fault log sensors (optional)
    fault_code:
      name: Fault Code
    fault_total_entries:
      name: Fault Total Entries
    fault_current_entry:
      name: Fault Current Entry
    fault_days_ago:
      name: Fault Days Ago

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
    filter1_running:
      name: "Filter 1 Running"
    filter2_running:
      name: "Filter 2 Running"

## Binary Sensors

The binary sensor platform provides various spa status indicators:

**Available Binary Sensors:**
- **blower**: Indicates if the blower is currently running
- **highrange**: Indicates if high range heating mode is active
- **circulation**: Indicates if the circulation pump is running
- **restmode**: Indicates if the spa is in rest/sleep mode
- **heatstate**: Indicates if the heater is currently active
- **connected**: Indicates if the component is actively communicating with the spa
- **filter1_running**: Indicates if filter 1 cycle is currently running
- **filter2_running**: Indicates if filter 2 cycle is currently running

### Filter Running Sensors

The **filter1_running** and **filter2_running** sensors automatically determine whether each filter cycle should be running based on the current spa time and configured filter schedule.

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
    # Fault log text sensors (optional)
    fault_message:
      name: "Fault Message"
    fault_log_time:
      name: "Fault Log Time"
    # Reminder text sensor (optional)
    reminder:
      name: "Reminder"

button:
  - platform: balboa_spa
    balboa_spa_id: spa
    sync_time:
      name: "Sync Spa Time"
    disable_filter2:
      name: "Disable Filter 2"
    request_fault_log:
      name: "Request Fault Log"
    clear_reminder:
      name: "Clear Reminder"
```

## Fault Monitoring

The component provides comprehensive fault monitoring capabilities to help diagnose spa issues:

### Fault Sensors

**Numeric Sensors:**
- `fault_code`: The numeric fault code (see fault codes table below)
- `fault_total_entries`: Total number of fault log entries stored in the spa
- `fault_current_entry`: The entry number of the current fault (0-23)
- `fault_days_ago`: Number of days since the fault occurred

**Text Sensors:**
- `fault_message`: Human-readable description of the fault (automatically converts fault codes to messages - see fault codes table below)
- `fault_log_time`: ISO 8601 formatted timestamp of when the fault occurred (e.g., "2026-01-20T15:52:21")

### Request Fault Log Button

The `request_fault_log` button manually triggers a fault log update from the spa. The fault log is automatically retrieved during startup, but this button allows you to refresh the fault information on demand.

### Fault Codes

The following fault codes are recognized by the component:

| Code | Message |
|------|---------|
| 15 | Sensors are out of sync |
| 16 | The water flow is low |
| 17 | The water flow has failed |
| 18 | The settings have been reset |
| 19 | Priming Mode |
| 20 | The clock has failed |
| 21 | The settings have been reset |
| 22 | Program memory failure |
| 26 | Sensors are out of sync -- Call for service |
| 27 | The heater is dry |
| 28 | The heater may be dry |
| 29 | The water is too hot |
| 30 | The heater is too hot |
| 31 | Sensor A Fault |
| 32 | Sensor B Fault |
| 34 | A pump may be stuck on |
| 35 | Hot fault |
| 36 | The GFCI test failed |
| 37 | Standby Mode (Hold Mode) |

**Note:** The spa only stores the most recent fault entry. When a new fault occurs, it overwrites the previous entry.

**Tip:** Use the `fault_message` text sensor to automatically get the human-readable fault description instead of manually looking up fault codes in this table.

## Reminder Monitoring

The component provides reminder monitoring capabilities to help track spa maintenance reminders:

### Reminder Text Sensor

The `reminder` text sensor displays the current reminder status from the spa. The spa can display various maintenance reminders:

**Reminder Types:**
- `None` - No active reminders
- `Clean Filter` - Time to clean or replace the filter
- `Check pH` - Check and adjust pH levels
- `Check Sanitizer` - Check and adjust sanitizer levels
- `Fault` - Indicates a fault condition (check fault sensors for details)

**Configuration:**
```yaml
text_sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    reminder:
      name: "Reminder"
```

### Clear Reminder Button

The `clear_reminder` button allows you to clear/acknowledge active reminders on the spa. This sends a clear notification command to the spa controller.

**Configuration:**
```yaml
button:
  - platform: balboa_spa
    balboa_spa_id: spa
    clear_reminder:
      name: "Clear Reminder"
```

**Usage:**
1. Monitor the `reminder` text sensor for active reminders
2. Perform the required maintenance (clean filter, adjust chemicals, etc.)
3. Press the `clear_reminder` button to acknowledge and clear the reminder

**Note:** The reminder feature is based on the Balboa protocol Type Code 0x13 (Status Update) which reports reminder status, and Type Code 0x11 with Item Code 0x03 which clears reminders. The specific reminders available may vary depending on your spa model and firmware version.


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

### ESP32-S2/S3/C3 Boards with Native USB (ESPHome 2025.10.0+)

**Important**: If you're using an ESP32-S2, ESP32-S3, or ESP32-C3 board with native USB support (e.g., `lolin_s2_mini`, `esp32-s3-devkitc-1`) with ESPHome 2025.10.0 or later, you **must** add the USB CDC build flag to your configuration:

```yaml
esphome:
  name: your_device_name
  platformio_options:
    board_build.extra_flags:
      - "-DARDUINO_USB_CDC_ON_BOOT=0"

esp32:
  board: lolin_s2_mini
  framework: 
    type: arduino
```

**Why this is required**: ESPHome 2025.10.0 upgraded to arduino-esp32 3.1.0, which has a breaking change affecting boards with native USB support. Setting `ARDUINO_USB_CDC_ON_BOOT=0` disables USB CDC on boot, forcing the board to use regular UART for Serial communication instead of USBSerial. Without this flag, compilation will fail with `USBSerial not declared` errors.

**Technical explanation**: The ESP32-S2/S3/C3 boards have native USB hardware. By default, arduino-esp32 3.1.0 tries to use USB CDC (making `Serial` use `USBSerial`), but this requires additional configuration. Setting the flag to `0` disables this feature and uses the traditional UART-based Serial interface, which works with standard ESPHome configurations.

**Note**: This flag is NOT needed for:
- ESP32 classic boards (e.g., `esp32dev`, `nodemcu-32s`)
- ESP-IDF framework (uses `type: esp-idf` instead of `type: arduino`)
- ESP8266 boards

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

## Filter 2 Switch

The Filter 2 switch allows you to enable and disable the secondary filter cycle while viewing its current state from the spa.

**Configuration:**
```yaml
switch:
  - platform: balboa_spa
    balboa_spa_id: spa
    filter2:
      name: "Filter 2"
```

**How it works:**
- The switch displays the current filter 2 enabled/disabled state from the spa
- When turned ON, it enables filter 2 using the configuration from the text components
- When turned OFF, it disables filter 2 (equivalent to the `disable_filter2` button)
- If no filter 2 configuration exists, turning ON will log an error and revert the switch to OFF

**Required setup for turning ON:**
Use the text components to set filter 2 configuration first:
```yaml
text:
  - platform: balboa_spa
    balboa_spa_id: spa
    filter2_start_time:
      name: "Set Filter 2 Start Time"
      mode: TEXT
    filter2_duration:
      name: "Set Filter 2 Duration"
      mode: TEXT
```

Set the start time and duration, then use the Filter 2 switch to enable/disable as needed.

**Coexistence with disable_filter2 button:**
Both the Filter 2 switch and the `disable_filter2` button can be used together. The switch will reflect state changes made by either control method. They are fully compatible for backward compatibility.

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

## Development & CI

### Manual CI Builds

The CI workflow can be manually triggered to test compatibility with specific ESPHome versions:

1. Go to the [Actions tab](../../actions/workflows/ci.yml)
2. Click "Run workflow"
3. Select the `main` branch
4. Optionally specify an ESPHome version (e.g., `2025.11.0`, `dev`, or leave as `stable`)
5. Click "Run workflow"

This is useful for:
- Testing compatibility with newly released ESPHome versions
- Validating changes against development versions
- Quick verification without waiting for automatic triggers

The workflow builds all test configurations (ESP32 Arduino, ESP32 IDF, and ESP8266) to ensure broad platform compatibility.

### ESP WebUI
![image](https://github.com/user-attachments/assets/af602be2-da9e-4880-8fb8-e7f7f9122977)

### Home Assistant UI
![image](https://github.com/user-attachments/assets/a37a7e08-94b2-4231-83ca-0ffc4646fbfa)

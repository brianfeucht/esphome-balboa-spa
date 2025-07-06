# Copilot Instructions for ESPHome Balboa Spa

## Project Overview
This repository contains an ESPHome external component for integrating Balboa spa controllers. It implements the proper ESPHome external component pattern to provide climate control, switches, sensors, and binary sensors for Balboa spa systems. The component communicates with Balboa controllers via UART/RS485 protocol.

## Repository Structure
```
esphome-balboa-spa/
├── components/
│   └── balboa_spa/
│       ├── __init__.py          # Component registration and setup
│       ├── balboa_spa.h         # Main component header
│       ├── balboa_spa.cpp       # Main component implementation
│       ├── climate.py           # Climate platform (thermostat)
│       ├── climate/             # Climate platform implementation
│       ├── switch.py            # Switch platform (jets, lights, blower)
│       ├── switch/              # Switch platform implementation
│       ├── sensor.py            # Sensor platform (status monitoring)
│       ├── sensor/              # Sensor platform implementation
│       ├── binary_sensor.py     # Binary sensor platform
│       └── binary_sensor/       # Binary sensor implementation
├── example.yaml                 # Example ESPHome configuration
└── README.md                    # Documentation and usage
```

## Key Technologies & Protocols

### Hardware Communication
- **Protocol**: Balboa proprietary protocol over RS485/UART
- **Baud Rate**: 115200 (configurable, may need adjustment for some models)
- **Data Format**: 8 data bits, no parity, 1 stop bit
- **Buffer Size**: 128 bytes (adjustable if CRC errors occur)

### ESPHome Integration
- **Framework**: ESPHome external component architecture
- **Target Platform**: ESP32 (primary), ESP8266 (may work with modifications)
- **Component Type**: Hub component with multiple platform integrations

## Development Guidelines

### Code Style & Standards
- Follow ESPHome coding standards and conventions
- Use proper ESPHome component lifecycle methods
- Implement proper error handling and logging using `ESP_LOG*` macros
- All configuration should use ESPHome's validation system
- Components must be modular and optional (user imports only what they need)

### Component Architecture
- **Hub Component**: `balboa_spa` serves as the main communication hub
- **Platform Components**: Each platform (climate, switch, sensor, binary_sensor) registers with the hub
- **Communication**: UART-based protocol implementation with CRC validation
- **State Management**: Component maintains spa state and synchronizes with hardware

### Configuration Patterns
```yaml
external_components:
  - source:
      type: git
      url: https://github.com/brianfeucht/esphome-balboa-spa
      ref: main

uart:
  id: spa_uart_bus
  tx_pin: GPIO37
  rx_pin: GPIO39
  baud_rate: 115200
  rx_buffer_size: 128

balboa_spa:
  id: spa
  spa_temp_scale: F  # or C based on spa configuration
```

### Platform Implementation
- **Climate**: Implements thermostat functionality with temperature control
- **Switch**: Controls jets (1-3), lights, blower, and other binary controls
- **Sensor**: Monitors spa status, heat state, circulation, etc.
- **Binary Sensor**: Provides binary status indicators for various spa states

## Known Issues & Considerations

### CRC Errors
- High frequency of CRC errors may occur due to:
  - Incorrect UART configuration (baud rate, buffer size)
  - Electrical interference from heaters and pumps
  - RS485 wiring issues (A/B wire swap)
- **Troubleshooting**: Adjust UART parameters, check wiring, add proper RS485 termination

### Hardware Compatibility
- **Tested Models**: Based on Balboa BP series controllers
- **Communication**: Requires RS485 interface (TTL to RS485 converter needed)
- **Power**: Requires 3.3V regulated power supply
- **Wiring**: Proper A/B wire identification crucial for communication

## Development Workflow

### Adding New Features
1. Identify the Balboa protocol message for the feature
2. Implement protocol parsing in the main component
3. Create appropriate platform component if needed
4. Add configuration validation
5. Update example configuration
6. Test with real hardware

### Testing & Validation
- Test all platforms independently and together
- Verify proper ESPHome lifecycle integration
- Test configuration validation
- Validate communication reliability
- Check for memory leaks and performance issues

### Protocol Implementation
- Refer to existing Balboa protocol documentation
- Implement proper message parsing and CRC validation
- Handle communication errors gracefully
- Maintain spa state synchronization
- Implement proper discovery and initialization

## Dependencies & Libraries

### ESPHome Requirements
- ESPHome core framework
- UART component for communication
- Standard ESPHome platform components

### External References
- Based on UART reader from [Dakoriki/ESPHome-Balboa-Spa](https://github.com/Dakoriki/ESPHome-Balboa-Spa)
- Protocol documentation from [balboa_worldwide_app](https://github.com/ccutrer/balboa_worldwide_app)
- Hardware wiring guides from community projects

## Configuration Best Practices

### Temperature Scale
- Set `spa_temp_scale` to match your spa's native configuration (C or F)
- This affects both display and control ranges

### Component Selection
- Only include platforms you actually need
- Each platform is optional and can be independently configured
- This reduces memory usage and complexity

### Hardware Configuration
- Use appropriate buffer sizes for your communication reliability
- Consider adjusting baud rate if experiencing communication issues
- Ensure proper power supply and RS485 interface

## Future Improvements

### TODO Items
- Investigate and resolve CRC error frequency
- Optimize UART configuration for better reliability
- Add support for additional spa models/configurations
- Improve error handling and recovery
- Add more comprehensive logging and diagnostics

### Enhancement Opportunities
- Advanced scheduling and automation features
- Energy monitoring capabilities
- Integration with additional Balboa features
- Support for multiple spa configurations
- Enhanced diagnostics and maintenance alerts

## Support & Community
- Issues should be reported with hardware model information
- Include ESPHome configuration and logs for troubleshooting
- Community contributions welcome following ESPHome standards
- Test changes with real hardware before submitting PRs

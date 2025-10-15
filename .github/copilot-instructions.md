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

## AI Development Workflow & Best Practices

### Background Agent Test Build Requirements
**CRITICAL**: Background agents MUST always perform test builds as part of any code modification workflow. This ensures component integrity and prevents breaking changes from being introduced.

#### Mandatory Build Validation Steps
1. **Before making changes**: Run initial build validation to establish baseline
2. **After each significant change**: Validate builds continue to work
3. **Before completing session**: Perform comprehensive build validation
4. **Use automated workflow**: Leverage `.github/workflows/copilot-setup-steps.yml` for validation

#### Test Build Execution Methods

##### Method 1: GitHub Actions Workflow (Recommended)
```bash
# Trigger the copilot-setup-steps workflow via GitHub API or manual dispatch
# This workflow uses the official esphome/build-action@v7.1.0 for consistent builds
gh workflow run copilot-setup-steps.yml --field validate_all=true

# Build only specific platforms
gh workflow run copilot-setup-steps.yml --field esp32_only=true
gh workflow run copilot-setup-steps.yml --field esp8266_only=true
```

##### Method 2: Local ESPHome Compilation
```bash
# Install ESPHome if not present
pip install esphome

# Validate all test configurations
esphome compile esp32_test_component.yaml
esphome compile esp32idf_test_component.yaml  
esphome compile esp8266_test_component.yaml
```

##### Method 3: Quick Syntax Validation
```bash
# Fast syntax check without full compilation
esphome config esp32_test_component.yaml
esphome config esp32idf_test_component.yaml
esphome config esp8266_test_component.yaml
```

#### Test Configuration Coverage
The project includes comprehensive test configurations that MUST be validated:

- **esp32_test_component.yaml**: ESP32 with Arduino framework
- **esp32idf_test_component.yaml**: ESP32 with ESP-IDF framework  
- **esp8266_test_component.yaml**: ESP8266 with Arduino framework

Each configuration tests all component platforms:
- Climate (thermostat functionality)
- Switch (jets, lights, blower controls)
- Fan (multi-speed jet controls)
- Sensor (status monitoring)
- Binary sensor (binary status indicators)
- Text sensor (configuration and time display)
- Button (control actions)
- Text (configuration input)

#### Build Failure Handling
When builds fail, background agents MUST:

1. **Identify root cause**: Parse compilation errors and warnings
2. **Fix systematically**: Address issues in logical order
3. **Verify incrementally**: Test fixes with targeted builds
4. **Document changes**: Commit working fixes with clear messages
5. **Full validation**: Ensure all configurations build before completion

#### Integration with Development Workflow
Background agents should integrate test builds into their standard workflow:

1. **Assessment Phase**: Check current build status
2. **Planning Phase**: Consider build impact of planned changes
3. **Execution Phase**: Validate builds after each logical change group
4. **Completion Phase**: Full build validation before session end

#### Continuous Integration Alignment
The copilot-setup-steps.yml workflow mirrors the main CI pipeline but provides:
- **Official ESPHome Action**: Uses `esphome/build-action@v7.1.0` for consistent, reliable builds
- **Manual trigger capability**: For background agent validation
- **Flexible platform selection**: Build only specific configurations if needed
- **Enhanced reporting**: Detailed analysis and artifact generation
- **Fail-fast behavior**: Quick identification of issues
- **Matrix strategy**: Parallel validation and builds for efficiency

#### Performance Considerations
- **Parallel builds**: The workflow builds platforms in parallel for speed
- **Caching**: Leverages ESPHome build caching to reduce build times
- **Selective building**: Can target specific platforms to save time during development
- **Early validation**: Syntax checking before expensive compilation

#### Documentation Requirements
When build issues are encountered and resolved:
1. **Document the issue**: Clear description of what failed
2. **Explain the solution**: Why the fix works
3. **Update patterns**: If it reveals a broader pattern issue
4. **Commit message**: Include build validation status

#### Success Metrics
A successful background agent session should achieve:
- ✅ All test configurations compile without errors
- ✅ No new compiler warnings introduced
- ✅ Component functionality preserved
- ✅ Memory usage within acceptable bounds
- ✅ Platform compatibility maintained

### Session Management & Context
- **Start with current state assessment**: Always check git status, recent commits, and file modifications before making changes
- **Use semantic_search first**: Understand the codebase structure before making targeted changes
- **Read files in large chunks**: Prefer reading 50+ lines at once rather than small sequential reads
- **Check compilation frequently**: Verify changes compile successfully using `esphome compile <config>.yaml`
- **Commit incrementally**: Make logical commits with descriptive messages for easier rollback and review

### Effective Tool Usage Patterns
- **grep_search for overview**: Use to get a high-level view of patterns across files
- **file_search for discovery**: Find files by pattern when structure is unknown
- **read_file for context**: Get sufficient context before making edits
- **replace_string_in_file for precision**: Use when exact string match is needed
- **insert_edit_into_file for flexibility**: Better for complex edits and new code

### Code Modification Workflow
1. **Assess**: Check current state and understand requirements
2. **Search**: Find all affected files and understand patterns
3. **Plan**: Identify the scope of changes needed
4. **Execute**: Make changes systematically, file by file
5. **Verify**: Check compilation and functionality
6. **Document**: Commit with clear messages

### Merge Conflict Resolution Strategy
- **Fetch latest first**: Always check what's on main before merging
- **Understand both sides**: Read the conflicting changes carefully
- **Preserve intent**: Maintain the purpose of both sets of changes
- **Test after merge**: Ensure compilation and functionality remain intact
- **Clean up artifacts**: Remove any merge markers or duplicate code

### Logging & Debugging Best Practices
- **Consistent TAG usage**: Use static const char *TAG = "Component.subcomponent" pattern
- **Standardized message format**: Follow "Category/subcategory: message" pattern
- **Appropriate log levels**: DEBUG for state changes, WARN for issues, ERROR for failures
- **Context in messages**: Include relevant data (values, states) in log output

### Common Pitfalls & Solutions

#### Variable-Length Arrays
```cpp
// WRONG - compiler warning
auto payload_length = std::snprintf(nullptr, 0, format, args...);
char buffer[payload_length + 1];  // VLA issue

// CORRECT - check result first
auto result = std::snprintf(nullptr, 0, format, args...);
if (result > 0) {
    char buffer[result + 1];
    // Use buffer...
}
```

#### Automated Replacements
- **Avoid broad sed/awk**: Can corrupt code structure
- **Use targeted replacements**: Focus on specific patterns
- **Verify each change**: Check that automated changes are correct
- **Fix corruption immediately**: Don't let broken code persist

#### Git Workflow
- **Check branch state**: Know what's committed vs. staged vs. modified
- **Preserve working changes**: Stash or commit before major operations
- **Clean merge strategy**: Resolve conflicts methodically
- **Test post-merge**: Always verify compilation after merging

### Performance Optimization
- **Batch operations**: Group related file operations together
- **Minimize tool calls**: Use fewer, more comprehensive operations
- **Cache context**: Keep relevant information in memory during session
- **Parallel where safe**: Use parallel operations for independent tasks

### Documentation Standards
- **Clear commit messages**: Describe what and why, not just what
- **Update relevant docs**: Modify README, examples, or instructions as needed
- **Code comments**: Explain complex logic, especially protocol handling
- **PR descriptions**: Include technical details, testing, and benefits

### Testing & Validation
- **Compile all configurations**: Test ESP32, ESP8266, and IDF variants
- **Check for warnings**: Address compiler warnings promptly
- **Verify functionality**: Ensure no behavioral changes unless intended
- **Run static analysis**: Look for potential issues in code patterns

### Communication & Collaboration
- **Explain decisions**: Document why certain approaches were chosen
- **Provide alternatives**: Mention trade-offs and other options considered
- **Include examples**: Show before/after code snippets
- **Clear status updates**: Keep user informed of progress and blockers

### Session-Specific Lessons Learned

#### ESP_LOG* Macro Standardization
- **Pattern recognition**: Use grep_search to find all instances before starting changes
- **Systematic approach**: Update all files consistently rather than piecemeal
- **Format standardization**: "Category/subcategory: message" with colon separator
- **TAG constants**: Define once per file as `static const char *TAG = "Component.name"`

#### Variable Naming Conventions (from main branch merge)
- **Descriptive names**: `input_queue` vs `Q_in`, `client_id` vs `id`
- **Consistent prefixes**: Use clear, unambiguous variable names
- **Avoid abbreviations**: Prefer readability over brevity
- **Context clarity**: Variable names should indicate their purpose

#### Merge Conflict Resolution Experience
- **Three-way understanding**: Original, main branch changes, working branch changes
- **Preserve all improvements**: Don't lose either set of enhancements
- **Manual verification**: Check each resolved conflict for correctness
- **Post-merge testing**: Always compile and test after conflict resolution

#### Code Quality Maintenance
- **Compiler warnings**: Address immediately, don't accumulate technical debt
- **Consistent formatting**: Maintain code style throughout changes
- **Comment preservation**: Keep relevant comments, update outdated ones
- **Error handling**: Maintain or improve error handling patterns

#### Git Workflow Optimization
- **Branch awareness**: Always know current branch and its state relative to remotes
- **Logical commits**: Each commit should represent a complete, logical change
- **Descriptive messages**: Commit messages should explain the intent and scope
- **Clean history**: Avoid merge commits when possible, prefer rebasing for clean history

#### Tool Usage Insights
- **grep_search efficiency**: Faster than multiple read_file calls for pattern discovery
- **replace_string_in_file precision**: Best for exact string replacements
- **Context importance**: Include enough surrounding code for unambiguous matching
- **Parallel operations**: Some operations can be batched for efficiency

### Workflow Efficiency Patterns

#### Discovery Phase (5-10% of time)
1. **git status** - Understand current state
2. **semantic_search** - Get high-level code overview  
3. **grep_search** - Find specific patterns to modify
4. **file_search** - Locate relevant files by pattern

#### Planning Phase (10-15% of time)
1. **Read key files** - Understand current implementation
2. **Identify scope** - Determine all files that need changes
3. **Plan approach** - Decide on systematic vs. targeted changes
4. **Consider dependencies** - Account for variable renames, merges, etc.

#### Execution Phase (60-70% of time)
1. **Make changes systematically** - One concept or file type at a time
2. **Test frequently** - Compile after major changes
3. **Commit incrementally** - Logical checkpoints for rollback
4. **Handle issues immediately** - Fix problems as they arise

#### Validation Phase (15-20% of time)
1. **Final compilation** - Test all configurations
2. **Pattern verification** - Ensure consistency across all changes
3. **Documentation update** - Record lessons learned
4. **Clean up** - Remove temporary files, unused code

#### Red Flags to Watch For
- **Too many tool calls**: Indicates inefficient approach
- **Repeated pattern searches**: Should batch discovery phase
- **Compilation failures**: Fix immediately, don't accumulate
- **Merge conflicts**: Address systematically, don't rush
- **Inconsistent changes**: Standardize patterns across all files

#### Success Indicators
- **Clean git history**: Logical, well-described commits
- **Successful compilation**: No errors or warnings
- **Consistent patterns**: Same approach used throughout
- **Preserved functionality**: No breaking changes
- **Good documentation**: Clear commit messages and code comments

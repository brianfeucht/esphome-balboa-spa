#!/usr/bin/env python3
"""
Test script to validate the 3-state jet protocol parsing logic
"""

def test_bit_extraction():
    """Test the new 2-bit extraction vs old 1-bit extraction"""
    
    # Test case: input_queue[16] with jets in different states
    # Let's say jet1=2 (high), jet2=1 (low), jet3=0 (off), jet4=2 (high)
    # This would be: jet4(bits 6-7)=10, jet3(bits 4-5)=00, jet2(bits 2-3)=01, jet1(bits 0-1)=10
    # Binary: 10000110 = 0x86 = 134
    
    test_byte = 0x86  # 10000110 in binary
    
    print(f"Test byte: 0x{test_byte:02x} (binary: {test_byte:08b})")
    print()
    
    # Old method (single bit extraction)
    print("Old method (single bits):")
    old_jet1 = (test_byte >> 1) & 0x01  # bit 1
    old_jet2 = (test_byte >> 3) & 0x01  # bit 3  
    old_jet3 = (test_byte >> 5) & 0x01  # bit 5
    old_jet4 = (test_byte >> 7) & 0x01  # bit 7
    print(f"  jet1: {old_jet1} (bit 1)")
    print(f"  jet2: {old_jet2} (bit 3)")
    print(f"  jet3: {old_jet3} (bit 5)")
    print(f"  jet4: {old_jet4} (bit 7)")
    print()
    
    # New method (2-bit extraction)
    print("New method (2-bit states):")
    new_jet1 = (test_byte >> 0) & 0x03  # bits 0-1
    new_jet2 = (test_byte >> 2) & 0x03  # bits 2-3
    new_jet3 = (test_byte >> 4) & 0x03  # bits 4-5
    new_jet4 = (test_byte >> 6) & 0x03  # bits 6-7
    print(f"  jet1: {new_jet1} (bits 0-1) -> {'off' if new_jet1==0 else 'low' if new_jet1==1 else 'high'}")
    print(f"  jet2: {new_jet2} (bits 2-3) -> {'off' if new_jet2==0 else 'low' if new_jet2==1 else 'high'}")
    print(f"  jet3: {new_jet3} (bits 4-5) -> {'off' if new_jet3==0 else 'low' if new_jet3==1 else 'high'}")
    print(f"  jet4: {new_jet4} (bits 6-7) -> {'off' if new_jet4==0 else 'low' if new_jet4==1 else 'high'}")
    print()
    
    # Test switch compatibility (old behavior)
    print("Switch platform mapping (for backward compatibility):")
    print(f"  jet1_switch: {'on' if new_jet1 > 0 else 'off'}")
    print(f"  jet2_switch: {'on' if new_jet2 > 0 else 'off'}")
    print(f"  jet3_switch: {'on' if new_jet3 > 0 else 'off'}")
    print(f"  jet4_switch: {'on' if new_jet4 > 0 else 'off'}")
    print()
    
    # Test fan platform mapping
    print("Fan platform mapping:")
    def spa_state_to_fan_speed(state):
        if state == 0: return 0  # Off
        elif state == 1: return 1  # Low
        elif state == 2: return 3  # High
        else: return 0
    
    print(f"  jet1_fan: speed {spa_state_to_fan_speed(new_jet1)} ({'off' if spa_state_to_fan_speed(new_jet1)==0 else 'low' if spa_state_to_fan_speed(new_jet1)==1 else 'high'})")
    print(f"  jet2_fan: speed {spa_state_to_fan_speed(new_jet2)} ({'off' if spa_state_to_fan_speed(new_jet2)==0 else 'low' if spa_state_to_fan_speed(new_jet2)==1 else 'high'})")
    print(f"  jet3_fan: speed {spa_state_to_fan_speed(new_jet3)} ({'off' if spa_state_to_fan_speed(new_jet3)==0 else 'low' if spa_state_to_fan_speed(new_jet3)==1 else 'high'})")
    print(f"  jet4_fan: speed {spa_state_to_fan_speed(new_jet4)} ({'off' if spa_state_to_fan_speed(new_jet4)==0 else 'low' if spa_state_to_fan_speed(new_jet4)==1 else 'high'})")

def test_toggle_logic():
    """Test the toggle calculation logic"""
    print("\n" + "="*50)
    print("TOGGLE CALCULATION LOGIC TEST")
    print("="*50)
    
    def calculate_toggles(current, target):
        """Calculate how many toggles needed to go from current to target state"""
        if current == target:
            return 0
        elif current == 0 and target == 1: return 1
        elif current == 0 and target == 2: return 2
        elif current == 1 and target == 0: return 2
        elif current == 1 and target == 2: return 1
        elif current == 2 and target == 0: return 1
        elif current == 2 and target == 1: return 2
        else: return 0
    
    states = ['off', 'low', 'high']
    print("Toggle transitions (jets cycle: off -> low -> high -> off):")
    print("Current -> Target: Toggles needed")
    for current in range(3):
        for target in range(3):
            toggles = calculate_toggles(current, target)
            print(f"  {states[current]:>4} -> {states[target]:<4}: {toggles}")

if __name__ == "__main__":
    test_bit_extraction()
    test_toggle_logic()
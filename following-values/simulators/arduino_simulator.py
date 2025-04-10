import json
import time
import random
import math

# Configuration
TRANSMISSION_INTERVAL_SECONDS = 1.0  # Interval in seconds (matches Arduino code)
INITIAL_BATTERY_LEVEL = 98.0
BATTERY_DRAIN_RATE = 0.01  # Percentage points per interval

# Realistic Value Ranges (example)
SPECTRAL_BASE = [150, 250, 300, 280, 220, 180] # Base values for V,B,G,Y,O,R
SPECTRAL_NOISE = 20 # Max random noise +/-
LUMINANCE_BASE = 500 # Lux
LUMINANCE_RANGE = 100 # Max random variation +/-

current_battery_level = INITIAL_BATTERY_LEVEL

def generate_realistic_spectral():
    """Generates slightly noisy spectral data around base values."""
    return [max(0, val + random.uniform(-SPECTRAL_NOISE, SPECTRAL_NOISE)) for val in SPECTRAL_BASE]

def generate_realistic_luminance():
    """Generates slightly noisy luminance data."""
    return max(0, LUMINANCE_BASE + random.uniform(-LUMINANCE_RANGE, LUMINANCE_RANGE))

def get_current_timestamp():
    """Gets the current Unix epoch timestamp."""
    return int(time.time())

def generate_sensor_data():
    """Generates a complete set of simulated sensor data."""
    global current_battery_level
    
    data = {
        "spectral": generate_realistic_spectral(),
        "luminance": generate_realistic_luminance(),
        "battery": round(current_battery_level, 2),
        "timestamp": get_current_timestamp()
    }
    
    # Simulate battery drain
    current_battery_level -= BATTERY_DRAIN_RATE
    if current_battery_level < 0:
        current_battery_level = 0 # Prevent negative battery level
        
    return data

if __name__ == "__main__":
    print("Starting Arduino Sensor Simulator...")
    print(f"Sending data every {TRANSMISSION_INTERVAL_SECONDS} second(s). Press Ctrl+C to stop.")
    
    try:
        while True:
            sensor_data = generate_sensor_data()
            json_output = json.dumps(sensor_data)
            
            # In a real scenario, this JSON would be sent via HaLow/GSM
            # For simulation, we just print it to the console
            print(json_output)
            
            time.sleep(TRANSMISSION_INTERVAL_SECONDS)
            
    except KeyboardInterrupt:
        print("Simulator stopped.") 
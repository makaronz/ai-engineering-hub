# Arduino Setup and Configuration

## Required Components
- Arduino Nano 33 IoT
- AS7262 Spectral Sensor
- TSL2591 Luminance Sensor
- HaLow Communication Module
- SIM800L GSM Module
- Li-Po Battery (3.7V, 2000mAh)
- Battery Charger Module
- Jumper Wires
- Breadboard (optional)

## Software Requirements
1. Arduino IDE 2.0 or later
2. Required Libraries:
   - AS7262 by SparkFun
   - Adafruit TSL2591 Library
   - ArduinoJson
   - RTCZero
   - WiFiNINA (for HaLow)
   - GSM (for SIM800L)

## Installation Steps

### 1. Install Required Libraries
1. Open Arduino IDE
2. Go to Tools > Manage Libraries
3. Install the following libraries:
   ```
   AS7262 by SparkFun
   Adafruit TSL2591 Library
   ArduinoJson
   RTCZero
   WiFiNINA
   GSM
   ```

### 2. Hardware Connections

#### AS7262 Spectral Sensor
```
AS7262 VIN -> 3.3V
AS7262 GND -> GND
AS7262 SDA -> A4
AS7262 SCL -> A5
```

#### TSL2591 Luminance Sensor
```
TSL2591 VIN -> 3.3V
TSL2591 GND -> GND
TSL2591 SDA -> A4
TSL2591 SCL -> A5
```

#### HaLow Module
```
HaLow VIN -> 3.3V
HaLow GND -> GND
HaLow TX -> RX
HaLow RX -> TX
HaLow RST -> D2
```

#### SIM800L GSM Module
```
SIM800L VIN -> 5V
SIM800L GND -> GND
SIM800L TX -> RX
SIM800L RX -> TX
SIM800L RST -> D3
```

#### Battery Monitoring
```
Battery + -> A0
Battery - -> GND
```

### 3. Software Configuration

1. Open the main.ino file in Arduino IDE
2. Configure the following parameters:
   ```cpp
   // WiFi Credentials
   const char* ssid = "your_HaLow_network_name";
   const char* password = "your_HaLow_password";
   
   // GSM Configuration
   const char* apn = "your_GSM_APN";
   const char* gprsUser = "your_GSM_username";
   const char* gprsPass = "your_GSM_password";
   ```

3. Set the transmission interval (default: 1000ms):
   ```cpp
   const unsigned long transmissionInterval = 1000;
   ```

### 4. Upload and Test

1. Connect Arduino to computer via USB
2. Select correct board: Tools > Board > Arduino Nano 33 IoT
3. Select correct port: Tools > Port > [your Arduino port]
4. Upload the code
5. Open Serial Monitor (115200 baud)
6. Verify sensor initialization and data transmission

## Calibration

### Spectral Sensor Calibration
1. Place sensor in complete darkness
2. Run calibration sequence
3. Record dark current values
4. Place sensor under known light source
5. Run calibration sequence
6. Record reference values

### Luminance Sensor Calibration
1. Place sensor under known light source
2. Measure with reference lux meter
3. Adjust gain settings if necessary
4. Record calibration values

## Troubleshooting

### Common Issues

1. **Sensor Initialization Failed**
   - Check I2C connections
   - Verify power supply
   - Check library installation

2. **Communication Issues**
   - Verify network credentials
   - Check antenna connections
   - Monitor signal strength

3. **Battery Issues**
   - Check voltage levels
   - Verify charging circuit
   - Monitor battery temperature

### Debug Mode

To enable debug mode, uncomment the following line in the code:
```cpp
#define DEBUG_MODE true
```

This will provide detailed serial output for troubleshooting.

## Maintenance

### Regular Checks
1. Battery level monitoring
2. Sensor calibration verification
3. Communication link testing
4. Firmware updates

### Safety Precautions
1. Handle Li-Po batteries with care
2. Avoid exposure to extreme temperatures
3. Keep sensors clean and protected
4. Regular inspection of connections 
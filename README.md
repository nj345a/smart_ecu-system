Smart Vehicle ECU System

Overview
The Smart Vehicle ECU System is an embedded systems project built using Arduino (ATmega328P) and ESP32. 
It is designed to simulate a basic Electronic Control Unit (ECU) used in vehicles for monitoring sensor data and taking actions based on real-time conditions.
The system monitors motion and temperature and provides alerts when abnormal behavior is detected. It also supports wireless communication 
using ESP32 for sending notifications.



Features
- Real-time motion detection using MPU6050 sensor  
- Temperature monitoring for safety conditions  
- Automatic motor/servo control based on sensor input  
- ESP32 WiFi connectivity  
- Telegram-based alert system for remote notifications  
- Serial communication between Arduino and ESP32  



Hardware Requirements
- Arduino Uno / ATmega328P  
- ESP32 module  
- MPU6050 Accelerometer + Gyroscope  
- Temperature sensor (LM35 or DHT sensor)  
- Servo motor or DC motor  
- Breadboard and jumper wires  
- Power supply (5V regulated)



Software Requirements
- Arduino IDE  
- ESP32 board support package  
- Telegram Bot API (for alerts)  
- USB drivers (CH340 or similar, if required)


Working Principle
1. Sensors continuously collect data (motion and temperature).  
2. Arduino processes the sensor values in real time.  
3. If values cross predefined thresholds (like sudden movement or high temperature), the system triggers an alert condition.  
4. The Arduino activates the required output (servo/motor control).  
5. Data is sent to the ESP32 via serial communication.  
6. ESP32 sends real-time alerts to the user via WiFi using a Telegram bot.


System Architecture
Arduino (ECU Core) → Sensor Data Processing → Decision Logic → Actuators  
                                     ↓  
                                ESP32 Module  
                                     ↓  
                        Telegram / Cloud Notifications  




 How to Run the Project
1. Upload Arduino code to ATmega328P / Arduino Uno  
2. Upload ESP32 code using Arduino IDE  
3. Connect all sensors according to the circuit diagram  
4. Configure WiFi credentials in ESP32 code  
5. Set up Telegram bot token and chat ID  
6. Power the system and test sensor outputs  


Applications
- Automotive safety monitoring systems  
- Basic ECU simulation for learning purposes  
- IoT-based vehicle tracking and alerts  
- Embedded systems and robotics projects  



Future Improvements
- Integration with GPS for location tracking  
- Cloud dashboard for real-time data visualization  
- Advanced sensor fusion for better accuracy  
- Machine learning-based anomaly detection  
- Mobile app for notifications and control  

Author
Nijil 
  


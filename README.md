# IoT-Based Centralized Real-Time Passenger Bus Monitoring System

## Overview
This project presents an IoT-based centralized real-time passenger bus monitoring system designed to improve passenger safety, emergency response, and operational efficiency in public transportation. The system integrates ESP32 microcontrollers, CAN (Controller Area Network) communication, GPS tracking, and multiple onboard sensors to enable reliable monitoring and control in automotive environments.

The architecture is divided into **Transmitter (TX)** and **Receiver (RX)** units interconnected via a CAN bus, ensuring noise-immune and fault-tolerant communication within the vehicle. Sensor data and alerts are also uploaded to a centralized IoT platform for remote supervision.

---

## Problem Statement
Traditional bus monitoring systems are largely limited to GPS tracking and lack integrated mechanisms for real-time safety monitoring and emergency response. Wireless-only communication inside vehicles is prone to interference, delay, and data loss, making it unsuitable for safety-critical applications.

There is a need for a **robust, centralized, and real-time system** that can:
- Continuously monitor onboard safety parameters
- Provide reliable in-vehicle communication
- Enable automatic emergency actions
- Offer centralized cloud-based monitoring

---

## System Architecture
The system consists of two major modules:

### Transmitter Unit (TX)
- Collects real-time data from onboard sensors
- Displays alerts locally using LCD and buzzer
- Transmits sensor data via CAN bus
- Uploads data to cloud using IoT connectivity

### Receiver Unit (RX)
- Receives CAN messages from TX unit
- Integrates GPS for real-time bus location tracking
- Controls actuators such as relay-driven water pump and servo motor
- Enables automated emergency response actions

---

## Hardware Components
- ESP32 Microcontroller
- MCP2515 CAN Controller with TJA1050 Transceiver
- GPS Module
- DHT11 Temperature & Humidity Sensor
- IR Sensor (Passenger Detection)
- Flame / Fire Sensor
- LCD Display
- Buzzer
- Relay Module
- DC Water Pump
- Servo Motor
- Regulated Power Supply

---

## Software & Tools
- Arduino IDE
- Embedded C / C++
- CAN communication libraries
- IoT cloud platform (for centralized monitoring)

---

## Key Features
- Real-time passenger and environmental monitoring
- Reliable CAN-based communication for automotive environments
- Noise-immune data transfer between ECUs
- GPS-based real-time bus tracking
- Local alert indication using LCD and buzzer
- Automated emergency control using actuators
- Centralized IoT-based data visualization

---

## Applications
- Public transportation safety systems
- Smart city transportation infrastructure
- School and college bus monitoring
- Fleet management systems
- Emergency and disaster response vehicles

---

## Advantages
- Improved passenger safety
- Fast emergency detection and response
- Fault-tolerant CAN communication
- Centralized real-time monitoring
- Scalable and cost-effective architecture
- Easy integration with existing vehicle systems

---

## Future Enhancements
- Mobile application for passengers and authorities
- AI-based analytics for predictive maintenance
- RFID or facial recognition for passenger counting
- Integration with smart traffic management systems
- Advanced sensors (gas, smoke, vibration)
- Video surveillance with edge computing
- Solar-powered modules for energy efficiency

---
## How to Run the Project

1. Connect ESP32 with MCP2515 CAN module using SPI interface.
2. Upload `CAN_Transmitter.ino` to the TX ESP32.
3. Upload `CAN_Receiver.ino` to the RX ESP32.
4. Power both units and ensure CAN_H and CAN_L are properly connected.
5. Observe sensor data on LCD and actuator response on RX side.

## Conclusion
This project demonstrates a reliable and scalable approach to intelligent public transport monitoring by combining CAN communication, IoT connectivity, GPS tracking, and sensor-based safety monitoring. The system enhances real-time visibility, emergency handling, and operational efficiency, making it suitable for modern smart transportation ecosystems.

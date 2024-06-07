Photos:
![EDUopen](https://github.com/RedstoneThomas/Green-Power-ECU-Software/assets/111314572/6f21c82b-25cb-41de-a1a7-ee9a1ea1e098)
![EDU Closed](https://github.com/RedstoneThomas/Green-Power-ECU-Software/assets/111314572/519a71ae-919d-4307-ab5a-2c0da322276b)
Arduino IDE required to operate: https://www.arduino.cc/en/software

# Circuit layout
- LCD, RTC and EEPROM connected via I2C Bus

- Switch 1 & 2 use the two interupts available

- Ardunio internal 10k ohm pullup to 5v enabled on D2 & D3 for switches

- OneWire bus uses D4, each DS18B20 is addressed with a unique address, aditional temp probes can be added to this bus, requires bus line to have a 4.7k ohm pullup to 5v

- Motor / Battery voltage measurement protected by 5.1v Zenner current limited by 100 ohm resistor potential divider resistors selected to minimise current draw 16M / 50M ohm will allow 0-24v to be detected without zenner being triggered, connected via A0/A1

- RTC accurate to 1mS could be improved

- EEPROM Used to store Lap count and individual lap times, but could be used to store other parameters

  

![image](https://github.com/edd-dunkin/Test/assets/108023882/e48c33df-7815-49db-a0cb-db6da02e4ee3)

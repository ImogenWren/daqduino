# DAQduino
 DAQ Arduino, Voltage and Current Data Aquisition Unit.
 
 ## Goal 
 DAQduino is a standalone DAQ device capable of logging data from 
a few sources to a CSV file on an SD card, sending data in realtime
to a PC, and displaying the data for a particular sensor on a screen
 
 ## Using Arduino for Data Aquisition
 
 _There are several different methods that could be used for basic Data Aquisition tasks with just a few sensors that are easy to integrate with the Arduino Platform._
 
 1. Arduino streams serial data in CSV format. 
    - Use Putty/Kitty to record log
    - Python program to log to file
    - LabVIEW program to log to file

 2. Arduino streams data in JSON format.
    - Use Putty/Kitty and export data later
    - Python to log to file

3. Arduino waits for serial message to release data packet
    - Putty/Kitty to record log
    - LabVIEW for full DAQ implementation
    - Python for basic IO Interface - command line?
    - Python Desktop App

4. Full Integration between arduino and desktop software.





### Functions:
 - 10v, 12v and 24v outputs for sensor power
 - 0 - 10v analog input
 - 0 - 5v TTL IO
 - 4 - 20mA input
 - OLED Screen
 - Encoder Control
 - Basic Scaling

#### Stretch Goals:
 - 0 - 10 v output
 - 0 - 20 mA output

### Communications:
- Serial Data
- Labveiw Integration?

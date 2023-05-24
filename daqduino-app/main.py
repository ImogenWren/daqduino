#!/usr/bin/env python3
"""
DAQduino Interface

- Open Serial commuinication port with arduino running DAQduino firmware
- Show live data streamed from DAQduino
- Display Data on Graphs
- Save Data to CSV File

"""

__author__ = "Imogen Heard"
__version__ = "0.1.0"
__license__ = "MIT"

import serial
from time import sleep

def findDACduino():
    print(f"searching for DAQduino...")
    return 14

def openSerial(com_port, baudrate = 115200):
    print(serial.__version__)
    print(f"Opening COM{com_port}")
    serial_port = serial.Serial(f"COM{com_port}", baudrate, timeout=1)
    print(serial_port)
    sleep(1)
    return serial_port


def readSerial(ref):
    while 1:
        data_list = []
        data_list.append = ref.readline()
        print(data_list)
        #if ref.in_waiting == 0:
            #break


def closeSerial(ref):
    print(f"Closing Serial Port \n {ref}")
    ref.close()

def showLiveGraph():
    print("Opening Graph Plotter")

def saveLog(filepath):
    print(f"Saving Log to Path: {filepath}")



def main():
    print("Starting DAQduino CLI")
    com_port = findDACduino()
    serial_port = openSerial(com_port)
    readSerial(serial_port)
    closeSerial(serial_port)









if __name__ == "__main__":
    main()
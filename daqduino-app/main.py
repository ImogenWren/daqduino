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
from serial.tools import list_ports

import matplotlib    #.pyplot as plt

from time import sleep

def findDACduino():
    print(f"searching for DAQduino...")
    #print(serial.tools.list_ports())
    ports = list_ports.comports()
    print(ports)
    return 14

def openSerial(com_port, baudrate = 115200):
    print(f"PySerial Version: {serial.__version__}")
    print(f"Opening COM{com_port}")
    serial_port = serial.Serial(f"COM{com_port}", baudrate, timeout=None)
    print(serial_port)
    sleep(2)
    return serial_port


def getHeader(ref):
    data_buffer = ref.readline()
    print(data_buffer)


def readSerial(ref):
    while 1:
        data_list = []
        buffer_size = ref.inWaiting()
        print(buffer_size)
        data_list.append(ref.read(buffer_size))
        #print(data_list)
        #if ref.in_waiting == 0:
        #    break



def readBuffer(ref):
    data_list = []
    #buffer_size = ref.in_waiting
    #print(f"Buffer Size: {buffer_size}")
    data_buffer = ref.readline()
    #print(data_buffer)
    return data_buffer

def decodeData(bytes_in):
    string_data = bytes_in.decode("utf-8")
    #print(string_data)
    #int_val = int.from_bytes(bytes_in,"big")
    int_val = getIntegers(string_data)
    #print(int_val)
    return int_val

def printData(data_point):


def closeSerial(ref):
    print(f"Closing Serial Port \n {ref}")
    ref.close()

def showLiveGraph():
    print("Opening Graph Plotter")

def saveLog(filepath):
    print(f"Saving Log to Path: {filepath}")


def getIntegers(string):
    numbers = [int(x) for x in string.split() if x.isnumeric()]
    try:
        number = numbers[0]
        return number
    except:
        print("No int in list")
        return None

def initGraph():
    print("Opening Graphing")

def updateGraph():
    print("Updating Graph")

def main():
    print("Starting DAQduino CLI")
    com_port = findDACduino()
    serial_port = openSerial(com_port)
    getHeader(serial_port)
    while 1:
        bytes_buffer = readBuffer(serial_port)
        data_point = decodeData(bytes_buffer)
        print(data_point)
    closeSerial(serial_port)









if __name__ == "__main__":
    main()
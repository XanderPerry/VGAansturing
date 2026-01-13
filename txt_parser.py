"""This python scripts parses .txt files over UART"""

import os
import serial
import serial.tools.list_ports
import time

def choose_from_list(question, options):
    """"This function asks for user input until a valid input from the given list is provided."""
    choice = None
    for option in options:
        print(option)

    while choice not in options:
        print(str(question))
        choice = input()
    print()
    return choice

def open_serial_port():
    """"This function asks which COM port to use and opens it."""
    ports = serial.tools.list_ports.comports()
    coms = []
    for port in ports:
        coms.append(port.device)

    com_port = choose_from_list("Please choose one of the ports listed above:", coms)

    return serial.Serial(com_port, 115200, timeout=1000)

SCRIPTS_PATH = "scripts/"

ser = open_serial_port()

script_titles = os.listdir(SCRIPTS_PATH)

file_wanted = choose_from_list("Please choose a file from the list above:", script_titles)

with open(SCRIPTS_PATH + file_wanted) as f:
    for command in f:
        print("Sent: " + command[:-1])
        ser.write(bytearray(command,'ascii'))

        received = str(ser.readline())
        error_code = received[received.find(" "):-5]
        print("Error code: " + error_code)

        # time.sleep(0.5)

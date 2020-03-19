import time
import busio
from digitalio import DigitalInOut, Direction, Pull
import board
import adafruit_ssd1306
import adafruit_rfm9x

import sys
import re

import mysql.connector
from mysql.connector import errorcode
import datetime
import math

import config

# I2C Interface
i2c = busio.I2C(board.SCL, board.SDA)

# Display
display = adafruit_ssd1306.SSD1306_I2C(128, 32, i2c, addr=0x3c)
display.fill(0)
display.show()
width = display.width
height = display.height

# LoRa Radio
CS = DigitalInOut(board.CE1)
RESET = DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 433.0)
rfm9x.tx_power = 23
prev_packet = None

# Regex for data retrieval
pattern = "\d{1,}.\d{2}"

# SQL command to add data to database
add_data = ("INSERT INTO sensor_data " 
            " (time, devID, analogue0, analogue1, analogue2, analogue3, analogue4, analogue5, bme_temperature, bme_pressure, bme_humidity, bme_gas_resistance, bme_altitude, pms_pm1_0_standard, pms_pm2_5_standard, pms_pm10_standard, pms_pm1_0_enviromental, pms_pm2_5_enviromental, pms_pm10_environmental, pms_particles_larger_than_0_3, pms_particles_larger_than_0_5, pms_particles_larger_than_1_0, pms_particles_larger_than_2_5, pms_particles_larger_than_5_0, pms_particles_larger_than_10, scd_co2, scd_temperature, scd_humidity, sgp_tvoc, sgp_h2, sgp_ethanol, si_visible_light, si_infrared, si_uv, emq_index) "
            " VALUES (%(time)s, %(devID)s, %(ana0)s, %(ana1)s, %(ana2)s, %(ana3)s, %(ana4)s, %(ana5)s, %(bmeTemp)s, %(bmePress)s, %(bmeHum)s, %(bmeRes)s, %(bmeAlt)s, %(pmsPm1S)s, %(pmsPm2S)s, %(pmsPm10S)s, %(pmsPm1E)s, %(pmsPm2E)s, %(pmsPm10E)s, %(pmsPart03)s, %(pmsPart05)s, %(pmsPart1)s, %(pmsPart2)s, %(pmsPart5)s, %(pmsPart10)s, %(scdCo2)s, %(scdTemp)s, %(scdHum)s, %(sgpTvoc)s, %(sgpH2)s, %(sgpEth)s, %(siVis)s, %(siIr)s, %(siUv)s, %(emqIn)s);")


def decode_try(packet):
    try:
        packet_text = str(packet, "utf-8")
    except:
        packet_text = "Error"
        print(str(getTime()) + " ERROR: " + str(sys.exc_info()))
    return packet_text

def getTime():
    return datetime.datetime.now()

def main():
    # Box lock variables
    waiting_for_data = False;
    expected_header = 255
    attempt = 0
    receiver_id = 99

    print(str(getTime()) + " LOG: Running data capture module")

    while True:
        time = math.floor(getTime().timestamp())
        packet = None
        display.fill(0)
        display.text('RasPi Lora', 35, 0, 1)

        packet = rfm9x.receive(timeout=99999, with_header=True)
        if packet is None:
            display.show()
            display.text("Haven't received anything", 35, 0, 1)
        else:
            try:
                display.fill(0)
                prev_packet = packet
                prev_packet = packet
                print(str(getTime()) + " LOG: Recording data")
                display.show()
                display.text("Received and working", 35, 0, 1)
                packet_text = decode_try(prev_packet[3:])
                data = re.findall(pattern, packet_text)
                print(data)
                env_data = {
                        'time': data[0],
                        'devID': data[1],
                        'ana0': data[2],
                        'ana1': data[3],
                        'ana2': data[4],
                        'ana3': data[5],
                        'ana4': data[6],
                        'ana5': data[7],
                        'bmeTemp': data[8],
                        'bmePress': data[9],
                        'bmeHum': data[10],
                        'bmeRes': data[11],
                        'bmeAlt': data[12],
                        'pmsPm1S': data[13],
                        'pmsPm2S': data[14],
                        'pmsPm10S': data[15],
                        'pmsPm1E': data[16],
                        'pmsPm2E': data[17],
                        'pmsPm10E': data[18],
                        'pmsPart03': data[19],
                        'pmsPart05': data[20],
                        'pmsPart1': data[21],
                        'pmsPart2': data[22],
                        'pmsPart5': data[23],
                        'pmsPart10': data[24],
                        'scdCo2': data[25],
                        'scdTemp': data[26],
                        'scdHum': data[27],
                        'sgpTvoc': data[28],
                        'sgpH2': data[29],
                        'sgpEth': data[30],
                        'siVis': data[31],
                        'siIr': data[32],
                        'siUv': data[33],
                        'emqIn': 0
                }
                db = mysql.connector.connect(**config.config)
                cursor = db.cursor()
                cursor.execute(add_data, env_data)
                db.commit()
                cursor.close()
                db.close()
                attempt = 5
                # Ignore noise (empty data packet)
            except IndexError:
                    continue;
        display.show()

main()

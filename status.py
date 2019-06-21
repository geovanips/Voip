#!/usr/bin/python
import asterisk.agi
import sys
import paho.mqtt.client as mqtt
import time
agi=asterisk.agi.AGI()

def on_message(client, userdata, message):
    agi.set_variable("STATUS",str(message.payload.decode("utf-8")))
    
########################################
broker_address="192.168.10.7"
client = mqtt.Client("Teste") #create new instance
client.on_message=on_message #attach function to callback
client.connect(broker_address) #connect to broker
client.loop_start() #start the loop
client.subscribe("statusrele")
client.publish("status","status")
time.sleep(3) # wait
client.loop_stop() #stop the loop

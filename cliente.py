#!/usr/bin/python
import asterisk.agi
import sys
import paho.mqtt.client as mqtt
agi=asterisk.agi.AGI()


op=sys.argv[1]

broker_address="192.168.10.7" 
#broker_address="iot.eclipse.org" #use external broker
client = mqtt.Client("Teste") #create new instance
client.connect(broker_address) #connect to broker
client.publish("rele", op)#publish


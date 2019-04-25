import paho.mqtt.client as mqtt
import time
client = mqtt.Client()

client.connect('127.0.0.1', 1883, 60)
client.publish("digging/motors/a", payload=0x20, qos=0, retain=False)
time.sleep(0.5)
client.publish("digging/motors/b", payload=0x30, qos=0, retain=False)
time.sleep(0.5)
client.publish("digging/linearActuator/a", payload=0x90, qos=0, retain=False)
time.sleep(0.5)
client.publish("digging/linearActuator/b", payload=0xA0, qos=0, retain=False)
time.sleep(0.5)

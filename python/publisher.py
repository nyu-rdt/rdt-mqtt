import paho.mqtt.client as mqtt

client = mqtt.Client()

client.connect('127.0.0.1', 1883, 60)
client.publish("hello", payload="asdf", qos=0, retain=False)

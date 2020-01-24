import paho.mqtt.client as mqtt
import time
data=”0.050*kWh”
while True:


print(data)
try:
client=mqtt.Client()
client.username_pw_set("hgztvlwh","qDz_-oAWSfBR")#replace with your user name and password
client.connect("tailor.cloudmqtt.com",	11261, 60)
client.publish("esp/slimmemeter", data)     #pi is topic
time.sleep(1)
except KeyboardInterrupt:
print("end")
client.disconnect()


# Raspberry use Py.script


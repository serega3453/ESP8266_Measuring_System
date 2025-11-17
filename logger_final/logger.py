import paho.mqtt.client as mqtt
import json
from datetime import datetime

BROKER = "192.6.0.167"   # свой IP
TOPIC = "esp8266/thermal"
LOGFILE = "thermal.log"

def on_connect(client, userdata, flags, rc, properties=None):
    print("Connected with code", rc)
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    payload_raw = msg.payload.decode(errors="replace").strip()
    ts = datetime.now().isoformat()

    # всегда логируем сырые данные
    print(f"[{ts}] TOPIC={msg.topic} RAW={payload_raw}")

    line = None
    try:
        data = json.loads(payload_raw)
        data["ts"] = ts
        line = json.dumps(data)
    except Exception:
        # не JSON — пишем как есть
        line = f'{ts} topic={msg.topic} payload={payload_raw}'

    with open(LOGFILE, "a") as f:
        f.write(line + "\n")

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, 1883, 60)
client.loop_forever()

import socket
import json
from datetime import datetime

PORT = 9500
LOGFILE = "thermal.log"

DESCR = {
    "T1": "Diode",
    "T2": "Air module",
    "T3": "M2",
    "T4": "M1",
    "SHT_T": "Overboard Temp",
    "SHT_RH": "Overboard Hum"
}

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", PORT))

print(f"Listening on UDP {PORT}")

while True:
    data, addr = sock.recvfrom(4096)
    raw = data.decode(errors="replace").strip()
    ts = datetime.now().isoformat()

    try:
        js = json.loads(raw)
        js["timestamp"] = ts

        # создаём строку с подписями
        annotated = []
        for k, v in js.items():
            if k in DESCR:
                annotated.append(f"{k} ({DESCR[k]}): {v}")
            else:
                annotated.append(f"{k}: {v}")

        line = " | ".join(annotated)
    except:
        line = f"{ts} RAW={raw}"

    print(line)

    with open(LOGFILE, "a") as f:
        f.write(line + "\n")

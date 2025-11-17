import socket
import json
from datetime import datetime

PORT = 9500
LOGFILE = "thermal.log"

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
        line = json.dumps(js)
    except:
        line = f"{ts} RAW={raw}"

    print(line)

    with open(LOGFILE, "a") as f:
        f.write(line + "\n")

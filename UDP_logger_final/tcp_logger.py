import socket
from datetime import datetime
import re

HOST = "192.6.0.205"
PORT = 8888
LOGFILE = "uart_eth.log"

DESCR = {
    "T1": "Diode",
    "T2": "Air module",
    "T3": "M2",
    "T4": "M1",
    "SHT_T": "Overboard Temp",
    "SHT_RH": "Overboard Hum"
}

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

print(f"Connected to {HOST}:{PORT}")

buffer = ""

while True:
    data = sock.recv(4096)
    if not data:
        break

    buffer += data.decode(errors="replace")

    while "\n" in buffer:
        line, buffer = buffer.split("\n", 1)
        line = line.strip()
        if not line:
            continue

        ts = datetime.now().isoformat()

        # ищем пары вида KEY: VALUE
        parts = re.findall(r"([A-Za-z0-9_]+):\s*([^|]+?)(?=\s{2,}|$|\|)", line)

        annotated = []
        for key, value in parts:
            key = key.strip()
            value = value.strip()
            if key in DESCR:
                annotated.append(f"{key} ({DESCR[key]}): {value}")
            else:
                annotated.append(f"{key}: {value}")

        final = f"{ts} | " + " | ".join(annotated)

        print(final)

        with open(LOGFILE, "a") as f:
            f.write(final + "\n")

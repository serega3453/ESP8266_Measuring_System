import socket
from datetime import datetime

HOST = "192.6.0.205"
PORT = 8888
LOGFILE = "uart_eth.log"

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

print(f"Connected to {HOST}:{PORT}")

buffer = ""  # буфер для недостающих хвостов

while True:
    data = sock.recv(4096)
    if not data:
        break

    buffer += data.decode(errors="replace")

    # Разбираем по \n
    while "\n" in buffer:
        line, buffer = buffer.split("\n", 1)
        line = line.strip()

        if not line:
            continue

        ts = datetime.now().isoformat()
        full = f"{ts} {line}"

        print(full)

        with open(LOGFILE, "a") as f:
            f.write(full + "\n")

import socket
from datetime import datetime

HOST = "192.168.5.8"
PORT = 8888
LOGFILE = "uart_eth.log"

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

print(f"Connected to {HOST}:{PORT}")

while True:
    data = sock.recv(4096)
    if not data:
        break

    raw = data.decode(errors="replace").strip()
    ts = datetime.now().isoformat()
    line = f"{ts} {raw}"

    print(line)

    with open(LOGFILE, "a") as f:
        f.write(line + "\n")

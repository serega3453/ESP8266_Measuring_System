sudo apt install mosquitto mosquitto-clients
sudo systemctl enable mosquitto
sudo systemctl start mosquitto

sudo nano /etc/mosquitto/mosquitto.conf
listener 1883 0.0.0.0
allow_anonymous true

sudo systemctl restart mosquitto

sudo netstat -tlnp | grep 1883

cd logger

python3 -m venv venv
source venv/bin/activate

pip install -r requirements.txt

python logger.py

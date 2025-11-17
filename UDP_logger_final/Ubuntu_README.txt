# Убедиться что Python установлен
python3 --version

# Если Python нет
sudo apt update
sudo apt install python3

# Создать каталог для логгера
mkdir udp_logger
cd udp_logger

# Скопировать сюда файл udp_logger.py

# Запустить логгер
python3 udp_logger.py

# Логи появятся в thermal.log

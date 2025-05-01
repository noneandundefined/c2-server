#!/bin/bash

set -e

HOME="$(git rev-parse --show-toplevel)"

sudo cp "$HOME/.sys/linux/bot.service" /etc/systemd/system/

sudo systemctl daemon-reload

sudo systemctl start bot.service
sudo systemctl status bot.service
sudo systemctl enable bot.service

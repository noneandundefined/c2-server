#!/bin/bash

set -e

# COLORS
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

HOME="$(git rev-parse --show-toplevel)"

ENV_PROD_FILE=".env.production"
ENV_FILE=".env"

if [ -f "$HOME/server/$ENV_PROD_FILE" ]; then
    echo -e "${BLUE}File $ENV_PROD_FILE already exists.${NC}"
else
    echo -e "${BLUE}Create file $ENV_PROD_FILE...${NC}"

    cd "$HOME/server"

    cat <<EOL > $ENV_PROD_FILE
SERVER_ADDR="127.0.0.1"
SERVER_PORT=49435
EOL

    echo -e "${GREEN}$ENV_PROD_FILE created!${NC}"
fi

if [ -f "$HOME/server/$ENV_FILE" ]; then
    echo -e "${BLUE}File $ENV_FILE already exists.${NC}"
else
    echo -e "${BLUE}Create file $ENV_FILE...${NC}"

    cd "$HOME/server"

    cat <<EOL > $ENV_FILE
SERVER_ADDR="127.0.0.1"
SERVER_PORT=49435
EOL

    echo -e "${GREEN}$ENV_FILE created!${NC}"
fi

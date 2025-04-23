# !/bin/bash

HOME="$(git rev-parse --show-toplevel)"

EXE_FILE="$HOME/.sys/windows/releases/icu-windows-x86_x64.setup-v1.0.0.exe"
if [ ! -f "$EXE_FILE" ]; then
  echo "Error: EXE file not found at $EXE_FILE"
  exit 1
fi

# tar -cvf "$HOME/.sys/windows/releases/icu-windows-x86_x64.setup-v1.0.0.tar" "$EXE_FILE"
# if [ $? -ne 0 ]; then
#   echo "Error: Failed to create .tar archive"
#   exit 1
# fi

zip -o "$HOME/.sys/windows/releases/icu-windows-x86_x64.setup-v1.0.0.zip" "$EXE_FILE"
if [ $? -ne 0 ]; then
  echo "Error: Failed to create .zip archive"
  exit 1
fi

echo "Files successfully compressed into .tar and .zip"

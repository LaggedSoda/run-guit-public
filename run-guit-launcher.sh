#!/bin/bash

# Get the current directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Run run-guit from the build folder relative to the script
pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY "$SCRIPT_DIR/build/run-guit"

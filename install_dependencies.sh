#!/bin/bash

set -e

echo "Installing PRISM dependencies..."

command_exists() {
	command -v "$1">/dev/null 2>&1
}

if (($EUID != 0)); then
	echo "Please run as root or with sudo" 
	exit 1 
fi

echo "Updating package lists..." apt update

#Install build essentials
echo "Installing build tools..." apt install -y build-essential cmake

echo "Installing Qt6..." apt install -y qt6-base-dev libqt6charts6-dev

#Install Google Test
echo "Installing Google Test..." apt install -y libgtest-dev

echo "All dependencies installed successfully!"

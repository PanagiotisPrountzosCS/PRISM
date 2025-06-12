#!/bin/bash

set -e

apt update
apt install -y \
	build-essential \
	cmake \
	make \
	libmosquitto-dev \
	git \
	libsdl2-dev

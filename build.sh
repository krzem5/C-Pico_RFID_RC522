#!/bin/bash
if [ ! -d "build" ]; then
	mkdir build
	cd build
	cmake ..
	cd ..
fi
cd build
make -j16&&[[ -d "$PICO_DRIVE_PATH" ]]&&cp rfid_rc522.uf2 "$PICO_DRIVE_PATH/rfid_rc522.uf2"
cd ..

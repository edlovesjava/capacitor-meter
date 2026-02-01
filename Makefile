# Makefile for capacitance meter project

.PHONY: build upload monitor sync clean arduino verify-sync all help

# Default target
help:
	@echo "Available targets:"
	@echo "  build        - Build PlatformIO project"
	@echo "  upload       - Build and upload to board"
	@echo "  monitor      - Open serial monitor"
	@echo "  sync         - Sync src/ to arduino/ folder"
	@echo "  arduino      - Build Arduino IDE version"
	@echo "  verify-sync  - Check if arduino/ is in sync"
	@echo "  clean        - Clean build artifacts"
	@echo "  all          - Build both versions"

# PlatformIO commands
build:
	pio run

upload:
	pio run -t upload

monitor:
	pio device monitor

clean:
	pio run -t clean
	rm -rf arduino/capacitance_meter_button/build

# Sync PlatformIO source to Arduino IDE folder
sync:
	python sync_arduino.py

# Build Arduino version (requires arduino-cli)
arduino: sync
	arduino-cli compile \
		--fqbn arduino:avr:nano:cpu=atmega328 \
		arduino/capacitance_meter_button

# Verify sync is up to date
verify-sync:
	@python sync_arduino.py
	@if [ -n "$$(git status --porcelain arduino/)" ]; then \
		echo "ERROR: Arduino folder is out of sync"; \
		git diff arduino/; \
		exit 1; \
	fi
	@echo "Arduino folder is in sync"

# Build both versions
all: build arduino

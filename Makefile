upload:
	platformio run -v --target upload

build:
	platformio run -v

serial:
	platformio serialports monitor

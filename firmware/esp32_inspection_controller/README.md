# ESP32 Inspection Controller

This firmware is an initial scaffold for the JAN-SAKSHYA AI field inspection controller. It has been committed as starter code; physical wiring, compile/upload and device-to-server behavior have **not** yet been verified on the team's hardware.

## Controls

- GPIO 18: Start Inspection
- GPIO 19: Evidence Submission Acknowledged
- GPIO 21: Flag for Review
- GPIO 23: buzzer control (use a suitable driver if required by the buzzer's voltage/current)

Buttons use internal pull-ups and connect to ground when pressed. Confirm the selected board pinout before wiring.

## Software dependencies

- Arduino IDE with an appropriate ESP32 board package
- ArduinoJson library (for JSON event payloads)
- WiFi and HTTPClient libraries included with the ESP32 Arduino core

## Configure before uploading

Open `esp32_inspection_controller.ino` in Arduino IDE and replace the Wi-Fi, API URL, device-key and demo complaint placeholders with values for your test environment. Do not commit real credentials. Use only a test network and simulated complaint IDs.

The API URL must point to the FastAPI host reachable from the ESP32 on the local network, and `X-Device-Key` must match the backend's `DEVICE_API_KEY`. The current sketch does not implement an offline queue: if Wi-Fi is unavailable, it prints a message and does not retain the event for later delivery.

## Important limitations

- The static complaint ID is only a demo convenience.
- The sketch sends event requests but is not a full device provisioning or production security system.
- Confirm endpoint payloads and acknowledgement handling against the running backend before demoing.
- Button events do not capture photos or prove physical repairs.
- Check electrical limits before attaching the buzzer; use an appropriate driver if necessary.

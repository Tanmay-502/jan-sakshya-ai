# ESP32 Inspection Controller

This firmware is an initial scaffold for the JAN-SAKSHYA AI field inspection controller. It is intended for bench testing and requires validation with the exact ESP32 board and buzzer.

## Controls

- GPIO 18: Start Inspection
- GPIO 19: Evidence Submission Acknowledged
- GPIO 21: Flag for Review
- GPIO 23: buzzer control (use a suitable driver if required by the buzzer's voltage/current)

Buttons use internal pull-ups and connect to ground when pressed. Confirm the selected board pinout before wiring.

## Configure before uploading

Open `esp32_inspection_controller.ino` in Arduino IDE and replace the Wi-Fi, API URL, device-key and demo complaint placeholders with values for your test environment. Do not commit real credentials. Use only a test network and simulated complaint IDs.

## Important limitations

- The static complaint ID is only a demo convenience.
- The sketch sends event requests but is not a full device provisioning or production security system.
- Confirm endpoint payloads and acknowledgement handling against the running backend before demoing.
- Button events do not capture photos or prove physical repairs.
- Check electrical limits before attaching the buzzer; use an appropriate driver if necessary.

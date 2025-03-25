# gps
This is an ESP32-WROOM-DA OTA update example using Arduino IDE.

Steps to Perform OTA Update:

1. Set Up Initial Firmware:
Open the .ino file in Arduino IDE.
Set the currentVersion to a lower version (e.g., 1.0).
Compile and upload the firmware to the ESP32.

2. Prepare Updated Firmware:
Update the currentVersion in the .ino file to a higher version (e.g., 2.0).
Compile the sketch and export the binary (.bin file).
Rename the binary file to firmware.bin.

3. Upload to GitHub:
Upload firmware.bin to your GitHub repository.
Update version.txt in the repository with 2.0.

4. Perform OTA Update on ESP32:
The ESP32 will check version.txt from GitHub.
If a higher version is available, it will download and install firmware.bin automatically.

This ensures seamless firmware updates over the air using GitHub as a remote storage server.


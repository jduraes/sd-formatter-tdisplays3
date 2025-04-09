*** DO NOT USE - IT DOES NOT WORK ***

# SD Card Formatter for LilyGo T-Display S3 AMOLED Plus

A comprehensive SD card formatting utility designed specifically for the LilyGo T-Display S3 AMOLED Plus board. This project provides a simple way to format microSD cards with detailed information display.

## Features (Current Version 0.1.0)

- SD card detection and initialization
- Detailed card information display (type, size, used space, free space)
- File listing with size information
- Complete FAT32 formatting via file deletion
- Interactive serial menu interface
- Verification of format operation
- Robust error handling

## Hardware Requirements

- **LilyGo T-Display S3 AMOLED Plus** (specifically this model, not other variants)
- MicroSD card to be formatted
- USB-C cable for connection to computer

## Project Structure

- `sd-formatter-tdisplays3.ino` - Main Arduino sketch with state machine and user interface
- `sd_operations.h` - Header file with SD card operation declarations
- `sd_operations.cpp` - Implementation of SD card operations (init, format, info)
- `README.md` - Project documentation

## Installation

### Dependencies

This project requires the following libraries:

1. **LilyGo_AMOLED Library** (essential for board configuration)
   - Download from: [GitHub - Xinyuan-LilyGO/T-Display-S3-AMOLED](https://github.com/Xinyuan-LilyGO/T-Display-S3-AMOLED)
   - Install via Arduino Library Manager: Search for "LilyGo AMOLED"
   - Or manually: Download ZIP → Arduino IDE → Sketch → Include Library → Add .ZIP Library

2. **Required Built-in Libraries** (pre-installed with Arduino IDE)
   - SPI.h - For SPI communication with SD card
   - SD.h - For SD card operations

### Arduino IDE Settings

Configure your Arduino IDE with these **exact settings**:

| Setting | Value |
|---------|-------|
| Board | "ESP32S3 Dev Module" |
| USB CDC On Boot | "Enable" |
| USB DFU On Boot | "Disable" |
| Flash Size | "16MB(128Mb)" |
| Flash Mode | "QIO 80MHz" |
| Partition Scheme | "16M Flash(3M APP/9.9MB FATFS)" |
| PSRAM | "OPI PSRAM" |
| Upload Mode | "UART0/Hardware CDC" |
| USB Mode | "Hardware CDC and JTAG" |

### Hardware Setup

1. Connect your LilyGo T-Display S3 AMOLED Plus to your computer via USB-C cable
2. Insert a microSD card into the onboard slot
3. Select the correct port in Arduino IDE (may appear as "USB Serial Device")
4. Upload the sketch

## Usage (Version 0.1.0 - Serial Interface)

### Using the Serial Menu

1. Open the Arduino Serial Monitor with settings:
   - Baud rate: 115200
   - Line ending: Newline

2. After upload, the device will:
   - Display a welcome message
   - Initialize the AMOLED display (required for pin configuration)
   - Initialize and detect the SD card
   - Show the main menu

3. The main menu offers three options:
   ```
   === MAIN MENU ===
   1. Show SD Card Information
   2. Format SD Card
   3. Reset Device
   
   Enter your choice (1-3):
   ```

4. Option 1: Show SD Card Information
   - Displays detailed card information:
     - Card type (SD, SDHC, MMC)
     - Card size in MB/GB
     - File listing (first 10 files with sizes)
     - Total file count
     - Used space
     - Free space

5. Option 2: Format SD Card
   - Asks for confirmation (y/n)
   - If confirmed, removes all files from the card
   - Shows progress percentage during formatting
   - Creates a test file to verify formatting success
   - Reports success or failure
   - Returns to main menu

6. Option 3: Reset Device
   - Restarts the ESP32 to reset all operations

### Output Example

```
====================================
SD Card Formatter for T-Display S3 AMOLED Plus
Version 0.1.0 - Serial Interface
====================================

Initializing display...
Display initialized successfully.
Board: LilyGo AMOLED T-Display S3 AMOLED

Initializing SD card...
Using SD card CS pin: 10
Initialization attempt 1... Success!

=== MAIN MENU ===
1. Show SD Card Information
2. Format SD Card
3. Reset Device

Enter your choice (1-3):
```

### Troubleshooting

If you encounter issues:

- **SD Card Not Detected**:
  - Ensure card is properly inserted in the slot
  - Try a different SD card to rule out card issues
  - Check if the card is formatted (try formatting in a computer first)

- **Display Initialization Failed**:
  - This is not critical as we only need display init for pin mapping
  - Ensure latest LilyGo_AMOLED library is installed

- **Format Operation Failed**:
  - Some cards may have write protection enabled
  - Try reinserting the card and resetting the device
  - Check serial output for specific error messages

- **Compilation Errors**:
  - Verify all libraries are correctly installed
  - Double-check Arduino IDE settings match the recommended configuration

## Development Roadmap

1. **Phase 1** ✓ - SD card operations with serial interface (Current)
   - Basic formatting functionality
   - Detailed serial output
   - Status reporting and error handling

2. **Phase 2** - AMOLED Display Interface
   - UI elements for displaying card information
   - Progress indicators for formatting
   - Status messages on display

3. **Phase 3** - Touch Input
   - Touch button implementation
   - Format type selection (FAT32, exFAT)
   - Interactive UI elements

4. **Phase 4** - Web Interface
   - WiFi access

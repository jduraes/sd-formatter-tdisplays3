/**
 * @file      sd-formatter-tdisplays3.ino
 * @brief     SD Card formatter for LilyGo T-Display S3 AMOLED Plus
 * @author    joao@duraes.com
 * @version   0.1.0
 * @date      2025-04-09
 * 
 * @note      Arduino Setting
 *            Tools ->
 *                  Board: "ESP32S3 Dev Module"
 *                  USB CDC On Boot: "Enable"
 *                  USB DFU On Boot: "Disable"
 *                  Flash Size: "16MB(128Mb)"
 *                  Flash Mode: "QIO 80MHz"
 *                  Partition Scheme: "16M Flash(3M APP/9.9MB FATFS)"
 *                  PSRAM: "OPI PSRAM"
 *                  Upload Mode: "UART0/Hardware CDC"
 *                  USB Mode: "Hardware CDC and JTAG"
 */

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <LilyGo_AMOLED.h>
#include "sd_operations.h"

// LilyGo AMOLED instance
LilyGo_Class amoled;

// Program state machine states
enum ProgramState {
  STATE_INIT,
  STATE_MENU,
  STATE_SHOW_INFO,
  STATE_FORMAT_CONFIRM,
  STATE_FORMATTING,
  STATE_FORMAT_COMPLETE,
  STATE_ERROR
};

// Current program state
ProgramState currentState = STATE_INIT;

// Error message for display
String errorMessage = "";

// Function prototypes
bool initDisplay();
void showMenu();
void processMenuInput();
void clearSerialBuffer();
void showHeader();

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Wait for serial monitor to open
  delay(3000);
  
  showHeader();
  
  // Initialize the AMOLED board (for correct pin mapping)
  bool displayInitialized = initDisplay();
  if (!displayInitialized) {
    Serial.println("WARNING: Display initialization failed.");
    Serial.println("Continuing with SD operations only...");
  }
  
  // Initialize SD card
  bool sdInitialized = initSDCard();
  if (!sdInitialized) {
    errorMessage = "SD card initialization failed. Please check card insertion.";
    currentState = STATE_ERROR;
    return;
  }
  
  // Transition to menu state
  currentState = STATE_MENU;
  showMenu();
}

void loop() {
  // State machine to handle different program states
  switch (currentState) {
    case STATE_INIT:
      // Should not reach here, initialization is done in setup()
      break;
      
    case STATE_MENU:
      if (Serial.available()) {
        processMenuInput();
      }
      break;
      
    case STATE_SHOW_INFO:
      showSDCardInfo();
      delay(1000);
      Serial.println("\nPress any key to return to menu...");
      clearSerialBuffer();
      while (!Serial.available()) {
        delay(100);
      }
      clearSerialBuffer();
      currentState = STATE_MENU;
      showMenu();
      break;
      
    case STATE_FORMAT_CONFIRM:
      Serial.println("\n*** WARNING: All data on the SD card will be lost! ***");
      Serial.println("Are you sure you want to format the SD card? (y/n)");
      
      clearSerialBuffer();
      char confirm = 0;
      while (true) {
        if (Serial.available()) {
          confirm = Serial.read();
          clearSerialBuffer();
          
          if (confirm == 'y' || confirm == 'Y') {
            currentState = STATE_FORMATTING;
            break;
          } else if (confirm == 'n' || confirm == 'N') {
            Serial.println("\nFormat cancelled.");
            delay(1000);
            currentState = STATE_MENU;
            showMenu();
            break;
          }
        }
        delay(100);
      }
      break;
      
    case STATE_FORMATTING:
      Serial.println("\nStarting SD card format...");
      delay(500);
      
      bool formatSuccess = formatSDCard();
      
      if (formatSuccess) {
        Serial.println("\nSD Card formatting completed successfully!");
        currentState = STATE_FORMAT_COMPLETE;
      } else {
        errorMessage = "Format operation failed. Please check SD card and try again.";
        currentState = STATE_ERROR;
      }
      
      delay(2000);
      currentState = STATE_MENU;
      showMenu();
      break;
      
    case STATE_FORMAT_COMPLETE:
      // Nothing special to do here, already handled in STATE_FORMATTING
      currentState = STATE_MENU;
      break;
      
    case STATE_ERROR:
      Serial.println("\n*** ERROR ***");
      Serial.println(errorMessage);
      Serial.println("\nPress any key to return to menu...");
      
      clearSerialBuffer();
      while (!Serial.available()) {
        delay(100);
      }
      clearSerialBuffer();
      
      // Try to return to menu if possible
      currentState = STATE_MENU;
      showMenu();
      break;
  }
  
  // Small delay to prevent CPU hogging
  delay(50);
}

// Initialize the display for pin mapping
bool initDisplay() {
  Serial.println("Initializing display...");
  
  bool result = false;
  
  // Try display initialization with error handling
  try {
    result = amoled.begin();
    if (result) {
      Serial.println("Display initialized successfully.");
      Serial.print("Board: LilyGo AMOLED ");
      Serial.println(amoled.getName());
    } else {
      Serial.println("Display initialization failed!");
    }
  } catch (...) {
    Serial.println("Exception during display initialization");
    result = false;
  }
  
  return result;
}

// Show program header
void showHeader() {
  Serial.println("\n\n====================================");
  Serial.println("SD Card Formatter for T-Display S3 AMOLED Plus");
  Serial.println("Version 0.1.0 - Serial Interface");
  Serial.println("====================================\n");
}

// Display the main menu
void showMenu() {
  Serial.println("\n=== MAIN MENU ===");
  Serial.println("1. Show SD Card Information");
  Serial.println("2. Format SD Card");
  Serial.println("3. Reset Device");
  Serial.println("\nEnter your choice (1-3):");
}

// Process menu selection
void processMenuInput() {
  char choice = Serial.read();
  clearSerialBuffer();
  
  switch (choice) {
    case '1':
      Serial.println("\nShowing SD Card Information...");
      currentState = STATE_SHOW_INFO;
      break;
      
    case '2':
      Serial.println("\nPreparing to format SD Card...");
      currentState = STATE_FORMAT_CONFIRM;
      break;
      
    case '3':
      Serial.println("\nResetting device...");
      delay(1000);
      ESP.restart();
      break;
      
    default:
      Serial.println("\nInvalid choice. Please try again.");
      showMenu();
      break;
  }
}

// Clear the serial input buffer
void clearSerialBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}

/**
 * Next development phases:
 * 
 * Phase 2: Implement AMOLED Display Interface
 * - Add basic UI elements for displaying card info
 * - Show formatting progress on display
 * - Create simple status indicators
 * 
 * Phase 3: Add Touch Input Support
 * - Create touchable buttons for menu navigation 
 * - Implement format type selection (FAT32, exFAT)
 * - Add confirmation dialogs
 * 
 * Phase 4: Create Web Interface
 * - Set up WiFi connection (AP mode)
 * - Implement web server
 * - Create web UI for remote formatting
 * - Enable format type selection via web
 */

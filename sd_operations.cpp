/**
 * @file      sd_operations.cpp
 * @brief     SD card operations implementation
 * @author    joao@duraes.com
 * @version   0.2.0
 * @date      2025-04-09
 */

#include "sd_operations.h"

// External SD Card CS pin defined in main sketch
extern const int SD_CS;

/**
 * Initialize the SD card
 * Uses ESP32-S3 pin configuration
 */
bool initSDCard() {
  Serial.println("Initializing SD card...");
  
  Serial.print("Using SD card CS pin: ");
  Serial.println(SD_CS);
  
  // Attempt to initialize SD card with retries
  bool sdInitialized = false;
  for (int i = 0; i < 3; i++) {
    Serial.print("Initialization attempt ");
    Serial.print(i + 1);
    Serial.print("... ");
    
    if (SD.begin(SD_CS)) {
      sdInitialized = true;
      Serial.println("Success!");
      break;
    }
    
    Serial.println("Failed!");
    delay(500);
  }
  
  return sdInitialized;
}

/**
 * Get the SD card size in MB
 */
uint64_t getSDCardSize() {
  return SD.cardSize() / (1024 * 1024);
}

/**
 * Show SD card information via serial output
 */
void showSDCardInfo() {
  uint64_t cardSize = getSDCardSize();
  uint64_t usedSpace = 0;
  uint64_t totalSpace = cardSize * 1024 * 1024; // Convert to bytes
  
  Serial.println("\n=== SD Card Information ===");
  
  // Card type
  uint8_t cardType = SD.cardType();
  Serial.print("Card Type: ");
  if (cardType == CARD_NONE) {
    Serial.println("No card detected");
    return;
  } else if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SD");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("Unknown");
  }
  
  // Card size
  Serial.print("Card Size: ");
  if (cardSize < 1024) {
    Serial.print(cardSize);
    Serial.println(" MB");
  } else {
    Serial.print(cardSize / 1024.0, 2);
    Serial.println(" GB");
  }
  
  // File system details
  Serial.println("\nScanning root directory...");
  
  File root = SD.open("/");
  if (!root) {
    Serial.println("Failed to open root directory!");
    return;
  }
  
  if (!root.isDirectory()) {
    Serial.println("Root is not a directory!");
    return;
  }
  
  // Count files
  int fileCount = 0;
  uint64_t totalBytes = 0;
  
  File entry = root.openNextFile();
  while (entry) {
    if (!entry.isDirectory()) {
      fileCount++;
      totalBytes += entry.size();
      
      // Show file details for the first 10 files
      if (fileCount <= 10) {
        Serial.print("  ");
        Serial.print(entry.name());
        Serial.print(": ");
        
        // Format the file size
        if (entry.size() < 1024) {
          Serial.print(entry.size());
          Serial.println(" B");
        } else if (entry.size() < 1024 * 1024) {
          Serial.print(entry.size() / 1024.0, 2);
          Serial.println(" KB");
        } else {
          Serial.print(entry.size() / (1024.0 * 1024.0), 2);
          Serial.println(" MB");
        }
      }
    }
    entry.close();
    entry = root.openNextFile();
  }
  
  // Show summary
  usedSpace = totalBytes;
  
  Serial.print("\nTotal files: ");
  Serial.println(fileCount);
  
  Serial.print("Used space: ");
  if (usedSpace < 1024) {
    Serial.print(usedSpace);
    Serial.println(" B");
  } else if (usedSpace < 1024 * 1024) {
    Serial.print(usedSpace / 1024.0, 2);
    Serial.println(" KB");
  } else if (usedSpace < 1024 * 1024 * 1024) {
    Serial.print(usedSpace / (1024.0 * 1024.0), 2);
    Serial.println(" MB");
  } else {
    Serial.print(usedSpace / (1024.0 * 1024.0 * 1024.0), 2);
    Serial.println(" GB");
  }
  
  Serial.print("Free space: ");
  uint64_t freeSpace = totalSpace - usedSpace;
  if (freeSpace < 1024) {
    Serial.print(freeSpace);
    Serial.println(" B");
  } else if (freeSpace < 1024 * 1024) {
    Serial.print(freeSpace / 1024.0, 2);
    Serial.println(" KB");
  } else if (freeSpace < 1024 * 1024 * 1024) {
    Serial.print(freeSpace / (1024.0 * 1024.0), 2);
    Serial.println(" MB");
  } else {
    Serial.print(freeSpace / (1024.0 * 1024.0 * 1024.0), 2);
    Serial.println(" GB");
  }
  
  root.close();
}

/**
 * Format the SD card
 */
bool formatSDCard() {
  Serial.println("\n=== Starting SD Card Format ===");
  
  // End the current SD card instance to release the card
  Serial.println("Releasing SD card...");
  SD.end();
  delay(500);
  
  // Reinitialize the SD card
  Serial.println("Reinitializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("Failed to reinitialize SD card");
    return false;
  }
  
  // Format the card by removing all files
  Serial.println("Removing all files...");
  File root = SD.open("/");
  if (!root) {
    Serial.println("Failed to open root directory");
    return false;
  }
  
  // Count files for progress reporting
  int fileCount = 0;
  int filesRemoved = 0;
  
  File countFile = root.openNextFile();
  while (countFile) {
    if (!countFile.isDirectory()) {
      fileCount++;
    }
    countFile.close();
    countFile = root.openNextFile();
  }
  root.close();
  
  Serial.print("Found ");
  Serial.print(fileCount);
  Serial.println(" files to remove");
  
  // Reopen root directory to remove files
  root = SD.open("/");
  if (!root) {
    Serial.println("Failed to reopen root directory");
    return false;
  }
  
  File file = root.openNextFile();
  while (file) {
    if (!file.isDirectory()) {
      String filename = file.name();
      file.close();
      
      Serial.print("Removing: ");
      Serial.print(filename);
      
      // Progress as percentage
      float percentComplete = (float)filesRemoved / fileCount * 100.0;
      Serial.print(" (");
      Serial.print(percentComplete, 1);
      Serial.print("%)... ");
      
      if (SD.remove("/" + filename)) {
        Serial.println("Success");
        filesRemoved++;
      } else {
        Serial.println("Failed");
      }
    } else {
      file.close();
    }
    
    file = root.openNextFile();
  }
  root.close();
  
  // Summary
  Serial.print("\nRemoved ");
  Serial.print(filesRemoved);
  Serial.print(" of ");
  Serial.print(fileCount);
  Serial.println(" files");
  
  // Create a test file to ensure formatting worked
  Serial.println("\nVerifying format by creating test file...");
  File testFile = SD.open("/format_test.txt", FILE_WRITE);
  if (!testFile) {
    Serial.println("Failed to create test file");
    return false;
  }
  
  testFile.println("SD Card formatted successfully on " + String(millis()));
  testFile.close();
  
  // Verify the test file exists and is readable
  testFile = SD.open("/format_test.txt");
  if (!testFile) {
    Serial.println("Failed to verify test file");
    return false;
  }
  
  String testContent = testFile.readString();
  testFile.close();
  
  if (testContent.length() > 0) {
    Serial.println("Test file created and verified successfully");
  } else {
    Serial.println("Test file verification failed - file is empty");
    return false;
  }
  
  // Check if any files remain (should just be our test file)
  root = SD.open("/");
  if (!root) {
    Serial.println("Failed to open root directory for verification");
    return false;
  }
  
  int remainingFiles = 0;
  File verifyFile = root.openNextFile();
  while (verifyFile) {
    if (!verifyFile.isDirectory()) {
      remainingFiles++;
      Serial.print("Remaining file: ");
      Serial.println(verifyFile.name());
    }
    verifyFile.close();
    verifyFile = root.openNextFile();
  }
  root.close();
  
  if (remainingFiles > 1) {
    Serial.println("WARNING: Some files could not be removed during formatting");
  } else {
    Serial.println("All files removed except the test file");
  }
  
  return true;
}

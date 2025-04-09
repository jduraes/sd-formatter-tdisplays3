/**
 * @file      sd_operations.h
 * @brief     SD card operations for serial-based formatter
 * @author    joao@duraes.com
 * @version   0.2.0
 * @date      2025-04-09
 */

#ifndef SD_OPERATIONS_H
#define SD_OPERATIONS_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

/**
 * Initialize the SD card
 * Uses fixed SD_CS pin defined in main sketch
 * @return true if SD card initialized successfully, false otherwise
 */
bool initSDCard();

/**
 * Format the SD card by removing all files
 * Creates a test file to verify formatting
 * @return true if formatting completed successfully, false otherwise
 */
bool formatSDCard();

/**
 * Show detailed SD card information via serial output
 * Includes card type, size, file count, used space, free space
 */
void showSDCardInfo();

/**
 * Get the SD card size in megabytes
 * @return Card size in MB
 */
uint64_t getSDCardSize();

// External SD Card CS pin
extern const int SD_CS;

#endif // SD_OPERATIONS_H

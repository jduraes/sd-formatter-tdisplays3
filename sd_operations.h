/**
 * @file      sd_operations.h
 * @brief     SD card operations for T-Display S3 AMOLED Plus
 * @author    joao@duraes.com
 * @version   0.1.0
 * @date      2025-04-09
 */

#ifndef SD_OPERATIONS_H
#define SD_OPERATIONS_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <LilyGo_AMOLED.h>

/**
 * Initialize the SD card
 * Uses board configuration from LilyGo_AMOLED library for pin mapping
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
 * Display detailed SD card information
 * Includes card type, size, file count, used space, free space
 */
void showSDCardInfo();

/**
 * Get the SD card size in megabytes
 * @return Card size in MB
 */
uint64_t getSDCardSize();

// External variables
extern const BoardsConfigure_t* boards;

#endif // SD_OPERATIONS_H

/**
 * @file      sd_operations.h
 * @brief     SD card operations for T-Display S3 AMOLED Plus
 * @author    joao@duraes.com
 * @version   0.1.0
 * @date      2025-04-09
 */

#ifndef SD_OPERATIONS_H
#define SD_OPERATIONS_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <LilyGo_AMOLED.h>

// Function prototypes
bool initSDCard();
bool formatSDCard();
void showSDCardInfo();
uint64_t getSDCardSize();
extern const BoardsConfigure_t* boards;

#endif // SD_OPERATIONS_H


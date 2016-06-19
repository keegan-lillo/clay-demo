/**
 * This file was generated with Enamel : http://gregoiresage.github.io/enamel
 */

#ifndef ENAMEL_H
#define ENAMEL_H

#include <pebble.h>

// -----------------------------------------------------
// Getter for 'BACKGROUND_COLOR'
GColor enamel_get_BACKGROUND_COLOR();
// -----------------------------------------------------

// -----------------------------------------------------
// Getter for 'TEXT_COLOR'
GColor enamel_get_TEXT_COLOR();
// -----------------------------------------------------

void enamel_init();

void enamel_deinit();

typedef void(EnamelSettingsReceivedCallback)(void);

void enamel_register_settings_received(EnamelSettingsReceivedCallback *callback);

#endif
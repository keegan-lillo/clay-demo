/**
 * This file was generated with Enamel : http://gregoiresage.github.io/enamel
 */

#include <pebble.h>
#include <pebble-events/pebble-events.h>
#include "enamel.h"

#ifndef ENAMEL_MAX_STRING_LENGTH
#define ENAMEL_MAX_STRING_LENGTH 30
#endif

#define ENAMEL_PKEY 100000000
#define ENAMEL_VALUE_PKEY ENAMEL_PKEY + 10

static EventHandle s_event_handle;
static EnamelSettingsReceivedCallback *s_settings_received_callback; 

static bool s_config_changed;

// -----------------------------------------------------
// Getter for 'BACKGROUND_COLOR'
static GColor s_BACKGROUND_COLOR;
GColor enamel_get_BACKGROUND_COLOR(){
	return s_BACKGROUND_COLOR;
}
static void prv_set_BACKGROUND_COLOR(uint8_t value){
	s_BACKGROUND_COLOR.argb = value;
}
// -----------------------------------------------------

// -----------------------------------------------------
// Getter for 'TEXT_COLOR'
static GColor s_TEXT_COLOR;
GColor enamel_get_TEXT_COLOR(){
	return s_TEXT_COLOR;
}
static void prv_set_TEXT_COLOR(uint8_t value){
	s_TEXT_COLOR.argb = value;
}
// -----------------------------------------------------


static uint16_t prv_get_inbound_size() {
	return 1
+ 7 + 4
+ 7 + 4
;
}

static void inbox_received_handle(DictionaryIterator *iter, void *context) {
	Tuple *tuple = NULL;
	bool changed = false;
	tuple = dict_find(iter, MESSAGE_KEY_BACKGROUND_COLOR);
	changed = changed || tuple;
	tuple ? prv_set_BACKGROUND_COLOR(GColorFromHEX(tuple->value->int32).argb) : false;
	tuple = dict_find(iter, MESSAGE_KEY_TEXT_COLOR);
	changed = changed || tuple;
	tuple ? prv_set_TEXT_COLOR(GColorFromHEX(tuple->value->int32).argb) : false;
	s_config_changed |= changed;
	if(changed && s_settings_received_callback){
		s_settings_received_callback();
	}
}

static uint16_t prv_save_generic_data(uint32_t startkey, const void *data, uint16_t size){
	uint16_t offset = 0;
	uint16_t total_w_bytes = 0;
	uint16_t w_bytes = 0;
	while(offset < size){
		w_bytes = persist_write_data(startkey + offset / PERSIST_DATA_MAX_LENGTH, data + offset, PERSIST_DATA_MAX_LENGTH);
		total_w_bytes += w_bytes;
		offset += PERSIST_DATA_MAX_LENGTH;
	}
	return total_w_bytes; 
}

static uint16_t prv_load_generic_data(uint32_t startkey, void *data, uint16_t size){
	uint16_t offset = 0;
	uint16_t total_r_bytes = 0;
	uint16_t expected_r_bytes = 0;
	uint16_t r_bytes = 0;
	while(offset < size){
		if(size - offset > PERSIST_DATA_MAX_LENGTH){
			expected_r_bytes = PERSIST_DATA_MAX_LENGTH;
		}
		else {
			expected_r_bytes = size - offset;
		}
		r_bytes = persist_read_data(startkey + offset / PERSIST_DATA_MAX_LENGTH, data + offset, expected_r_bytes);
		total_r_bytes += r_bytes;
		if(r_bytes != expected_r_bytes){
			break; 
		}
		offset += PERSIST_DATA_MAX_LENGTH;
	}
	return total_r_bytes;
}

static uint32_t prv_get_pkey(uint32_t *hashes_, uint16_t size_, uint32_t hash_){
	for(uint16_t i=0; i<size_; i++){
		if(hashes_[i] == hash_)
			return ENAMEL_VALUE_PKEY + i;
	}
	return 0;
}


void enamel_init(){
	uint16_t key_size = 0;
	uint32_t *hashes = NULL;
	if(persist_exists(ENAMEL_PKEY)) 
	{
		key_size = persist_read_int(ENAMEL_PKEY);
		hashes = malloc(key_size * sizeof(uint32_t));
		if(hashes) {
			prv_load_generic_data(ENAMEL_PKEY + 1, hashes, key_size * sizeof(uint32_t));
		}
		else {
			key_size = 0;
		}
	}
	
	uint32_t pkey = 0;
	pkey = prv_get_pkey(hashes, key_size, 2420576764);
	if (pkey && persist_exists(pkey)) {
		prv_set_BACKGROUND_COLOR(persist_read_int(pkey));
	}
	else {
		prv_set_BACKGROUND_COLOR(GColorFromHEX(0x000055).argb);
	}
	pkey = prv_get_pkey(hashes, key_size, 1726428591);
	if (pkey && persist_exists(pkey)) {
		prv_set_TEXT_COLOR(persist_read_int(pkey));
	}
	else {
		prv_set_TEXT_COLOR(GColorFromHEX(0xffffff).argb);
	}

	s_config_changed = false;
	s_settings_received_callback = NULL;
	s_event_handle = events_app_message_register_inbox_received(inbox_received_handle, NULL);
	events_app_message_request_inbox_size(prv_get_inbound_size());

	if(hashes){
		free(hashes);
	}
}

void enamel_deinit(){
	if(s_config_changed){
	uint32_t pkey = ENAMEL_VALUE_PKEY;
	uint16_t num_keys = 0;
	uint32_t keys[2];
	
	keys[num_keys++] = 2420576764;
	persist_write_int(pkey++, s_BACKGROUND_COLOR.argb);
	keys[num_keys++] = 1726428591;
	persist_write_int(pkey++, s_TEXT_COLOR.argb);

	persist_write_int(ENAMEL_PKEY, num_keys);
	prv_save_generic_data(ENAMEL_PKEY + 1, keys, num_keys * sizeof(uint32_t));
	}


	s_config_changed = false;
	events_app_message_unsubscribe(s_event_handle);
	s_settings_received_callback = NULL;
}

void enamel_register_settings_received(EnamelSettingsReceivedCallback *callback){
	s_settings_received_callback = callback;
}

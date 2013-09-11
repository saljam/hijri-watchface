#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "src/resource_ids.auto.h"
#include "hijri.h"

#define MY_UUID { 0xAF, 0x59, 0x80, 0xFF, 0x41, 0xFA, 0x44, 0x43, 0x85, 0x9F, 0x99, 0xEE, 0xD9, 0x9E, 0x51, 0xA2 }
PBL_APP_INFO(MY_UUID,
	"Hijri", "0x65.net",
	1, 0,
	DEFAULT_MENU_ICON,
	APP_INFO_WATCH_FACE);

Window window;
TextLayer hijriLayer;
TextLayer dateLayer;
TextLayer timeLayer;

void handle_tick(AppContextRef ctx, PebbleTickEvent *t)
{
	HijriDate hijri = unix2hijri(time(NULL));
	static char hijriText[] = "Jumada al-awwal 16";
	snprintf(hijriText, sizeof(hijriText), "%s %d", hijriMonths[hijri.month], hijri.day);	
	text_layer_set_text(&hijriLayer, hijriText);
	
	PblTm now;
	get_time(&now);
	static char timeText[] = "00:00";
	string_format_time(timeText, sizeof(timeText), "%H:%M", &now);
	text_layer_set_text(&timeLayer, timeText);
	
	static char dateText[] = "September 15";
	string_format_time(dateText, sizeof(dateText), "%B %e", &now);
	text_layer_set_text(&dateLayer, dateText);
}

void handle_init(AppContextRef ctx)
{
	window_init(&window, "Hijri");
	window_stack_push(&window, true);
	window_set_background_color(&window, GColorBlack);
	
	resource_init_current_app(&APP_RESOURCES);
	
	text_layer_init(&hijriLayer, GRect(8, 20, 144-8, 168-68));
	text_layer_set_text_color(&hijriLayer, GColorWhite);
	text_layer_set_background_color(&hijriLayer, GColorClear);
	text_layer_set_font(&hijriLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
	layer_add_child(&window.layer, &hijriLayer.layer);
	
	text_layer_init(&timeLayer, GRect(8, 50, 144-8, 168-92));
	text_layer_set_text_color(&timeLayer, GColorWhite);
	text_layer_set_background_color(&timeLayer, GColorClear);
	text_layer_set_font(&timeLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49)));
	layer_add_child(&window.layer, &timeLayer.layer);
	
	text_layer_init(&dateLayer, GRect(8, 115, 144-8, 168-92));
	text_layer_set_text_color(&dateLayer, GColorWhite);
	text_layer_set_background_color(&dateLayer, GColorClear);
	text_layer_set_font(&dateLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
	layer_add_child(&window.layer, &dateLayer.layer);
	
	handle_tick(ctx, NULL);
}

void pbl_main(void *params)
{
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init,
		.tick_info = {
			.tick_handler = &handle_tick,
			.tick_units = MINUTE_UNIT
		}
	};
	app_event_loop(params, &handlers);
}

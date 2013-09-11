#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "layout.h"

Window window;
TextLayer hijriLayer;
TextLayer dateLayer;
TextLayer timeLayer;

void init_layout()
{
	window_init(&window, "Hijri");
	window_stack_push(&window, true);
	window_set_background_color(&window, GColorBlack);
	
	text_layer_init(&hijriLayer, GRect(8, 20, 144-8, 168-68));
	text_layer_set_text_color(&hijriLayer, GColorWhite);
	text_layer_set_background_color(&hijriLayer, GColorClear);
	text_layer_set_font(&hijriLayer,
		fonts_load_custom_font(
			resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
	layer_add_child(&window.layer, &hijriLayer.layer);
	
	text_layer_init(&timeLayer, GRect(8, 50, 144-8, 168-92));
	text_layer_set_text_color(&timeLayer, GColorWhite);
	text_layer_set_background_color(&timeLayer, GColorClear);
	text_layer_set_font(&timeLayer,
		fonts_load_custom_font(
			resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49)));
	layer_add_child(&window.layer, &timeLayer.layer);
	
	text_layer_init(&dateLayer, GRect(8, 115, 144-8, 168-92));
	text_layer_set_text_color(&dateLayer, GColorWhite);
	text_layer_set_background_color(&dateLayer, GColorClear);
	text_layer_set_font(&dateLayer,
		fonts_load_custom_font(
			resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
	layer_add_child(&window.layer, &dateLayer.layer);
	
}

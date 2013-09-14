#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "out/res/resource_ids.auto.h"
#include "layout.h"

#define WIDTH 144
#define HEIGHT 168

Window window;
TextLayer hijriLayer;
TextLayer dateLayer;
TextLayer timeLayer;

void init_layout()
{
	window_init(&window, "Hijri");
	window_stack_push(&window, true);
	window_set_background_color(&window, GColorBlack);
	
	int margin = 8;
	int top = 10;
	int width = WIDTH-2*margin;
	int dateHeight = 32;
	int timeHeight = 80;
	
	text_layer_init(&hijriLayer, GRect(margin, top, width, dateHeight));
	text_layer_set_text_alignment(&hijriLayer, GTextAlignmentRight);
	text_layer_set_text_color(&hijriLayer, GColorWhite);
	text_layer_set_background_color(&hijriLayer, GColorClear);
	text_layer_set_font(&hijriLayer,
		fonts_load_custom_font(
			resource_get_handle(RESOURCE_ID_FONT_KACSTBOOK_26)));
	layer_add_child(&window.layer, &hijriLayer.layer);
	
	text_layer_init(&timeLayer, GRect(margin, top+dateHeight, width, timeHeight));
	text_layer_set_text_alignment(&timeLayer, GTextAlignmentCenter);
	text_layer_set_text_color(&timeLayer, GColorWhite);
	text_layer_set_background_color(&timeLayer, GColorClear);
	text_layer_set_font(&timeLayer,
		fonts_load_custom_font(
			resource_get_handle(RESOURCE_ID_FONT_KACSTBOOK_SUBSET_55)));
	layer_add_child(&window.layer, &timeLayer.layer);
	
	text_layer_init(&dateLayer, GRect(margin, top+dateHeight+timeHeight, width, dateHeight));
	text_layer_set_text_color(&dateLayer, GColorWhite);
	text_layer_set_background_color(&dateLayer, GColorClear);
	text_layer_set_font(&dateLayer,
		fonts_load_custom_font(
			resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
	layer_add_child(&window.layer, &dateLayer.layer);
	
}

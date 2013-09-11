#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "hijri.h"
#include "layout.h"

#define MY_UUID { 0xAF, 0x59, 0x80, 0xFF, 0x41, 0xFA, 0x44, 0x43, 0x85, 0x9F, 0x99, 0xEE, 0xD9, 0x9E, 0x51, 0xA2 }
PBL_APP_INFO(MY_UUID,
	"Hijri", "0x65.net",
	1, 0,
	DEFAULT_MENU_ICON,
	APP_INFO_WATCH_FACE);

char hijriText[] = "Jumada al-awwal 16";
char dateText[] = "September 15";
char timeText[] = "00:00";

void updateHijri(AppContextRef ctx, int t)
{
	HijriDate hijri = unix2hijri(t);
	snprintf(hijriText, sizeof(hijriText), "%s %d", hijriMonths[hijri.month], hijri.day);	
	text_layer_set_text(&hijriLayer, hijriText);
}

void updateGregorian(AppContextRef ctx, PblTm *t)
{
	string_format_time(dateText, sizeof(dateText), "%B %e", t);
	text_layer_set_text(&dateLayer, dateText);
}

void updateTime(AppContextRef ctx, PblTm *t)
{
	string_format_time(timeText, sizeof(timeText), "%H:%M", t);
	text_layer_set_text(&timeLayer, timeText);
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *t)
{
	updateTime(ctx, t->tick_time);
	// Only update the date on a new day
	if (t->tick_time->tm_hour == 0 && t->tick_time->tm_min == 0) {
		updateGregorian(ctx, t->tick_time);
		updateHijri(ctx, time(NULL));
	}
}

void handle_init(AppContextRef ctx)
{
	resource_init_current_app(&APP_RESOURCES);
	
	init_layout();
	
	PblTm now;
	get_time(&now);
	updateTime(ctx, &now);
	updateGregorian(ctx, &now);
	updateHijri(ctx, time(NULL));
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

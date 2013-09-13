#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "hijri.h"
#include "src/resource_ids.auto.h"
#include "layout.h"

#define MY_UUID { 0xAF, 0x59, 0x80, 0xFF, 0x41, 0xFA, 0x44, 0x43, 0x85, 0x9F, 0x99, 0xEE, 0xD9, 0x9E, 0x51, 0xA2 }
PBL_APP_INFO(MY_UUID,
	"Hijri", "0x65.net",
	1, 0,
	DEFAULT_MENU_ICON,
	APP_INFO_WATCH_FACE);

#define LABEL_LEN 128

char timeTxt[LABEL_LEN];
char arTimeTxt[LABEL_LEN];
char hijriTxt[LABEL_LEN];
char arHijriTxt[LABEL_LEN];
char dateTxt[LABEL_LEN];

void updateHijri(AppContextRef ctx, int t)
{
	HijriDate hijri = unix2hijri(t);
	
	snprintf(hijriTxt, sizeof(hijriTxt), "%d %s", hijri.day, hijriMonths[hijri.month]);
	
	shape(hijriTxt, arHijriTxt, sizeof(arHijriTxt));
	
	text_layer_set_text(&hijriLayer, arHijriTxt);
}

void updateGregorian(AppContextRef ctx, PblTm *t)
{
	string_format_time(dateTxt, sizeof(dateTxt), "%B %e", t);
	text_layer_set_text(&dateLayer, dateTxt);
}

void updateTime(AppContextRef ctx, PblTm *t)
{
	string_format_time(timeTxt, sizeof(timeTxt), "%H:%M", t);
	shape(timeTxt, arTimeTxt, sizeof(arTimeTxt));
	text_layer_set_text(&timeLayer, arTimeTxt);
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

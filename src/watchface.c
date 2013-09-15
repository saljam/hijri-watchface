#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#include "pebble_os.h"
#include "pebble_app.h"
#include "gen/resource_ids.auto.h"

#include "hijri.h"
#include "layout.h"
#include "ConvertUTF.h"

#define MY_UUID { 0xAF, 0x59, 0x80, 0xFF, 0x41, 0xFA, 0x44, 0x43, 0x85, 0x9F, 0x99, 0xEE, 0xD9, 0x9E, 0x51, 0xA2 }
PBL_APP_INFO(MY_UUID,
	"Hijri", "0x65.net",
	1, 0,
	DEFAULT_MENU_ICON,
	APP_INFO_WATCH_FACE);

#define LABEL_LEN 128
#define INTWIDTH 4

wchar_t timeTxt[LABEL_LEN];
char utfTimeTxt[LABEL_LEN];
wchar_t hijriTxt[LABEL_LEN];
char utfHijriTxt[LABEL_LEN];
char dateTxt[LABEL_LEN];
char debugTxt[LABEL_LEN];

int wifmt(wchar_t *out, int d, int zeros)
{
	wchar_t *conv = L"0123456789";
	int n = 0;
	wchar_t buf[INTWIDTH + 1], *p;
	p = buf + INTWIDTH;
	
	*p-- = L'\0';
	while (d) {
		int i = d % 10;
		d /= 10;
		*p-- = conv[i];
		n++;
		zeros--;
	}
	while (zeros > 0) {
		*p-- = conv[0];
		zeros--;
		n++;
	}
	wcscat(out, p+1);
	return n;
}

void updateHijri(AppContextRef ctx, int t)
{
	HijriDate hijri = unix2hijri(t);
	
	// Can't use swprintf becaues it uses the heap. Until that's fixed we do it manually.
	//swprintf(hijriTxt, sizeof(hijriTxt), L"%d %s", hijri.day, hijriMonths[hijri.month]);
	*hijriTxt = L'\0';
	wifmt(hijriTxt, hijri.day, 1);
	wcscat(hijriTxt, L" ");
	wcscat(hijriTxt, hijriMonths[hijri.month]);
	
	shape(hijriTxt, sizeof(hijriTxt));
	
	//setlocale(LC_ALL, "en_US.utf8");
	//wcstombs(utfHijriTxt, hijriTxt, sizeof hijriTxt);
	const UTF32 * pA = (UTF32*)hijriTxt;
	UTF8 * pB = ( UTF8*)utfHijriTxt;
	ConvertUTF32toUTF8(&pA, pA + 15, &pB, pB + 30, lenientConversion);
	text_layer_set_text(&hijriLayer, utfHijriTxt);
}

void updateGregorian(AppContextRef ctx, PblTm *t)
{
	string_format_time(dateTxt, sizeof(dateTxt), "%B %e", t);
	text_layer_set_text(&dateLayer, dateTxt);
}

void updateTime(AppContextRef ctx, PblTm *t)
{
	*timeTxt = L'\0';
	wifmt(timeTxt, t->tm_hour, 2);
	wcscat(timeTxt, L":");
	wifmt(timeTxt, t->tm_min, 2);
	
	shape(timeTxt, sizeof(timeTxt));
	
	const UTF32 * pA = (UTF32*)timeTxt;
	UTF8 * pB = ( UTF8*)utfTimeTxt;
	ConvertUTF32toUTF8(&pA, pA + 15, &pB, pB + 30, lenientConversion);
	text_layer_set_text(&timeLayer, utfTimeTxt);
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

#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#include <pebble.h>

#include "hijri.h"
#include "ConvertUTF.h"

#define LABEL_LEN 255
#define INTWIDTH 4

#define WIDTH 144
#define HEIGHT 168

Window *window;
TextLayer *hijriLayer;
TextLayer *dateLayer;
TextLayer *timeLayer;

wchar_t timeTxt[LABEL_LEN];
char utfTimeTxt[LABEL_LEN];
wchar_t hijriTxt[LABEL_LEN];
char utfHijriTxt[LABEL_LEN];
wchar_t dateTxt[LABEL_LEN];
char utfDateTxt[LABEL_LEN];

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

void updateHijri(time_t t)
{
	HijriDate hijri = unix2hijri(t);
	
	// Can't use swprintf becaues it uses the heap. Until that's fixed we do it manually.
	//swprintf(hijriTxt, sizeof(hijriTxt), L"%d %s", hijri.day, hijriMonths[hijri.month]);
	*hijriTxt = L'\0';
	wcscat(hijriTxt, hijriMonths[hijri.month]); shape(hijriTxt, sizeof(hijriTxt));
	wcscat(hijriTxt, L" ");
	wifmt(hijriTxt, hijri.day, 1); shape(hijriTxt, sizeof(hijriTxt));
	
	const UTF32 * pA = (UTF32*)hijriTxt;
	UTF8 * pB = ( UTF8*)utfHijriTxt;
	ConvertUTF32toUTF8(&pA, pA + 15, &pB, pB + 30, lenientConversion);
	text_layer_set_text(hijriLayer, utfHijriTxt);
}

void updateGregorian(struct tm *t)
{
	*dateTxt = L'\0';
	wcscat(dateTxt, arGregorianMonths[t->tm_mon]); shape(dateTxt, sizeof(dateTxt));
	wcscat(dateTxt, L" ");
	wifmt(dateTxt, t->tm_mday, 1); shape(dateTxt, sizeof(dateTxt));
	
	const UTF32 * pA = (UTF32*)dateTxt;
	UTF8 * pB = ( UTF8*)utfDateTxt;
	ConvertUTF32toUTF8(&pA, pA + 15, &pB, pB + 30, lenientConversion);
	text_layer_set_text(dateLayer, utfDateTxt);
}

void updateTime(struct tm *t)
{
	*timeTxt = L'\0';
	wifmt(timeTxt, t->tm_hour, 2);
	wcscat(timeTxt, L":");
	wifmt(timeTxt, t->tm_min, 2);
	shape(timeTxt, sizeof(timeTxt));
	
	const UTF32 * pA = (UTF32*)timeTxt;
	UTF8 * pB = ( UTF8*)utfTimeTxt;
	ConvertUTF32toUTF8(&pA, pA + 15, &pB, pB + 30, lenientConversion);
	text_layer_set_text(timeLayer, utfTimeTxt);
}

void handleTick(struct tm *t, TimeUnits units_changed)
{
	updateTime(t);
	// Only update the date on a new day
	if (t->tm_hour == 0 && t->tm_min == 0) {
		updateGregorian(t);
		updateHijri(time(NULL));
	}
}

void initLayout()
{
	window = window_create();
	window_stack_push(window, true);
	window_set_background_color(window, GColorBlack);
	
	int margin = 8;
	int top = 10;
	int width = WIDTH-2*margin;
	int dateHeight = 32;
	int timeHeight = 80;
	
	hijriLayer = text_layer_create(GRect(margin, top, width, dateHeight));
	text_layer_set_text_alignment(hijriLayer, GTextAlignmentRight);
	text_layer_set_text_color(hijriLayer, GColorWhite);
	text_layer_set_background_color(hijriLayer, GColorClear);
	text_layer_set_font(hijriLayer,
		fonts_load_custom_font(
			resource_get_handle(RESOURCE_ID_FONT_KACSTBOOK_26)));
	layer_add_child((Layer *)window, (Layer *)hijriLayer);
	
	timeLayer = text_layer_create(GRect(margin, top+dateHeight, width, timeHeight));
	text_layer_set_text_alignment(timeLayer, GTextAlignmentCenter);
	text_layer_set_text_color(timeLayer, GColorWhite);
	text_layer_set_background_color(timeLayer, GColorClear);
	text_layer_set_font(timeLayer,
		fonts_load_custom_font(
			resource_get_handle(RESOURCE_ID_FONT_KACSTBOOK_SUBSET_55)));
	layer_add_child((Layer *)window, (Layer *)timeLayer);
	
	dateLayer = text_layer_create(GRect(margin, top+dateHeight+timeHeight, width, dateHeight));
	text_layer_set_text_color(dateLayer, GColorWhite);
	text_layer_set_background_color(dateLayer, GColorClear);
	text_layer_set_font(dateLayer,
		fonts_load_custom_font(
			resource_get_handle(RESOURCE_ID_FONT_KACSTBOOK_26)));
	layer_add_child((Layer *)window, (Layer *)dateLayer);
}


int main(void)
{
	initLayout();
	
	time_t unix = time(NULL);
	struct tm *local = localtime(&unix);
	updateTime(local);
	updateGregorian(local);
	updateHijri(unix);
	free(local);
	
	tick_timer_service_subscribe(MINUTE_UNIT, &handleTick);
	
	app_event_loop();
}

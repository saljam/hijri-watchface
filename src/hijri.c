#include <wctype.h>
#include <wchar.h>
#include "hijri.h"

#define secondsPerMinute 60
#define secondsPerHour (60 * 60)
#define secondsPerDay (24 * 60 * 60)

#define daysPer30Years (11*355 + 19*354)
#define daysPer2Months (29+30)
#define daysPerEvenMonth (30)

// The closest start of the 30-year cycle to unix 0.
// 1 Muharram 1380, 26 June 1960
int zeroTime = -300326400;
int yearAtZero = 1380;

int daysPerYear[] = {
	354, 355, 354, 354, 355, 354, 354, 354, 354, 355,
	354, 354, 355, 354, 355, 354, 354, 355, 354, 354,
	355, 354, 354, 355, 354, 355, 354, 354, 355, 354,
};

wchar_t easternDigit[] = {
	L'\u0660',
	L'\u0661',
	L'\u0662',
	L'\u0663',
	L'\u0664',
	L'\u0665',
	L'\u0666',
	L'\u0667',
	L'\u0668',
	L'\u0669',
};

char * hijriMonthsEn[] = {
	"Muharram",
	"Safar",
	"Rabi' al-awwal",
	"Rabi' al-thani",
	"Jumada al-awwal",
	"Jumada al-thani",
	"Rajab",
	"Sha'aban",
	"Ramadan",
	"Shawwal",
	"Dhu al-Qi'dah",
	"Dhu al-Hijjah",
};

wchar_t * hijriMonths[] = {
	L"محرم",
	L"صفر",
	L"ربيع الأول",
	L"ربيع الآخر",
	L"جمادى الأولى",
	L"جمادى الآخرة",
	L"رجب",
	L"شعبان",
	L"رمضان",
	L"شوال",
	L"\uFE93\uFEAA\uFECC\uFED8\uFEDF\uFE8D \uFEED\uFEAB", //"ذو القعدة",
	L"ذو الحجة",
};

// For now we just do the digits. I should add full alphabet shaping soon.
int shape(wchar_t *in, int n)
{
	if (n == 0 || *in == L'\0') {
		return 0;
	}
	
	// Reshape ascii digits to eastern
	if (*in >= L'0' && *in <= L'9') {
		int d = *in - L'0';
		*in = easternDigit[d];
	}
	
	in++;
	return shape(in, n-1);
}

HijriDate unix2hijri(int t)
{
	HijriDate date;
	
	int d = (t - zeroTime) / secondsPerDay;
	
	int n = d / daysPer30Years;
	date.year = 30 * n;
	d -= daysPer30Years * n;
	
	// TODO Rewrite loop to something like daysBefore from the go time package.
	int i = 0;
	while (d > daysPerYear[i]) {
		d -= daysPerYear[i];
		i++;
		date.year++;
	}
	date.year += yearAtZero;
	
	n = d / daysPer2Months;
	date.month = 2 * n;
	d -= daysPer2Months * n;
	
	if (d > daysPerEvenMonth) {
		date.month++;
		d -= daysPerEvenMonth;
	}
	
	date.day = d;
	
	return date;
}

#include <wctype.h>
#include <wchar.h>
#include "hijri.h"
#include "unicode.h"

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
	L"ذو القعدة",
	L"ذو الحجة",
};

wchar_t * arGregorianMonths[] = {
	L"يناير",
	L"فبراير",
	L"مارس",
	L"أبريل",
	L"مايو",
	L"يونيو",
	L"يوليو",
	L"أغسطس",
	L"سبتمبر",
	L"أكتوبر",
	L"نوفمبر",
	L"ديسمبر",
};

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

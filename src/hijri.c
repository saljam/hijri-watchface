#include "hijri.h"
#include <ctype.h>

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

char * easternDigit[] = {
	"\u0660",
	"\u0661",
	"\u0662",
	"\u0663",
	"\u0664",
	"\u0665",
	"\u0666",
	"\u0667",
	"\u0668",
	"\u0669",
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

char * hijriMonths[] = {
	"محرم",
	"صفر",
	"ربيع الأول",
	"ربيع الآخر",
	"جمادى الأولى",
	"جمادى الآخرة",
	"رجب",
	"شعبان",
	"رمضان",
	"شوال",
	"\uFE93\uFEAA\uFECC\uFED8\uFEDF\uFE8D \uFEED\uFEAB", //"ذو القعدة",
	"ذو الحجة",
};

// For now we just do the digits. I should add full alphabet shaping soon.
int shape(char *in, char *out, int n)
{
	if (n == 0 || *in == '\0') {
		*out = *in;
		return 0;
	}
	
	// Reshape ascii digits to eastern
	if (isdigit((int) *in)) {
		int d = *in - '0';
		*out++ = easternDigit[d][0];
		*out++ = easternDigit[d][1];
		in++;
	} else {
		*out++ = *in++;
	}
	
	return shape(in, out, n-1);
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

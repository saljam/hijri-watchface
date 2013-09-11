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

char * hijriMonths[] = {
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

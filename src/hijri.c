#include <wctype.h>
#include <wchar.h>
#include "hijri.h"
#include "shaping.h"

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

void strrev(wchar_t *start, int n)
{
	wchar_t *end = start;
	while (*end !=  L'\0' && end < start+n) {
		end++;
	}
	end--;
	
	while (start < end) {
		wchar_t tmp = *start;
		*start++ = *end;
		*end-- = tmp;
	}
}

int shape(wchar_t *in, int n)
{
	int rev = 0;
	enum joiningType prev = U;
	int i;
	for (i = 0; i < n && in[i] != L'\0'; i++) {
		// numeral
		if (in[i] >= L'0' && in[i] <= L'9') {
			int d = in[i] - L'0';
			in[i] = easternDigit[d];
		} else if (in[i] >= L'؀' && in[i] <= L'ۿ') {
			rev = 1;
			enum joiningType next = U;
			if (in[i+1] >= L'؀' && in[i+1] <= L'ۿ') {
				next = joinTable[in[i+1] - joinTableOffset];
			}
			wchar_t g;
			if (prev & L && next & R) {
				g = arConv[in[i] - joinTableOffset].medial;
			} else if (prev & L) {
				g = arConv[in[i] - joinTableOffset].final;
			} else if (next & R) {
				g = arConv[in[i] - joinTableOffset].initial;
			} else {
				g = arConv[in[i] - joinTableOffset].isolated;
			}
			prev = joinTable[in[i] - joinTableOffset];
			in[i] = g;
		} else {
			prev = U;
		}
	}
	
	// Reverse the string if it has arabic. Not really good enough but does the job for date names on a watch...
	if (rev) {
		strrev(in, n);	
	}

	return i;
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

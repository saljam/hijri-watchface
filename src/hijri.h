typedef struct {
	int year;
	int month;
	int day;
} HijriDate;

extern wchar_t * hijriMonths[];
extern wchar_t * arGregorianMonths[];

HijriDate unixtohijri(int t, int dayCorrection);

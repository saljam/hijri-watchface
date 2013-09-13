typedef struct {
	int year;
	int month;
	int day;
} HijriDate;

extern wchar_t * hijriMonths[];

HijriDate unix2hijri(int t);
int shape(wchar_t *in, int n);

typedef struct {
	int year;
	int month;
	int day;
} HijriDate;

extern char * hijriMonths[];

HijriDate unix2hijri(int t);

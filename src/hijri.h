typedef struct {
	int year;
	int month;
	int day;
} HijriDate;

extern char * hijriMonths[];
extern char * arabicDigit[];

HijriDate unix2hijri(int t);
int shape(char *in, char *out, int n);

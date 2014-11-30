// This file was generated from ArabicShaping.txt from the Unicode Character Database
// TODO(s) write a script that does this automatically, instead of a series of Edits

int shape(wchar_t *in, int n);
void strrev(wchar_t *start, int n);

#define AR_START L'؀'

extern wchar_t easternDigit[];

typedef struct {
	wchar_t isolated;
	wchar_t final;
	wchar_t medial;
	wchar_t initial;
} arGlyph;

extern arGlyph arForms[];

enum joiningType {
	R = 1,
	L = 2,
	D = 3,
	C = 7,
	U = 0
};

extern enum joiningType joinTable[];

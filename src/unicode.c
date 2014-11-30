#include <wctype.h>
#include <wchar.h>
#include <string.h>
#include "unicode.h"


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

int ligature(wchar_t *in, int n)
{
	int i;
	for (i = 0; i < n && in[i] != L'\0'; i++) {
		switch (in[i]) {
		case L'ﻟ':	// Initial
			switch (in[i+1]) {
				case L'ﺎ':
					in[i] = L'ﻻ';
					break;
				case L'ﺃ':
					in[i] = L'ﻷ';
					break;
				case L'ﺂ':
					in[i] = L'ﻵ';
					break;
				default:
					continue;
			}
			break;
		case L'ﻠ':	// Midial
			switch (in[i+1]) {
				case L'ﺎ':
					in[i] = L'ﻼ';
					break;
				case L'ﺃ':
					in[i] = L'ﻸ';
					break;
				case L'ﺂ':
					in[i] = L'ﻶ';
					break;
				default:
					continue;
			}
			break;
		default:
			continue;
		}
		
		memcpy(&in[i+1], &in[i+2], n-i-2);
	}
	
	return i;
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
				next = joinTable[in[i+1] - AR_START];
			}
			wchar_t g;
			if (prev & L && next & R) {
				g = arForms[in[i] - AR_START].medial;
			} else if (prev & L) {
				g = arForms[in[i] - AR_START].final;
			} else if (next & R) {
				g = arForms[in[i] - AR_START].initial;
			} else {
				g = arForms[in[i] - AR_START].isolated;
			}
			prev = joinTable[in[i] - AR_START];
			in[i] = g;
		} else {
			prev = U;
		}
	}
	
	ligature(in, n);	
	
	// Reverse the string if it has arabic. Not really good enough but does the job for date names on a watch...
	if (rev) {
		strrev(in, n);	
	}

	return i;
}

FREINDLY_VERSION = VERSION
VERSION_DEF_NAME = APP_RESOURCES

RESOURCES = $R/FONT_KACSTBOOK_26.pfo
$R/FONT_KACSTBOOK_26.pfo: res/fonts/KacstBook.ttf
	python $(SDK)/tools/font/fontgen.py pfo 26 --filter '[ :0-9٠-٩ﺀ-ﻼ]' $< $@
RESOURCES += $R/FONT_KACSTBOOK_SUBSET_55.pfo
$R/FONT_KACSTBOOK_SUBSET_55.pfo: res/fonts/KacstBook.ttf
	python $(SDK)/tools/font/fontgen.py pfo 55 --filter '[ :٠-٩]' $< $@
RESOURCES += $R/FONT_ROBOTO_CONDENSED_21.pfo
$R/FONT_ROBOTO_CONDENSED_21.pfo: res/fonts/Roboto-Condensed.ttf
	python $(SDK)/tools/font/fontgen.py pfo 21 $< $@

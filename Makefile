NOW := $(shell date '+%N')
REQ_FW := 1
SDK := $(PEBBLE_HOME)/Pebble/sdk


# Code

AS = arm-none-eabi-gcc
AR = arm-none-eabi-ar
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

CFLAGS = -std=c99 \
	-mcpu=cortex-m3  -mthumb\
	-ffunction-sections -fdata-sections \
	-g -Os
CFLAGS += -Wall -Wextra -Werror \
	-Wno-unused-parameter -Wno-error=unused-function -Wno-error=unused-variable 
CFLAGS += -fPIE -I. -I$(SDK)/include -DRELEASE -c

LDFLAGS = -mcpu=cortex-m3 -mthumb -Wl,--gc-sections -Wl,--warn-common -Os -fPIE -Wl,-Map,bin/pebble-app.map,--emit-relocs
LDFLAGS += -T$(SDK)/pebble_app.ld
LIBS = -L$(SDK)/lib -lpebble

O = bin/obj
R = bin/res

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:src/%.c=$O/%.o)

bin/bundle.pbw: dirs $(SDK)/tools/mkbundle.py $O/pebble-app.bin res/resource_map.json $R/app_resources.pbpack
	python $(SDK)/tools/mkbundle.py \
		--watchapp $O/pebble-app.bin \
		--watchapp-timestamp $(NOW) \
		--req-fw $(REQ_FW) \
		--resources $R/app_resources.pbpack \
		--resource-map res/resource_map.json \
		--resources-timestamp $(NOW)\
		-o $@ -v

$O/%.o: src/%.c gen/resource_ids.auto.h
	$(CC) $(CFLAGS) $< -o $@

$O/pebble-app.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

$O/pebble-app.raw.bin: $O/pebble-app.elf
	arm-none-eabi-objcopy -R .stack -R .bss -O binary $< $@

$O/pebble-app.bin: $O/pebble-app.raw.bin $O/pebble-app.elf
	cp $< $@
	python $(SDK)/waftools/inject_metadata.py $@


# Resources
# TODO the rest of /home/s/pebble-sdk/Pebble/sdk/waftools/process_resources.py

FREINDLY_VERSION = VERSION
VERSION_DEF_NAME = APP_RESOURCES
RESOURCES = $R/FONT_KACSTBOOK_26.pfo
$R/FONT_KACSTBOOK_26.pfo: res/fonts/KacstBook.ttf
	python $(SDK)/tools/font/fontgen.py pfo 26 --filter '[ :0-9٠-٩ﺍ-ﻳ]' $< $@
RESOURCES += $R/FONT_KACSTBOOK_SUBSET_55.pfo
$R/FONT_KACSTBOOK_SUBSET_55.pfo: res/fonts/KacstBook.ttf
	python $(SDK)/tools/font/fontgen.py pfo 55 --filter '[ :٠-٩]' $< $@
RESOURCES += $R/FONT_ROBOTO_CONDENSED_21.pfo
$R/FONT_ROBOTO_CONDENSED_21.pfo: res/fonts/Roboto-Condensed.ttf
	python $(SDK)/tools/font/fontgen.py pfo 21 $< $@

gen/resource_ids.auto.h: $R/app_resources.pbpack
	python $(SDK)/tools/generate_resource_code.py \
		resource_header $@ $(VERSION_DEF_NAME) $(FREINDLY_VERSION) \
		$(NOW) pebble_os.h $R/app_resources.pbpack.data $(foreach r, $(RESOURCES), $r $(r:$R/%.pfo=%))

$R/app_resources.pbpack: $(RESOURCES) $R/app_resources.pbpack.manifest $R/app_resources.pbpack.table $R/app_resources.pbpack.data
	cat $R/app_resources.pbpack.manifest $R/app_resources.pbpack.table $R/app_resources.pbpack.data > $@

$R/app_resources.pbpack.data: $(RESOURCES)
	cat $(RESOURCES) > $@

$R/app_resources.pbpack.table: $(RESOURCES)
	python $(SDK)/tools/pbpack_meta_data.py table $@ $(RESOURCES)

$R/app_resources.pbpack.manifest: $(RESOURCES) $R/app_resources.pbpack.data
	python $(SDK)/tools/pbpack_meta_data.py manifest $@ $(words $(RESOURCES)) $(NOW) $(FREINDLY_VERSION) \
 		$R/app_resources.pbpack.data


.PHONY: dirs
dirs:
	mkdir -p gen
	mkdir -p $O
	mkdir -p $R

.PHONY: clean
clean:
	$(RM) $(OBJECTS)
	$(RM) $(RESOURCES)
	$(RM) $O/pebble-app.bin $O/pebble-app.raw.bin $O/pebble-app.elf
	$(RM) $R/app_resources.pbpack.table $R/app_resources.pbpack.manifest $R/app_resources.pbpack.data $R/app_resources.pbpack
	$(RM) gen/resource_ids.auto.h
	$(RM) bin/bundle.pbw

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

LDFLAGS = -mcpu=cortex-m3 -mthumb -Wl,--gc-sections -Wl,--warn-common -Os -fPIE -Wl,-Map,out/pebble-app.map,--emit-relocs
LDFLAGS += -T$(SDK)/pebble_app.ld
LIBS = -L$(SDK)/lib -lpebble


SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

out/bundle.pbw: dirs $(SDK)/tools/mkbundle.py out/pebble-app.bin resources/src/resource_map.json out/res/app_resources.pbpack
	python $(SDK)/tools/mkbundle.py \
		--watchapp out/pebble-app.bin \
		--watchapp-timestamp $(NOW) \
		--req-fw $(REQ_FW) \
		--resources out/res/app_resources.pbpack \
		--resource-map resources/src/resource_map.json \
		--resources-timestamp $(NOW)\
		-o $@ -v

%.o: %.c out/res/resource_ids.auto.h
	$(CC) $(CFLAGS) $< -o $@

out/pebble-app.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

out/pebble-app.raw.bin: out/pebble-app.elf
	arm-none-eabi-objcopy -R .stack -R .bss -O binary $< $@

out/pebble-app.bin: out/pebble-app.raw.bin out/pebble-app.elf
	cp $< $@
	python $(SDK)/waftools/inject_metadata.py $@


# Resources

FREINDLY_VERSION = VERSION
VERSION_DEF_NAME = APP_RESOURCES
RESOURCES = out/res/FONT_KACSTBOOK_26.pfo
out/res/FONT_KACSTBOOK_26.pfo: resources/src/fonts/KacstBook.ttf
	python $(SDK)/tools/font/fontgen.py pfo 26 --filter '[ :0-9٠-٩ﺍ-ﻳ]' $< $@
RESOURCES += out/res/FONT_KACSTBOOK_SUBSET_55.pfo
out/res/FONT_KACSTBOOK_SUBSET_55.pfo: resources/src/fonts/KacstBook.ttf
	python $(SDK)/tools/font/fontgen.py pfo 55 --filter '[ :٠-٩]' $< $@
RESOURCES += out/res/FONT_ROBOTO_CONDENSED_21.pfo
out/res/FONT_ROBOTO_CONDENSED_21.pfo: resources/src/fonts/Roboto-Condensed.ttf
	python $(SDK)/tools/font/fontgen.py pfo 21 $< $@

out/res/resource_ids.auto.h: out/res/app_resources.pbpack
	python $(SDK)/tools/generate_resource_code.py \
		resource_header $@ $(VERSION_DEF_NAME) $(FREINDLY_VERSION) \
		$(NOW) pebble_os.h out/res/app_resources.pbpack.data $(foreach r, $(RESOURCES), $r $(r:out/res/%.pfo=%))

out/res/app_resources.pbpack: $(RESOURCES) out/res/app_resources.pbpack.manifest out/res/app_resources.pbpack.table out/res/app_resources.pbpack.data
	cat out/res/app_resources.pbpack.manifest out/res/app_resources.pbpack.table out/res/app_resources.pbpack.data > $@

out/res/app_resources.pbpack.data: $(RESOURCES)
	cat $(RESOURCES) > $@

out/res/app_resources.pbpack.table: $(RESOURCES)
	python $(SDK)/tools/pbpack_meta_data.py table $@ $(RESOURCES)

 out/res/app_resources.pbpack.manifest: $(RESOURCES) out/res/app_resources.pbpack.data
	python $(SDK)/tools/pbpack_meta_data.py manifest $@ $(words $(RESOURCES)) $(NOW) $(FREINDLY_VERSION) \
 		out/res/app_resources.pbpack.data


.PHONY: dirs
dirs:
	mkdir -p out/res

.PHONY: clean
clean:
	$(RM) $(OBJECTS)
	$(RM) $(RESOURCES)
	$(RM) out/pebble-app.bin out/pebble-app.raw.bin out/pebble-app.elf out/bundle.pbw
	$(RM) out/res/app_resources.pbpack.table out/res/app_resources.pbpack.manifest out/res/app_resources.pbpack.data out/res/app_resources.pbpack
	$(RM) out/res/resource_ids.auto.h

export PATH	:= $(DEVKITARM)/bin:$(PATH)

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
OC=arm-none-eabi-objcopy

INCLUDES=-I. -I./include

ARCH=-march=armv6k -mtune=mpcore -mlittle-endian
CFLAGS:=-fshort-wchar -fomit-frame-pointer -ffast-math -std=gnu99 -Os -ffunction-sections -g -mword-relocations $(ARCH) $(INCLUDES) $(DEFINES)
LDFLAGS:=-nostartfiles --specs=crs.specs -Wl,-gc-sections $(ARCH)
OCFLAGS=

OBJ_DIR := obj

SRC_DIR := src
OUT_DIR := obj obj/spiderhttpc obj/$(TYPE)

define makedownload
	@echo "Building \"$(3)\" for $(1)"
	@make -s obj/code.bin DEFINES="-D$(1) -D$(2)" TYPE=$(3)
	@mkdir -p out/$(1)
	@mv obj/code.bin out/$(1)/$(3).bin
	@rm -rf $(OBJ_DIR)
endef

define makedownloadwithrop
	@echo "Building \"$(5)\" for $(2)"
	@make -s obj/code.bin DEFINES="-D$(2) -D$(4)" TYPE=$(5)
	@make -s -C rop3ds DownloadCode.dat ASFLAGS="-D$(2) -D$(4) -DARM_CODE=../obj/code.bin"
	@mv rop3ds/DownloadCode.dat out/$(1).rop
	@rm obj/code.bin
	@rm -rf $(OBJ_DIR)
endef	

SRCS := $(wildcard $(SRC_DIR)/spiderhttpc/*.c) $(wildcard $(SRC_DIR)/$(TYPE)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

single: TARGET = single
single: download downloadwithrop

dlx: TARGET = dlx
dlx: download downloadwithrop

download:
	$(call makedownload,SPIDER_4X,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_5X,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_9X,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_42_CN,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_45_CN,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_5X_CN,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_9X_CN,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_4X_KR,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_5X_KR,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_9X_KR,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_4X_TW,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_5X_TW,NO_SPIDER_DG,$(TARGET))
	$(call makedownload,SPIDER_9X_TW,NO_SPIDER_DG,$(TARGET))
	
downloadwithrop:
	$(call makedownloadwithrop,17498,SPIDER_4X,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17538C45,SPIDER_45_CN,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17538C42,SPIDER_42_CN,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17538K,SPIDER_4X_KR,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17538T,SPIDER_4X_TW,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17552,SPIDER_5X,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17552C,SPIDER_5X_CN,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17552K,SPIDER_5X_KR,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17552T,SPIDER_5X_TW,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17567,SPIDER_9X,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17567C,SPIDER_9X_CN,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17567K,SPIDER_9X_KR,0,NO_SPIDER_DG,$(TARGET))
	$(call makedownloadwithrop,17567T,SPIDER_9X_TW,0,NO_SPIDER_DG,$(TARGET))
	@cp rop3ds/download.html.template out/download.html	
	
obj/%.o: src/%.c | dirs
	$(CC) -c -o $@ $< $(CFLAGS)

obj/%.o: src/%.s | dirs
	$(CC) -x assembler-with-cpp $(ARCH) -c $^ -o $@

obj/code.elf: $(OBJS)
	@echo $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

obj/code.bin: obj/code.elf
	$(OC) $(OCFLAGS) -O binary $^ $@

dirs: ${OUT_DIR}

${OUT_DIR}:
	mkdir -p ${OUT_DIR}

.PHONY: clean

clean:
	rm -rf *~ obj out *.elf *.bin

BUILD_DIR = ./build

COMMON_FLAGS := -D DASH_TESTING
CC_FLAGS := $(COMMON_FLAGS) `pkg-config --cflags gtk+-3.0` -g
LD_FLAGS := $(COMMON_FLAGS) `pkg-config --libs gtk+-3.0` -lm -g -lavcodec -lavutil -lyuv

TESTING_SRCS := $(wildcard gtk/*.cpp) $(wildcard gtk/*.c)
TESTING_INCLUDES := -I ./gtk
TESTING_OBJS := $(TESTING_SRCS:%=$(BUILD_DIR)/%.o)

APP_DIR := ./src
APP_SRCS := $(shell find $(APP_DIR) -type f -name "*.c*")
APP_INCLUDES := -I ./src -I ./src/sprites
APP_OBJS := $(APP_SRCS:$(APP_DIR)/%=$(BUILD_DIR)/src/%.o)

DBC_DIR := ./lib/Formula-DBC
DBC_SRCS := $(shell find -L $(DBC_DIR)/c_files -name "*.c")
DBC_INCLUDES := -I $(DBC_DIR)
DBC_OBJS := $(DBC_SRCS:$(DBC_DIR)/c_files/%=$(BUILD_DIR)/formula_dbc/%.o)

LVGL_DIR := ./.pio/libdeps/teensy40/lvgl
LVGL_SRCS := $(shell find $(LVGL_DIR)/src -type f -name "*.c")
LVGL_INCLUDES := $(LVGL_DIR)/
LVGL_INCLUDES := $(foreach d, $(LVGL_INCLUDES),-I $d)
LVGL_OBJS :=  $(LVGL_SRCS:$(LVGL_DIR)/src/%=$(BUILD_DIR)/lvgl/%.o)

INCLUDES := $(APP_INCLUDES) $(TESTING_INCLUDES) $(DBC_INCLUDES) $(LVGL_INCLUDES)
OBJS := $(APP_OBJS) $(DBC_OBJS) $(LVGL_OBJS) $(TESTING_OBJS)

$(BUILD_DIR)/dash: $(OBJS)
	@[ -d $(@D) ] || mkdir -p $(@D)
	gcc $(OBJS) -o $@ $(LD_FLAGS)

$(BUILD_DIR)/src/%.cpp.o: src/%.cpp src/config.h
	@[ -d $(@D) ] || mkdir -p $(@D)
	g++ $(CC_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/src/%.c.o: src/%.c src/config.h
	@[ -d $(@D) ] || mkdir -p $(@D)
	gcc $(CC_FLAGS) $(INCLUDES) -I ./.pio/libdeps/teensy40/ -c $< -o $@

$(BUILD_DIR)/gtk/%.cpp.o: gtk/%.cpp src/config.h
	@[ -d $(@D) ] || mkdir -p $(@D)
	g++ $(CC_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/gtk/%.c.o: gtk/%.c src/config.h
	@[ -d $(@D) ] || mkdir -p $(@D)
	gcc $(CC_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/lvgl/%.c.o: $(LVGL_DIR)/src/%.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	gcc $(CC_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/formula_dbc/%.c.o: $(DBC_DIR)/c_files/%.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	gcc $(CC_FLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)


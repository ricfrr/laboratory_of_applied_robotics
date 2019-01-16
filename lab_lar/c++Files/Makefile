CFLAGS := -Wall -std=gnu90 -D_GNU_SOURCE -I../brick/common
CXXFLAGS :=  -std=c++11 -I../brick/common -I/usr/include/eigen3 `pkg-config --cflags opencv` -Wno-ignored-attributes
LDFLAGS := -L/usr/lib -lbluetooth -lm -pthread `pkg-config --libs opencv`

BUILD_DIR := ./build
BUILD_DIR_COMMON := $(BUILD_DIR)/common
BUILD_DIR_LIB := $(BUILD_DIR)/libs

MAIN_DIR := .
COMMON_DIR := ../brick/common
LIB_DIR := ./libs

MAIN_FILES := $(wildcard $(MAIN_DIR)/*.cpp)
MAIN_OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(addsuffix .o, $(basename $(MAIN_FILES)))))

LIB_FILES := $(wildcard $(LIB_DIR)/*.c)
LIB_OBJS := $(addprefix $(BUILD_DIR_LIB)/, $(notdir $(addsuffix .o, $(basename $(LIB_FILES)))))

COMMON_FILES := $(wildcard $(COMMON_DIR)/*.c)
COMMON_OBJS := $(addprefix $(BUILD_DIR_COMMON)/, $(notdir $(addsuffix .o, $(basename $(COMMON_FILES)))))

APP := PCClient

all: $(MAIN_OBJS) $(LIB_OBJS) $(COMMON_OBJS)
	$(CXX) $(MAIN_OBJS) $(LIB_OBJS) $(COMMON_OBJS) $(LDFLAGS) -o $(APP)

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BUILD_DIR_COMMON}: | ${BUILD_DIR}
	mkdir -p ${BUILD_DIR_COMMON}

${BUILD_DIR_LIB}: | ${BUILD_DIR}
	mkdir -p ${BUILD_DIR_LIB}

$(BUILD_DIR)/%.o: $(MAIN_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) -c $< $(CXXFLAGS) -o $@

$(BUILD_DIR_COMMON)/%.o: $(COMMON_DIR)/%.c | $(BUILD_DIR_COMMON)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR_LIB)/%.o: $(LIB_DIR)/%.c | $(BUILD_DIR_LIB)
	$(CC) -c $< $(CFLAGS) -o $@

cleanobj:
	rm -rf $(BUILD_DIR)

clean: cleanobj
	rm -f $(APP)

.PHONY : all cleanobj clean

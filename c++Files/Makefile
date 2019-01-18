PRODUCT := lar


INCDIR := -IHeaders -IVisualizing -IAPI 
SRCDIRS := Source Visualizing API 
OBJDIR := obj

CXX := g++
LINKER := g++
INCDIRS := $(INCDIR)

#CXXFLAGS := -std=c++11 -Wno-pragmas -Wall -Wextra $(shell pkg-config --libs --cflags opencv)
COMPILE_FLAGS := -std=c++11 -Wno-pragmas -Wall -Wextra
LINKING_FLAGS := $(shell pkg-config --libs --cflags opencv tesseract)

SRCFILES := $(shell find $(SRCDIRS) -name "*.cpp" -type f)
OBJFILES := $(SRCFILES:%.cpp=$(OBJDIR)/%.o)
DEPFILES := $(OBJFILES:.o=.d)
DIRS = $(subst /,/,$(sort $(dir $(OBJFILES))))

#OLD
#OBJFILES := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCFILES))
#DEPFILES := $(OBJFILES:%.o=%.d)


$(PRODUCT): main.cpp $(OBJFILES)
	$(LINKER) $^ -o $@  $(LINKING_FLAGS)

$(OBJDIR)/%.o: %.cpp
	@echo $(DIRS)
	mkdir -p $(DIRS)
	$(CXX) $(COMPILE_FLAGS) $(INCDIRS) -MMD -c $< -o $@

-include $(DEPFILES)

clean:
	@rm -rf obj/* $(PRODUCT)

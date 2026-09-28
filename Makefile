CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g

DIRS = Adapter Mediator
TARGET = campusguard
BUILD = build

INCLUDES = $(addprefix -I,$(DIRS))
SRCS = $(wildcard *.cpp) $(foreach d,$(DIRS),$(wildcard $(d)/*.cpp))
OBJS = $(addprefix $(BUILD)/,$(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET)

-include $(DEPS)

.PHONY: all run valgrind clean
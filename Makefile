CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g

DIRS = Adapter Command Compisite Facade Mediator State
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

# Builds tests/coverage_main.cpp with every source except the real main.cpp
coverage:
	$(CXX) -std=c++11 -g -O0 --coverage $(INCLUDES) \
		tests/coverage_main.cpp \
		$(filter-out main.cpp,$(SRCS)) \
		-o coverage_test
	./coverage_test

clean:
	rm -rf $(BUILD) $(TARGET) coverage_test *.gcda *.gcno *.gcov

-include $(DEPS)

.PHONY: all run valgrind coverage clean
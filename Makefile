CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g

DIRS = adapter mediator
TARGET = campusguard
BUILD = build

ifeq ($(OS),Windows_NT)
    EXE = .exe
    RUN = $(TARGET)$(EXE)
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RMDIR = if exist "$(subst /,\,$(1))" rmdir /s /q "$(subst /,\,$(1))"
    RMFILE = if exist "$(1)" del /q "$(1)"
else
    EXE =
    RUN = ./$(TARGET)
    MKDIR = mkdir -p $(1)
    RMDIR = rm -rf $(1)
    RMFILE = rm -f $(1)
endif

INCLUDES = $(addprefix -I,$(DIRS))
SRCS = $(wildcard *.cpp) $(foreach d,$(DIRS),$(wildcard $(d)/*.cpp))
OBJS = $(addprefix $(BUILD)/,$(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)

all: $(TARGET)$(EXE)

$(TARGET)$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD)/%.o: %.cpp
	@$(call MKDIR,$(patsubst %/,%,$(dir $@)))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

run: $(TARGET)$(EXE)
	$(RUN)

valgrind: $(TARGET)$(EXE)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:
	@$(call RMDIR,$(BUILD))
	@$(call RMFILE,$(TARGET)$(EXE))

-include $(DEPS)

.PHONY: all run valgrind clean
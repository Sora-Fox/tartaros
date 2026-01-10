TARGET = deleted
TEST_TARGET = test
SRC = main.cpp  scheduler.cpp
TEST_SRC = $(filter-out $(SRC),src/main.cpp) test/test-map.cpp test/test-module.cpp

BUILD ?= build
BINDIR = $(BUILD)/bin
PREFIX = /usr/local
OBJ = $(addprefix $(BUILD)/,$(SRC:.cpp=.o))
TEST_OBJ = $(addprefix $(BUILD)/,$(TEST_SRC:.cpp=.o))
DEP = $(OBJ:.o=.d) $(TEST_OBJ:.o=.d)

override CPPFLAGS += -MMD -MP -Iinclude
override CXXFLAGS += -std=c++23 -pedantic

ifneq ($(COLORLESS), 1)
    COLOR_OFF = \e[0m
    COLOR_CXX = \e[32m
    COLOR_LD  = \e[1;32m
endif

override quiet =
ifeq ($(findstring s,$(firstword -$(MAKEFLAGS))),s)
    override quiet = 1
endif

override Q =
ifneq ($(VERBOSE),1)
    override Q = @
endif

.DEFAULT_GOAL := all
.PHONY: all
all: $(BINDIR)/$(TARGET)

$(OBJ) $(TEST_OBJ): $(BUILD)/%.o: %.cpp
	$(if $(quiet),,@echo -e "[CXX] $(COLOR_CXX)Compiling $(subst $(BUILD)/,,$@)$(COLOR_OFF)")
	@mkdir -p $(@D)
	$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@

$(BINDIR)/$(TARGET): $(OBJ)
	$(if $(quiet),,@echo -e "[LD ] $(COLOR_LD)Linking $(subst $(BUILD)/,,$@) (executable)$(COLOR_OFF)")
	@mkdir -p $(@D)
	$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) $^ $(LOADLIBES) $(LDLIBS) -o $@


$(BINDIR)/$(TEST_TARGET): $(TEST_OBJ)
	$(if $(quiet),,@echo -e "[LD ] $(COLOR_LD)Linking $(subst $(BUILD)/,,$@) (executable)$(COLOR_OFF)")
	@mkdir -p $(@D)
	$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean
clean:
	$(if $(quiet),,@echo -e "[RM ] Removing build artifacts")
	$(Q)$(RM) -r $(BUILD)

.PHONY: run
run: $(BUILD)/$(TARGET)
	$(Q)$(if $(TIMEOUT),timeout --signal=KILL $(TIMEOUT)s )$(if $(VALGRIND),valgrind $(VALGRIND)) $(abspath $<) $(ARGS)

.PHONY: test
test: $(BUILD)/$(TEST_TARGET)
	$(Q)$(if $(TIMEOUT),timeout --signal=KILL $(TIMEOUT)s )$(if $(VALGRIND),valgrind $(VALGRIND)) $(abspath $<) $(ARGS)

.PHONY: help
help:
	@echo "Targets:"
	@echo "    make all              -- Build $(TARGET)"
	@echo "    make run              -- Build and execute $(TARGET)"
	@echo "    make test             -- Build and execute $(TEST_TARGET)"
	@echo "    make clean            -- Remove build artifacts"
	@echo "    make help             -- Display this message"
	@echo ""
	@echo "Options:"
	@echo "    make ARGS=<...>       -- Run program with specified agruments"
	@echo "    make TIMEOUT=N        -- Run via timeout, kill after N seconds"
	@echo "    make VALGRIND=ARGS    -- Run via valgrind with given arguments"
	@echo "    make BUILD=<...>      -- Specify build directory (default: build)"
	@echo "    make VERBOSE=1        -- Enable verbose output"
	@echo "    make COLORLESS=1      -- Disable colorful output"
	@echo "    make -s               -- Silent mode"

.PHONY: install
install: all
	$(if $(quiet),,@echo "[IN ] Installing to $(DESTDIR)$(PREFIX)")
	$(Q)install -Dm755  $(abspath $(BINDIR)/$(TARGET)) -t $(DESTDIR)$(PREFIX)/bin

.PHONY: uninstall
uninstall:
	$(if $(quiet),,@echo "[UN] Removing installation")
	$(Q)$(RM) $(DESTDIR)$(PREFIX)/bin/$(TARGET)



-include $(DEP)

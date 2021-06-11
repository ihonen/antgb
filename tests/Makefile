REPO_DIR    := $(shell readlink --canonicalize ..)
SOURCES_DIR := $(REPO_DIR)/src

EMULATOR_DIR     := $(SOURCES_DIR)/emulator
EMULATOR_SOURCES := $(shell find ../src/emulator -name *.cc)

TESTS_DIR    := $(REPO_DIR)/tests
TEST_SOURCES := $(TESTS_DIR)/blargg/*.cc

GTEST_DIR     := $(TESTS_DIR)/googletest/googletest
GTEST_SOURCES := \
	$(GTEST_DIR)/src/gtest_main.cc \
	$(GTEST_DIR)/src/gtest-death-test.cc \
	$(GTEST_DIR)/src/gtest-filepath.cc \
	$(GTEST_DIR)/src/gtest-matchers.cc \
	$(GTEST_DIR)/src/gtest-port.cc \
	$(GTEST_DIR)/src/gtest-printers.cc \
	$(GTEST_DIR)/src/gtest-test-part.cc \
	$(GTEST_DIR)/src/gtest-typed-test.cc \
	$(GTEST_DIR)/src/gtest.cc

INCLUDE_FLAGS += -I$(REPO_DIR) -I$(SOURCES_DIR) -I$(TESTS_DIR) -I$(GTEST_DIR) -I$(GTEST_DIR)/include

CXX      ?= g++
CXXFLAGS += -std=c++17 -O2 -Wall -Wextra $(INCLUDE_FLAGS)
LDLIBS   += -lpthread

OUTPUT := $(TESTS_DIR)/emulatortests

tests: tests-compile tests-run

tests-compile:
	$(CXX) -o $(OUTPUT) $(EMULATOR_SOURCES) $(TEST_SOURCES) $(GTEST_SOURCES) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

tests-run:
	$(OUTPUT)

clean:
	rm $(OUTPUT)

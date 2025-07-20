all: build
.PHONY: all

build:
	@cmake --build build -- -j 12 --no-print-directory
.PHONY: build

setup:
	mkdir -p build && \
		cmake -S . -B build
.PHONY: setup

test: $(TEST_FILES)
	GTEST_COLOR=1 ctest --test-dir build --output-on-failure -j12 && \
		./build/bin/main_test
.PHONY: test

debug:
	gdb -x .gdbinit ./bin/6502_cpu_emulator
.PHONY: debug

run: all
	@./bin/6502_cpu_emulator
.PHONY: run

clean:
	@rm -rf build bin/6502_cpu_emulator
.PHONY: clean

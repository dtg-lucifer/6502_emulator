TEST_FILES := $(shell find . -name "*_test.cc")

all: build
.PHONY: all

build:
	cd build && make --no-print-directory
.PHONY: build

setup:
	mkdir -p build && \
		cd build && \
		cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && \
		ln -sf build/compile_commands.json compile_commands.json && \
		cd ..
.PHONY: setup

test: $(TEST_FILES)
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake --build build -- --no-print-directory
	cd build && ctest
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

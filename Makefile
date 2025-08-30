all: setup build run
.PHONY: all

build:
	@$(MAKE) -C build \
		-j 12 \
        --no-print-directory
.PHONY: build

setup:
	@mkdir -p build && \
		cmake -S . -B build \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DENABLE_TESTING=OFF && \
        ln -sf build/compile_commands.json compile_commands.json
.PHONY: setup

setup-testing:
	@mkdir -p build && \
		cmake -S . -B build \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DENABLE_TESTING=ON && \
        ln -sf build/compile_commands.json compile_commands.json
.PHONY: setup-testing

debug:
	gdb -x .gdbinit ./build/bin/6502_cpu_emulator
.PHONY: debug

run:
	./build/bin/6502_cpu_emulator
.PHONY: run

assemble:
	./build/bin/6502_assembler programs/counter.s
.PHONY: assemble

assemble-verbose:
	./build/bin/6502_assembler -v programs/counter.s
.PHONY: assemble-verbose

test: setup-testing build
	./build/bin/emulator_test
.PHONY: test

debug-test: setup-testing build
	gdb -x .gdbinit ./build/bin/6502_cpu_emulator
.PHONY: debug-test

clean:
	@rm -rf build
.PHONY: clean

reset-test:
	@cmake -S . -B build -DENABLE_TESTING=OFF
	@echo "Testing has been disabled"
.PHONY: reset-test

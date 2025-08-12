all: setup build run
.PHONY: all

nes: setup build-nes run-nes
.PHONY: nes

build:
	@$(MAKE) -C build \
		-j 12 \
        --no-print-directory
.PHONY: build

build-nes:
	@$(MAKE) -C build \
		-j 12 \
		nes_emulator \
        --no-print-directory
.PHONY: build-nes

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

run-nes:
	./build/bin/nes_emulator
.PHONY: run-nes

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

setup-dependencies:
	@echo "Installing required dependencies..."
	@if command -v apt-get >/dev/null; then \
		sudo apt-get update && \
		sudo apt-get install -y \
			libsdl2-dev \
			libglew-dev \
			libglfw3-dev; \
	elif command -v brew >/dev/null; then \
		brew install \
			sdl2 \
			glew \
			glfw; \
	else \
		echo "Please install SDL2, GLEW, and GLFW manually"; \
	fi
.PHONY: setup-dependencies

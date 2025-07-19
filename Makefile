.PHONY: all run clean

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

run: all
	@./bin/6502_cpu_emulator
.PHONY: run

clean:
	@rm -rf build bin/6502_cpu_emulator
.PHONY: clean

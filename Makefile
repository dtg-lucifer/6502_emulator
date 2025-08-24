all: setup build run
.PHONY: all

# Library and API targets
library: setup
	@$(MAKE) -C build emulator_core emulator_core_shared \
		-j 12 \
		--no-print-directory
	@echo "Library built successfully:"
	@echo "  - Static library: build/libemulator_core.a"
	@echo "  - Shared library: build/libemulator_core.so"
.PHONY: library

install: library
	@sudo $(MAKE) -C build install \
		--no-print-directory
	@echo "Library installed successfully"
.PHONY: install

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

# Binary creation and API example targets
create-binary: setup
	@$(MAKE) -C build create_test_binary \
		-j 12 \
		--no-print-directory
	@echo "Binary creator built successfully"
.PHONY: create-binary

simple-binary: create-binary
	@mkdir -p build/bin/binaries
	@./build/bin/create_test_binary build/bin/binaries/simple.bin 1
	@echo "Simple binary created at build/bin/binaries/simple.bin"
.PHONY: simple-binary

full-binary: create-binary
	@mkdir -p build/bin/binaries
	@./build/bin/create_test_binary build/bin/binaries/full.bin 2
	@echo "Full binary created at build/bin/binaries/full.bin"
.PHONY: full-binary

api-example: setup
	@$(MAKE) -C build emulator_api_example \
		-j 12 \
		--no-print-directory
	@echo "API example built successfully"
.PHONY: api-example

run-api-example: api-example simple-binary
	@./build/bin/emulator_api_example build/bin/binaries/simple.bin
.PHONY: run-api-example

run-api-example-full: api-example full-binary
	@./build/bin/emulator_api_example build/bin/binaries/full.bin
.PHONY: run-api-example-full

# Build everything and run the API example
api-demo: simple-binary full-binary api-example
	@echo "\n=== Running API example with simple binary ==="
	@./build/bin/emulator_api_example build/bin/binaries/simple.bin
	@echo "\n=== Running API example with full binary ==="
	@./build/bin/emulator_api_example build/bin/binaries/full.bin
.PHONY: api-demo

# External project example targets
external-project:
	@echo "Building external project example..."
	@./tools/build_external_project.sh -s examples/external_project/my_6502_project.cpp -o build/bin/my_6502_project -y
	@echo "External project built successfully at build/bin/my_6502_project"
.PHONY: external-project

run-external: external-project simple-binary
	@echo "Running external project example..."
	@cd build/bin && LD_LIBRARY_PATH=.. ./my_6502_project binaries/simple.bin
.PHONY: run-external

# Help target to display available commands
help:
	@echo "Available targets:"
	@echo "  make              - Build and run the emulator"
	@echo "  make build        - Build all targets"
	@echo "  make run          - Run the emulator"
	@echo "  make test         - Run the test suite"
	@echo "  make clean        - Remove build directory"
	@echo ""
	@echo "Library targets:"
	@echo "  make library      - Build static and shared libraries"
	@echo "  make install      - Install libraries and headers"
	@echo ""
	@echo "Binary targets:"
	@echo "  make create-binary - Build the binary creator utility"
	@echo "  make simple-binary - Create a simple test binary"
	@echo "  make full-binary   - Create a comprehensive test binary"
	@echo ""
	@echo "API example targets:"
	@echo "  make api-example     - Build the API example program"
	@echo "  make run-api-example - Run API example with simple binary"
	@echo "  make api-demo        - Create binaries and run API examples"
	@echo ""
	@echo "External project examples:"
	@echo "  make external-project - Build the external project example"
	@echo "  make run-external     - Build and run the external project example"
.PHONY: help

#ifndef MEMORY_MAPPER_H
#define MEMORY_MAPPER_H

#include <functional>
#include <map>
#include <vector>

#include "memory.h"
#include "types.h"

// Memory handler function signatures
using MemoryReadHandler = std::function<byte(u32)>;
using MemoryWriteHandler = std::function<void(u32, byte)>;

struct MemoryRange {
    u32 start;
    u32 end;
    MemoryReadHandler read_handler;
    MemoryWriteHandler write_handler;

    // Check if address is in this range
    bool contains(u32 address) const { return address >= start && address <= end; }
};

class MemoryMapper {
   private:
    Mem& memory;
    std::vector<MemoryRange> handlers;

   public:
    MemoryMapper(Mem& mem) : memory(mem) {}

    // Register handlers for specific memory regions
    void registerHandler(u32 start_addr, u32 end_addr, MemoryReadHandler read_handler,
                         MemoryWriteHandler write_handler) {
        handlers.push_back({start_addr, end_addr, read_handler, write_handler});
    }

    // Read from memory with handlers
    byte read(u32 address) {
        for (const auto& handler : handlers) {
            if (handler.contains(address)) {
                return handler.read_handler(address);
            }
        }
        // Default to regular memory access
        return memory[address];
    }

    // Write to memory with handlers
    void write(u32 address, byte value) {
        for (const auto& handler : handlers) {
            if (handler.contains(address)) {
                handler.write_handler(address, value);
                return;
            }
        }
        // Default to regular memory access
        memory[address] = value;
    }
};

#endif  // MEMORY_MAPPER_H

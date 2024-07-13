#include "vxpch.h"
#include "CommandBufferPool.h"

namespace Vertex {

    CommandBuffer* CommandBufferPool::get()
    {
        if (!pool.empty()) {
            CommandBuffer* cmdBuffer = pool.back();
            pool.pop_back();
            return cmdBuffer;
        }
        return new CommandBuffer();
    }

    void CommandBufferPool::reown(CommandBuffer* cmdBuffer)
    {
        cmdBuffer->clear();
        pool.emplace_back(cmdBuffer);
    }
}

#include "vxpch.h"
#include "CommandBufferPool.h"

namespace Vertex {

    CommandBuffer* CommandBufferPool::get()
    {
        if (pool == nullptr)
        {
            VX_CORE_CRITICAL("POOL IS NULL");
        }

        if (pool->empty()) {
            CommandBuffer* newBuffer = new CommandBuffer();
            
            return newBuffer;
        }
        else {
            CommandBuffer* cmdBuffer = pool->back();
            pool->pop_back();
            
            return cmdBuffer;
        }
    }

    void CommandBufferPool::reown(CommandBuffer* cmdBuffer)
    {
        cmdBuffer->clear();
        pool->emplace_back(cmdBuffer);
    }
}

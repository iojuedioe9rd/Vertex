#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include "Vertex/Core/Base.h"
#include "CommandBuffer.h"


namespace Vertex
{
    class VERTEX_API CommandBufferPool {
    private:
        std::vector<CommandBuffer*>* pool;

    public:
        CommandBufferPool()
        {
            pool = new std::vector<CommandBuffer*>();
            if (pool == nullptr)
            {
                VX_CORE_CRITICAL("POOL IS NULL");
            }
        }

        ~CommandBufferPool() {
            for (auto cmdBuffer : *pool) {
                delete cmdBuffer;
            }
            delete pool;
        }

        CommandBuffer* get();



        void reown(CommandBuffer* cmdBuffer);
    };

}
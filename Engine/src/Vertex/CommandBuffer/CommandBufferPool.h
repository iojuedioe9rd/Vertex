#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include "Vertex/Core.h"
#include "CommandBuffer.h"

namespace Vertex
{
    class VERTEX_API CommandBufferPool {
    private:
        std::vector<CommandBuffer*> pool;

    public:
        ~CommandBufferPool() {
            for (auto cmdBuffer : pool) {
                delete cmdBuffer;
            }
        }

        CommandBuffer* get();



        void reown(CommandBuffer* cmdBuffer);
    };

}
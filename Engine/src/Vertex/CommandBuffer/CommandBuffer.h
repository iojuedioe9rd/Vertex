#pragma once
#include <vector>
#include "CommandTypes.h"
namespace Vertex
{
    class CommandBuffer {
    private:
        std::vector<ICommand*> commands;

    public:
        ~CommandBuffer() {
            for (auto cmd : commands) {
                delete cmd;
            }
        }

        template<typename Callable, typename... Args>
        void addCommand(Callable&& callable, Args&&... args) {
            auto boundFunction = std::bind(std::forward<Callable>(callable), std::forward<Args>(args)...);
            commands.push_back(new LambdaCommand(boundFunction));
        }

        void executeCommands() {
            for (auto cmd : commands) {
                if (cmd)
                {
                    cmd->execute();
                }
                
            }
        }

        void clear() {
            
            VX_CORE_INFO("clear CommandBuffer: {0}", commands.size() - sizeof(ICommand));
            for (auto cmd : commands) {
                delete cmd;
            }
            commands.clear();
        }
    };
}

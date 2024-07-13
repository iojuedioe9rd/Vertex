#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <memory>

namespace Vertex {

    // Base class for commands
    class ICommand {
    public:
        virtual ~ICommand() {}
        virtual void execute() = 0;
    };

    // Lambda Command wrapping a std::function
    class LambdaCommand : public ICommand {
    private:
        std::function<void()> func;

    public:
        LambdaCommand(std::function<void()> func) : func(func) {}
        void execute() override {
            func();
        }
    };
}
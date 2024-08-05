#pragma once
#include "spdlog/sinks/base_sink.h"
#include "Vertex/ImGui/ImGuiWindows/ConsoleWindow.h"



template<typename Mutex>
class my_sink : public spdlog::sinks::base_sink <Mutex>
{
    
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {

        // log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
        // msg.raw contains pre formatted log

        // If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
        spdlog::memory_buf_t formatted;
        
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        
        Vertex::ImGuiWindows::ConsoleData* data = new Vertex::ImGuiWindows::ConsoleData();

        
        data->msg = fmt::to_string(formatted);

        data->isCoreLogger = 0;

        int j = 0;
        const char* name = "VERTEX";
        for (int i = 0; i < 7; i++)
        {
            
            if (*(msg.logger_name.begin()) == *(name))
            {
                j++;
            }
        }


        if (j >= 5) { data->isCoreLogger = 1; }

        //data->isCoreLogger = msg.logger_name.begin() == "VERTEX";

        Vertex::ImGuiWindows::ConsoleWindow::Get().UpdateLogMsg(data);
        
    }

    void flush_() override
    {
        std::cout << std::flush;
    }
};

using my_sink_mt = my_sink<std::mutex>;
using my_sink_st = my_sink<spdlog::details::null_mutex>;
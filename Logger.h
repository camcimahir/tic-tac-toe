#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>
#include "imgui/imgui.h"

enum class LogLevel {
    USER_INPUT,
    NORMAL,
    WARNING,
    ERROR
};


class Logger {
    private:
        struct LogEntry {
            std::string message;
            LogLevel level;
        };
        std::vector<LogEntry> logEntries; 

        bool m_AutoScroll = true;
        char m_InputBuffer[256] = {};

        Logger(){
            logFile.open("log.txt", std::ios::out); 
            if (!logFile.is_open()) {
                std::cerr << "Failed to open log file." << std::endl;
            }
        };
        ~Logger(){
            if (logFile.is_open()) {
                logFile.close();
            }
        }

    public:
        std::ofstream logFile;
        bool m_ShowUserInput = true;
        bool m_ShowNormal = true;
        bool m_ShowWarning = true;
        bool m_ShowError = true;

        char inputBuffer[256] = {};

        static Logger& GetInstance() {
            static Logger instance;
            return instance;
        }


        void Log(const std::string& message, LogLevel level){

            //get current time
            using namespace std::chrono;
            auto now = system_clock::now();
            auto time_t_now = system_clock::to_time_t(now);
            tm timeinfo{};
            localtime_s(&timeinfo, &time_t_now);
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            std::ostringstream timestamp;            
            timestamp << std::put_time(&timeinfo, "%H:%M:%S")
              << "." << std::setfill('0') << std::setw(3)
              << ms.count();

            std::string logEntry = "[" + timestamp.str() + "] " + message;
            logEntries.push_back({logEntry, level});

            if (logFile.is_open()) {
                logFile << logEntry << std::endl;
            }
        }

        void Draw(const char* title){
    
            ImGui::Begin(title);

            ImGui::DockSpaceOverViewport();
            ImGui::ShowDemoWindow();
            //ImGui::LogButtons();
            ImGui::SameLine();
            if (ImGui::Button("options")){
                ImGui::OpenPopup("Options");
            }
            ImGui::SameLine();
            if (ImGui::Button("clear")){
                Logger::GetInstance().clearLog();
            }
            ImGui::SameLine();
            if (ImGui::Button("Test Info")){
                Logger::GetInstance().logInfo("This is a test info message.", LogLevel::NORMAL);
            }
            ImGui::SameLine();
            if (ImGui::Button("Test Warning")){
                Logger::GetInstance().logInfo("This is a test warning message.", LogLevel::WARNING);
            }
            ImGui::SameLine();
            if (ImGui::Button("Test Error")){
                Logger::GetInstance().logInfo("This is a test error message.", LogLevel::ERROR);
            }
            ImGui::SameLine();
            if (ImGui::Button("Log to File")) {
                    ExportLogsToFile();
            }

            ImGui::Separator();

            //ImGui::Checkbox("Show Normal", &m_ShowNormal);
            ImGui::SameLine();


            ImGui::Checkbox("User Input ", &m_ShowUserInput);
            ImGui::SameLine();
            ImGui::Checkbox("Normal ", &m_ShowNormal);
            ImGui::SameLine();
            ImGui::Checkbox("Warning ", &m_ShowWarning);
            ImGui::SameLine();
            ImGui::Checkbox("Error ", &m_ShowError);
            ImGui::Separator();

            ImGui::BeginChild("Scrolling", ImVec2(0, -35));
            for (const auto& entry : logEntries) {
            
                if (entry.level == LogLevel::NORMAL && !m_ShowNormal) {
                    continue;
                }
                if (entry.level == LogLevel::USER_INPUT && !m_ShowUserInput) {
                    continue;
                }
                if (entry.level == LogLevel::WARNING && !m_ShowWarning) {
                    continue;
                }
                if (entry.level == LogLevel::ERROR && !m_ShowError) {
                    continue;
                }
                
                if (entry.level == LogLevel::USER_INPUT) {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "%s", entry.message.c_str());
                } else if (entry.level == LogLevel::NORMAL) {
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", entry.message.c_str());
                } else if (entry.level == LogLevel::WARNING) {
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", entry.message.c_str());
                } else if (entry.level == LogLevel::ERROR) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", entry.message.c_str());
                }
            }
            if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::Separator();
            ImGui::EndChild();

            ImGui::BeginChild("Input", ImVec2(0, 0), false);
   
            if (ImGui::InputText("Console", inputBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue)) 
            {
                Log("USER: " + std::string(inputBuffer), LogLevel::USER_INPUT);
                inputBuffer[0] = '\0';
            }

            ImGui::EndChild();
            ImGui::End();
            if (ImGui::BeginPopup("Options")){
                ImGui::Text("Options would go here.");
                ImGui::EndPopup();
            }
        }

        void ExportLogsToFile() {
            std::ofstream exportFile("../../game_logs.txt", std::ios::out);
            if (!exportFile.is_open()) {
                Log("Failed to export logs to file!", LogLevel::ERROR);
                return;
            }

            for (const auto& entry : logEntries) {
                exportFile << entry.message << std::endl;
            }

            exportFile.close();

        }

        void logInfo(const std::string& message, LogLevel level) {
            
            if (level == LogLevel::USER_INPUT) {
                Log("USER: " + message, level); // this line actually doesn't get called look at Draw function 
            } else if (level == LogLevel::NORMAL) {
                //ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", message.c_str());
                Log("INFO: " + message, level);
            } else if (level == LogLevel::WARNING) {
                //ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", message.c_str());
                Log("WARNING: " + message, level);
            } else if (level == LogLevel::ERROR) {
                //ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", message.c_str());
                Log("ERROR: " + message, level);
            } 
        }

        void clearLog() {
            logEntries.clear();
        }

};
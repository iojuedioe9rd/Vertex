#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <algorithm>

// Include your FakeFS header
#include <Vertex/CustomFileFormat/FakeFS.h>
using namespace Vertex;



class CLI {
public:
    CLI(FakeFS* fileSystem) : fs(fileSystem) {}

    void run() {
        std::string input;
        VX_INFO("Vertex CLI started. Type 'help' for a list of commands.");

        while (true) {
            VX_INFO("> ");
            std::getline(std::cin, input);

            if (input == "exit") {
                break;
            }

            parseCommand(input);
        }
    }

private:
    FakeFS* fs;

    void parseCommand(const std::string& input) {
        std::istringstream iss(input);
        std::string command;
        std::vector<std::string> args;

        if (!(iss >> command)) {
            VX_ERROR("No command provided.");
            return;
        }

        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        if (command == "create") {
            createFile(args);
        }
        else if (command == "update") {
            updateFile(args);
        }
        else if (command == "read") {
            readFile(args);
        }
        else if (command == "delete") {
            deleteFile(args);
        }
        else if (command == "list") {
            listFiles();
        }
        else if (command == "create_riff") {
            createRIFF(args);
        }
        else if (command == "help") {
            showHelp();
        }
        else {
            VX_ERROR("Unknown command. Type 'help' for a list of commands.");
        }
    }

    void createFile(const std::vector<std::string>& args) {
        if (args.size() < 2) {
            VX_ERROR("Insufficient arguments. Usage: create <filename> <content>");
            return;
        }
        std::string filename = args[0];
        std::string content = join(args, 1);
        if (fs->createFile(filename, content)) {
            VX_INFO("File created: " + filename);
        }
        else {
            VX_ERROR("File already exists.");
        }
    }

    void updateFile(const std::vector<std::string>& args) {
        if (args.size() < 2) {
            VX_ERROR("Insufficient arguments. Usage: update <filename> <content>");
            return;
        }
        std::string filename = args[0];
        std::string content = join(args, 1);
        if (fs->updateFile(filename, content)) {
            VX_INFO("File updated: " + filename);
        }
        else {
            VX_ERROR("File does not exist.");
        }
    }

    void readFile(const std::vector<std::string>& args) {
        if (args.size() < 1) {
            VX_ERROR("Insufficient arguments. Usage: read <filename>");
            return;
        }
        std::string filename = args[0];
        std::string content = fs->readFile(filename);
        if (!content.empty()) {
            VX_INFO("File content: " + content);
        }
        else {
            VX_ERROR("File not found.");
        }
    }

    void deleteFile(const std::vector<std::string>& args) {
        if (args.size() < 1) {
            VX_ERROR("Insufficient arguments. Usage: delete <filename>");
            return;
        }
        std::string filename = args[0];
        if (fs->deleteFile(filename)) {
            VX_INFO("File deleted: " + filename);
        }
        else {
            VX_ERROR("File not found.");
        }
    }

    void listFiles() {
        fs->listFiles();
    }

    void createRIFF(const std::vector<std::string>& args) {
        if (args.size() < 2) {
            VX_ERROR("Insufficient arguments. Usage: create_riff <folder_path> <riff_file_path> [compress]");
            return;
        }
        std::string folderPath = args[0];
        std::string riffFilePath = args[1];
        bool compress = args.size() > 2 && args[2] == "compress";

        if (fs->createRIFFFromDirectory(folderPath, riffFilePath, compress)) {
            VX_INFO("RIFF file created: " + riffFilePath);
        }
        else {
            VX_ERROR("Failed to create RIFF file.");
        }
    }

    void showHelp() {
        VX_INFO("Available commands:");
        VX_INFO("create <filename> <content>  - Create a new file with specified content.");
        VX_INFO("update <filename> <content>  - Update an existing file with new content.");
        VX_INFO("read <filename>              - Read the content of a file.");
        VX_INFO("delete <filename>            - Delete a file.");
        VX_INFO("list                         - List all files.");
        VX_INFO("create_riff <folder_path> <riff_file_path> [compress] - Create a RIFF file from a directory. Optionally compress.");
        VX_INFO("help                         - Show this help message.");
        VX_INFO("exit                         - Exit the CLI.");
    }

    std::string join(const std::vector<std::string>& args, size_t start) {
        std::ostringstream oss;
        for (size_t i = start; i < args.size(); ++i) {
            if (i > start) {
                oss << " ";
            }
            oss << args[i];
        }
        return oss.str();
    }
};

int main() {

    Logger::Init();

    std::string encryptionKey;
    bool useChecksum = true; // Toggle checksum usage based on your requirements

    // Prompt user for encryption key
    VX_INFO("Enter encryption key: ");
    std::getline(std::cin, encryptionKey);

    // Create FakeFS instance with user-provided encryption key
    FakeFS* fs = new FakeFS(encryptionKey, useChecksum);

    // Initialize and run CLI
    CLI cli(fs);
    cli.run();

    // Clean up
    delete fs;
    return 0;
}
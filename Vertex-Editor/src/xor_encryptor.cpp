#include "xor_encryptor.h"
#include "../resource.h"
#include <iostream>
#include <random>
#include <iomanip>
#include <windows.h> // For GetComputerNameA

// Function to generate a random byte
std::uint8_t randomByte() {
    static std::random_device rd;  // Random number generator
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 255); // Use int for distribution
    return static_cast<std::uint8_t>(distribution(generator)); // Cast the result to uint8_t
}

// Function to get the hostname
std::string getHostname() {
    char hostname[256];
    DWORD size = sizeof(hostname);
    GetComputerNameA(hostname, &size);
    return std::string(hostname);
}

// XOR encryption function with machine-dependent padding
std::string xorEncryptDecrypt(const std::string& data, const std::string& key, size_t& originalLength) {
    std::string paddedData = std::string();
    std::string hostname = getHostname();

    // Generate padding based on the hostname
    paddedData += hostname; // Start with hostname

    // Perform XOR encryption
    std::string output(data.size(), '\0');
    for (size_t i = 0; i < data.size(); ++i) {
        output[i] = data[i] ^ key[i % key.size()] ^ paddedData[i % paddedData.size()]; // XOR operation
    }

    // Store the original length for decryption
    originalLength = data.size();
    return output;
    
}

std::string LoadXorKey()
{
    HINSTANCE hInstance = GetModuleHandle(NULL); // Get the instance handle
    char buffer[1024]; // Buffer to hold the string
    if (LoadStringA(hInstance, IDS_STRING108, buffer, sizeof(buffer))) {
        return std::string(buffer); // Return the loaded string as std::string
    }
    else {
        return ""; // Return an empty string on failure
    }
}

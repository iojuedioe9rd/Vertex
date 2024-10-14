#pragma once
#include <windows.h>
#include <string>

// Function to encrypt/decrypt using XOR with machine-dependent padding
std::string xorEncryptDecrypt(const std::string& data, const std::string& key, size_t& originalLength);

std::string LoadXorKey();
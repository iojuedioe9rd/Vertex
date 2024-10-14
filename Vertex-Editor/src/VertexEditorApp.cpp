#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>
#include "VXEntities.h"
#include "EditorLayer.h"
#include "xor_encryptor.h"



std::string bytesToString(const unsigned char* data, size_t length) {
	return std::string(reinterpret_cast<const char*>(data), length);
}

namespace Vertex {

	class VertexEditor : public Application
	{
	public:
		VertexEditor()
			: Application("Cataclysmic Blaze")
		{
			srand(5335);
			std::string plaintext = "Hello, World!";
			std::string key = "mysecretkey"; // Define your key

			size_t originalLength = 0;

			// Encrypt the plaintext with machine-dependent padding
			std::string ciphertext = xorEncryptDecrypt(plaintext, key, originalLength);

			// Output the results
			std::cout << "Ciphertext (hex): ";
			for (unsigned char c : ciphertext) {
				std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
			}
			std::cout << std::endl;

			// Decrypting (using the same function)
			std::string decryptedText = xorEncryptDecrypt(ciphertext, key, originalLength);

			// Extract the original plaintext by using the original length
			std::string originalText = decryptedText.substr(0, originalLength); // Use originalLength
			std::cout << "Decrypted: " << originalText << std::endl;


			
			
			VXEntities_INIT(VXEntities_INIT_USE_EVERYTHING);
			PushLayer(new EditorLayer());

			

			
		}

		~VertexEditor()
		{

			VXEntities_FREE();
		}
	};

	
	Application* Vertex::CreateApp()
	{
		return new Vertex::VertexEditor();
	}
}


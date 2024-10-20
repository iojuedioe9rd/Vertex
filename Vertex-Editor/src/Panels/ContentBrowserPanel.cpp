#include "ContentBrowserPanel.h"
#include "Vertex/Audio/AudioManager.h"
#include <Vertex/ImGui/ImGuiLink.h>

bool endsWith(const std::string& fullString, const std::string& ending)
{
	// Check if the ending string is longer than the full
	// string
	if (ending.size() > fullString.size())
		return false;

	// Compare the ending of the full string with the target
	// ending
	return fullString.compare(fullString.size() - ending.size(), ending.size(), ending) == 0;
}

namespace Vertex
{
	// TODO: Once we have projects, change this
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGuiLink::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGuiLink::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string filenameString = relativePath.filename().string();
			if (directoryEntry.is_directory())
			{
				if (ImGuiLink::Button(filenameString.c_str()))
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGuiLink::Button(filenameString.c_str()))
				{
					if (endsWith(filenameString, ".wav"))
					{
						Audio* audio = AudioManager::GetAudioFromFileName((path.string()).c_str());
						audio->Play();
					}
				}
			}
		}

		ImGuiLink::End();
	}
}
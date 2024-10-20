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
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
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


		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGuiLink::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGuiLink::Columns(columnCount, 0, false);

		
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string filenameString = relativePath.filename().string();

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGuiLink::ImageButton((void*)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			if (ImGuiLink::IsItemHovered() && ImGuiLink::IsMouseDoubleClicked(ImGuiLink::ImGuiMouseButton::Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			ImGuiLink::TextWrapped(filenameString);

			ImGuiLink::NextColumn();
		}

		ImGuiLink::Columns(1);

		ImGuiLink::End();
	}
}
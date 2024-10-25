#include "ContentBrowserPanel.h"
#include "Vertex/Audio/AudioManager.h"
#include <Vertex/ImGui/ImGuiLink.h>
#include "../resource.h"

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
	extern  const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{

		m_DirectoryIcon = Texture2D::CreateWin(IDB_PNG2, "PNG");
		m_FileIcon = Texture2D::CreateWin(IDB_PNG3, "PNG");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGuiLink::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
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
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = relativePath.filename().string();

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGuiLink::ImageButton((void*)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGuiLink::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGuiLink::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGuiLink::EndDragDropSource();
			}

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
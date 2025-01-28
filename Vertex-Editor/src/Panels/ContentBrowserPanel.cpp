#include "ContentBrowserPanel.h"
#include "Vertex/Audio/AudioManager.h"
#include <Vertex/ImGui/ImGuiLink.h>
#include <Vertex/AssetManager/AssetManager.h>
#include "../resource.h"
#include <Vertex/AssetManager/EditorAssetManager.h>

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

	ContentBrowserPanel::ContentBrowserPanel(std::filesystem::path path)
		: m_CurrentDirectory(g_AssetPath), m_BaseDirectory(g_AssetPath), m_ThumbnailCache(CreateRef<ThumbnailCache>(path))
	{

		m_TreeNodes.push_back(TreeNode(".", 0));

		m_DirectoryIcon = GET_Texture2D_AssetManager("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = GET_Texture2D_AssetManager("Resources/Icons/ContentBrowser/FileIcon.png");

		RefreshAssetTree();

		m_Mode = Mode::Asset;
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		

		ImGui::Begin("Content Browser");

		const char* label = m_Mode == Mode::Asset ? "Asset" : "File";
		if (ImGui::Button(label))
		{
			m_Mode = m_Mode == Mode::Asset ? Mode::FileSystem : Mode::Asset;
		}

		if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
		{
			ImGui::SameLine();
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}


		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		if (m_Mode == Mode::Asset)
		{
			TreeNode* node = &m_TreeNodes[0];

			auto currentDir = std::filesystem::relative(m_CurrentDirectory);
			for (const auto& p : currentDir)
			{
				// if only one level
				if (node->Path == currentDir)
					break;

				if (node->Children.find(p) != node->Children.end())
				{
					node = &m_TreeNodes[node->Children[p]];
					continue;
				}
				else
				{
					// can't find path
					return;
				}

			}

			for (const auto& [item, treeNodeIndex] : node->Children)
			{
				bool isDirectory = std::filesystem::is_directory(currentDir / item);

				std::string itemStr = item.generic_string();

				ImGui::PushID(itemStr.c_str());
				Ref<Texture2D> icon = isDirectory ? m_DirectoryIcon : m_FileIcon;

				if (!isDirectory)
				{
					icon = nullptr;
					if (!icon)
					{
						icon = m_FileIcon;
					}
				}

				

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 }) && !isDirectory && item.extension() == ".wav")
				{
					AudioManager::GetAudioFromFileName(currentDir / item, false)->Play();
					VX_INFO("Playing sound: {0}", item.generic_string());
				}

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete"))
					{
						VX_CORE_ASSERT(false, "Not implemented");
					}
					

					ImGui::EndPopup();
				}

				if (ImGui::BeginDragDropSource())
				{
					AssetHandle handle = m_TreeNodes[treeNodeIndex].Handle;
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", &handle, sizeof(AssetHandle));
					ImGui::EndDragDropSource();
				}


				ImGui::PopStyleColor();
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (isDirectory)
						m_CurrentDirectory /= item.filename();
				}

				ImGui::TextWrapped(itemStr.c_str());

				ImGui::NextColumn();

				ImGui::PopID();
			}
		}
		else
		{
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				const auto& path = directoryEntry.path();
				std::string filenameString = path.filename().string();

				ImGui::PushID(filenameString.c_str());

				// TODO THUMBNAIL
				auto relativePath = std::filesystem::relative(path);
				Ref<Texture2D> thumbnail = m_DirectoryIcon;
				if (!directoryEntry.is_directory())
				{
					thumbnail = m_ThumbnailCache->GetOrCreateThumbnail(relativePath);
					if (!thumbnail)
						thumbnail = m_FileIcon;
				}
				
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				
				glm::vec2 size = glm::vec2(thumbnail->GetWidth(), thumbnail->GetHeight());
				ImGui::ImageButton((ImTextureID)thumbnail->GetRendererID(), { thumbnailSize * glm::normalize(size).x, thumbnailSize * glm::normalize(size).y }, { 0, 1 }, { 1, 0 });

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Import"))
					{
						
						((EditorAssetManager*)(void*)g_AssetManagerBase.get())->ImportAsset(relativePath);
						RefreshAssetTree();
					}
					ImGui::EndPopup();
				}

				ImGui::PopStyleColor();
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
						m_CurrentDirectory /= path.filename();
				}

				ImGui::TextWrapped(filenameString.c_str());

				ImGui::NextColumn();

				ImGui::PopID();
			}
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();

		/*
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

		ImGuiLink::End();*/
	}

	void ContentBrowserPanel::RefreshAssetTree()
	{
		const auto& assetRegistry = ((EditorAssetManager*)(void*)g_AssetManagerBase.get())->GetAssetRegistry();
		for (const auto& [handle, metadata] : assetRegistry)
		{
			uint32_t currentNodeIndex = 0;

			for (const auto& p : metadata.FilePath)
			{
				auto it = m_TreeNodes[currentNodeIndex].Children.find(p.generic_string());
				if (it != m_TreeNodes[currentNodeIndex].Children.end())
				{
					currentNodeIndex = it->second;
				}
				else
				{
					// add node
					TreeNode newNode(p, handle);
					newNode.Parent = currentNodeIndex;
					m_TreeNodes.push_back(newNode);

					m_TreeNodes[currentNodeIndex].Children[p] = m_TreeNodes.size() - 1;
					currentNodeIndex = m_TreeNodes.size() - 1;
				}

			}
		}
	}
}
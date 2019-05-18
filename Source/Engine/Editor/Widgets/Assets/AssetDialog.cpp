// Copyright 2015-2019 Piperift - All rights reserved

#include "AssetDialog.h"
#include <imgui/imgui.h>
#include <imgui/imgui_stl.h>


#ifdef _WIN32
void AssetDialog::FillRoots()
{
	/*DWORD drivesMask = GetLogicalDrives();
	for (TCHAR drive = 'A'; drive <= 'Z'; drive++, drivesMask >>= 1) {
		if (!(drivesMask & 1)) continue;
		TCHAR rootPath[4];
		TCHAR volumeName[MAX_PATH + 1];
		rootPath[0] = drive;
		rootPath[1] = ':';
		rootPath[2] = '\\';
		rootPath[3] = '\0';
		if (!GetVolumeInformation(rootPath, volumeName, MAX_PATH + 1, nullptr, nullptr, nullptr, nullptr, 0))
		{
			continue;
		}
#ifdef UNICODE
		int needed;
		LPSTR str;

		needed = WideCharToMultiByte(CP_UTF8, 0, rootPath, -1, NULL, 0, NULL, NULL);
		if (needed <= 0) continue;
		str = (LPSTR)_malloca(needed);
		WideCharToMultiByte(CP_UTF8, 0, rootPath, -1, str, needed, NULL, NULL);
		std::string root = str;
		_freea(str);

		needed = WideCharToMultiByte(CP_UTF8, 0, volumeName, -1, NULL, 0, NULL, NULL);
		if (needed <= 0) continue;
		str = (LPSTR)_malloca(needed);
		WideCharToMultiByte(CP_UTF8, 0, volumeName, -1, str, needed, NULL, NULL);
		std::string label = root + " (" + str + ")";
		_freea(str);
#else
		std::string root = rootName;
		std::string label = root + " (" + volumeName + ")";
#endif
		roots.Add({root, label});
	}*/
}
#else
void AssetDialog::FillRoots() { roots.Add({"/", "(root)"}); }
#endif

void AssetDialog::OpenDialog()
{
	ImGui::OpenPopup(title.c_str());
	selectedItems.Empty();
	newFile = "";
}

bool AssetDialog::Draw()
{
	bool bDone = false;
	if (ImGui::BeginPopupModal(title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (bDirty)
		{
			FillRoots();

			std::filesystem::space_info spaceInfo = std::filesystem::space(currentPath);

			for (auto& p : std::filesystem::directory_iterator(currentPath))
			{
				if (p.is_directory())
				{
					directories.Add(p.path());
				}
				else
				{
					auto status = p.status();
					auto lastWrite = std::filesystem::last_write_time(p);

					DateTime lastWriteDate{ DateTime::SysTime{ lastWrite.time_since_epoch() } };
					files.Add({
						p.path(),
						std::filesystem::file_size(p),
						lastWriteDate
					});
				}
			}

			if (sorter.order != EDialogSortOrder::Unsorted)
			{
				std::sort(files.begin(), files.end(), sorter);
			}
			std::sort(directories.begin(), directories.end());
			bDirty = false;
		}

		const bool bGoHome = ImGui::Button("Home");
		bool bGoUp = false;
		Path navigatePath;

		ImGui::SameLine();

		const Path relative = std::filesystem::relative(currentPath, FileSystem::GetAssetsPath());
		ImGui::Text(relative.string().c_str());
		{
			ImGui::BeginChild("Directories", ImVec2(250, 350), true, ImGuiWindowFlags_HorizontalScrollbar);
			if (ImGui::TreeNode("Roots"))
			{
				for (auto& p : roots)
				{
					if (ImGui::Selectable(p.label.c_str(), false, 0, ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
					{
						navigatePath = p.root;
					}
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Directories", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (FileSystem::IsAssetPath(currentPath.parent_path()) &&
					ImGui::Selectable("..", false, 0, ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
				{
					bGoUp = true;
				}
				for (auto& p : directories)
				{
					if (ImGui::Selectable(p.filename().string().c_str(), false, 0, ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
					{
						navigatePath = p;
					}
				}
				ImGui::TreePop();
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();

		AssetDialogFile* selected = nullptr;
		{
			ImGui::BeginChild("Files", ImVec2(500, 350), true, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::Columns(3);
			ImGui::SetColumnWidth(1, 60.f);

			const String name = CString::Printf("%s Name", sorter.GetOrderSymbol(EDialogSortType::Name));
			if (ImGui::Selectable(name.c_str()))
			{
				if (sorter.type != EDialogSortType::Name)
				{
					sorter.type = EDialogSortType::Name;
					sorter.order = EDialogSortOrder::SortDown;
				}
				else
				{
					sorter.SwitchToNextOrder();
				}
				ResetCache();
			}
			ImGui::NextColumn();

			const String size = CString::Printf("%s Size", sorter.GetOrderSymbol(EDialogSortType::Size));
			if (ImGui::Selectable(size.c_str()))
			{
				if (sorter.type != EDialogSortType::Size)
				{
					sorter.type = EDialogSortType::Size;
					sorter.order = EDialogSortOrder::SortDown;
				}
				else
				{
					sorter.SwitchToNextOrder();
				}
				ResetCache();
			}
			ImGui::NextColumn();

			const String date = CString::Printf("%s Date", sorter.GetOrderSymbol(EDialogSortType::Date));
			if (ImGui::Selectable(date.c_str()))
			{
				if (sorter.type != EDialogSortType::Date)
				{
					sorter.type = EDialogSortType::Date;
					sorter.order = EDialogSortOrder::SortDown;
				}
				else
				{
					sorter.SwitchToNextOrder();
				}
				ResetCache();
			}
			ImGui::NextColumn();
			ImGui::Separator();

			for (auto& file : files)
			{
				const std::string filenameStr = file.path.filename().string();
				const String label = CString::Printf("##%s", filenameStr.c_str());

				if (ImGui::Selectable(label.c_str(), file.bSelected, ImGuiSelectableFlags_SpanAllColumns))
				{
					for (auto& f : files)
					{
						f.bSelected = false;
					}
					file.bSelected = true;

					if (mode == EDialogMode::NewAsset)
					{
						newFile = String{ filenameStr.c_str(), filenameStr.size() };
					}
				}
				ImGui::SameLine();
				ImGui::Text(file.path.filename().string().c_str());
				ImGui::NextColumn();
				String sizeValue = CString::ParseMemorySize(file.size);
				ImGui::Text(sizeValue.c_str());
				ImGui::NextColumn();
				const String dateStr = file.dateTime.ToString();
				ImGui::Text(dateStr.c_str());
				ImGui::NextColumn();

				if (file.bSelected)
				{
					selected = &file;
				}
			}
			ImGui::EndChild();
		}

		String selectedStr;
		bool bGotSelected = selected;

		if (mode == EDialogMode::NewAsset)
		{
			ImGui::Text(currentPath.string().c_str());
			ImGui::SameLine();

			String label = CString::Printf("##%s Filename", title.c_str());
			ImGui::InputText(label.c_str(), label);

			selectedStr = newFile;
			bGotSelected = !newFile.empty();
		}
		else
		{
			selectedStr = FileSystem::ToString(currentPath / Path(selected ? selected->path : "..."));
			ImGui::Text(selectedStr.c_str());
		}

		if (!bGotSelected)
		{
			const ImVec4 lolight = ImGui::GetStyle().Colors[ImGuiCol_TextDisabled];
			ImGui::PushStyleColor(ImGuiCol_Button, lolight);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, lolight);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, lolight);
		}

		if (ImGui::Button("OK", ImVec2(120, 30)) && bGotSelected)
		{
			selectedItems.Empty(true, 1);
			selectedItems.Add(selectedStr);
			ImGui::CloseCurrentPopup();
			bDone = true;
		}

		if (!bGotSelected)
		{
			ImGui::PopStyleColor(3);
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 30)))
		{
			ImGui::CloseCurrentPopup();
			bDone = true;
		}
		ImGui::EndPopup();

		if (bGoUp)
		{
			currentPath = currentPath.parent_path();
			ResetCache();
		}
		else if (!navigatePath.empty())
		{
			currentPath = currentPath / navigatePath;
			ResetCache();
		}
		else if (bGoHome)
		{
			currentPath = FileSystem::GetAssetsPath();
			ResetCache();
		}
	}
	return bDone;
}
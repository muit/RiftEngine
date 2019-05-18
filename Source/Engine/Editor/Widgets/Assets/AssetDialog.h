#pragma once

#include "Core/Files/FileSystem.h"
#include "Core/Misc/DateTime.h"

enum class EDialogMode : u8 {
	NewAsset,
	SelectFolder,
	SelectAsset
};

enum class EDialogSortOrder : u8 {
	Unsorted,
	SortDown,
	SortUp
};

enum class EDialogSortType : u8 {
	Name,
	Size,
	Date
};

struct AssetDialogFile
{
	Path path;
	size_t size;
	DateTime dateTime;
	bool bSelected;
};

struct AssetDialogSorter
{
	EDialogSortOrder order = EDialogSortOrder::Unsorted;
	EDialogSortType  type  = EDialogSortType::Name;

	bool operator()(const AssetDialogFile& a, const AssetDialogFile& b) const
	{
		switch (type)
		{
		case EDialogSortType::Name:
			return SortType(a.path, b.path);
		case EDialogSortType::Size:
			return !SortType(a.size, b.size);
		case EDialogSortType::Date:
			return SortType(a.dateTime, b.dateTime);
		}
		return false;
	}

	void SwitchToNextOrder()
	{
		// Increment enum by 1 looping
		order = EDialogSortOrder((u8(order) + 1) % 3);
	}

	const char* GetOrderSymbol(EDialogSortType inType) const
	{
		if (type == inType)
		{
			switch (order)
			{
			case EDialogSortOrder::SortDown: return "v";//R"(▾)";
			case EDialogSortOrder::SortUp:   return "^";//R"(▴)";
			}
		}
		return " ";
	}

private:

	template<typename T>
	inline bool SortType(const T& a, const T& b) const
	{
		switch (order)
		{
		case EDialogSortOrder::SortDown: return a < b;
		case EDialogSortOrder::SortUp:   return a > b;
		}
		return false;
	}
};

struct AssetDialog
{
public:
	bool bDirty = true;
	Path currentPath;
	EDialogMode mode;
	String title;

	struct Root {
		Path root;
		String label;
	};
	TArray<Root> roots;
	TArray<Path> directories;

	TArray<AssetDialogFile> files;
	TArray<String> selectedItems;

	String newFile;

	AssetDialogSorter sorter;


	AssetDialog(String title, EDialogMode mode = EDialogMode::NewAsset)
		: title{title}
		, mode{mode}
		, currentPath{ FileSystem::GetAssetsPath() }
	{}


	void OpenDialog();
	bool Draw();

	const TArray<String>& GetSelected() const { return selectedItems; }

  private:

	void FillRoots();
	void ResetCache() {
		bDirty = true;
		roots.Empty(false);
		directories.Empty(false);
		files.Empty(false);
	}
};

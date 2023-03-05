#include "Resource.h"

namespace Resource
{
	void Import(std::string const filepath, void (* const pImportFunc)(std::string const& filePath))
	{
		if (std::filesystem::exists(filepath))
		{
			if (std::filesystem::is_regular_file(filepath))
			{
				std::string file = filepath;
				pImportFunc(file);
			}
			else
			{
				for (std::filesystem::path const& path : std::filesystem::recursive_directory_iterator(filepath))
				{
					if (std::filesystem::is_regular_file(path))					// 폴더 자체를 import 할 때 오류가 생기므로 exception
					{
						//printf("%s\n", path.string().c_str());				// test
						std::string file = path.string();
						//std::replace(file.begin(), file.end(), '\\', '/');	// 문자열 치환
						pImportFunc(file);
					}
				}
			}
		}
	}
}
#pragma once
#include <filesystem>

namespace Resource { void Import(std::string const, void (* const pImportFunc)(std::string const& filePath)); }
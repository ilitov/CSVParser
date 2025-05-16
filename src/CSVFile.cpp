#include <fstream>
#include <charconv>
#include <algorithm>

#include "CSVFile.hpp"

namespace {
	struct CloseOnExit {
		~CloseOnExit() {
			m_file.clear();
			m_file.close();
		}

		std::ifstream &m_file;
	};

	std::vector<std::string> parseColumnNames(std::ifstream &file) {
		std::vector<std::string> columns;

		std::string line;
		std::getline(file, line);

		std::size_t startOffset = 0;
		std::size_t commaOffset = 0;

		while ((commaOffset = line.find_first_of(',', startOffset)) != std::string::npos) {
			columns.push_back(line.substr(startOffset, commaOffset - startOffset));

			// Do a bit of filtering just in case
			std::string &name = columns.back();
			auto it = std::remove_if(name.begin(), name.end(), [](char c) { return c <= 0 || !std::isalnum(c); });
			name.erase(it, name.end());

			// Store names in lowercase to handle basic typos
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);

			startOffset = commaOffset + 1;
		}

		// Don't forget the last column name
		columns.push_back(line.substr(startOffset));

		return columns;
	}

	std::vector<CSVFile::ColumnDataType> parseColumns(std::ifstream &file, std::size_t count) {
		std::vector<CSVFile::ColumnDataType> columns{ count };

		std::string line;
		while (std::getline(file, line)) {
			std::size_t startOffset = 0;
			std::size_t commaOffset = 0;

			for (std::size_t i = 0; i < count; ++i) {
				commaOffset = line.find_first_of(',', startOffset);

				CSVFile::DataType value{};
				std::from_chars(line.c_str() + startOffset, line.c_str() + std::min(line.size(), commaOffset), value);

				columns[i].push_back(value);

				startOffset = commaOffset + 1;
			}
		}

		return columns;
	}
}

CSVFile::CSVFile() {
	// Add dummy empty data. The assumption is that all columns must contain at least 1 element
	m_columns.emplace_back();
}

bool CSVFile::appendColumn(const std::string& columnName, ColumnDataType columnData) {
	if (m_nameToIndex.find(columnName) != m_nameToIndex.end() || std::size(columnData) == 0) {
		return false;
	}

	const std::size_t index = m_columns.size();

	m_nameToIndex[columnName] = index;
	m_columns.push_back(std::move(columnData));

	return true;
}

const CSVFile::ColumnDataType& CSVFile::getColumn(const std::string &name) const {
	const auto indexIt = m_nameToIndex.find(name);
	if (indexIt == m_nameToIndex.end()) {
		return m_columns[SENTINEL];
	}

	return m_columns[indexIt->second];
}

std::optional<CSVFile> readCSVFile(std::string_view filePath) {
	std::ifstream file{ filePath.data() };
	CloseOnExit closeOnExit{ file };

	if (!file) {
		return std::nullopt;
	}

	CSVFile parsedFile;

	std::vector<std::string> columnNames = parseColumnNames(file);

	std::vector<CSVFile::ColumnDataType> columnData = parseColumns(file, columnNames.size());

	for (std::size_t i = 0; i < columnNames.size(); ++i) {
		parsedFile.appendColumn(columnNames[i], std::move(columnData[i]));
	}

	return std::optional{ std::move(parsedFile) };
}

#ifndef _CSV_FILE_HPP_
#define _CSV_FILE_HPP_

#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <optional>
#include <string_view>

class CSVFile {
	static const std::size_t SENTINEL = 0;

public:
	using DataType = std::int32_t;
	using ColumnDataType = std::vector<DataType>;

public:
	CSVFile();

	bool appendColumn(const std::string& columnName, ColumnDataType columnData);

	const ColumnDataType& getColumn(const std::string& columnName) const;

private:
	std::map<std::string, std::size_t> m_nameToIndex;
	std::vector<ColumnDataType> m_columns;
};

std::optional<CSVFile> readCSVFile(std::string_view filePath);

#endif // !_CSV_FILE_HPP_

#include <iostream>
#include <limits>

#include "CSVFile.hpp"
#include "DataProcessor.hpp"
#include "CommandHandlers.hpp"

template <typename T>
void runPrompt(const CSVFile &csvFile, const CommandHandlers<T> &handlers) {
	while (std::cin) {
		std::cout << "> ";

		char op{};
		std::string lhs, rhs;

		std::cin >> lhs >> op >> rhs;

		std::transform(lhs.begin(), lhs.end(), lhs.begin(), ::tolower);
		std::transform(rhs.begin(), rhs.end(), rhs.begin(), ::tolower);

		const CSVFile::ColumnDataType &lhsColumn = csvFile.getColumn(lhs);
		if (lhsColumn.empty()) {
			std::cout << "Incorrect FIRST column name\n";
			continue;
		}

		const auto &handler = handlers.resolveHandler(op);

		if (!handler) {
			std::cout << "Invalid binary op: " << op << '\n';
			continue;
		}

		const CSVFile::ColumnDataType &rhsColumn = csvFile.getColumn(rhs);
		if (rhsColumn.empty()) {
			std::cout << "Incorrect SECOND column name\n";
			continue;
		}

		CSVFile::ColumnDataType result = lhsColumn;
		handler(result, rhsColumn);

		for (auto val : result) {
			std::cout << val << '\n';
		}
	}

	std::cout << "Terminating...\n";
}

template <typename T>
CommandHandlers<T> createCommandHandlers() {
	const auto createHandler = [](auto binaryOp) {
		return [binaryOp](auto &lhs, const auto &rhs) {
			return processData(lhs, rhs, binaryOp);
		};
	};

	CommandHandlers<T> handlers;

	handlers.registerHandler('+', createHandler(std::plus{}));

	handlers.registerHandler('-', createHandler(std::minus{}));

	handlers.registerHandler('*', createHandler(std::multiplies{}));

	handlers.registerHandler('/', createHandler([](auto lhs, auto rhs) {
		if constexpr (std::is_integral_v<decltype(lhs)>) {
			if (rhs == 0) {
				return std::numeric_limits<decltype(lhs)>::max();
			}
		}

		return lhs / rhs;
	}));

	return handlers;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "Expected usage: csv_binary <path to csv file>\n";
		return 1;
	}

	std::optional<CSVFile> csvFile = readCSVFile(argv[1]);
	if (!csvFile.has_value()) {
		std::cout << "Failed to read file: " << argv[1] << '\n';
		return 1;
	}

	const CommandHandlers<CSVFile::ColumnDataType> handlers = createCommandHandlers<CSVFile::ColumnDataType>();

	runPrompt(csvFile.value(), handlers);

	return 0;
}

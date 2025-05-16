#ifndef _DATA_PROCESSOR_HPP_
#define _DATA_PROCESSOR_HPP_

#include <vector>
#include <algorithm>
#include <execution>

template <typename T, typename BinaryOp>
void processData(std::vector<T> &lhs, const std::vector<T> &rhs, BinaryOp op) {
	const auto length = std::min(std::size(lhs), std::size(rhs));

	std::transform(std::execution::par_unseq
		, std::begin(lhs)
		, std::begin(lhs) + length
		, std::begin(rhs)
		, std::begin(lhs)
		, op
	);
}

#endif // !_DATA_PROCESSOR_HPP_

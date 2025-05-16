#include <gtest/gtest.h>

#include <vector>

#include "DataProcessor.hpp"
#include "CommandHandlers.hpp"

TEST(DataProcessorTests, GenericProcessData) {
	const int N = 42;

	std::vector<int> a(N), b(N);

	std::iota(a.begin(), a.end(), 0);
	std::iota(b.begin(), b.end(), 0);

	std::vector<int> res1(N);

	for (int i = 0; i < N; ++i) {
		res1[i] = a[i] + b[i];
	}

	std::vector<int> res2 = a;

	processData(res2, b, std::plus{});

	for (int i = 0; i < N; ++i) {
		EXPECT_EQ(res1[i], res2[i]);
	}
}

TEST(CommandHandlersTests, NoHandlers) {
	CommandHandlers<std::vector<int>> handlers;

	ASSERT_EQ(handlers.resolveHandler('+'), nullptr);
	ASSERT_EQ(handlers.resolveHandler('-'), nullptr);
	ASSERT_EQ(handlers.resolveHandler('/'), nullptr);
	ASSERT_EQ(handlers.resolveHandler('?'), nullptr);
}

TEST(CommandHandlersTests, PlusHandler) {
	const int N = 42;

	std::vector<int> a(N), b(N);

	std::iota(a.begin(), a.end(), 0);
	std::iota(b.begin(), b.end(), 0);

	std::vector<int> res = a;

	CommandHandlers<std::vector<int>> handlers;
	handlers.registerHandler('+', [](auto &lhs, const auto &rhs) {
		for (std::size_t i = 0; i < lhs.size(); ++i) {
			lhs[i] += rhs[i];
		}
	});

	auto handler = handlers.resolveHandler('+');
	ASSERT_NE(handler, nullptr);

	handler(res, b);

	for (std::size_t i = 0; i < a.size(); ++i) {
		EXPECT_EQ(res[i], a[i] + b[i]);
	}
}

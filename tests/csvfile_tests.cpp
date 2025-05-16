#include <gtest/gtest.h>

#include "CSVFile.hpp"

TEST(CSVFileTests, GetInvalidColumn) {
	CSVFile csv;

	EXPECT_NO_THROW(csv.getColumn("abc"));
	EXPECT_NO_THROW(csv.getColumn("xyz"));

	EXPECT_EQ(csv.getColumn("abc").size(), 0);
	EXPECT_EQ(csv.getColumn("xyz").size(), 0);
}

TEST(CSVFileTests, AddColumnTwice) {
	CSVFile csv;

	EXPECT_TRUE(csv.appendColumn("Column1", std::vector{ 1,2,3 }));
	EXPECT_FALSE(csv.appendColumn("Column1", std::vector{ 1,2,3 }));
}

TEST(CSVFileTests, AddEmptyColumn) {
	CSVFile csv;

	EXPECT_FALSE(csv.appendColumn("Column1", std::vector<CSVFile::DataType>{}));
}

TEST(CSVFileTests, AddValidColumn) {
	CSVFile csv;

	const auto columnA = std::vector{ 1,2,3 };
	const auto columnB = std::vector{ 4,5,6 };

	EXPECT_TRUE(csv.appendColumn("Column1", columnA));
	EXPECT_TRUE(csv.appendColumn("Column2", columnA));
	EXPECT_TRUE(csv.appendColumn("Column3", columnB));

	EXPECT_NE(csv.getColumn("Column1").size(), 0);
	EXPECT_EQ(csv.getColumn("Column1"), csv.getColumn("Column2"));
	EXPECT_NE(csv.getColumn("Column1"), csv.getColumn("Column3"));
}

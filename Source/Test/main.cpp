// Copyright 2015-2019 Piperift - All rights reserved

//#include <Core/Engine.h>

#include <gtest/gtest.h>
#include <Core/Engine.h>
#include <tracy/Tracy.hpp>


TEST(MyTest, TestName) {
	EXPECT_EQ(1, 2);
}

TEST(MyTest, TestName2) {
	EXPECT_EQ(2, 2);
}

TEST(MyTest, TestName3) {
	EXPECT_EQ(2, 3);
}

int main(int argc, char** argv) {
	TracyMessageL("Tick");

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

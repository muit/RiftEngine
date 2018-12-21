// Copyright 2015-2019 Piperift - All rights reserved

//#include <Core/Engine.h>

#include <gtest/gtest.h>
#include <Core/Engine.h>
#include <tracy/Tracy.hpp>


/*TEST(MyTest, TestName) {
	EXPECT_EQ(1, 2);
}*/

int main(int argc, char** argv) {
	TracyMessageL("Tick");

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

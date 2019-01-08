// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/TypeTraits.h"
#include "Eigen/Core.h"
#include "Eigen/Geometry.h"


using v2 = Eigen::Matrix<float, 2, 1>;
using v2_u32 = Eigen::Matrix<uint32, 2, 1>;

using v3 = Eigen::Matrix<float, 3, 1>;


/** Non reflected vectors */

using v4 = Eigen::Matrix<float, 4, 1>;

using v2_u8 = Eigen::Matrix<uint8, 2, 1>;
using v3_u8 = Eigen::Matrix<uint8, 3, 1>;
using v4_u8 = Eigen::Matrix<uint8, 4, 1>;

using v2_i32 = Eigen::Matrix<int32, 2, 1>;
using v3_i32 = Eigen::Matrix<int32, 3, 1>;
using v4_i32 = Eigen::Matrix<int32, 4, 1>;

using v3_u32 = Eigen::Matrix<uint32, 3, 1>;
using v4_u32 = Eigen::Matrix<uint32, 4, 1>;

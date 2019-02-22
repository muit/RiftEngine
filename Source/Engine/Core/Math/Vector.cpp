// Copyright 2015-2019 Piperift - All rights reserved

#include "Vector.h"
#include "Core/Platform/PlatformMisc.h"
#include "Core/Serialization/Archive.h"

const v3 v3::Forward{ v3::UnitY() };
const v3 v3::Right  { v3::UnitX() };
const v3 v3::Up     { v3::UnitZ() };


Rotator Quat::ToRotator() const
{
	const float SingularityTest = z() * x() - w() * y();
	const float YawY = 2.f*(w() * z() + x() * y());
	const float YawX = (1.f - 2.f*(Math::Square(y()) + Math::Square(z())));

	// reference
	// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

	// this value was found from experience, the above websites recommend different values
	// but that isn't the case for us, so I went through different testing, and finally found the case
	// where both of world lives happily.
	const float SINGULARITY_THRESHOLD = 0.4999995f;
	Rotator RotatorFromQuat;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch() = -90.f;
		RotatorFromQuat.Yaw() = Math::Atan2(YawY, YawX) * Math::RADTODEG;
		RotatorFromQuat.Roll() = Rotator::NormalizeAxis(-RotatorFromQuat.Yaw() - (2.f * Math::Atan2(x(), w()) * Math::RADTODEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch() = 90.f;
		RotatorFromQuat.Yaw() = Math::Atan2(YawY, YawX) * Math::RADTODEG;
		RotatorFromQuat.Roll() = Rotator::NormalizeAxis(RotatorFromQuat.Yaw() - (2.f * Math::Atan2(x(), w()) * Math::RADTODEG));
	}
	else
	{
		RotatorFromQuat.Pitch() = Math::FastAsin(2.f*(SingularityTest)) * Math::RADTODEG;
		RotatorFromQuat.Yaw()   = Math::Atan2(YawY, YawX) * Math::RADTODEG;
		RotatorFromQuat.Roll()  = Math::Atan2(-2.f*(w() * x() + y() * z()), (1.f - 2.f*(Math::Square(x()) + Math::Square(y())))) * Math::RADTODEG;
	}

	return RotatorFromQuat;
}

Quat Quat::FromRotator(Rotator rotator)
{
	const float DIVIDE_BY_2 = Math::DEGTORAD / 2.f;
	float SP, SY, SR;
	float CP, CY, CR;

	Math::SinCos(&SP, &CP, rotator.Pitch() * DIVIDE_BY_2);
	Math::SinCos(&SY, &CY, rotator.Yaw()   * DIVIDE_BY_2);
	Math::SinCos(&SR, &CR, rotator.Roll()  * DIVIDE_BY_2);

	Quat RotationQuat{
		 CR * CP*CY + SR * SP*SY,
		 CR * SP*SY - SR * CP*CY,
		-CR * SP*CY - SR * CP*SY,
		 CR * CP*SY - SR * SP*CY
	};

#if ENABLE_NAN_DIAGNOSTIC || DO_CHECK
	// Very large inputs can cause NaN's. Want to catch this here
	ensureMsgf(!RotationQuat.ContainsNaN(), TEXT("Invalid input to FRotator::Quaternion - generated NaN output: %s"), *RotationQuat.ToString());
#endif

	return RotationQuat;
}

Quat Quat::LookAt(v3 origin, v3 dest)
{
	return Quat::FromTwoVectors(dest - origin, v3::Up);
}

float Rotator::ClampAxis(float Angle)
{
	// returns Angle in the range (-360,360)
	Angle = Math::Fmod(Angle, 360.f);

	if (Angle < 0.f)
	{
		// shift to [0,360) range
		Angle += 360.f;
	}

	return Angle;
}

FORCEINLINE float Rotator::NormalizeAxis(float Angle)
{
	// returns Angle in the range [0,360)
	Angle = ClampAxis(Angle);

	if (Angle > 180.f)
	{
		// shift to (-180,180]
		Angle -= 360.f;
	}

	return Angle;
}

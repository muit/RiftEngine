// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <tracy/Tracy.hpp>

#define ScopedGameZone(Name)     ZoneScopedNC(Name, 0x459bd1)
#define ScopedGraphicsZone(Name) ZoneScopedNC(Name, 0x94d145)
#define ScopedZone(Name, Color) ZoneScopedNC(Name, 0x##Color)
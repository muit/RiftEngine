// Copyright 2015-2019 Piperift - All rights reserved

#include <cstdio>
#include <iostream>

#include <Core/Engine.h>
#include <Core/Events/Broadcast.h>

void DoOne() { Log::Message("One"); }

class AA {
public:
	void DoTwo() { Log::Message("Two"); }
};

class MyObject : public Object {
	CLASS(MyObject, Object)

public:
	void DoTwo() { Log::Message("Two"); }
};


int main(int, char**)
{
	Broadcast bc{};

	auto handleOne = bc.Bind(&DoOne);

	AA a;
	bc.Bind(&a, &AA::DoTwo);

	{
		GlobalPtr<MyObject> b = Create<MyObject>();

		bc.Bind(b.AsPtr(), &MyObject::DoTwo);
		bc.DoBroadcast();

		bc.Unbind(handleOne);
	}
	// Object has been deleted here. Third bind wont get called
	bc.DoBroadcast();

	bc.UnbindAll(&a);

	Engine::StartEngine();

	return 0;
}

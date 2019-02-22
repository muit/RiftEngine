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
	void DoThree() { Log::Message("Three"); }
	void DoThreeFloat(float val) { Log::Message("Three %f", val); }
};


int main(int, char**)
{
	{
		// Events demo
		Broadcast bc{};

		EventHandle handleOne = bc.Bind(&DoOne);
		AA a;
		bc.Bind(&a, &AA::DoTwo);
		{
			GlobalPtr<MyObject> b = Create<MyObject>();

			bc.Bind(*b, &MyObject::DoThree);
			bc.DoBroadcast();
		}
		// Object has been deleted here. Third bind wont get called
		bc.DoBroadcast();

		bc.Unbind(handleOne);
		bc.UnbindAll(&a);


		Broadcast<float> cc{};
		bc.Bind(&a, &AA::DoTwo);
		{
			GlobalPtr<MyObject> b = Create<MyObject>();

			cc.Bind(*b, &MyObject::DoThreeFloat);
			cc.DoBroadcast(5.f);
		}
	}

	Quat q = Quat::LookAt({ 0, 0, 0 }, { 1, 0, 0 });
	Rotator r = q.ToRotator();

	v3 v = q.GetForward();

	Engine::StartEngine();

	return 0;
}

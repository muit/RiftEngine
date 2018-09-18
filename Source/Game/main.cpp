
#include <cstdio>
#include <iostream>
#include <World/World.h>

using namespace std;



int main(int, char**)
{
	cout << "*** GAME ***" << endl;

	World world{};
	world.Start();

	Name myName { "Hey" };
	Name myName2{ "Hey" };
	Name myName3{ "Hey2" };
	const Name myName4{ myName3 };

	cout << (myName  == myName2) << endl; // 1
	cout << (myName2 == myName3) << endl; // 0
	cout << (myName3 == myName4) << endl; // 1

	String a;
	cin >> a;

	return 0;
}

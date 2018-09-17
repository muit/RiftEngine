
#include <cstdio>
#include <iostream>
#include <string>

#include <World/World.h>

using namespace std;

int main(int, char**)
{
	cout << "*** GAME ***" << endl;

	World world{};
	world.Start();

	string a;
	cin >> a;

	return 0;
}

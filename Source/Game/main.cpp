
#include <cstdio>
#include <iostream>
#include <string>

#include <World.h>
#include "Game.h"
#include "Player/Player.h"

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

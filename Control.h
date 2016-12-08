/*  header file for control functions */
/*		// Tips: Read Carefully!  //
*   -document everything you write, comment it and be aware of the readability of your code, and chose meaningfull nanes for variables 
*   -write comments before your functions to tell the reader how to use it and how it works, you are not the only working with it
*   -test every thing you develop before you move to next part, make a temporary main function in test file and test your code, keep copy of test main in its folder
*   -don't leave your work before deadline, plz we don't have much time
*   -check group for updates
*   -upload your edits to project to github and notifiy your group of them daily
*   -write pseudo-code comments in function before making it, to make it easy to develop, read and modify the function later
*/

#pragma once
#include <iostream>
#include <fstream>
#include "Data.h"
#include "Const.h"
#include "Graph.h"
using namespace std;


/*	Functions Prototypes  */
namespace Control
{
	enum Mode {INTERACTIVE, STEP, SILENT};
	Mode GetMode();

	void Start();
	void End(Castle &c);
	void Read(Castle&);
	void Loop(Castle &c);
	bool HasFinished(const Castle &c);
	void Interact(const Mode &mode);

	void Refresh(const Castle &c, const int &timer);
	void DrawEnemies(const Castle &c, const int &timer);
	void DrawEnemies(const Tower* T, const int &region, const int &timer);
	void DrawEnemies(Enemy* head, const int &region, const int &timer);
	void DrawEnemies(Enemy* enemies[], const int &size, const int &region);
} 
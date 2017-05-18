/*  header file for control functions */
#ifndef __Control_h__
#define __Control_h__

#include <iostream>
#include <fstream>
#include <iomanip>
#include "Data.h"
#include "Const.h"
#include "Graph.h"
#include "Sound.h"
using namespace std;

namespace Control
{
	enum Mode {INTERACTIVE, STEP, SILENT};
	Mode GetMode();

	void Start();
	void End(Castle &c);
	void Read(Castle&);
	void Loop(Castle &c);
	void Interact(const Mode &mode);

	void Refresh(const Castle &c, const int &timer);
	void DrawEnemies(const Castle &c, const int &timer);
	void DrawEnemies(const Tower* T, const int &region, const int &timer);
	void DrawEnemies(Enemy* head, const int &region, const int &timer);
	void DrawEnemies(Enemy* enemies[], const int &size, const int &region);
} 

#endif  /* __Control_h__ */
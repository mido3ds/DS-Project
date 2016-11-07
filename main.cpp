/*
*   DS Project CMP-2020 Cairo Uni., Castle Defending Simulator
*	Here is the Logic of the simulator and the main function
*   Team: Mahmoud Adas, Mahmoud Yousri, Khaled Sabry
*   Started 1-Nov-16, Delivered: -----
*/

// project header files
#include "Graph.h"
#include "Data.h"
#include "Const.h"
#include "Control.h"


int main()
{
	SetWindow();
	Castle c; CASTLE::Initialize(c);

	Control::Read(&c);

	DrawCastle(c, 0);
}
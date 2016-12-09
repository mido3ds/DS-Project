#pragma once 

// contains all constants used in program

#ifndef NULL
#define NULL            nullptr
#endif

#define NUM_OF_TOWERS	4
#define CmdWidth		150
#define CmdHeight		50
#define CastleWidth		30
#define CastleLength	20
#define CastleXStrt		(CmdWidth/2-(CastleWidth/2))
#define CastleYStrt		(CmdHeight/2-(CastleLength/2))
#define TowerWidth      7
#define TowerLength     3
#define EnemyShape		219  //ASCII code of enemy char shape 

#define SECOND          1000			// in milliseconds
#define MIN_DISTANCE_FROM_CASTLE    2
#define DEFAULT 5

#define space(num) setw(num)
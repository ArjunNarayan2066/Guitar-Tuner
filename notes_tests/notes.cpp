#include <iostream>
#include <string.h>

using namespace std;

int main()
{
	
int values [3][2] =
{
	{14,5},
	{12,3},
	{4, 7},
};
	
char * notes[32][2] = 
	{{"55.000", "A1" },
	{"58.270", "Bb1"},
	{"61.740", "B1" },
	{"65.410", "C2" },
	{"69.300", "Db2"},
	{"73.420", "D2" },
	{"77.780", "Eb2"},
	{"82.410", "E2" },
	{"87.310", "F2" },
	{"92.500", "Gb2"},
	{"98.000", "G2" },
	{"103.83", "Ab2"},
	{"110.00"," A2" },
	{"116.54", "Bb2"},
	{"123.47", "B2" },
	{"130.81", "C3" },
	{"138.59", "Db3"},
	{"146.83", "D3" },
	{"155.56", "Eb3"},
	{"164.81", "E3" },
	{"174.61", "F3" },
	{"185.00", "Gb3"},
	{"196.00", "G3" },
	{"207.65", "Ab3"},
	{"220.00", "A3" },
	{"233.08", "Bb3"},
	{"246.94", "B3" },
	{"261.63", "C4" },
	{"277.18", "Db4"},
	{"293.66", "D4" },
	{"311.13", "Eb4"},
	{"329.63", "E4" }};

	printf("hi\n");
	printf("%s and %s", notes[15][0], notes[15][1]);

	return 0;

}


#include <stdlib.h>
#include "time.h"
#include "random.h"

static bool first = true;

float getRangedRandom(float min, float max)
{
	if (first)
	{
		srand((unsigned int) time(NULL)); //seeding for the first time only!
		first = false;
	}

	float r = (float) rand();

    float r1 = (float)r/(float)RAND_MAX;

    r1 *= (max-min);

    r1 += min;

    return r1;
}

int getRangedRandom(int min, int max)
{
	if (first)
	{
		srand((unsigned int) time(NULL)); //seeding for the first time only!
		first = false;
	}

    int r = rand();

    float r2 = (float)r/(float)RAND_MAX;

    r2 *= ((float)max-(float)min);
	int r1 = (int) r2;

    r1 += min;

    return r1;

}
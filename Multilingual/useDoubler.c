#include <stdio.h>

void jbcdoubler_(float* x, float* y);

int main (int argc, char** argv)
{
	float two = 2.0;
	float four = 1.0;
	jbcdoubler_(&two, &four);
	printf("doubled %f = %f\n", 2.0, four);
}

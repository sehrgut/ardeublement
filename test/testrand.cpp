#include <stdio.h>
#include <time.h>
#include "../ardeublement/Util.cpp"

int main (int args, char *argv[]) {
	FastRand::srand(time(0));
	FastRand::srandom(time(0));
	int i;
	for (i=0;i<1000000;i++)
//		Gaussian::gauss8(10,20);
		printf("%d\n", Gaussian::gauss8(21, 108));
}
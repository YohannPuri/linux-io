#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	char *newPath;
	if(argc != 1)
	{
		newPath = argv[1];
	}


	printf("%s\n", newPath);

	return 0;

}
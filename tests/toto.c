#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int	main()
{
	char	*str1 = malloc(17);
	char	*str2 = malloc(17);
	char	*str3 = malloc(17);
	char	*str4 = malloc(17);

	printf("pagesize = %d\n", getpagesize());

	strcpy(str1, "Bonjour1");
	strcpy(str2, "Bonjour2");
	strcpy(str3, "Bonjour3");
	strcpy(str4, "Bonjour4");

	printf("%s\n", str1);
	printf("%s\n", str2);
	printf("%s\n", str3);
	printf("%s\n", str4);

	free(str1);
	free(str2);
	free(str3);
	free(str4);
	return 0;
}

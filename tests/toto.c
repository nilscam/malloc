#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int	main()
{
	char	*str = malloc(17);

	strcpy(str, "Bonjour");
	printf("%s\n", str);
	free(str);
	return 0;
}

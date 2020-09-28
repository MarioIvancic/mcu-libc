
// TODO: define INT_SIZE based on INT_MAX macro

#define INT_SIZE 4

// 6 bytes for 16-bit, 11 bytes for 32-bit, 21 bytes for 64-bit
#define BUFFER_SIZE (INT_SIZE * 2 + INT_SIZE / 2 + 1)

char *_suitoa(unsigned int i)
{
	static char buff[BUFFER_SIZE];
	char *p = &buff[BUFFER_SIZE - 1];
	*p-- = '\0';
	do
    {
        *p-- = '0' + i % 10;
		i /= 10;
	} while (i > 0);
	return p + 1;
}

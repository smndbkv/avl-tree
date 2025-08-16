#include <time.h>
#include "avl_tree.h"

#define MAX_PRINT 10

int main(int argc, char **argv)
{
	char *filename = nullptr;
	double t = 0;
	avl_tree<student> *a;

	if (argc != 2)
	{
		printf("Usage: %s filename\n", argv[0]);
		return 0;
	}
	filename = argv[1];

	t = clock();
	FILE *fp;
	if (!(fp = fopen(filename, "r")))
	{
		printf("Cannot open file %s\n", filename);
		return 0;
	}
	if (!(a = new avl_tree<student>))
	{
		printf("Cannot allocate memory\n");
		return 0;
	}
	io_status st = a->read(fp);
	switch (st)
	{
	case io_status::eof:
		printf("Cannot open file %s\n", filename);
		break;
	case io_status::format:
		printf("Cannot read file %s\n", filename);
		break;
	case io_status::memory:
		printf("Cannot allocate memory\n");
		break;
	case io_status::open:
		break;
	case io_status::create:
		break;
	case io_status::success:
		break;
	}
	if (st != io_status::success)
	{
		fclose(fp);
		return 0;
	}
	fclose(fp); // прочитали дерево
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf("Initialization time: %.2f\n", t);
	printf("Initial avl_tree<student>:\n");
	a->print(MAX_PRINT);

	delete a;
	return 0;
}

#include "Category.h"

Category Category::Internal()
{
	return Category("Opération Interne", 0);
}

bool operator==(const Category& c1, const Category& c2)
{
	return c1.name == c2.name;
}

bool operator!=(const Category& c1, const Category& c2)
{
	return !(c1 == c2);
}
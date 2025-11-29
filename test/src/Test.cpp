#include "ApplicationTest.h"

#include "AmountTest.h"

int main()
{
	ApplicationTest appTest;

	// Amount tests
	appTest.AddTest(amountStringTest);
	appTest.AddTest(amountAdditionTest);
	appTest.AddTest(amountSubstractionTest);
	appTest.AddTest(amountMultiplicationTest);
	appTest.AddTest(negativeAmountTest);

	// Other tests...

	appTest.Run();
}
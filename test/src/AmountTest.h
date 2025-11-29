#pragma once

#include "Test.h"

#include "domain/Amount.h"

const Test amountStringTest(
	"Amount string test",
	[](Verifier& verify)
	{
		Amount amount(0);
		verify.IsTrue(amount.GetString().substr(0, 4) == "0.00", "Amount(0) should be written 0.00€");

		amount = 2;
		verify.IsTrue(amount.GetString().substr(0, 4) == "0.02", "Amount(2) should be written 0.02€");

		amount = 40;
		verify.IsTrue(amount.GetString().substr(0, 4) == "0.40", "Amount(40) should be written 0.40€");

		amount = 255;
		verify.IsTrue(amount.GetString().substr(0, 4) == "2.55", "Amount(255) should be written 2.55€");

		amount = 1298;
		verify.IsTrue(amount.GetString().substr(0, 5) == "12.98", "Amount(1298) should be written 12.98€");

		amount = -532;
		verify.IsTrue(amount.GetString().substr(0, 5) == "-5.32", "Amount(-532) should be written -5.32€");
	}
);

const Test amountAdditionTest(
	"Amount addition test",
	[](Verifier& verify)
	{
		Amount a1(3);
		Amount a2(532);
		verify.IsTrue((a1 + a2).GetValue() == 535, "Amount(3) + Amount(532) should equal Amount(535)");

		a1 = -3;
		verify.IsTrue((a1 + a2).GetValue() == 529, "Amount(-3) + Amount(532) should equal Amount(529)");
	}
);

const Test amountSubstractionTest(
	"Amount substraction test",
	[](Verifier& verify)
	{
		Amount a1(3);
		Amount a2(532);
		verify.IsTrue((a1 - a2).GetValue() == -529, "Amount(3) - Amount(532) should equal Amount(-529)");
		verify.IsTrue((a2 - a1).GetValue() == 529, "Amount(532) - Amount(3) should equal Amount(529)");
	}
);

const Test amountMultiplicationTest(
	"Amount multiplication test",
	[](Verifier& verify)
	{
		Amount a(321);
		verify.IsTrue((a * 3).GetValue() == 963, "Amount(321) * 3 should equal Amount(963)");
		verify.IsTrue((a * -3).GetValue() == -963, "Amount(321) * -3 should equal Amount(-963)");
	}
);

const Test negativeAmountTest(
	"Negative amount test",
	[](Verifier& verify)
	{
		Amount a(321);
		verify.IsTrue(-a.GetValue() == -321, "-Amount(321) should equal Amount(-321)");
	}
);
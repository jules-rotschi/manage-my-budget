#pragma once

#include <vector>

#include "Amount.h"
#include "BankAccount.h"

class Accountant
{
public:
	Accountant(const std::vector<BankAccount>& accounts);

	Amount GetMonthlyAmount(int year, int month) const;
	Amount GetMonthlyAmount(int year, int month, int categoryIndex) const;
	Amount GetYearlyAmount(int year) const;
	Amount GetYearlyAmount(int year, int categoryIndex) const;
	Amount GetMonthlySavings(int year, int month);
	Amount GetYearlySavings(int year);

private:
	const std::vector<BankAccount>& m_accounts;
};


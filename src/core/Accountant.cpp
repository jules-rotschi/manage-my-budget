﻿#include "Accountant.h"

#include <assert.h>

Accountant::Accountant(const std::vector<BankAccount>& accounts)
	: m_accounts(accounts) {}

Amount Accountant::GetMonthlyAmount(int year, int month) const
{
	assert((month >= 1 && month <= 12) && "Month argument must be within [1;12].");

	Amount amount = 0;

	for (const BankAccount& account : m_accounts) {
			amount += account.GetMonthlyAmount(year, month);
	}

	return amount;
}

Amount Accountant::GetMonthlyAmount(int year, int month, int categoryIndex) const
{
	assert((month >= 1 && month <= 12) && "Month argument must be within [1;12].");

	Amount amount = 0;

	for (const BankAccount& account : m_accounts) {
			amount += account.GetMonthlyAmount(year, month, categoryIndex);
	}

	return amount;
}

Amount Accountant::GetYearlyAmount(int year) const
{
	Amount amount = 0;

	for (const BankAccount& account : m_accounts) {
			amount += account.GetYearlyAmount(year);
	}

	return amount;
}

Amount Accountant::GetYearlyAmount(int year, int categoryIndex) const
{
	Amount amount = 0;

	for (const BankAccount& account : m_accounts) {
			amount += account.GetYearlyAmount(year, categoryIndex);
	}

	return amount;
}

Amount Accountant::GetMonthlySavings(int year, int month)
{
	assert((month >= 1 && month <= 12) && "Month argument must be within [1;12].");

	Amount savings = 0;

	for (const BankAccount& account : m_accounts) {
		if (account.type != AccountType::SAVING) continue;

		for (const Operation& operation : account.r_Operations()) {
			if (operation.year != year || operation.month != month) continue;

			savings += operation.amount;
		}
	}

	return savings;
}

Amount Accountant::GetYearlySavings(int year)
{
	Amount savings = 0;

	for (const BankAccount& account : m_accounts) {
		if (account.type != AccountType::SAVING) continue;

		for (const Operation& operation : account.r_Operations()) {
			if (operation.year != year) continue;

			savings += operation.amount;
		}
	}

	return savings;
}
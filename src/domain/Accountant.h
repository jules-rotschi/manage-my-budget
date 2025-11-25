#pragma once

#include <vector>

class Amount;
class Profile;

class Accountant
{
public:
	Accountant(const Profile& profile);

	Amount GetBudgetTotal() const;

	Amount GetMonthlyAmount(int year, int month) const;
	Amount GetMonthlyAmount(int year, int month, int categoryIndex) const;
	Amount GetYearlyAmount(int year) const;
	Amount GetYearlyAmount(int year, int categoryIndex) const;
	Amount GetMonthlySavings(int year, int month) const;
	Amount GetYearlySavings(int year) const;

private:
	const Profile& m_profile;
};


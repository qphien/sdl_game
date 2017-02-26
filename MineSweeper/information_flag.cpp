#include "flag_informaiton.h"

bool operator==(const I_F &left, const I_F &right)
{
	if (left.x == right.x && left.y == right.y && left.click_number == right.click_number)
		return true;
	return false;
}
#ifndef __FLAG_INFORMATION_H__
#define __FLAG_INFORMATION_H__
struct I_F
{
	int x;
	int y;
	int click_number = -1;
};
bool operator==(const I_F &left, const I_F &right);
#endif

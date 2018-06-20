#pragma once

#ifndef ClassTag
#define ClassTag

class MontyHallDoor
{
public:
	int remaining_door_to_choose[3];

	int prize_door = 0;
	int clicked_time = 0;

	void clear_door_array(int* this_array);
private:

};

void MontyHallDoor::clear_door_array(int* this_array)
{
	ZeroMemory(this_array, 3);
}

#endif // !ClassTag

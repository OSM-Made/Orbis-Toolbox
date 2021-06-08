#pragma once

class GamePad
{
private:

public:
	enum Buttons
	{
		Left = 1U,
		Up,
		Right = 4U,
		Down = 8U,
		Square = 16U,
		Triangle = 32U,
		Circle = 64U,
		Cross = 128U,
		Start = 256U,
		Select = 512U,
		L = 1024U,
		R = 2048U,
		L2 = 4096U,
		R2 = 8192U,
		L3 = 16384U,
		R3 = 32768U,
		Enter = 65536U,
		Back = 131072U
	};

	static bool IsDown(int button);
};
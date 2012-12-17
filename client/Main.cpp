#include "stdafx.h"

#include "SocketHandler.h"
#include "AuthWindow.h"

int main()
{
	Fl::scheme("gtk+");
	AuthWindow win(400, 250, "Les Brutes");

	return Fl::run();
}

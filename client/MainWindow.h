#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../serveur/Config.h"

class MainWindow : public Fl_Window, public SocketHandler::Session
{
	public:
		MainWindow(int w, int h, const char* name, SocketHandler* s, Brute* brute) : handler(s), brute(brute), Fl_Window(w, h, name)
		{
			begin();
				Fl_Group* cellule = new Fl_Group(0, 0, 512, 500);
				{
				}
				cellule->end();
			end();
			show();
		}

		virtual ~MainWindow() {}

		virtual void OnRead() {}

	private:
		SocketHandler* handler;
		Brute* brute;
};

#endif
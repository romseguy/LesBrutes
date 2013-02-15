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
					Fl_Text_Display* helpdisp = new Fl_Text_Display(0,0,512,500);
					Fl_Text_Buffer* helpbuff = new Fl_Text_Buffer();
					helpdisp->buffer(helpbuff);
					helpdisp->textsize(15);
					std::stringstream ss;
					ss << "La cellule de " << brute->getLogin() << std::endl;
					ss << "Niveau : " << static_cast<int>(brute->getLevel()) << std::endl;
					ss << "Vie : " << static_cast<int>(brute->getHp()) << std::endl;
					ss << "Force : " << static_cast<int>(brute->getStrength()) << std::endl;
					ss << "Rapidité : " << static_cast<int>(brute->getSpeed()) << std::endl;

					helpbuff->text(ss.str().c_str());
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
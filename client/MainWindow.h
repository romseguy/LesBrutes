#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AuthWindow.h"

class MainWindow : public Fl_Window, public SocketHandler::Session
{
	public:
		MainWindow(int w, int h, const char* name, SocketHandler* s, Brute* brute) : handler(s), brute(brute), Fl_Window(w, h, name)
		{
			std::ostringstream ss;
			fl_register_images();

			menubar = new Fl_Menu_Bar(0, 0, w, 30);
			menubar->add("Fichier/Logout", 0, menu_cb, (void*) this);
			menubar->add("Fichier/Quitter", 0, menu_cb, (void*) this);
			menubar->add("Combat/Rechercher", 0, menu_cb, (void*) this);
			menubar->add("Combat/Lancer", 0, menu_cb, (void*) this);

			Fl_Group* me = new Fl_Group(0, 50, w/2, h/2);
			{
				// portrait brute
				Fl_JPEG_Image* jpg = new Fl_JPEG_Image("Portrait", brute->getPortrait());
				Fl_Box* box = new Fl_Box(w/2 - jpg->w(), 100, jpg->w(), jpg->h());
				box->image(jpg);

				// nom brute
				Fl_Text_Display* disp_name = new Fl_Text_Display(0, 50, w/2, 40);
				Fl_Text_Buffer*  buff_name = new Fl_Text_Buffer();
				disp_name->buffer(buff_name);
				disp_name->color(FL_BACKGROUND_COLOR);
				disp_name->textsize(20);

				ss << "La cellule de " << brute->getLogin();

				buff_name->text(ss.str().c_str());

				// infos brute
				Fl_Text_Display* disp = new Fl_Text_Display(20, 100, w/2 - jpg->w() - 20, jpg->h());
				Fl_Text_Buffer*  buff = new Fl_Text_Buffer();
				disp->buffer(buff);
				disp->color(FL_BACKGROUND_COLOR);
				disp->textsize(15);

				ss.clear(); ss.str("");
				ss << "Niveau : "      << static_cast<int>(brute->getLevel())    << std::endl;
				ss << "Vie : "         << static_cast<int>(brute->getHp())       << std::endl;
				ss << "Force : "       << static_cast<int>(brute->getStrength()) << std::endl;
				ss << "Rapidité : "    << static_cast<int>(brute->getSpeed())    << std::endl;

				buff->text(ss.str().c_str());
			}
			me->end();

			end();
			show();
		}

		virtual ~MainWindow() {}
		virtual void OnRead() {}

		// menubar : static callback
		static void menu_cb(Fl_Widget* w, void* data)
		{
	        MainWindow *o = (MainWindow*) data;
			o->menu_cb2();
		}

		// menubar : class callback
		void menu_cb2()
		{
			char picked[80];
			menubar->item_pathname(picked, sizeof(picked) - 1);

			if (strcmp(picked, "Fichier/Quitter") == 0)
				exit(0);

			if (strcmp(picked, "Fichier/Logout") == 0)
				handler->set_session(new AuthWindow(400, 400, "Les Brutes : Connexion", handler));
		}

	private:
		SocketHandler* handler;
		Brute* brute;
		Fl_Menu_Bar *menubar;
};

#endif
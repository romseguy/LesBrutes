#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AuthWindow.h"

class MainWindow : public Fl_Window, public SocketHandler::Session
{
	public:
		MainWindow(int w, int h, const char* name, SocketHandler* s, Brute* brute) : w(w), h(h), handler(s), me(brute), Fl_Window(w, h, name)
		{
			menubar = new Fl_Menu_Bar(0, 0, w, 30);
			menubar->add("Fichier/Logout", 0, menu_cb, (void*) this);
			menubar->add("Fichier/Quitter", 0, menu_cb, (void*) this);
			menubar->add("Combat/Rechercher", 0, menu_cb, (void*) this);
			menubar->add("Combat/Lancer", 0, menu_cb, (void*) this);

			std::ostringstream ss;
			fl_register_images();

			// affichage de notre brute
			Fl_Group* left = new Fl_Group(0, 50, w/2, h/2-40);
			{
				// portrait
				Fl_JPEG_Image* jpg = new Fl_JPEG_Image("Portrait", me->getPortrait());
				Fl_Box* box = new Fl_Box(w/2 - jpg->w(), 100, jpg->w(), jpg->h());
				box->image(jpg);

				// nom
				Fl_Text_Display* disp_name = new Fl_Text_Display(5, 50, w/2 - 5, 40);
				Fl_Text_Buffer*  buff_name = new Fl_Text_Buffer();
				disp_name->buffer(buff_name);
				disp_name->color(FL_BACKGROUND_COLOR);
				disp_name->textsize(20);
				ss << "La cellule de " << me->getLogin();
				buff_name->text(ss.str().c_str());

				// infos
				Fl_Text_Display* disp = new Fl_Text_Display(20, 100, w/2 - jpg->w() - 20, jpg->h());
				Fl_Text_Buffer*  buff = new Fl_Text_Buffer();
				disp->buffer(buff);
				disp->color(FL_BACKGROUND_COLOR);
				disp->textsize(15);

				ss.clear(); ss.str("");
				ss << "Niveau : "      << static_cast<int>(me->getLevel())    << std::endl;
				ss << "Vie : "         << static_cast<int>(me->getHp())       << std::endl;
				ss << "Force : "       << static_cast<int>(me->getStrength()) << std::endl;
				ss << "Rapidité : "    << static_cast<int>(me->getSpeed())    << std::endl;

				buff->text(ss.str().c_str());
			}
			left->end();

			end();
			show();
		}

		virtual ~MainWindow() {}

		virtual void OnRead() {}

		bool HandleSearch();
		bool HandleCombat();

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

			if (strcmp(picked, "Combat/Rechercher") == 0)
				if (!HandleSearch())
					fl_alert("La commande de recherche a échouée");

			if (strcmp(picked, "Combat/Lancer") == 0)
				if (!HandleCombat())
					fl_alert("La commande de combat a échouée");
		}

	private:
		int w, h;
		SocketHandler* handler;
		Brute* me;
		Fl_Menu_Bar *menubar;
};

#endif
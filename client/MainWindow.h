#ifndef MAINWINDOW_H
#define MAINWINDOW_H

void quit_cb(Fl_Widget*, void*) { exit(0); }

Fl_Menu_Item menutable[] = {
	{"&Fichier",                    0, 0, 0, FL_SUBMENU},
	{"&Déconnexion",	            0, 0, 0, FL_MENU_DIVIDER},
	{"&Quitter",	                FL_ALT+'q', quit_cb, 0, FL_MENU_DIVIDER},
	{0},
	{"&Combat",                     FL_F+2, 0, 0, FL_SUBMENU},
	{"Rechercher un adversaire",	FL_ALT+'r',	0},
	{"Lancer combat !",	            FL_ALT+'g',	0, 0, FL_MENU_DIVIDER},
};

class MainWindow : public Fl_Window, public SocketHandler::Session
{
	public:
		MainWindow(int w, int h, const char* name, SocketHandler* s, Brute* brute) : handler(s), brute(brute), Fl_Window(w, h, name)
		{
			std::ostringstream ss;
			fl_register_images();

			begin();
				Fl_Menu_Bar* menubar = new Fl_Menu_Bar(0, 0, w, 30);
				menubar->menu(menutable);

				Fl_Group* left = new Fl_Group(0, 50, w/2, h-50);
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
				left->end();

				Fl_Group* right = new Fl_Group(w/2, 50, w/2, h-50);
				{
				}
				right->end();
			end();
			show();
		}

		virtual ~MainWindow() { delete brute; }

		virtual void OnRead() {}

	private:
		SocketHandler* handler;
		Brute* brute;
};

#endif
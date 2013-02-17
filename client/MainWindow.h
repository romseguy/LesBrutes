#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class MainWindow : public Fl_Window, public SocketHandler::Session
{
	public:
		MainWindow(int w, int h, const char* name, SocketHandler* s, Brute* brute) : handler(s), brute(brute), Fl_Window(w, h, name)
		{
			fl_register_images();

			begin();
				Fl_Group* left = new Fl_Group(0, 0, w/2, h);
				{
					Fl_Text_Display* disp = new Fl_Text_Display(0, 0, w/2, h);
					Fl_Text_Buffer*  buff = new Fl_Text_Buffer();
					disp->buffer(buff);
					disp->textsize(15);

					std::stringstream ss;
					ss << "La cellule de " << brute->getLogin()                      << std::endl;
					ss << "Niveau : "      << static_cast<int>(brute->getLevel())    << std::endl;
					ss << "Vie : "         << static_cast<int>(brute->getHp())       << std::endl;
					ss << "Force : "       << static_cast<int>(brute->getStrength()) << std::endl;
					ss << "Rapidité : "    << static_cast<int>(brute->getSpeed())    << std::endl;

					buff->text(ss.str().c_str());
				}
				left->end();

				Fl_Group* right = new Fl_Group(w/2, 0, w/2, h);
				{
					Fl_JPEG_Image* jpg = new Fl_JPEG_Image("Portrait", brute->getPortrait());
					Fl_Box* box = new Fl_Box(w/2 , 0, jpg->w(), jpg->h());
					box->image(jpg);
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
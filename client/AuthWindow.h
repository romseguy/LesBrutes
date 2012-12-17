#ifndef _WINDOW_H
#define _WINDOW_H

class AuthWindow : public Fl_Window
{
	public:
		AuthWindow(int w, int h, const char* name = 0) : Fl_Window(w, h, name)
		{
			// pas besoin de delete ces pointeurs, FLTK s'en charge quand Fl_Window est fermée
			begin();
				Fl_Input*           txt_login        = new Fl_Input(150, 60, 100, 25, "Login");
				Fl_Secret_Input*    txt_pwd          = new Fl_Secret_Input(150, 100, 100, 25, "Mot de passe");
				Fl_Button*          but_connexion    = new Fl_Button(150, 140, 100, 25, "Connexion");
			end();
			but_connexion->callback(SocketHandler::open);
			show();
		}
};

#endif
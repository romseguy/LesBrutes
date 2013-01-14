#ifndef _WINDOW_H
#define _WINDOW_H

#include "../serveur/Config.h"

class AuthWindow : public Fl_Window, public SocketHandler::Session
{
	public:
		AuthWindow(int w, int h, const char* name = 0, SocketHandler* s = NULL) : handler(s), Fl_Window(w, h, name)
		{
			// pas besoin de delete ces pointeurs, FLTK s'en charge quand Fl_Window est ferm�e
			begin();
				txt_login = new Fl_Input(150, 60, 100, 25, "Login");
				txt_pwd = new Fl_Secret_Input(150, 100, 100, 25, "Mot de passe");
				Fl_Button* but_connexion = new Fl_Button(150, 140, 100, 25, "Connexion");
			end();
			
			but_connexion->callback(but_connexion_cb, (void*) handler);
			show();
		}

		virtual ~AuthWindow() { if (handler) delete handler; }

		virtual void OnRead();

		// Callbacks
		static void but_connexion_cb(Fl_Widget* w, void* data)
		{
			((SocketHandler*) data)->open();
			((SocketHandler*) data)->handle_input();
		}

	private:
		SocketHandler* handler;
		Fl_Input* txt_login;
		Fl_Secret_Input* txt_pwd;
};

#endif

#ifndef _WINDOW_H
#define _WINDOW_H

#include "../serveur/Config.h"

class AuthWindow : public Fl_Window, public SocketHandler::Session
{
	public:
		AuthWindow(int w, int h, const char* name = 0, SocketHandler* s = NULL) : handler(s), Fl_Window(w, h, name)
		{
			// pas besoin de delete ces pointeurs, FLTK s'en charge quand Fl_Window est fermée
			begin();
				txt_login               = new Fl_Input(150, 60, 100, 25, "Login");
				txt_pwd                 = new Fl_Secret_Input(150, 100, 100, 25, "Mot de passe");
				Fl_Button* but_login    = new Fl_Button(150, 140, 100, 25, "Connexion");
				Fl_Button* but_register = new Fl_Button(150, 170, 100, 25, "Inscription");
			end();
			
			but_login->callback(but_login_cb, (void*) this);
			but_register->callback(but_register_cb, (void*) this);
			show();
		}

		virtual ~AuthWindow() { if (handler) delete handler; }

		virtual void OnRead();

		bool HandleLogon();
		bool HandleRegister();

		// Callbacks des boutons
		static void but_login_cb(Fl_Widget* w, void* data)
		{
			if (((AuthWindow*) data)->HandleLogon())
				fl_alert("La connexion a réussi");
			else
				fl_alert("La connexion a échouée");
		}

		static void but_register_cb(Fl_Widget* w, void* data)
		{
			if (((AuthWindow*) data)->HandleRegister())
				fl_alert("Compte créé avec succès");
			else
				fl_alert("Echec dans la création du compte");
		}

	private:
		SocketHandler* handler;
		Fl_Input* txt_login;
		Fl_Secret_Input* txt_pwd;
};

#endif

#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include "../serveur/Config.h"

class AuthWindow : public Fl_Window, public SocketHandler::Session
{
	public:
		AuthWindow(int w, int h, const char* name = 0, SocketHandler* s = NULL) : handler(s), Fl_Window(w, h, name)
		{
			resize(0, 0, 400, 400);

				form_login = new Fl_Group(0, 0, 400, 400);
				{
					size_t offset_x = 100;
					size_t offset_y = 120;
					txt_login               = new Fl_Input        (offset_x,    offset_y+0,   200, 25, "Login");
					txt_pwd                 = new Fl_Secret_Input (offset_x,    offset_y+30,  200, 25, "Mot de passe");
					Fl_Button* but_login    = new Fl_Button       (offset_x+50, offset_y+60,  100, 25, "Connexion");
					Fl_Button* but_register = new Fl_Button       (offset_x+50, offset_y+90,  100, 25, "Inscription");

					but_login->callback(but_login_cb, (void*) this);
					but_register->callback(but_register_cb, (void*) this);
				}
				form_login->end();

			end();
			show();
		}

		virtual ~AuthWindow() { if (handler) delete handler; }

		virtual void OnRead();

		bool HandleLogon();
		bool HandleRegister();

		// Callbacks des boutons
		static void but_login_cb(Fl_Widget* w, void* data)
		{
			if (!((AuthWindow*) data)->HandleLogon())
				fl_alert("La commande de connexion a échouée");
		}

		static void but_register_cb(Fl_Widget* w, void* data)
		{
			if (!((AuthWindow*) data)->HandleRegister())
				fl_alert("La commande de création de compte a échouée");
		}

	private:
		SocketHandler* handler;
		Fl_Group* form_login;
		Fl_Input* txt_login;
		Fl_Secret_Input* txt_pwd;
};

#endif

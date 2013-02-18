#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

class AuthWindow : public Fl_Window, public SocketHandler::Session
{
	public:
		AuthWindow(int w, int h, const char* name, SocketHandler* s) : handler(s), Fl_Window(w, h, name)
		{
			form_login = new Fl_Group(0, 0, w, h);
			{
				size_t offset_x = 120;
				size_t offset_y = 120;
				txt_login               = new Fl_Input        (offset_x,    offset_y+0,   200, 25, "Login");
				txt_pwd                 = new Fl_Secret_Input (offset_x,    offset_y+30,  200, 25, "Mot de passe");
				Fl_Button* but_register = new Fl_Button       (offset_x+50, offset_y+60,  100, 25, "Inscription");
				Fl_Button* but_login    = new Fl_Button       (offset_x+50, offset_y+90,  100, 25, "Connexion");

				but_login->callback(but_login_cb, (void*) this);
				but_register->callback(but_register_cb, (void*) this);
			}
			form_login->end();

			end();
			show();
		}

		virtual ~AuthWindow() {}
		virtual void OnRead() {}

		// Ces m�thodes retournent false en cas d'erreur au niveau de l'envoi ou de la r�ception des messages
		bool HandleLogon();
		bool HandleRegister();

		// Callbacks des boutons
		static void but_login_cb(Fl_Widget* w, void* data)
		{
			if (!((AuthWindow*) data)->HandleLogon())
				fl_alert("La commande de connexion a �chou�e");
		}

		static void but_register_cb(Fl_Widget* w, void* data)
		{
			if (!((AuthWindow*) data)->HandleRegister())
				fl_alert("La commande de cr�ation de compte a �chou�e");
		}

	private:
		SocketHandler* handler;
		Fl_Group* form_login;
		Fl_Input* txt_login;
		Fl_Secret_Input* txt_pwd;
};

#endif

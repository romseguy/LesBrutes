#include "stdafx.h"

#include "../serveur/SocketHandler.h"
#include "../serveur/ByteBuffer.h"
#include "../serveur/AuthCodes.h"
#include "../serveur/Brute.h"

#include "MainWindow.h"

bool MainWindow::HandleSearch()
{
	// requête pour récupérer le login d'un opposant du même niveau
	ByteBuffer packet;
	packet << uint8_t(GET_OPPONENT_C);
	packet << uint16_t(me->getLogin().length() + 1);
	packet << me->getLogin();

	if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

	// réponse
	ByteBuffer buf;
	buf.resize(4);

	if (!handler->recv_soft((char*) buf.contents(), 4)) return false;
	if (buf[0] != GET_OPPONENT_S) return false;

	if (buf[1] == OPPONENT_KO)
	{
		fl_alert("Aucun adversaire trouvé");
		return true;
	}

	uint16_t restant;
	buf.rpos(2);
	buf >> restant;
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(4), restant)) return false;

	buf.rpos(4);
	std::string login;
	buf >> login;

	// requête pour récupérer les infos de la brute adverse
	packet.clear();
	packet << uint8_t(INFO_BRUTE_C);
	packet << uint16_t(login.length() + 1);
	packet << login;

	if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

	// réponse
	buf.clear();
	buf.resize(3);

	if (!handler->recv_soft((char*) buf.contents(), 3)) return false;
	if (buf[0] != INFO_BRUTE_S) return false;

	buf.rpos(1);
	buf >> restant;
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(3), restant)) return false;

	buf.rpos(3);
	uint8_t portraitId, level, hp, strength, speed;
	buf >> portraitId >> level >> hp >> strength >> speed;

	// requête pour récupérer l'image du portrait
	packet.clear();
	packet << uint8_t(GET_IMAGE_C);
	packet << uint8_t(portraitId);

	if (!handler->send_soft((char*) packet.contents(), packet.size())) return false;

	// réponse image portrait
	buf.clear();
	buf.resize(3);

	if (!handler->recv_soft((char*) buf.contents(), 3)) return false;
	if (buf[0] != GET_IMAGE_S) return false;

	buf.rpos(1);
	buf >> restant;
	buf.resize(restant + buf.size());

	if (!handler->recv_soft((char*) buf.contents(3), restant)) return false;

	Brute* brute_opp = new Brute(login, "", level, hp, strength, speed, buf.contents(3));

	// modification IHM
	std::ostringstream ss;
	fl_register_images();

	begin();
		size_t w_found = w/2 - 100;

		Fl_Text_Display* disp_found = new Fl_Text_Display((w/2 - w_found)/2, h/2 - 30, w_found, 30);
		Fl_Text_Buffer*  buff_found = new Fl_Text_Buffer();
		disp_found->buffer(buff_found);
		disp_found->color(FL_BACKGROUND_COLOR);
		disp_found->textsize(15);
		buff_found->text("Un adversaire a été trouvé !!");

		size_t left_opp_y = h/2 + 100;

		Fl_Group* left_opp = new Fl_Group(0, left_opp_y, w/2, h/2);
		{
			// portrait opposant
			Fl_JPEG_Image* jpg_opp = new Fl_JPEG_Image("Portrait", brute_opp->getPortrait());
			Fl_Box* box_opp = new Fl_Box(w/2 - jpg_opp->w(), left_opp_y + 50, jpg_opp->w(), jpg_opp->h());
			box_opp->image(jpg_opp);

			// nom opposant
			Fl_Text_Display* disp_name_opp = new Fl_Text_Display(5, left_opp_y, w/2 - 5, 40);
			Fl_Text_Buffer*  buff_name_opp = new Fl_Text_Buffer();
			disp_name_opp->buffer(buff_name_opp);
			disp_name_opp->color(FL_BACKGROUND_COLOR);
			disp_name_opp->textsize(20);
			ss << "La cellule de " << brute_opp->getLogin();
			buff_name_opp->text(ss.str().c_str());

			// infos opposant
			Fl_Text_Display* disp_opp = new Fl_Text_Display(20, left_opp_y + 50, w/2 - jpg_opp->w() - 20, jpg_opp->h());
			Fl_Text_Buffer*  buff_opp = new Fl_Text_Buffer();
			disp_opp->buffer(buff_opp);
			disp_opp->color(FL_BACKGROUND_COLOR);
			disp_opp->textsize(15);

			ss.clear(); ss.str("");
			ss << "Niveau : "      << static_cast<int>(brute_opp->getLevel())    << std::endl;
			ss << "Vie : "         << static_cast<int>(brute_opp->getHp())       << std::endl;
			ss << "Force : "       << static_cast<int>(brute_opp->getStrength()) << std::endl;
			ss << "Rapidité : "    << static_cast<int>(brute_opp->getSpeed())    << std::endl;

			buff_opp->text(ss.str().c_str());
		}
		left_opp->end();
	end();
	redraw();

	return true;
}

bool MainWindow::HandleCombat()
{
	// modification IHM
	begin();
		Fl_Group* right = new Fl_Group(w/2, 50, w/2, h-50);
		{
			Fl_Text_Display* disp_title = new Fl_Text_Display(w/2 + 20, 50, w/2 - 25, 40);
			Fl_Text_Buffer*  buff_title = new Fl_Text_Buffer();
			disp_title->buffer(buff_title);
			disp_title->color(FL_BACKGROUND_COLOR);
			disp_title->textsize(20);
			buff_title->text("Résultat du combat :");

			Fl_Text_Display* disp_resultat = new Fl_Text_Display(w/2 + 20, 100, w/2 - 25, h - 110);
			Fl_Text_Buffer*  buff_resultat = new Fl_Text_Buffer();
			disp_resultat->buffer(buff_resultat);
			disp_resultat->color(FL_BACKGROUND_COLOR);
			disp_resultat->textsize(15);
			buff_resultat->text("Victoire de ...");
		}
		right->end();
	end();
	redraw();

	return true;
}
// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <winsock2.h>
#include <windows.h>

#include <string>
#include <list>
#include <vector>
#include <map>

#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Button.h>
#include <FL/Fl_Input.h>
#include <FL/Fl_Secret_Input.h>
#include <FL/Fl_Widget.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

using namespace std;
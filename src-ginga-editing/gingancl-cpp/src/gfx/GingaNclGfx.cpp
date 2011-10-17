/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
*******************************************************************************/

#include "../../include/gfx/GingaNclGfx.h"

#include "system/io/interface/content/text/IFontProvider.h"
#include "system/io/interface/output/ISurface.h"

#include "util/Color.h"
using namespace ::br::pucrio::telemidia::util;

#include "player/IPlayer.h"
using namespace ::br::pucrio::telemidia::ginga::core::player;

#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace ncl {
	int GingaNclGfx::w = 0;
	int GingaNclGfx::h = 0;
	vector<AnimePlayer*>* GingaNclGfx::animes = NULL;
	IWindow* GingaNclGfx::bg = NULL;
	IWindow* GingaNclGfx::top = NULL;
	IWindow* GingaNclGfx::bottom = NULL;

	void GingaNclGfx::show(int l, int t, int w, int h) {
		int size, compRollerW, compRollerH, initX, initY;
		double rate;
		vector<int> x;
		vector<int> y;
		AnimePlayer* p;
		IWindow* win;
		ISurface* s;
		vector<string>* mrls, *mrlsAux;
		IComponentManager* cm = IComponentManager::getCMInstance();

		release();

		GingaNclGfx::w = w;
		GingaNclGfx::h = h;
		rate = 0.2;
		initX = (w/2) - ((550 * rate)/2);
		initY = (h/2) - ((530 * rate)/2);

		x.push_back(initX + (130 * rate));
		x.push_back(initX + (240 * rate));
		x.push_back(initX + (340 * rate));
		x.push_back(initX + (400 * rate));
		x.push_back(initX + (340 * rate));
		x.push_back(initX + (240 * rate));
		x.push_back(initX + (130 * rate));
		x.push_back(initX + (60 * rate));

		y.push_back(initY + (90 * rate));
		y.push_back(initY + (50 * rate));
		y.push_back(initY + (90 * rate));
		y.push_back(initY + (190 * rate));
		y.push_back(initY + (330 * rate));
		y.push_back(initY + (380 * rate));
		y.push_back(initY + (330 * rate));
		y.push_back(initY + (190 * rate));

		compRollerW = 115 * rate;
		compRollerH = 113 * rate;

		mrls = new vector<string>;
		mrls->push_back("/usr/local/lib/ginga/files/img/roller/b1.png");
		mrls->push_back("/usr/local/lib/ginga/files/img/roller/b2.png");
		mrls->push_back("/usr/local/lib/ginga/files/img/roller/b3.png");
		mrls->push_back("/usr/local/lib/ginga/files/img/roller/b4.png");
		mrls->push_back("/usr/local/lib/ginga/files/img/roller/b5.png");
		mrls->push_back("/usr/local/lib/ginga/files/img/roller/b6.png");
		mrls->push_back("/usr/local/lib/ginga/files/img/roller/b7.png");
		mrls->push_back("/usr/local/lib/ginga/files/img/roller/b8.png");

		animes = new vector<AnimePlayer*>;
		bg = ((WindowCreator*)(cm->getObject("Window")))(l, t, w, h);
		s = ((ImageRenderer*)(cm->getObject("ImageRenderer")))(
				(char*)("/usr/local/lib/ginga/files/img/roller/loading.png"));

		bg->setCaps(bg->getCap("ALPHACHANNEL"));
		bg->draw();
		bg->show();
		bg->raiseToTop();

		bg->renderFrom(s);

		delete s;
		s = NULL;

		bottom = ((WindowCreator*)(cm->getObject("Window")))(
				5, h - 30, w / 2, 30);

		bottom->setCaps(bottom->getCap("ALPHACHANNEL"));
		bottom->draw();
		bottom->setBackgroundColor(0, 0, 0, 0);
		bottom->show();
		bottom->raiseToTop();

		top = ((WindowCreator*)(cm->getObject("Window")))(5, 5, w / 2, 30);
		top->setCaps(top->getCap("ALPHACHANNEL"));
		top->draw();
		top->setBackgroundColor(0, 0, 0, 0);
		top->show();
		top->raiseToTop();

		size = mrls->size();
		for (unsigned int i = 0; i < size; i++) {
			mrlsAux = new vector<string>(*mrls);
			p = createAnimePlayer(mrlsAux);
			win = ((WindowCreator*)(cm->getObject("Window")))(
					x[i], y[i], compRollerW, compRollerH);

			win->setCaps(win->getCap("ALPHACHANNEL"));
			win->draw();
			win->show();
			win->raiseToTop();

			p->setVoutWindow(win);
			animes->push_back(p);
			roll(mrls);
		}

		delete mrls;
		mrls = NULL;

		for (unsigned int i = 0; i < size; i++) {
			((*animes)[i])->play();
		}
	}

	void GingaNclGfx::roll(vector<string>* mrls) {
		mrls->insert(mrls->begin(), (*mrls)[mrls->size() - 1]);
		mrls->pop_back();
	}

	AnimePlayer* GingaNclGfx::createAnimePlayer(vector<string>* mrls) {
		AnimePlayer* ap;

		ap = new AnimePlayer(mrls);
		delete mrls;
		mrls = NULL;
		return ap;
	}

	void GingaNclGfx::release() {
		AnimePlayer* p;
		vector<AnimePlayer*>::iterator i;

		if (animes != NULL) {
			i = animes->begin();
			while (i != animes->end()) {
				p = *i;
				p->stop();
				delete p;
				++i;
			}

			delete animes;
			animes = NULL;
		}

		if (bg != NULL) {
			bg->clearContent();
			bg->hide();
			delete bg;
			bg = NULL;
		}

		if (top != NULL) {
			top->clearContent();
			top->hide();
			delete top;
			top = NULL;
		}

		if (bottom != NULL) {
			bottom->clearContent();
			bottom->hide();
			delete bottom;
			bottom = NULL;
		}
	}

	void GingaNclGfx::showTop(string text) {
		if (animes == NULL || bg == NULL) {
			return;
		}

		showText(top, text);
	}

	void GingaNclGfx::showBottom(string text) {
		if (animes == NULL || bg == NULL) {
			return;
		}

		showText(bottom, text);
	}

	void GingaNclGfx::showText(IWindow* window, string text) {
		ISurface* s;
		IColor* fontColor;
		IColor* bgColor;
		string fontUri;
		int fontSize;

		fontUri = "/usr/local/lib/ginga/files/font/decker.ttf";
		fontSize = 12;

		if (!fileExists(fontUri)) {
			cout << "GingaNclGfx::show Warning! File not found: '";
			cout << fontUri.c_str() << "'" << endl;
			return;
		}

		IComponentManager* cm = IComponentManager::getCMInstance();
		IFontProvider* font = NULL;

		window->clearContent();
		window->show();
		if (cm != NULL) {
			font = ((FontProviderCreator*)(cm->getObject("FontProvider")))(
					fontUri.c_str(), fontSize);

			s = ((SurfaceCreator*)(cm->getObject("Surface")))(NULL, 0, 0);
			s->setParent(window);
		}

		bgColor = new Color("black");
		fontColor = new Color("white");

		if (font != NULL) {
			s->setFont(font);
			s->setColor(fontColor);

			font->playOver(
					(void*)s,
					text.c_str(), 0, 0, (0x00000004 | 0x00000000));

			delete font;
			font = NULL;

			delete bgColor;
			bgColor = NULL;

			delete s;
			s = NULL;
		}

		delete fontColor;
		fontColor = NULL;

		window->validate();
		window->raiseToTop();
	}
}
}
}
}
}

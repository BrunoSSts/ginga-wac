/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware 
Ginga (Ginga-NCL).

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob 
os termos da Licen�a Publica Geral GNU versao 2 conforme publicada pela Free 
Software Foundation.

Este programa eh distribu�do na expectativa de que seja util, porem, SEM 
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

#include "../include/ChannelPlayer.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace player {
	ChannelPlayer::ChannelPlayer(map<string, Player*>* objs) : Player("") {
		this->objectMap = objs;
		this->selectedPlayer = NULL;
		this->hasParent = false;
	}

	ChannelPlayer::~ChannelPlayer() {
		wclog << "ChannelPlayer destructor" << endl;
		map<string, Player*>::iterator players;
		Player* objectPlayer;

		players = objectMap->begin();
		while (players != objectMap->end()) {
			objectPlayer = players->second;
			delete objectPlayer;
			objectPlayer = NULL;
			++players;
		}

		objectMap->clear();
	}

	Player* ChannelPlayer::getSelectedPlayer() {
		wclog << "ChannelPlayer::getSelectedPlayerAdd = '";
		wclog << selectedPlayer << endl;
		if (selectedPlayer == NULL) {
			wclog << "ChannelPlayer::getSelectedPlayerAdd == NULL";
			wclog << endl;
			return NULL;
		}
		return selectedPlayer;
	}

	map<string, Player*>* ChannelPlayer::getPlayerMap() {
		wclog << "ChannelPlayer::getPlayerMap" << endl;
		if (objectMap == NULL || objectMap->empty()) {
			return NULL;
		}

		return objectMap;
	}

	Player* ChannelPlayer::getPlayer(string objectId) {
		Player* newSelected;

		wclog << "ChannelPlayer::getPlayer" << endl;
		if (objectMap->count(objectId) != 0) {
			newSelected = (*objectMap)[objectId];
			if (newSelected != NULL && newSelected != selectedPlayer) {
				return newSelected;
			}
		}

		return NULL;
	}

	void ChannelPlayer::select(Player* selObject) {
		if (selectedPlayer != NULL) {
			selectedPlayer->removeListener(this);
		}

		this->selectedPlayer = selObject;
		if (selectedPlayer != NULL) {
			selectedPlayer->addListener(this);
		}
	}

	double ChannelPlayer::getMediaTime() {
		wclog << "ChannelPlayer::getMediaTime = '";
		wclog << selectedPlayer->getMediaTime() << "'" << endl;
		return selectedPlayer->getMediaTime();
	}

	void ChannelPlayer::setSurfacesParent(void* parent) {
		map<string, Player*>::iterator players;
		Player* avPlayer;

		players = objectMap->begin();
		while (players != objectMap->end()) {
			avPlayer = players->second;
			if (avPlayer->getSurface() != NULL &&
					avPlayer->getSurface()->getParent() != parent) {

				avPlayer->getSurface()->setParent(parent);
			}
			++players;
		}
		hasParent = true;
	}

	Surface* ChannelPlayer::getSurface() {
		wclog << endl << "ChannelPlayer::getSurface" << endl << endl;
		if (selectedPlayer != NULL) {
			return selectedPlayer->getSurface();
		}
		return NULL;
	}

	void ChannelPlayer::play() {
		if (selectedPlayer != NULL) {
			if (!hasParent && selectedPlayer->getSurface() != NULL &&
					selectedPlayer->getSurface()->getParent() != NULL) {

				setSurfacesParent(selectedPlayer->getSurface()->getParent());
				selectedPlayer->play();

			} else if (!(((AVPlayer*)selectedPlayer)->getHasVisual())) {
				selectedPlayer->play();
			}
		}
		Player::play();
	}

	void ChannelPlayer::pause() {
		if (selectedPlayer != NULL) {
			selectedPlayer->pause();
		}
		Player::pause();
	}

	void ChannelPlayer::resume() {
		if (selectedPlayer != NULL) {
			selectedPlayer->resume();
		}
		Player::resume();
	}

	void ChannelPlayer::stop() {
		Player* objectPlayer;
		map<string, Player*>::iterator players;

		players = objectMap->begin();
		while (players != objectMap->end()) {
			objectPlayer = players->second;
			objectPlayer->stop();
			++players;
		}
		hasParent = false;
		Player::stop();
	}

	void ChannelPlayer::updateStatus(
			short code, string parameter, short type) {

		notifyListeners(code, parameter, type);
	}

	void ChannelPlayer::setPropertyValue(
			string name, string value, double duration, double by) {

		Player* objectPlayer;
		map<string, Player*>::iterator players;

		//TODO: set brightness, ...
		if (name == "soundLevel") {
			players = objectMap->begin();
			while (players != objectMap->end()) {
				objectPlayer = players->second;
				objectPlayer->setPropertyValue(name, value, duration, by);
				++players;
			}
		}

		Player::setPropertyValue(name, value, duration, by);
	}
}
}
}
}
}
}

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

#include "../../../include/LinkTriggerCondition.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace ncl {
namespace model {
namespace link {
	LinkTriggerCondition::LinkTriggerCondition() : LinkCondition(), Thread() {
		listener = NULL;
		delay = 0.0;
		typeSet.insert("LinkTriggerCondition");
	}

	LinkTriggerCondition::~LinkTriggerCondition() {
		wclog << "LinkTriggerCondition::~LinkTriggerCondition" << endl;
		listener = NULL;
	}

	void LinkTriggerCondition::notifyConditionObservers(short status) {
		if (listener != NULL) {
			switch (status) {
				case LinkTriggerListener::CONDITION_SATISFIED:
					listener->conditionSatisfied((void*)this);
					break;

				case LinkTriggerListener::EVALUATION_STARTED:
					listener->evaluationStarted();
					break;

				case LinkTriggerListener::EVALUATION_ENDED:
					listener->evaluationEnded();
					break;
			}
		}
	}

	void LinkTriggerCondition::setTriggerListener(
		    LinkTriggerListener* listener) {

		this->listener = listener;
	}

	LinkTriggerListener* LinkTriggerCondition::getTriggerListener() {
		return listener;
	}

	double LinkTriggerCondition::getDelay() {
		return delay;
	}

	void LinkTriggerCondition::setDelay(double delay) {
		if (delay < 0)
			this->delay = 0;
		else
			this->delay = delay;
	}

	void LinkTriggerCondition::conditionSatisfied(void *condition) {
		if (delay > 0) {
			Thread::start();
		} else {
			notifyConditionObservers(LinkTriggerListener::CONDITION_SATISFIED);
		}
	}

	void LinkTriggerCondition::run() {
		wclog << "LinkTriggerCondition::run sleeping ";
		wclog << delay << "... " << endl;
		::usleep((long)(delay * 1000));
		wclog << "done!" << endl;
		notifyConditionObservers(LinkTriggerListener::CONDITION_SATISFIED);
	}
}
}
}
}
}
}
}

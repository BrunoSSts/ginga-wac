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

#ifndef _PROCEDURALPLAYERADAPTER_H_
#define _PROCEDURALPLAYERADAPTER_H_

#include "ncl/interfaces/LabeledAnchor.h"
using namespace ::br::pucrio::telemidia::ncl::interfaces;

#include "../../model/ProceduralExecutionObject.h"
using namespace ::br::pucrio::telemidia::ginga::ncl::model::components;

#include "../FormatterPlayerAdapter.h"
using namespace ::br::pucrio::telemidia::ginga::ncl::adapters;

#include "../INclEditListener.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace ncl {
namespace adapters {
namespace procedural {
	class ProceduralPlayerAdapter : public FormatterPlayerAdapter {
		private:
			INclEditListener* editingCommandListener;
			pthread_mutex_t eventMutex;

		protected:
			map<string, FormatterEvent*>* preparedEvents;
			FormatterEvent* currentEvent;

		public:
			ProceduralPlayerAdapter();
			virtual ~ProceduralPlayerAdapter();

			void setNclEditListener(INclEditListener* listener);
			virtual bool hasPrepared();
			virtual bool prepare(
					ExecutionObject* object, FormatterEvent* mainEvent);

		protected:
			void prepare(FormatterEvent* event);

		public:
			virtual bool start();
			virtual bool stop();
			virtual bool pause();
			virtual bool resume();
			virtual bool abort();
			virtual bool unprepare();
			virtual void naturalEnd();
			virtual void updateStatus(
					short code,
					string parameter="",
					short type=10);

			virtual void setCurrentEvent(FormatterEvent* event)=0;

		protected:
			bool startEvent(string anchorId, short type);
			bool stopEvent(string anchorId, short type);
			bool abortEvent(string anchorId, short type);
			bool pauseEvent(string anchorId, short type);
			bool resumeEvent(string anchorId, short type);

			void lockEvent();
			void unlockEvent();
			FormatterEvent* getEvent(string interfaceId);
	};
}
}
}
}
}
}
}

#endif //_ProceduralPlayerAdapter_H_

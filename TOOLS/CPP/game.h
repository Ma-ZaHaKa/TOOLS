#pragma once
#include "memory.h"
#include "interfaces.h"


namespace game
{
	namespace interfaces
	{
		ivengineclient* engine; // client
		void find_them()
		{
			engine = memory::pinterface<ivengineclient>("engine.dll", "VEngineClient014");			
		}
	}

	

	void find()
	{
		interfaces::find_them();
	}
}

#define _engine game::interfaces::engine

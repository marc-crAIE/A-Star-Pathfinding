#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

#include "Game.h"

using namespace Pinecone;

namespace PineconeGame
{
	class PineconeApp : public Application
	{
	public:
		PineconeApp()
			: Application()
		{
			PushLayer(new Game());
		}
	};
}

Application* Pinecone::CreateApplication()
{
	return new PineconeGame::PineconeApp();
}
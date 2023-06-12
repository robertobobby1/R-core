#pragma once
#include "rcpch.h"
#include "Services/Server/Server.h"

namespace RC {

	class WindowsServer : public Server {
	public:
		WindowsServer(const ServerInput& input);
		~WindowsServer();

		virtual void Init() override;
		virtual void OnUpdate() override; 

		void MainThreadLoop();
		void WorkerThreadLoop();

	private:
		void Setup();
	};
}
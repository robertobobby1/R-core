#pragma once
#include "Services/Server/Server.h"

namespace RC {

	class LinuxServer : public Server {
	public:
		LinuxServer(const ServerInput& input);
		~LinuxServer() = default;

		virtual void Init() override;
		virtual void OnGuiUpdate() override {};
		virtual void Run() override;

	private:
	};
}
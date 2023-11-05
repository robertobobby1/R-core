#pragma once
#include "Services/Server/Server.h"

namespace RC {

	class LinuxServer : public Server {
	public:
		LinuxServer(const ServerInput& input);
		~LinuxServer() = default;

		virtual void Run() override;

	private:
	};
}
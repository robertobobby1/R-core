#pragma once
#include "Services/Server/Server.h"

namespace RC {

	class LinuxServer : public Server {
	public:
		LinuxServer(const ServerInput& input);
		~LinuxServer();

		virtual void Init() override;
		virtual void OnUpdate() override;

	private:
	};
}
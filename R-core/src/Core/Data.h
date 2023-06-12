#pragma once
#include "rcpch.h"

#include "Core/Dispatch.h"

namespace RC {

	class ServerData : public Dispatchable {
	public: 

		ServerData() = default;
		~ServerData() = default;

		virtual inline DispatchableType GetType() override { return DispatchableType::ServerDataType; }
		static  inline DispatchableType GetStaticType() { return DispatchableType::ServerDataType; }

		uint8_t m_maxPoolSize = 1;
		uint32_t m_port = 0;
	};
}
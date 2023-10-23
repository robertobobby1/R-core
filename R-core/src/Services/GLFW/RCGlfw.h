#pragma once

#include "rcpch.h"
#include "Core/Service.h"


namespace RC {
	class RCGlfw : public Service {
	public: 
		RCGlfw() : Service() {}
		~RCGlfw() = default;

		virtual void Init() override;
		virtual void OnUpdate() override;
		virtual bool IsUniqueService() const override { return true; }

		static std::shared_ptr<RCGlfw> Create();

		static void PrintOpenGLInfo();
	};
}
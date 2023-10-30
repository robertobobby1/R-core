#pragma once 

#include "Services/ImGui/RCGui.h"

namespace RC {
	class SkeletonGui : public RCGui {
	public:
		SkeletonGui() : RCGui() {}
		~SkeletonGui() = default;

		virtual void Init() override;
		virtual void Run() override;

		virtual bool IsUniqueService() const override { return true; }

		virtual void Begin();
		virtual void End();
	};
}
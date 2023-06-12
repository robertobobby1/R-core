#pragma once

namespace RC {

	enum DispatchableType {
		None = 0, 
		ServerDataType
	};

	class Dispatchable {
	public:

		virtual ~Dispatchable() = default;

		virtual DispatchableType GetType() = 0;
		bool operator==(const Dispatchable& other) const { return this->m_tag == other.m_tag; }

		int m_tag = 0;
		bool m_handled = false;

	};

	class Dispatcher {
	public:
		Dispatcher(Dispatchable& dispatchable) 
			: m_dispatchable(dispatchable) {}

		template<typename Type, typename Func>
		void Dispatch(const Func& callback) 
		{
			if (m_dispatchable.GetType() == Type::GetStaticType()) 
			{
				m_dispatchable.m_handled = true;
				callback(static_cast<Type&>(m_dispatchable));
			}
		}

	private:
		Dispatchable& m_dispatchable;
	};
}
#pragma once 

namespace RC {	// forward DependencyDescriber, it will be defined at the end of the file
	// DONT MOVE IT FROM THERE OR IT WILL CRASH AT EXECUTION
	class Service;
	
	struct DependencyDescriber
	{
		std::shared_ptr<Service> dep;
		bool tryToFind;
		const char* depName;

		DependencyDescriber(const char* _depName, std::shared_ptr<Service> _dependency, bool _tryToFind)
			: dep(_dependency), tryToFind(_tryToFind), depName(_depName){ }
	};
}
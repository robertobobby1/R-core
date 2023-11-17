#pragma once

namespace RC {

    class Service;

    struct DependencyDescriber {
        std::shared_ptr<Service> dep;
        bool tryToFind;
        const char* depName;

        DependencyDescriber(const char* _depName, std::shared_ptr<Service> _dependency,
                            bool _tryToFind)
            : dep(_dependency), tryToFind(_tryToFind), depName(_depName) {}
    };
}  // namespace RC
#pragma once

#include <SalCore/Core.hpp>

namespace sal {
    class DataModel;

    class Service {
    public:
        Service(DataModel &dataModel) : _dataModel(dataModel) {}
        virtual ~Service() = default;

        DataModel &GetDataModel() const { return _dataModel; }

        virtual bool IsInitialized() const = 0;
        virtual String ToString() const = 0;

    private:
        virtual bool Initialize() = 0;
        virtual void Terminate() = 0;

    private:
        DataModel &_dataModel;

        friend class DataModel;
    };
}

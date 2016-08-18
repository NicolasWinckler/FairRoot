/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "boost/program_options.hpp"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"
#include "FairMQDevice.h"
#include "runSimpleMQStateMachine.h"


// forward declaration 
void addCustomOptions(boost::program_options::options_description&);

// forward declaration (friend to FairMQDevice)
template<typename T>
std::function<void(T&, FairMQProgOptions&)> DeviceManagerCallback();



// policy based design - use policy to customize
////////////////////////////////////////////////////
// 1) config default policy
struct ConfigDefaultPolicy
{
    int Init(int argc, char** argv, FairMQProgOptions& config)
    {
        boost::program_options::options_description customOptions("Custom options");
        addCustomOptions(customOptions);
        config.AddToCmdLineOptions(customOptions);
        config.ParseAll(argc, argv);
        return 0;
    }
    
};

////////////////////////////////////////////////////
// 2) Device default policy
template<typename T>
struct DeviceDefaultPolicy
{
    protected:
    
    void SetRun(T& device,FairMQProgOptions& config)
    {
        DeviceManagerCallback<T>()(device,config);
    }
    void SetInit(T& device,FairMQProgOptions& config)
    {
    }
};

// another device policy that take a function name (not friend with FairMQDevice)
template<typename T>
struct FuncNameDevicePolicy
{
    template<typename F>
    void DefineSetRun(F&& func)
    {
        fCallback = std::forward<F>(func);
    }
    
    protected:
    void SetRun(T& device,FairMQProgOptions& config)
    {
        fCallback(device, config);
    }
    void SetInit(T& device,FairMQProgOptions& config)
    {
    }

    std::function<void(T&,FairMQProgOptions&)> fCallback;
};

////////////////////////////////////////////////////
// 3) Device Manager -> Host class deriving from policies
template<   typename DeviceType, 
            typename DevicePolicy = DeviceDefaultPolicy<DeviceType>,
            typename ConfigPolicy = ConfigDefaultPolicy
        >
struct DeviceManager : protected ConfigPolicy, protected DevicePolicy
{
    int StartApp(int argc, char** argv)
    {
        FairMQProgOptions config;
        ConfigPolicy::Init(argc,argv,config);

        DeviceType device;
        DevicePolicy::SetInit(device,config);
        DevicePolicy::SetRun(device,config);
        runStateMachine(device,config);
        return 0;
    }

};


//////////////////////////////////////////////////// MACRO HELPER FOR OVERLOADING
#define CAT( A, B ) A ## B
#define SELECT( NAME, NUM ) CAT( NAME ## _, NUM )

#define GET_COUNT( _1, _2, _3, _4, _5, _6 /* ad nauseam */, COUNT, ... ) COUNT
#define VA_SIZE( ... ) GET_COUNT( __VA_ARGS__, 6, 5, 4, 3, 2, 1 )

#define VA_SELECT( NAME, ... ) SELECT( NAME, VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)
#define BUILD_MAIN( ... ) VA_SELECT( BUILD_MAIN_IMPL, __VA_ARGS__ )


//////////////////////////////////////////////////// Default build
#define BUILD_MAIN_IMPL_1(DeviceType)  \
int main( int argc, char* argv[] )     \
{                                      \
    DeviceManager<DeviceType> man;     \
    man.StartApp(argc,argv);           \
    return 0;                          \
}

//////////////////////////////////////////////////// Build with function name policy
#define BUILD_MAIN_IMPL_2(DeviceType, FunctionName)                      \
int main( int argc, char* argv[] )                                       \
    {                                                                    \
        DeviceManager<DeviceType, FuncNameDevicePolicy<DeviceType> > man;   \
        man.DefineSetRun(FunctionName);                                  \
        man.StartApp(argc,argv);                                         \
        return 0;                                                        \
    }


//////////////////////////////////////////////////// Hide the function boiler plate
#define WRITE_RUN_START(DeviceType)                                     \
template<> \
std::function<void(DeviceType&, FairMQProgOptions&)> DeviceManagerCallback<DeviceType>()    \
{                                                                               \
    return [] (DeviceType& device, FairMQProgOptions& config)                   \
    {                                                                           \
        device.SetRun([&device,&config]()                                       \
        {                                                                       

#define WRITE_RUN_END() \
        }); \
    }; \
}


//////////////////////////////////////////////////// same for function name API
#define WRITE_RUNFUNCTION_START(DeviceType, FunctionName)         \
void FunctionName(DeviceType& device, FairMQProgOptions& config)  \
{\
    device.SetRun([&device,&config]() \
    {

#define WRITE_RUNFUNCTION_END() \
    });\
}

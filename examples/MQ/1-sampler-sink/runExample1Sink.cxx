/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include <iostream>

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"
#include "FairMQDevice.h"
#include "runSimpleMQStateMachine.h"

int main(int argc, char** argv)
{
    try
    {
        FairMQProgOptions config;
        config.ParseAll(argc, argv);

        FairMQDevice sink;

        sink.OnData("data", [](std::unique_ptr<FairMQMessage>& msg)
        {
            LOG(INFO) << "Received message: \""
                      << std::string(static_cast<char*>(msg->GetData()), msg->GetSize())
                      << "\"";
        });

        runStateMachine(sink, config);
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit";
        return 1;
    }

    return 0;
}

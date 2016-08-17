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

using namespace std;

int main(int argc, char** argv)
{
    try
    {
        FairMQProgOptions config;
        config.ParseAll(argc, argv);

        FairMQDevice sink;

        sink.OnData("data", [](unique_ptr<FairMQMessage>& msg, int index)
        {
            LOG(INFO) << "Received message: \""
                      << string(static_cast<char*>(msg->GetData()), msg->GetSize())
                      << "\" on channel data[" << index << "].";

            return true;
        });

        runStateMachine(sink, config);
    }
    catch (exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit";
        return 1;
    }

    return 0;
}

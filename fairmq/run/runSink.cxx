/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include <iostream>

#include <boost/program_options.hpp>

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"
#include "FairMQDevice.h"
#include "runSimpleMQStateMachine.h"

using namespace boost::program_options;

int main(int argc, char** argv)
{
    try
    {
        int numMsgs;

        FairMQProgOptions config;
        config.GetCmdLineOptions().add_options()
            ("num-msgs", value<int>(&numMsgs)->default_value(0), "Number of messages to receive");
        config.ParseAll(argc, argv);

        FairMQDevice sink;

        int numReceivedMsgs = 0;

        sink.SetPreRun([&sink, &numMsgs]()
        {
            LOG(INFO) << "Starting the benchmark and expecting to receive " << numMsgs << " messages.";
        });

        sink.SetRun([&sink, &numMsgs, &numReceivedMsgs]()
        {
            std::unique_ptr<FairMQMessage> msg(sink.NewMessage());

            if (sink.Receive(msg, "data-in") >= 0)
            {
                if (numMsgs > 0)
                {
                    ++numReceivedMsgs;
                    if (numReceivedMsgs >= numMsgs)
                    {
                        return false;
                    }
                }
            }

            return true;
        });

        sink.SetPostRun([&numReceivedMsgs]()
        {
            LOG(INFO) << "Received " << numReceivedMsgs << " messages, leaving RUNNING state.";
        });

        runStateMachine(sink, config);
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: "
                   << e.what() << ", application will now exit";
        return 1;
    }

    return 0;
}

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
using namespace std;

int main(int argc, char** argv)
{
    try
    {
        int msgSize;
        int numMsgs;

        FairMQProgOptions config;
        config.GetCmdLineOptions().add_options()
            ("msg-size", value<int>(&msgSize)->default_value(1000), "Message size in bytes")
            ("num-msgs", value<int>(&numMsgs)->default_value(0),    "Number of messages to send");
        config.ParseAll(argc, argv);

        FairMQDevice sampler;

        int numSentMsgs = 0;

        unique_ptr<FairMQMessage> baseMsg; // uninitialized - transport not yet initialized!

        sampler.SetPreRun([&sampler, &baseMsg, &msgSize, &numMsgs]()
        {
            LOG(INFO) << "Starting benchmark Run with message size of " << msgSize << " and number of messages " << numMsgs << ".";
            baseMsg = unique_ptr<FairMQMessage>(sampler.NewMessage(msgSize));
        });

        sampler.SetRun([&sampler, &baseMsg, &numSentMsgs, &msgSize, &numMsgs]()
        {
            unique_ptr<FairMQMessage> msg(sampler.NewMessage());
            msg->Copy(baseMsg);

            if (sampler.Send(msg, "data-out") >= 0)
            {
                ++numSentMsgs;
                if (numMsgs > 0)
                {
                    if (numSentMsgs == numMsgs)
                    {
                        return false;
                    }
                }

                return true;
            }
            else
            {
                LOG(WARN) << "Transfer canceled by interrupt";
                return false;
            }
        });

        sampler.SetPostRun([&numSentMsgs]()
        {
            LOG(INFO) << "Sent " << numSentMsgs << " messages, leaving RUNNING state.";
        });

        runStateMachine(sampler, config);
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit";
        return 1;
    }

    return 0;
}

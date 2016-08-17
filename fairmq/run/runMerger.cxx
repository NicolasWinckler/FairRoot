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
#include "FairMQParts.h"
#include "runSimpleMQStateMachine.h"

using namespace boost::program_options;

int main(int argc, char** argv)
{
    try
    {
        int multipart;

        FairMQProgOptions config;
        config.GetCmdLineOptions().add_options()
            ("multipart", value<int>(&multipart)->default_value(1), "Handle multipart payloads");
        config.ParseAll(argc, argv);

        FairMQDevice merger;

        // std::unique_ptr<FairMQPoller> poller;
        // int numInputs;

        if (multipart)
        {
            merger.OnMultipartData("data-in", [&merger](FairMQParts& payload, int index)
            {
                if (merger.Send(payload, "data-out") < 0)
                {
                    LOG(DEBUG) << "Transfer interrupted";
                    return false;
                }

                return true;
            });
        }
        else
        {
            merger.OnData("data-in", [&merger](std::unique_ptr<FairMQMessage>& msg, int index)
            {
                if (merger.Send(msg, "data-out") < 0)
                {
                    LOG(DEBUG) << "Transfer interrupted";
                    return false;
                }

                return true;
            });
        }

        // merger.SetPreRun([&merger, &poller, &numInputs]()
        // {
        //     numInputs = merger.fChannels.at("data-in").size();
        //     poller = std::unique_ptr<FairMQPoller>(merger.NewPoller({ "data-in" }));
        // });

        // if (multipart)
        // {
        //     merger.SetRun([&merger, &poller, &numInputs]()
        //     {
        //         poller->Poll(100);

        //         // Loop over the data input channels.
        //         for (int i = 0; i < numInputs; ++i)
        //         {
        //             // Check if the channel has data ready to be received.
        //             if (poller->CheckInput(i))
        //             {
        //                 FairMQParts payload;

        //                 if (merger.Receive(payload, "data-in", i) >= 0)
        //                 {
        //                     if (merger.Send(payload, "data-out") < 0)
        //                     {
        //                         LOG(DEBUG) << "Transfer interrupted";
        //                         return false;
        //                     }
        //                 }
        //                 else
        //                 {
        //                     LOG(DEBUG) << "Transfer interrupted";
        //                     return false;
        //                 }
        //             }
        //         }

        //         return true;
        //     });
        // }
        // else
        // {
        //     merger.SetRun([&merger, &poller, &numInputs]()
        //     {
        //         poller->Poll(100);

        //         // Loop over the data input channels.
        //         for (int i = 0; i < numInputs; ++i)
        //         {
        //             // Check if the channel has data ready to be received.
        //             if (poller->CheckInput(i))
        //             {
        //                 unique_ptr<FairMQMessage> payload(merger.NewMessage());

        //                 if (merger.Receive(payload, "data-in", i) >= 0)
        //                 {
        //                     if (merger.Send(payload, "data-out") < 0)
        //                     {
        //                         LOG(DEBUG) << "Transfer interrupted";
        //                         return false;
        //                     }
        //                 }
        //                 else
        //                 {
        //                     LOG(DEBUG) << "Transfer interrupted";
        //                     return false;
        //                 }
        //             }
        //         }

        //         return true;
        //     });
        // }

        runStateMachine(merger, config);
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: "
                   << e.what() << ", application will now exit";
        return 1;
    }

    return 0;
}

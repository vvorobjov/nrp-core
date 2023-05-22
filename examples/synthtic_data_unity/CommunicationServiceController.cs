/*
 * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2022-2023 Josip Josifovski, Krzysztof Lebioda
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

using UnityEngine;
using System.Threading.Tasks;
using System;
using System.Collections.Generic;
using Grpc.Core;
using EngineGrpc;
using Google.Protobuf;
using System.Text;
using System.Threading;
using Google.Protobuf.Collections;
using Google.Protobuf.WellKnownTypes;
using UnityProto;
using UnityEngine.Perception.GroundTruth;
using UnityEngine.Perception.Randomization.Scenarios;
using UnityEngine.Perception.Randomization.Randomizers;

/// <summary>
/// Enum for the possible remote proedures calls, new ones to be added if the game should support more
/// </summary>
enum ServiceProcedure { NONE, INITIALIZE, RESET, SHUTDOWN, GET_DATAPACKS, SET_DATAPACKS, RUN_LOOP_STEP }

class CommunicationServiceController : MonoBehaviour
{
    /// <summary>
    /// The currently invoked remote procedure
    /// </summary>
    private ServiceProcedure _invokedProcedure;

    private readonly object _locker = new object();

    /// <summary>
    /// gRPC server responsible for listening to requests and providing responses
    /// </summary>
    private Server _server;

    /// <summary>
    /// The ip address on which the server is registered, usually localhost when both client and server are on the same machine, can be a configurable parameter in future
    /// </summary>
    private string _ipAddress = "localhost";

    /// <summary>
    /// Port on which the server should be listening, it should be accessable from outside if clients are supposed to connect from different machine, can be a configurable parameter in future
    /// </summary>
    private int _port = 50051;

    /// <summary>
    /// A camera controller of an overhead camera, provides the image
    /// </summary>
    //private CameraController _cameraController;

    /// <summary>
    /// TImeStep controller, responsible for running the game simulation
    /// </summary>
    private TimeStepController _timeStepController;

 
    /// <summary>
    /// Catches unity camera and returns current frame
    /// </summary>
    private ImageSender _imageSender;
    /// <summary>
    /// A handle that is used to block the thread which recieves the remote procedure call until the procedure is executed in the main Unity thread (necessary as UnityEngine calls have to run in the main thread)
    /// </summary>
    private AutoResetEvent _stopWaitHandle;

    /// <summary>
    /// The total simulated time so far in nanoseconds
    /// </summary>
    private long _totalSimulatedTime = 0;
    private long _shutdownCounter = 0;


    /// ===========================Data holders for the requests and responses of the procedures, necessary for syncing the data between the main Untty thread and the server thread
    private InitializeRequest _initializeRequest = null;
    private InitializeReply _initializeReply = null;

    private ResetRequest _resetRequest = null;
    private ResetReply _resetReply = null;

    private ShutdownRequest _shutdownRequest = null;
    private ShutdownReply _shutdownReply = null;

    private RunLoopStepRequest _runLoopStepRequest = null;
    private RunLoopStepReply _runLoopStepReply = null;

    private SetDataPacksRequest _setDataPacksRequest = null;
    private SetDataPacksReply _setDataPacksReply = null;

    private GetDataPacksRequest _getDataPacksRequest = null;
    private GetDataPacksReply _getDataPacksReply = null;
    public PoseEstimationScenario scenario;


    /// ===============================================================================================================================================================================


    private void Awake()
    {
        Physics.autoSimulation = false;
        DontDestroyOnLoad(transform.gameObject);
        _stopWaitHandle = new AutoResetEvent(false);
        _invokedProcedure = ServiceProcedure.NONE;
        Debug.Log("[CommControl] Starting gRPC server");
        StartServer();
        Debug.Log("[CommControl] gRPC Server started");
    }

    private void initializeUser(string simulationConfigJson)
    {
        // User-defined initialization code should go here
    }

    private void resetUser()
    {
        // User-defined reset code should go here
    }

    private void shutdownUser()
    {
        // User-defined shutdown code should go here

        // Shutdown in 2s
        _shutdownCounter = 100;
    }

    private void setDataPacksUser(IList<DataPackMessage> dataPacks)
    {
        
    }

    private long runLoopUser(long timeStep)
    {
        // User-defined runLoop code should go here

        scenario.letItGo();
        scenario.Move();
        // Lazy initialization
        if(_timeStepController == null)
        {
            _timeStepController = FindObjectOfType<TimeStepController>();
        }

        return (long)_timeStepController.RunGame(_runLoopStepRequest.TimeStep);
    }

    private List<DataPackMessage> getDataPacksUser()
    {
        List<DataPackMessage> dataPacks = new List<DataPackMessage>();

        // Image Section
        // Lazy initialization
        if (_imageSender == null)
        {
            _imageSender = FindObjectOfType<ImageSender>();
        }


        byte[] frameBytes = _imageSender.GetCurrentFrame();

        UnityProto.BImage imageProto = new UnityProto.BImage();
        imageProto.Image = Google.Protobuf.ByteString.CopyFrom(frameBytes);

        DataPackMessage imageDataPack = new DataPackMessage();
        imageDataPack.DataPackId = new DataPackIdentifier();
        
        imageDataPack.DataPackId.DataPackName = "image";
        imageDataPack.DataPackId.DataPackType = "frame";
        imageDataPack.DataPackId.EngineName   = "unity";
        imageDataPack.Data = Any.Pack(imageProto);

        dataPacks.Add(imageDataPack);

        
        return dataPacks;
    }

    private void FixedUpdate()
    {
        if(Physics.autoSimulation)
        {
            return;
        }

        if(_shutdownCounter > 0)
        {
            _shutdownCounter--;
            if(_shutdownCounter == 0)
            {
                Application.Quit();
            }
        }

        lock(_locker)
        {
            if(_invokedProcedure != ServiceProcedure.NONE)
            {
                switch(_invokedProcedure)
                {
                    case ServiceProcedure.INITIALIZE:
                        initializeUser(_initializeRequest.Json);
                        _totalSimulatedTime = 0;
                        _initializeReply = new InitializeReply { Json = "1"};
                        break;

                    case ServiceProcedure.RESET:
                        resetUser();
                        _totalSimulatedTime = 0;
                        _resetReply = new ResetReply { Json = "1" };
                        break;

                    case ServiceProcedure.SHUTDOWN:
                        shutdownUser();
                        _shutdownReply = new ShutdownReply { Json = "" };
                        break;

                    case ServiceProcedure.RUN_LOOP_STEP:
                        long simulatedTimeStep = runLoopUser(_runLoopStepRequest.TimeStep);

                        _totalSimulatedTime += simulatedTimeStep;
                        _runLoopStepReply = new RunLoopStepReply { EngineTime =_totalSimulatedTime };
                        break;

                    case ServiceProcedure.SET_DATAPACKS:
                        setDataPacksUser(_setDataPacksRequest.DataPacks);
                        _setDataPacksReply = new SetDataPacksReply();
                        break;

                    case ServiceProcedure.GET_DATAPACKS:
                        List<DataPackMessage> dataPacks = getDataPacksUser();

                        _getDataPacksReply = new GetDataPacksReply();
                        _getDataPacksReply.DataPacks.Add(dataPacks);

                        break;
                }

                _invokedProcedure = ServiceProcedure.NONE;
                _stopWaitHandle.Set();
            }
        }
    }

    private void OnDisable()
    {
        Debug.Log("[CommControl] Stopping server");
        StopServer();
    }


    public InitializeReply InitializeAsync(InitializeRequest request)
    {
        _initializeRequest = request;
        lock(_locker)
        {
            _invokedProcedure = ServiceProcedure.INITIALIZE;
        }
        _stopWaitHandle.WaitOne();
        return _initializeReply;
    }

    public ResetReply ResetAsync(ResetRequest request)
    {
        _resetRequest = request;
        lock(_locker)
        {
            _invokedProcedure = ServiceProcedure.RESET;
        }
        _stopWaitHandle.WaitOne();
        return _resetReply;
    }

    public ShutdownReply ShutdownAsync(ShutdownRequest request)
    {
        _shutdownRequest = request;
        lock(_locker)
        {
            _invokedProcedure = ServiceProcedure.SHUTDOWN;
        }
        _stopWaitHandle.WaitOne();
        return _shutdownReply;
    }

    public GetDataPacksReply GetDataPacksAsync(GetDataPacksRequest request)
    {
        _getDataPacksRequest = request;
        lock(_locker)
        {
            _invokedProcedure = ServiceProcedure.GET_DATAPACKS;
        }
        _stopWaitHandle.WaitOne();
        return _getDataPacksReply;
    }

    public RunLoopStepReply RunLoopStepAsync(RunLoopStepRequest request)
    {
        _runLoopStepRequest = request;
        lock(_locker)
        {
            _invokedProcedure = ServiceProcedure.RUN_LOOP_STEP;
        }
        _stopWaitHandle.WaitOne();
        return _runLoopStepReply;
    }

    public SetDataPacksReply SetDataPacksAsync(SetDataPacksRequest request)
    {
        _setDataPacksRequest = request;
        lock(_locker)
        {
            _invokedProcedure = ServiceProcedure.SET_DATAPACKS;
        }
        _stopWaitHandle.WaitOne();
        return _setDataPacksReply;
    }

    public void StartServer()
    {
        _server = new Server
        {
            Services = { EngineGrpcService.BindService(new EngineGrpcServiceImpl(this)) },
            Ports = { new ServerPort(_ipAddress, _port, ServerCredentials.Insecure) }
        };
        _server.Start();

    }

    public void StopServer()
    {
        _server.ShutdownAsync().Wait();
    }

    /// <summary>
    /// An implementation of the service.
    /// If new remote procedure is defined in the proto file, a new Unity classes Service.cs and ServiceGrpc.cd should be generated and imported in the project to replace the existing ones.
    /// Then, the new method should be implemented as the ones below
    /// </summary>
    class EngineGrpcServiceImpl : EngineGrpcService.EngineGrpcServiceBase
    {

        private CommunicationServiceController _communicationServiceControlller;

        public EngineGrpcServiceImpl(CommunicationServiceController communicationServiceControlller)
        {
            _communicationServiceControlller = communicationServiceControlller;
        }

        public override Task<InitializeReply> initialize(InitializeRequest request, ServerCallContext context)
        {
            Debug.Log("[CommControl] Initialize request received");
            return Task.FromResult(_communicationServiceControlller.InitializeAsync(request));
        }

        public override Task<ShutdownReply> shutdown(ShutdownRequest request, ServerCallContext context)
        {
            return Task.FromResult(_communicationServiceControlller.ShutdownAsync(request));
        }

        public override Task<ResetReply> reset(ResetRequest request, ServerCallContext context)
        {
        return Task.FromResult(_communicationServiceControlller.ResetAsync(request));
        }

        public override Task<RunLoopStepReply> runLoopStep(RunLoopStepRequest request, ServerCallContext context)
        {
            return Task.FromResult(_communicationServiceControlller.RunLoopStepAsync(request));
        }

        public override Task<SetDataPacksReply> setDataPacks(SetDataPacksRequest request, ServerCallContext context)
        {
            return Task.FromResult(_communicationServiceControlller.SetDataPacksAsync(request));
        }

        public override Task<GetDataPacksReply> getDataPacks(GetDataPacksRequest request, ServerCallContext context)
        {
            return Task.FromResult(_communicationServiceControlller.GetDataPacksAsync(request));
        }
    }
}
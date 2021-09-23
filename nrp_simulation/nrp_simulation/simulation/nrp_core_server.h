/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
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

#ifndef NRP_CORE_SERVER_H
#define NRP_CORE_SERVER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <grpcpp/grpcpp.h>

#include "nrp_protobuf/nrp_server.grpc.pb.h"

/*!
 * \brief NRP Server class, responsible for handling simulation control requests coming from the client application or script
 *
 * The class has two roles:
 * * receive simulation control requests from the client application
 * * relay received requests to the main thread of NRP Core and wait for response
 *
 * The class is an instance of gRPC Service, which means that it will have a pool of worker threads.
 * Every request coming from the client will be handled by a separate thread from the pool.
 * The natural approach to request handling would be to call proper methods of the SimulationManager directly
 * from the callbacks (for example, SimulationManager::initFTILoop() from NrpCoreServer::init()).
 * This proved to cause problems with certain python libraries, like OpenCV, that may be used in the transceiver functions.
 * The problem seems to arise from the fact, that the python code was executed in a worker thread, and not
 * in the thread that spawned the python interpreter (the main thread of NRP Core). The solution is to create
 * a producer-consumer relation between the NRP Server threads and the main thread. The NRP Server relays (produces)
 * the requests to the main thread, which consumes them and returns a status back to the NRP Server.
 */
class NrpCoreServer : public NrpCore::NrpCore::Service
{
	public:
		enum class RequestType { None, Init, RunLoop, Shutdown };

		/*!
		 * \brief Constructor. Spawns an instance of gRPC server with given address.
		 * \param address Address of the gRPC server
		 */
		NrpCoreServer(const std::string & address);

		/*!
		 * \brief Returns the type of request coming from the client
		 */
		RequestType getRequestType() const;

		/*!
		 * \brief Returns number of iterations of runLoop requested by the client
		 */
		unsigned getNumIterations() const;

		/*!
		 * \brief Puts the current thread to sleep until a request is available
		 *
		 * The function is supposed to be called by the consumer thread.
		 * It should be called in combination with markRequestAsProcessed function.
		 */
		void waitForRequest();

		/*!
		 * \brief Signals the server that the request has been processed
		 *
		 * The function is supposed to be called by the consumer thread, after
		 * the pending request has been consumed. If the function isn't called, the
		 * producer thread will not wake up! It must be called after waitForRequest function.
		 */
		void markRequestAsProcessed();

		/*!
		 * \brief Signals the server, that there was an error during request handling
		 *
		 * \param errorMessage Message describing what went wrong
		 */
		void markRequestAsFailed(const std::string & errorMessage);

	private:

		/*!
		 * \brief Clears the request variables and sets the request type to none
		 */
		void resetRequest();

		/*!
		 * \brief Indicates if there's a request pending
		 */
		bool isRequestPending() const;

		/*!
		 * \brief A helper function used by the request callbacks
		 *
		 * The function will block the callback thread until the request has been processed by the consumer.
		 *
		 * \param lock        Lock acquired by the callback
		 * \param requestType Type of the request to be prepared for the consumer
		 * \return            Status of the request. In case of failure, it will contain grpc::StatusCode::CANCELLED
		 *                    and the error message returned by the consumer.
		 */
		grpc::Status requestHelper(std::unique_lock<std::mutex> & lock, RequestType requestType);

		/*!
		 * \brief Callback for the initialization request coming from the client
		 */
		grpc::Status initialize(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::EmptyMessage *) override;

		/*!
		 * \brief Callback for the run loop request coming from the client
		 */
		grpc::Status runLoop(grpc::ServerContext * , const NrpCore::RunLoopMessage * message, NrpCore::EmptyMessage *) override;

		/*!
		 * \brief Callback for the shutdown request coming from the client
		 */
		grpc::Status shutdown(grpc::ServerContext * , const NrpCore::EmptyMessage * , NrpCore::EmptyMessage *) override;

		/*!
		 * \brief Mutex used for synchronization of consumer and producer threads with conditional variables
		 */
		std::mutex _mutex;

		/*!
		 * \brief Conditional variable used to deliver requests to the consumer
		 */
		std::condition_variable _consumerConditionalVar;

		/*!
		 * \brief Conditional variable used to pass responses back to the producer
		 */
		std::condition_variable _producerConditionalVar;

		/*!
		 * \brief Lock used by the functions called from the consumer threads
		 */
		std::unique_lock<std::mutex> _lock;

		/*!
		 * \brief Pointer to the gRPC server class
		 */
		std::unique_ptr<grpc::Server> _server;

		/*!
		 * \brief Type of the pending request
		 */
		RequestType _requestType = RequestType::None;

		/*!
		 * \brief Number of iterations of runLoop requested by the client
		 */
		unsigned _numIterations = 0;

		/*!
		 * \brief Helper structure holding request status and error messages
		 */
		struct RequestStatus
		{
			bool        failed       = false;
			std::string errorMessage = "";
		} _requestStatus;
};

#endif // NRP_CORE_SERVER_H

// EOF

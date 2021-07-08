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

#ifndef NRP_LOGGER_H
#define NRP_LOGGER_H

#ifdef PRODUCTION_RELEASE
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#else
#define SPDLOG_TRACE_ON
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <spdlog/spdlog.h>

#include <semaphore.h>

/*!
 * \brief trace log macro. It is voided by defining \PRODUCTION_RELEASE
 */
#define NRP_LOGGER_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)


/*!
 * \brief NRP Logging functions
 */
class NRPLogger
{
public:

	/*!
	 * \brief The wrapper type for log levels
	 */
	typedef spdlog::level::level_enum level_t;

	/*!
	 * \brief The creation of the configurable instance of spdlog, that is set to default logger, with default settings.
	 * \param loggerName The name of the logger, that is to be displayed in message and placed as log file prefix
	 */
	NRPLogger(
		std::string loggerName = _defaultLoggerName.data());

	/*!
	 * \brief The creation of the configurable instance of spdlog, that is set to default logger.
	 * 
	 * If the \launcher is set true, then the constructor tries to save the logger settings to the shared memory object.
	 * Otherwise, the constructor tries to load this settings from the shared memory object. In case of success, 
	 * these  settings are applied to the logger to be created, otherwise the provided settings are used.
	 * 
	 * \param loggerName The name of the logger, that is to be displayed in message and placed as log file prefix
	 * \param logDir The location for the log files
	 * \param fileLogLevel The minimum log level to be put to the log files
	 * \param consoleLogLevel The minimum log level to be printed in console
	 * \param doSavePars Save logger parameters to shared memory or not (load them instaed)
	 */
	NRPLogger(
		std::string loggerName,
		NRPLogger::level_t fileLogLevel,
		NRPLogger::level_t consoleLogLevel,
		std::string logDir,
		bool doSavePars = false);

	~NRPLogger();

	/*!
	 * \brief Wrapper function for converting enumed log level into string
	 * \param level The numbered representation of the log level
	 */
	static std::string level_to_string(const NRPLogger::level_t &level)
	{
		return spdlog::level::to_string_view(level).data();
	}

	/*!
	 * \brief Wrapper function for getting enumed log level from string. Non-valid string is converted to enum::off
	 * \param level The string representation of the log level
	 */
	static NRPLogger::level_t level_from_string(const std::string &level)
	{
		return spdlog::level::from_str(level);
	}

	/*!
	 * \brief Shutdown default logger
	 */
	static void shutdownDefault();

	/*!
	 * \brief Flush default logger
	 */
	void flush();

	/*!
	 * \brief Get default NRPLogger
	 */
	static spdlog::logger &nrpLogger();

	/*!
	 * \brief Logging function type, is used by Exception
	 */
	using spdlog_out_fcn_t = void (&)(const std::string &);

	/*!
	 * \brief NRP logging function with message formatting for debug level
	 * \param fmt Message format string in fmt library style https://fmt.dev/latest/index.html
	 * \param args Arguments for substitution into format string #fmt
	 */
	template <typename FormatString, typename... Args>
	static void debug(const FormatString &fmt, const Args &...args)
	{
		spdlog::default_logger()->log(spdlog::level::debug, fmt, args...);
	}

	/*!
	 * \brief NRP logging function with message formatting for info level
	 * \param fmt Message format string in fmt library style https://fmt.dev/latest/index.html
	 * \param args Arguments for substitution into format string #fmt
	 */
	template <typename FormatString, typename... Args>
	static void info(const FormatString &fmt, const Args &...args)
	{
		spdlog::default_logger()->log(spdlog::level::info, fmt, args...);
	}

	/*!
	 * \brief NRP logging function with message formatting for warning level
	 * \param fmt Message format string in fmt library style https://fmt.dev/latest/index.html
	 * \param args Arguments for substitution into format string #fmt
	 */
	template <typename FormatString, typename... Args>
	static void warn(const FormatString &fmt, const Args &...args)
	{
		spdlog::default_logger()->log(spdlog::level::warn, fmt, args...);
	}

	/*!
	 * \brief NRP logging function with message formatting for error level
	 * \param fmt Message format string in fmt library style https://fmt.dev/latest/index.html
	 * \param args Arguments for substitution into format string #fmt
	 */
	template <typename FormatString, typename... Args>
	static void error(const FormatString &fmt, const Args &...args)
	{
		spdlog::default_logger()->log(spdlog::level::err, fmt, args...);
	}

	/*!
	 * \brief NRP logging function with message formatting for critical error level
	 * \param fmt Message format string in fmt library style https://fmt.dev/latest/index.html
	 * \param args Arguments for substitution into format string #fmt
	 */
	template <typename FormatString, typename... Args>
	static void critical(const FormatString &fmt, const Args &...args)
	{
		spdlog::default_logger()->log(spdlog::level::critical, fmt, args...);
	}

	/*!
	 * \brief NRP logging function for debug level
	 * \param msg The message to be logged
	 */
	template <typename Message>
	static void debug(const Message &msg)
	{
		spdlog::default_logger()->log(spdlog::level::debug, msg);
	}

	/*!
	 * \brief NRP logging function for info level
	 * \param msg The message to be logged
	 */
	template <typename Message>
	static void info(const Message &msg)
	{
		spdlog::default_logger()->log(spdlog::level::info, msg);
	}

	/*!
	 * \brief NRP logging function for warning level
	 * \param msg The message to be logged
	 */
	template <typename Message>
	static void warn(const Message &msg)
	{
		spdlog::default_logger()->log(spdlog::level::warn, msg);
	}

	/*!
	 * \brief NRP logging function for error level
	 * \param msg The message to be logged
	 */
	template <typename Message>
	static void error(const Message &msg)
	{
		spdlog::default_logger()->log(spdlog::level::err, msg);
	}

	/*!
	 * \brief NRP logging function for critical error level
	 * \param msg The message to be logged
	 */
	template <typename Message>
	static void critical(const Message &msg)
	{
		spdlog::default_logger()->log(spdlog::level::critical, msg);
	}

private:
	/*!
	 * \brief Default Name of NRP Logger
	 */
	static constexpr std::string_view _defaultLoggerName = "nrp_core";

	/*!
	 * \brief Base filename for log files
	 */
	std::string _baseFilename;

	/*!
	 * \brief logger name, that is displayed
	 */
	std::string _loggerName;

	/*!
	 * \brief The minimum log level to be put to the log files
	 */
	NRPLogger::level_t _fileLogLevel;

	/*!
	 * \brief The minimum log level to be printed in console
	 */
	NRPLogger::level_t _consoleLogLevel;

	/*!
	 * \brief location of the log files
	 */
	std::string _logDir;

	/*!
	 * \brief console destination
	 */
	FILE *_consoleOut;

	/*!
	 * \brief The logger is created from launcher and propagates its settings to child processes (saves them to shared memory).
	 */
	bool _doSavePars;

	/*!
	 * \brief Default location of the log files
	 */
	static constexpr std::string_view _defaultLogDir = "log";

	/*!
	 * \brief The name of the shared memory object for saving/loading the logger parameters
	 */
	static constexpr std::string_view _sharedMemCfgName = "/NRPLogger";

	/*!
	 * \brief The function for registering the configured spdlog as a default one.
	 *
	 * First, this function checks the name \ref _loggerName and replaces it by default value 
	 * \ref _defaultLoggerName, in case \ref _loggerName is empty. 
	 * Then, the function checks if the logger with such name exists and ommits the creation of the new logger 
	 * in case it finds such a logger.
	 * If the creation of the new logger is needed, then the two sinks (into file and into console) are created
	 * with corresponding \ref _fileLogLevel and \ref _consoleLogLevel levels.
	 * The log file name is defined as _logDir/_loggerName-timestamp<%Y%m%d-%H%M%S>-pid.log
	 * The created logger is registered as spdlog default logger
	 */
	void registerDefaultLogger();

	/*!
	 * \brief The function tries to fill shared memory object with the properties of the logger to be constructed.
	 * \return the bool result of the success
	 */
	bool setSharedMemoryForLauncher();

	/*!
	 * \brief The function tries to load the properties of the logger to be constructed from shared memory object.
	 * \return the bool result of the success
	 */
	bool getSharedMemoryFromLauncher();

	/*!
	 * \brief The function tries close and unlink the shared memory object in case of operations failure.
	 * \param fd shared memory file descriptor
	 * \return false
	 */
	bool shutdownSharedMemory(int& fd);

	/*!
	 * \brief Logger data struct to be saved into shared memory object
	 */
	struct LoggerConfig {
		sem_t  sem1;			/**< A semaphore indicating that the launcher has saved the settings */ 
		uint fileLogLevel;		/**< Equivalent  \ref _fileLogLevel*/ 
		uint consoleLogLevel;	/**< Equivalent  \ref _consoleLogLevel*/
		size_t logDirLen;		/**< The number of symbols in _logDir*/
		char logDir[1024];		/**< The char buffer for storage of the _logDir*/
	};
};

#endif // NRP_LOGGER_H

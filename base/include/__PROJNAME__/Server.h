#ifndef __PROJNAMEUPPER___SERVER_H_
#define __PROJNAMEUPPER___SERVER_H_

/// @file
/// Server
/// __DATE__ __TIME__

// __PROJNAME__ includes
#include <__PROJNAME__/Detail/ConnectionManager.h>

// asio includes
#include <asio.hpp>

// STL includes
#include <string>

namespace __PROJNAME__
{
	/// @brief Server represents a TCP server
	class Server
	{
	public:
		/// @brief Creates a server on the specified address and port
		/// @param address The address to listen on
		/// @param port The port to listen on
		/// @throws std::error_code
		Server(const std::string& address, const std::string& port);
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		/// @brief Runs the server
		/// @throws std::error_code
		void Run();
	private:
		using Proto_t = asio::ip::tcp;
		
		/// @brief Awaits a signal
		void AwaitSignals() noexcept;

		/// @brief Starts the acceptor
		/// @throws std::error_code
		void StartAcceptor(const Proto_t::endpoint& endpoint);
		/// @brief Accepts connections
		void Accept() noexcept;

		asio::io_context m_worker;
		Proto_t::acceptor m_acceptor;
		Detail::ConnectionManager m_connectionManager;
		asio::signal_set m_signals;
	};
}

#endif
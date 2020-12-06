#ifndef __PROJNAMEUPPER___DETAIL_CONNECTION_H_
#define __PROJNAMEUPPER___DETAIL_CONNECTION_H_

/// @file
/// Connection
/// __DATE__ __TIME__

// asio includes
#include <asio.hpp>

// STL includes
#include <array>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

namespace __PROJNAME__
{
	namespace Detail
	{
		class ConnectionManager;

		/// @brief Connection represents a TCP connection
		class Connection :
			public std::enable_shared_from_this<Connection>
		{
		public:
			using Proto_t = asio::ip::tcp;
			using Socket_t = Proto_t::socket;

			/// @brief Creates a connection with a manager and active socket
			/// @param connectionManager The connection manager
			/// @param socket The connected socket
			Connection(ConnectionManager& connectionManager,
				Socket_t socket) noexcept;

			/// @brief Starts processing the connection
			void Start() noexcept;
			/// @brief Stops processing the connection
			void Stop() noexcept;

			/// @brief Queues a packet to be sent to the endpoint
			/// @param data The data to send
			/// @tparam Data_t The type of data
			template<typename... Data_t>
			void SendPacket(Data_t&&... data) noexcept
			{
				bool sendInProgress = (m_outgoingQueue.empty() == false);
				m_outgoingQueue.emplace(std::forward<Data_t>(data)...);
				if (sendInProgress == false)
					Write();
			}
		private:
			constexpr static size_t BUF_SIZE = 16384;

			/// @brief Stops processing the connection.
			/// Called only by ConnectionManager
			void StopImpl() noexcept;

			/// @brief Reads from the socket
			void Read() noexcept;

			/// @brief Sends data from the queue
			void Write() noexcept;

			ConnectionManager& m_connectionManager;
			Proto_t::endpoint m_remoteEndpoint;
			Socket_t m_socket;
			std::array<char, BUF_SIZE> m_incomingBuf;
			std::queue<std::vector<char>> m_outgoingQueue;

			friend class ConnectionManager;
		};
	}
}

#endif
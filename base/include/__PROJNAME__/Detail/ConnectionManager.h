#ifndef __PROJNAMEUPPER___DETAIL_CONNECTIONMANAGER_H_
#define __PROJNAMEUPPER___DETAIL_CONNECTIONMANAGER_H_

/// @file
/// Connection Manager
/// __DATE__ __TIME__

// __PROJNAME__ includes
#include <__PROJNAME__/Detail/Connection.h>

// STL includes
#include <memory>
#include <unordered_set>

namespace __PROJNAME__
{
	namespace Detail
	{
		/// @brief ConnectionManager manages TCP connections
		class ConnectionManager
		{
		public:
			using ConnectionPtr_t = std::shared_ptr<Connection>;
			using ConnectionSet_t = std::unordered_set<ConnectionPtr_t>;

			ConnectionManager() = default;
			ConnectionManager(const ConnectionManager&) = delete;
			ConnectionManager& operator=(const ConnectionManager&) = delete;

			/// @brief Starts tracking a connection and begins processing
			/// @param conn The connection
			void Start(const ConnectionPtr_t& conn) noexcept;

			/// @brief Stops tracking a connection and shuts it down
			/// @param conn The connection
			void Stop(const ConnectionPtr_t& conn) noexcept;

			/// @brief Stops all connections and shuts them down
			void StopAll() noexcept;
		private:
			ConnectionSet_t m_connections;
		};
	}
}

#endif
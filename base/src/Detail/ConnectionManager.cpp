#include <__PROJNAME__/Detail/ConnectionManager.h>

using __PROJNAME__::Detail::ConnectionManager;

void ConnectionManager::Start(const ConnectionPtr_t& conn) noexcept
{
	m_connections.insert(conn);
	conn->Start();
}

void ConnectionManager::Stop(const ConnectionPtr_t& conn) noexcept
{
	conn->StopImpl();
	m_connections.erase(conn);
}

void ConnectionManager::StopAll() noexcept
{
	for (const ConnectionPtr_t& conn : m_connections)
		conn->StopImpl();
	m_connections.clear();
}
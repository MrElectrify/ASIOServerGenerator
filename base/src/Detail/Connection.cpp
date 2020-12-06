#include <__PROJNAME__/Detail/Connection.h>

#include <__PROJNAME__/Common.h>
#include <__PROJNAME__/Detail/ConnectionManager.h>

#include <utility>

using __PROJNAME__::Detail::Connection;

Connection::Connection(ConnectionManager& connectionManager,
	Socket_t socket) noexcept : m_connectionManager(connectionManager),
	m_remoteEndpoint(socket.remote_endpoint()), m_socket(std::move(socket)),
	m_incomingBuf() {}

void Connection::Start() noexcept
{
	Read();
}

void Connection::Stop() noexcept
{
	// ConnectionManager will actually stop the connection
	m_connectionManager.Stop(shared_from_this());
}

void Connection::StopImpl() noexcept
{
	std::error_code ignored;
	m_socket.close(ignored);
	SPDLOG_INFO("Shut down connection from {}:{}",
		m_remoteEndpoint.address().to_string(),
		m_remoteEndpoint.port());
}

void Connection::Read() noexcept
{
	m_socket.async_read_some(asio::buffer(m_incomingBuf),
		[this, self = shared_from_this()] (const std::error_code& ec, size_t numRecv)
		{
			if (!ec)
			{
				SPDLOG_DEBUG("Received {} bytes from {}:{}",
					numRecv, m_remoteEndpoint.address().to_string(),
					m_remoteEndpoint.port());
				// send them back
				SendPacket(m_incomingBuf.begin(), m_incomingBuf.begin() + numRecv);
				Read();
			}
			else if (ec == asio::error::eof)
			{
				SPDLOG_INFO("Connection at {}:{} disconnected gracefully",
					m_remoteEndpoint.address().to_string(),
					m_remoteEndpoint.port());
				Stop();
			}
			else if (ec != asio::error::operation_aborted)
			{
				SPDLOG_ERROR("Connection at {}:{} disconnected on read: {}",
					m_remoteEndpoint.address().to_string(),
					m_remoteEndpoint.port(), ec.message());
				Stop();
			}
		});
}

void Connection::Write() noexcept
{
	asio::async_write(m_socket, asio::buffer(m_outgoingQueue.front()),
		[this, self = shared_from_this()] (const std::error_code& ec, size_t numSent)
		{
			if (!ec)
			{
				SPDLOG_DEBUG("Sent {} bytes to {}:{}", numSent,
					m_remoteEndpoint.address().to_string(),
					m_remoteEndpoint.port());
				// pop off the front
				m_outgoingQueue.pop();
				// if there remain packets to be sent, send them
				if (m_outgoingQueue.empty() == false)
					Write();
			}
			else if (ec != asio::error::operation_aborted)
			{
				SPDLOG_ERROR("Connection at {}:{} disconnected on write: {}",
					m_remoteEndpoint.address().to_string(),
					m_remoteEndpoint.port(), ec.message());
			}
		});
}
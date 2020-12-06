#include <__PROJNAME__/Server.h>

#include <__PROJNAME__/Common.h>

using __PROJNAME__::Server;

Server::Server(const std::string& address, const std::string& port)
	: m_worker(), m_acceptor(m_worker), m_signals(m_worker)
{
	// setup spdlog to print all messages
	spdlog::set_level(spdlog::level::debug);
	SPDLOG_INFO("Server starting");
	// listen for signals
	m_signals.add(SIGINT);
	m_signals.add(SIGTERM);
	AwaitSignals();
	// resolve local address and listen
	Proto_t::resolver resolver(m_worker);
	std::error_code ec;
	const auto res = resolver.resolve(address, port, ec);
	if (ec)
		throw ec;
	// start the acceptor
	StartAcceptor(res->endpoint());
	Accept();
}

void Server::Run()
{
	m_worker.run();
}

void Server::AwaitSignals() noexcept
{
	m_signals.async_wait(
		[this] (const std::error_code& ec, int signo)
		{
			if (ec)
				return;
			SPDLOG_INFO("Intercepted signal {}. Shutting down", signo);
			m_acceptor.close();
			//m_connectionManager.StopAll();
		});
	SPDLOG_DEBUG("Awaiting signals");
}

void Server::StartAcceptor(const Proto_t::endpoint& endpoint)
{
	std::error_code ec;
	m_acceptor.open(Proto_t::v4(), ec);
	if (ec)
		throw ec;
	m_acceptor.bind(endpoint, ec);
	if (ec)
		throw ec;
	m_acceptor.listen(Proto_t::acceptor::max_connections, ec);
	if (ec)
		throw ec;
	SPDLOG_INFO("Listening on {}:{}",
		endpoint.address().to_string(), endpoint.port());
}

void Server::Accept() noexcept
{
	m_acceptor.async_accept(
		[this] (const std::error_code& ec, Proto_t::socket socket)
		{
			// check if it was closed
			if (m_acceptor.is_open() == false)
				return;
			if (!ec)
			{
				SPDLOG_INFO("Accepted connection from {}:{}",
					socket.remote_endpoint().address().to_string(),
					socket.remote_endpoint().port());
				// start the connection processing
				m_connectionManager.Start(
					std::make_shared<Detail::Connection>(
						m_connectionManager, std::move(socket)));
			}
			else
				SPDLOG_ERROR("Error accepting connection: {}",
					ec.message());
			Accept();
		});
}
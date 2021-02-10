#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

//porta default, pode ser configurada no config.txt
unsigned short int porta = 1234;

struct Config {
    int	porta;
};

void loadConfig(Config& config) {
    std::ifstream fin("config.txt");
    std::string line;
    while (getline(fin, line)) {
        std::istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("porta") != -1)
            sin >> config.porta;
    }
}


using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

//Usaremos shared_ptr e enable_shared_from_this porque queremos manter o objeto
//tcp_connection ativo enquanto houver uma operação que se refira a ele.
class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new tcp_connection(io_service));
  }

  tcp::socket& socket()
  {
    return socket_;
  }
	//Chamamos boost::asio::async_write() para servir os dados ao cliente.
  void start()
  {
		//Dados a serem enviados são armazenados no membro da classe message,
    message_ = make_daytime_string();
		//notas
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

private:
  tcp_connection(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
};


class tcp_server
{
public:
	//O construtor inicializa um acceptor para escutar na porta TCP 1234.
  tcp_server(boost::asio::io_service& io_service)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), porta))
  {
    start_accept();
  }

private:
	//A função cria um socket e inicia uma operação de aceitação assíncrona
	//para esperar nova conexão.
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(io_service_);

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }
//A função handle_accept() é chamada quando a operação start_accept() termina.
//Ele atende a solicitação do cliente e então chama start_accept () para iniciar nova operação de aceitação.
  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};

int main()
{
	//carrega config.txt
	Config config;
	loadConfig(config);
	porta = config.porta;
	std::cout << porta << '\n';
  try
  {
		//cria um objeto de servidor para aceitar conexões de clientes
		boost::asio::io_service io_service;
		tcp_server server(io_service);
		//Executa o objeto para que ele execute operações assíncronas
		io_service.run();
  }
  	catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

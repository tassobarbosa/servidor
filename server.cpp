//importing libraries
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;

//porta default, pode ser configurada no config.txt
unsigned short int porta = 1234;
int tamanho;
struct Config {
    int	porta;
    int tamanho;
};
void loadConfig(Config& config) {
    std::ifstream fin("config.txt");
    std::string line;
    while (getline(fin, line)) {
        std::istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("porta") != -1)
            sin >> config.porta;
        else if (line.find("tamanho") != -1)
            sin >> config.tamanho;
    }
}

//Cria nome para cada conexão baseado na hora
std::string make_daytime_string()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"teb_%Y%m%d%H%M%S.txt",timeinfo);
  return (buffer);
}


//Usaremos shared_ptr e enable_shared_from_this porque queremos manter o objeto
//tcp_connection ativo enquanto houver uma operação que se refira a ele.
class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
private:
  tcp::socket sock;
  std::string message="Hello From tcp_server!";
  enum { max_length = 1024 };
  char data[max_length];

public:
  typedef boost::shared_ptr<tcp_connection> pointer;
  tcp_connection(boost::asio::io_service& io_service): sock(io_service){}
  //criação de pointer
  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new tcp_connection(io_service));
  }
  //ciação do socket
  tcp::socket& socket()
  {
    return sock;
  }
  //Chamamos boost::asio::async_write() para servir os dados ao cliente.
  void start()
  {
    sock.async_read_some(
        boost::asio::buffer(data, max_length),
        boost::bind(&tcp_connection::handle_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

    sock.async_write_some(
        boost::asio::buffer(message, max_length),
        boost::bind(&tcp_connection::handle_write,
                  shared_from_this(),
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
  }

  void handle_read(const boost::system::error_code& err, size_t bytes_transferred)
  {
    if (!err) {

         cout << "conteudo: "<< data << endl;
         cout << "data: " << make_daytime_string() << endl;

          std::ofstream myfile;
          myfile.open (make_daytime_string().c_str());
          myfile << "Writing this to a file.\n";
          myfile.close();


    } else {
         std::cerr << "error: " << err.message() << std::endl;
         sock.close();
    }
  }
  void handle_write(const boost::system::error_code& err, size_t bytes_transferred)
  {
    if (!err) {
       cout << "tcp_server sent Hello message!"<< endl;
    } else {
       std::cerr << "error: " << err.message() << endl;
       sock.close();
    }
  }
};


class tcp_server
{
public:
//O construtor inicializa um acceptor para escutar na porta TCP.
  tcp_server(boost::asio::io_service& io_service): acceptor_(io_service, tcp::endpoint(tcp::v4(), porta))
  {
     start_accept();
  }
  void handle_accept(tcp_connection::pointer connection, const boost::system::error_code& err)
  {
    if (!err) {
      connection->start();
    }
    start_accept();
  }
private:
  //A função cria um socket e inicia uma operação de aceitação assíncrona
  //para esperar nova conexão.
   tcp::acceptor acceptor_;
   void start_accept()
   {
    // socket
     tcp_connection::pointer connection = tcp_connection::create(acceptor_.get_io_service());

    //accept operation assincrona e espera por nova conexao
     acceptor_.async_accept(connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, connection,
        boost::asio::placeholders::error));
  }
};

int main(int argc, char *argv[])
{
  //carrega config.txt
  Config config;
  loadConfig(config);
  porta = config.porta;
  tamanho = config.tamanho;
  std::cout << porta << '\n';
  std::cout << tamanho << '\n';
  try
    {
    boost::asio::io_service io_service;
    tcp_server tcp_server(io_service);
    io_service.run();
    }
  catch(std::exception& e)
    {
    std::cerr << e.what() << endl;
    }
  return 0;
}

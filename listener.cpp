//Sebastian Thiems 2013
//
//RT Customz
//www.42Volt.de

#include "listener.h"

Listener::Listener()
{	
	max_length = 1024;
	green_ = blue_ = red_ = 400;
	show_temp_ = false;
	toggle_points_ = false;
}

int Listener::getGreen()
{
	return green_;
}

int Listener::getBlue()
{
	return blue_;
}

int Listener::getRed()
{
	return red_;
}

int Listener::getTemp()
{
	return temp_;
}

bool Listener::showTemp()
{
	return show_temp_;
}

bool Listener::togglePoints()
{
	return toggle_points_;
}

void Listener::setShowTemp(bool state)
{
	show_temp_ = state;
}

void Listener::listen()
{

	udp::socket sock(io_service_, udp::endpoint(udp::v4(), 4242));

  	for (;;)
  	{
		char data[max_length];
   	 	udp::endpoint sender_endpoint;
   	 	size_t length = sock.receive_from(
    		boost::asio::buffer(data, max_length), sender_endpoint);

		std::string input(data);
		std::vector<std::string> s;
		boost::algorithm::split(s, input, boost::is_any_of(":"),boost::token_compress_on);

		/*for(int i = 0; i < s.size(); i++) 
			std::cout << s[i] << std::endl;*/
		
		if(s[0] == "Color")
		{
			green_	= atoi(s[1].c_str());
			blue_ 	= atoi(s[2].c_str());
			red_	= atoi(s[3].c_str());
		}

		if(s[0] == "Temp")
		{
			show_temp_ = true;
			temp_ = atoi(s[1].c_str());
		}

		if(s[0] == "TogglePoints")
		{
			toggle_points_ = atoi(s[1].c_str());
		}
  	}
}



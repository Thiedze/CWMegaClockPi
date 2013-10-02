//Sebastian Thiems 2013
//
//RT Customz
//www.42Volt.de

#include <iostream>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::udp;

class Listener
{
public: 
	Listener();
	int getGreen();
	int getBlue();
	int getRed();
	int getTemp();
	void setShowTemp(bool state);

	bool showTemp();
	bool togglePoints();

	void listen();

private:
	int green_;
	int blue_;
	int red_;
	int temp_;

	bool show_temp_;
	bool toggle_points_;

	boost::asio::io_service io_service_;
	int max_length;
};

//Sebastian Thiems 2013
//
//RT Customz
//www.42Volt.de

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <cmath>

#include "spi.h"
#include "listener.h"

int main(int argc, char *argv[])
{
	MYSPI myspi;
	Listener listener;	

	boost::thread* thr = new boost::thread(
				boost::bind(&Listener::listen, &listener));

	time_t rawtime;
	tm * timeinfo;
	char buffer[80];

	int temp_show_time = 100;  //in 50 ms steps

	while(1)
	{
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%X", timeinfo);
		
		myspi.setColor(	listener.getRed(), 
				listener.getGreen(), 
				listener.getBlue());

		if(listener.showTemp() == false)
		{
			for(int i = 7; i >=0; i--)
			{
				if(buffer[i] != ':' && listener.togglePoints() == false)
				{
					myspi.convertTo12Bit(buffer[i], true);
					myspi.transfer();
				}

				if(buffer[i] != ':' && listener.togglePoints() == true)
				{
					if(buffer[7] % 2 == 1 || buffer[7] == 0)
					{
						myspi.convertTo12Bit(buffer[i], true);
						myspi.transfer();
					}
					else
					{
						myspi.convertTo12Bit(buffer[i], false);
						myspi.transfer();
					}
				}
			}
		}
		else if(listener.showTemp() == true)
		{
			myspi.convertTo12Bit('<', false);	
			myspi.transfer();

			char buffer [33];
			int temp = std::abs(listener.getTemp());

			int e = temp % 10;			
			sprintf (buffer, "%d", e);
			myspi.convertTo12Bit(buffer[0], false);
			myspi.transfer();

			int z = (temp/10) % 10;
			sprintf (buffer, "%d", z);
			myspi.convertTo12Bit(buffer[0], false);
			myspi.transfer();

			if(listener.getTemp() < 0)
				myspi.convertTo12Bit('-', false);
			else
				myspi.convertTo12Bit('+', false);

			myspi.transfer();			

			temp_show_time--;
			
			if(temp_show_time < 0)
			{
				temp_show_time = 100;
				listener.setShowTemp(false);
			}
		
		}
	
		myspi.latch_blanck();
		delay(10);
	}

	close(myspi.getDeviceNumber());

	return 1;
}

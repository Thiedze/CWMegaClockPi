//Sebastian Thiems 2013
//
//RT Customz
//www.42Volt.de

#include <wiringPi.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <time.h>

//Reihenfolge 				/*Punkt*/ /*B*/ /*C*/ /*D*/   /*E*/  /*G*/   /*F*/  /*A*/               
#define zero_without_point (string[]) 	{ "aus", "an" , "an" , "an" , "an" , "aus", "an" ,  "an"  }
#define zero_with_point (string[]) 	{ "an" , "an" , "an" , "an" , "an" , "aus", "an" ,  "an"  }
#define one_without_point (string[]) 	{ "aus", "an" , "an" , "aus", "aus", "aus", "aus",  "aus" }
#define one_with_point (string[]) 	{ "an" , "an" , "an" , "aus", "aus", "aus", "aus",  "aus" }
#define two_without_point (string[]) 	{ "aus", "an" , "aus", "an" , "an" , "an" , "aus",  "an"  }
#define two_with_point (string[])  	{ "an" , "an" , "aus", "an" , "an" , "an" , "aus",  "an"  }
#define three_without_point (string[])  { "aus", "an" , "an" , "an" , "aus", "an" , "aus",  "an"  }
#define three_with_point (string[])  	{ "an" , "an" , "an" , "an" , "aus", "an" , "aus",  "an"  }
#define four_without_point (string[])   { "aus", "an" , "an" , "aus", "aus", "an" , "an" ,  "aus" }
#define four_with_point (string[])  	{ "an" , "an" , "an" , "aus", "aus", "an" , "an" ,  "aus" }
#define five_without_point (string[])	{ "aus", "aus", "an" , "an" , "aus", "an" , "an" ,  "an"  }
#define five_with_point (string[])  	{ "an" , "aus", "an" , "an" , "aus", "an" , "an" ,  "an"  }
#define six_without_point (string[]) 	{ "aus", "aus", "an" , "an" , "an" , "an" , "an" ,  "an"  }
#define six_with_point (string[])  	{ "an" , "aus", "an" , "an" , "an" , "an" , "an" ,  "an"  }
#define seven_without_point (string[])  { "aus", "an" , "an" , "aus", "aus", "aus", "aus",  "an"  }
#define seven_with_point (string[])  	{ "an" , "an" , "an" , "aus", "aus", "aus", "aus",  "an"  }
#define eight_without_point (string[])  { "aus", "an" , "an" , "an" , "an" , "an" , "an" ,  "an"  }
#define eight_with_point (string[])  	{ "an" , "an" , "an" , "an" , "an" , "an" , "an" ,  "an"  }
#define nine_without_point (string[])  	{ "aus", "an" , "an" , "an" , "aus", "an" , "an" ,  "an"  }
#define nine_with_point (string[])  	{ "an" , "an" , "an" , "an" , "aus", "an" , "an" ,  "an"  }
#define off (string[])  		{ "aus", "aus", "aus", "aus", "aus", "aus", "aus",  "aus" }
#define grad (string[])			{ "aus", "an", 	"aus", "aus", "aus", "an" , "an" ,  "an"  }
#define minus (string[])		{ "aus", "aus",	"aus", "aus", "aus", "an",  "aus",  "aus" }

#define MODE 		(uint8_t)	0			//SPI-Mode
#define BITS		(uint8_t)	8			//Bits per word
#define SPEED		(uint32_t)	500000			//Geschwindigkeit hier 50kHz
#define DELAY		(uint16_t)	0			//Default ist 0
#define DEVICENAME	(char [])	{"/dev/spidev0.0"}	//SPI-Devices auf Raspberry Pi von der Blacklist nehmen

#define LATCH		(int)		10			//GPIO 8 am Raspberry Pi
#define BLANCK		(int)		11			//GPIO 9 am Raspberry Pi

#define ARRAYSIZE 			36			//Größe des Array, welches über SPI versendet wird

#define PORTNUM 			4711			//Port Nummer auf die gehört werden soll


using namespace std;

class MYSPI
{
	public:
		void transfer();
		int getDeviceNumber(){return m_deviceNumber;}
		void convertTo12Bit(char sign, bool point);
		void latch_blanck();
		void close_spi();
		MYSPI();
		void setColor(int green, int blue, int red);

		int m_sockfd, m_newsockfd;
		socklen_t m_clilen;
		char m_buffer[256];
		struct sockaddr_in m_serv_addr, m_cli_addr;

	private:

		void doYourWork(char work[]);
		void initServer();

		uint8_t m_data[36];
		int m_deviceNumber; //Device Number nach dem open
		int m_green;
		int m_blue;
		int m_red;
		
		void initSPI();
		void initGpio();
		void setDeviceNumber(int deviceNumber);
		void setData(string *segments);				
};

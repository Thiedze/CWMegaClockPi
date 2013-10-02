//Sebastian Thiems 2013
//
//RT Customz
//www.42Volt.de

#include "spi.h"
#include <string.h>

using namespace std;

void MYSPI::latch_blanck()
{
	digitalWrite(BLANCK, LOW);
	digitalWrite(LATCH, HIGH);
        digitalWrite(LATCH, LOW);
	digitalWrite(BLANCK, HIGH);
}

void MYSPI::setColor(int green, int blue, int red)
{
	this->m_green = green;
	this->m_blue = blue;
	this->m_red = red;
}

void MYSPI::setData(string *segments)
{
        uint8_t transfer_m_data[36];

        for(int i = 0; i < 36; i++)
        {
                transfer_m_data[i] = 0x00;
		this->m_data[i] = 0x00; 
        }

	
	uint8_t nibbelArray[72];
	
	for(int i = 0; i < 72; i++)
	{
		nibbelArray[i] = 0x00;
	}

	//Nibbelweise die Farben schreiben.
	bool show = false;
	for(int segNibbel = 0; segNibbel < 72; segNibbel = segNibbel+9)
	{
		//Unterscheidung ob Segment angezeigt werden soll oder nicht  
		if(segments[segNibbel/9] == "an")
			show = true;
		else
			show = false;
		if(show == true)
		{
			nibbelArray[segNibbel+2] = this->m_blue & 0x0F;
			nibbelArray[segNibbel+1] = (this->m_blue >> 4) & 0x0F; 
			nibbelArray[segNibbel]	 = (this->m_blue >> 8) & 0x0F;

			nibbelArray[segNibbel+5] = this->m_green & 0x0F;
			nibbelArray[segNibbel+4] = (this->m_green >> 4) & 0x0F;
			nibbelArray[segNibbel+3] = (this->m_green >> 8) & 0x0F;

			nibbelArray[segNibbel+8] = this->m_red & 0x0F;
			nibbelArray[segNibbel+7] = (this->m_red >> 4) & 0x0F;
			nibbelArray[segNibbel+6] = (this->m_red >> 8) & 0x0F;
			
			show = false;
		}
	}
	
	//Aus zwei Nibblen ein Byte machen, da wir zum senden ein uint_8 Array mit 36 Elementen benötigen
	int index = 0;
	for(int segNibbel = 0; segNibbel < 72; segNibbel = segNibbel+2, index++)
	{
		this->m_data[index] = (nibbelArray[segNibbel] << 4) & 0xF0;
		this->m_data[index] |=  nibbelArray[segNibbel+1];
	}
}

void MYSPI::convertTo12Bit(char sign, bool point)
{
        switch(sign)
        {
                case '0':
                {       
			if(point == true)
                        	setData(zero_with_point);
			else
				setData(zero_without_point);
			break;
                }
                case '1':
                {       
                        if(point == true)
                        	setData(one_with_point);
			else
				setData(one_without_point);
			break;
                }
                case '2':
                {
			if(point == true)
                        	setData(two_with_point);
			else
				setData(two_without_point);
			break;
                }
                case '3':
                {
                       if(point == true)
                        	setData(three_with_point);
			else
				setData(three_without_point);
			break;
                }
                case '4':
                {
                       if(point == true)
                        	setData(four_with_point);
			else
				setData(four_without_point);
			break;
                }
                case '5':
                {
                      if(point == true)
                        	setData(five_with_point);
			else
				setData(five_without_point);
			break;  
                }
                case '6':
                {
                        if(point == true)
                        	setData(six_with_point);
			else
				setData(six_without_point);
			break;
                }
                case '7':
                {
                       if(point == true)
                        	setData(seven_with_point);
			else
				setData(seven_without_point);
			break;
                } 
		case '8':
                {
                     if(point == true)
                        	setData(eight_with_point);
			else
				setData(eight_without_point);
			break;   
                }
                case '9':         
                {
                        if(point == true)
                        	setData(nine_with_point);
			else
				setData(nine_without_point);
			break;
                }
		case '<':
		{
			setData(grad);
			break;
		}
		case '-':
		{
			setData(minus);
			break;
		}
                default:
		{
                        setData(off);
		}
        }
}

void MYSPI::transfer()
{
	int ret;
	
	//Dient zum Empfangen (Wird hier aber nicht witer beachtet/benutzt)
	uint8_t rx[ARRAYSIZE] = {0, };
	
	struct spi_ioc_transfer tr = {
		tr.tx_buf = (unsigned long)m_data,
		tr.rx_buf = (unsigned long)rx,
		tr.len = ARRAYSIZE,
		tr.speed_hz = SPEED,
		tr.bits_per_word = BITS,
		tr.cs_change = 1,
	};

	//Hier wird geschrieben
	ret = ioctl(this->m_deviceNumber, SPI_IOC_MESSAGE(1), &tr);
	
	if (ret < 1){
		cout << "can't send spi message" << endl; }
}




void MYSPI::setDeviceNumber(int deviceNumber)
{
	this->m_deviceNumber = deviceNumber;
}

MYSPI::MYSPI()
{

	for(int i = 0; i < 36; i++)
	{
		this->m_data[i] = 0x00;
	}
	
	setColor(0,0,0);
	initSPI();
	initGpio();
	initServer();
	
}

void MYSPI::initServer()
{
	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockfd < 0) 
		printf("ERROR opening socket");
	bzero((char *) &m_serv_addr, sizeof(m_serv_addr));
	//portno = atoi(argv[1]);

	m_serv_addr.sin_family = AF_INET;
	m_serv_addr.sin_addr.s_addr = INADDR_ANY;
	m_serv_addr.sin_port = htons(PORTNUM);

	if (bind(m_sockfd, (struct sockaddr *) &m_serv_addr, sizeof(m_serv_addr)) < 0) 
		printf("ERROR on binding");

	listen(m_sockfd,5);
	m_clilen = sizeof(m_cli_addr);
}


void MYSPI::initGpio()
{
	if (wiringPiSetup () == -1)
    		return ;

	pinMode(LATCH, OUTPUT); //Latch
	pinMode(BLANCK, OUTPUT); //Blanck
}

void MYSPI::initSPI()
{
	int ret = 0;
	
	int deviceNumber = open(DEVICENAME, O_RDWR);
	if (ret < 0){
		cout << "can't open device"<< endl;}

	ioctl(deviceNumber, SPI_IOC_WR_MODE, MODE);

	ioctl(deviceNumber, SPI_IOC_RD_MODE, MODE);

	ioctl(deviceNumber, SPI_IOC_WR_BITS_PER_WORD, BITS);

	ioctl(deviceNumber, SPI_IOC_RD_BITS_PER_WORD, BITS);

	ioctl(deviceNumber, SPI_IOC_WR_MAX_SPEED_HZ, SPEED);

	ioctl(deviceNumber, SPI_IOC_RD_MAX_SPEED_HZ, SPEED);

	setDeviceNumber(deviceNumber);
}



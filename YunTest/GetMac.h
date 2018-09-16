#pragma once

struct macaddr
{
	char macstring[50];
};

class CGetMac
{
public:
	CGetMac(void);
	~CGetMac(void);
	int getLocalMac(unsigned char * mac);
	vector<macaddr> macstring;
	std::string macip;
};


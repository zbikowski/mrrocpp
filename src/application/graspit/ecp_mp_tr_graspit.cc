#include <netdb.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <string.h>

#include "ecp_mp_tr_graspit.h"

using namespace std;

namespace mrrocpp {
namespace ecp_mp {
namespace transmitter {

TRGraspit::TRGraspit(TRANSMITTER_ENUM _transmitter_name, const char* _section_name, task::task& _ecp_mp_object):
	transmitter (_transmitter_name, _section_name, _ecp_mp_object){
}

TRGraspit::~TRGraspit(){

}

void TRGraspit::TRconnect(const char *host,unsigned short int serverPort){
	int socketDesc;
	struct sockaddr_in serverAddress;
	struct hostent *hostInfo;
	char c;

	hostInfo=gethostbyname(host);
	if (hostInfo==NULL) {
		cerr<<"problem interpreting host: "<<host<<"\n";
		throw ecp_mp::transmitter::transmitter::transmitter_error(lib::SYSTEM_ERROR,0);
	}

	cout<<"host ok\n";

	socketDesc = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDesc < 0) {
		cerr << "cannot create socket\n";
		throw ecp_mp::transmitter::transmitter::transmitter_error(lib::SYSTEM_ERROR,0);
	}

	cout<<"socket ok\n";

	serverAddress.sin_family = hostInfo->h_addrtype;
	std::memcpy((char *) &serverAddress.sin_addr.s_addr,hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(serverPort);

	if (connect(socketDesc,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) {
		cerr << "cannot connect\n";
		throw ecp_mp::transmitter::transmitter::transmitter_error(lib::SYSTEM_ERROR,0);
	}

	cout<<"connect ok\n";

	socketDescriptor=socketDesc;
}

void TRGraspit::TRdisconnect(){
	close(socketDescriptor);
}

bool TRGraspit::t_read(){
	if (recv(socketDescriptor, &from_va.graspit, sizeof(from_va.graspit), 0) < 0) {
		cerr << "didn't get response from server?";
		close(socketDescriptor);
		throw ecp_mp::transmitter::transmitter::transmitter_error(lib::SYSTEM_ERROR,0);
	}

	cout<<"read ok\n";

	return true;
}

} // namespace transmitter
} // namespace ecp_mp
} // namespace mrrocpp

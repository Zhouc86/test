// CMakeUDP.cpp: 定义应用程序的入口点。
//

#include "CMakeUDP.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string.h>
using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	int udp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_socket_fd == -1)
	{
		cout << "udp_socket_fd FAIL." << endl;
	}

	struct sockaddr_in  local_addr = { 0 };
	local_addr.sin_family = AF_INET; //使用IPv4协议
	local_addr.sin_port = htons(21000);   //网络通信都使用大端格式
	//local_addr.sin_addr.s_addr = INADDR_ANY;//让系统检测本地网卡，自动绑定本地IP
	local_addr.sin_addr.s_addr = inet_addr("192.168.1.110");//让系统检测本地网卡，自动绑定本地IP

	int ret = bind(udp_socket_fd, (struct sockaddr*)&local_addr, sizeof(local_addr));
	if (ret < 0)
	{
		cout << "bind  FAIL." << endl;
	}
	else
	{
		cout << "recv  ready." << endl;
	}

	struct pollfd poll_fd;
	poll_fd.fd = udp_socket_fd;
	poll_fd.events = POLLIN;

	struct sockaddr_in  src_addr = { 0 };  //用来存放对方(信息的发送方)的IP地址信息
	socklen_t  len = sizeof(src_addr);	//地址信息的大小
	char buf[1024] = { 0 };//消息缓冲区
	while (1)
	{
		int ret = poll(&poll_fd, 1, 1000);
		if (ret < 0)
		{
			cout << "poll  perror." << endl;
			continue;
		}
		if (ret == 0)
		{
			cout << "poll timeout" << endl;
			continue;
		}
		if (poll_fd.revents == POLLIN)
		{
			ret = recvfrom(udp_socket_fd, buf, sizeof(buf), 0, (struct sockaddr*)&src_addr, &len);
			if (ret == -1)
			{
				cout << "ret == -1." << endl;
			}
			printf("[%s:%d]", inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port));//打印消息发送方的ip与端口号
			printf("buf=%s\n", buf);
			memset(buf, 0, sizeof(buf));//清空存留消息
		}

	}

	return 0;
}

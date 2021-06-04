#include "pthread.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "fcntl.h"
#include "signal.h"
#include "sys/stat.h"
#include "sys/termios.h"
#include "sys/ioctl.h"
#include "sys/msg.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/soundcard.h"
#include "dirent.h"

#include <time.h>
#include <string.h>
#include <netdb.h>
#include <sys/mman.h>

#include "MyAll.h"



#define HTTP_PORT  6200

typedef void (*Http_MsgHandler_Callback)(int socket_fd, char *http_data);

typedef struct
{
	char*							command;
	Http_MsgHandler_Callback 			msghandler ;
}HTTP_DATA_TAG_MSG_TBL;

#define HTTP_CMD_NULL				""
#define HTTP_CMD_LOGIN				"GET / "
#define HTTP_CMD_LOGIN_SUBMIT		"GET /login?"
#define HTTP_CMD_LOGIN_ERR			"GET /log_err?"
#define HTTP_CMD_PAGE1				"GET /page1.htm"


void prc_cmd_login(int socket_fd, char *data);
void prc_cmd_login_submit(int socket_fd, char *data);
void prc_cmd_login_err(int socket_fd, char *data);
void prc_cmd_page1(int socket_fd, char *data);



HTTP_DATA_TAG_MSG_TBL http_data_tag_msg_tbl_init[]=
{
	{ HTTP_CMD_LOGIN, 		(Http_MsgHandler_Callback)prc_cmd_login },
	{ HTTP_CMD_LOGIN_SUBMIT, 	(Http_MsgHandler_Callback)prc_cmd_login_submit},
	{ HTTP_CMD_LOGIN_ERR, 	(Http_MsgHandler_Callback)prc_cmd_login_err},
	{ HTTP_CMD_PAGE1, 		(Http_MsgHandler_Callback)prc_cmd_page1},

	
	{ HTTP_CMD_NULL, NULL }
};

typedef struct http_receive_thread_s
{
	pthread_t pid;
	int connfd;
	
}HTTP_RECEIVE_THREAD_S;

const char frame_info[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<frameset frameborder=\"no\">\
		<frame name=\"m\" src=\"login.htm\" frameborder=\"no\" scrolling=\"no\" noresize=\"noresize\" marginhight=0>\
	</frameset>\
</html>";

const char login[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<center>\
		<h2>用户登录</h2>\
	</center>\
	<body>\
		<form method=\"get\" action=\"login\">\
			<table width=\"100%%\" border=\"0\">\
				<tr><td align=\"center\">Username<input type=\"text\" name=\"username\" /></td></tr>\
				<tr><td align=\"center\">Password<input type=\"password\" name=\"password\" /></td></tr>\
				<tr><td align=\"center\"><input type=\"submit\" value=\"登陆\" name=\"submit\" /></td></tr>\
			</table>\
		</form>\
	</body>\
</html>";

const char log_err[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body>\
		<center>\
			<form action=\"log_err\" method=\"get\">\
				<table align=\"center\" cellpadding=\"10\">\
					<tr bgcolor=\"#00FF33\">\
						<td align=\"center\">Error<br>Invalid User name or Password</td>\
					</tr>\
					<tr>\
						<td align=\"center\"><input type=\"submit\" name=\"back\" value=\"ok\"></td>\
					</tr>\
				</table>\
			</form>\
		</center>\
	</body>\
</html>";


const char page1[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<p align=\"center\">  设备ID:%d      </p>\
		<p align=\"center\">  版本号:%d.%d     </p>\
	</body>\
</html>";




					
//解析出name的原始信息，以及信息所在的位置以及长度
unsigned int find_name_from_recv_data(char *name, unsigned char *s, unsigned char *buffer, unsigned int *p_len, unsigned int buffer_size)
{
	unsigned  int pos = 0, len;
	int buf_len = 0;
	len = strlen(name);

	while (*s  && (*s != 0x0a) && (*s != 0x0d))//不比较换行后的字符串
	{
		if ((*s == *name) && !memcmp(name, s, len))
		{
			s += len;
			pos += len;

			if (*s == '=')
			{
				s++;
				pos++;

				while ((*s != '&') && (*s) && (*s != 0x0a) && (*s != 0x0d))
				{
					*buffer = *s;
					buffer ++;
					s ++;
					pos ++;
					buf_len ++;

					if (buf_len + 1 >= buffer_size)
					{
						break;
					}
				}
				*buffer = '\0';
				*p_len = buf_len;
				return pos;
			}
		}

		pos++;
		s++;
	}

	return 0;
}

void *http_receive_thread(void *param)
{
	HTTP_RECEIVE_THREAD_S *my_param = (HTTP_RECEIVE_THREAD_S*) param;
	struct sockaddr_in remoteaddr;
	int socklen = sizeof(struct sockaddr_in);
	while(1)
	{
		char data[3000];
		int reciveLength = recvfrom(my_param->connfd, data, 1500, 0, (struct sockaddr *)&remoteaddr, (socklen_t*)&socklen);
		if (reciveLength>0)
		{
			data[reciveLength] = '\0';
			printf("RECEIVE: connfd=%d, length=%d\n%s\n", my_param->connfd, reciveLength, data);
			int i = 0;
			
//			while (http_data_tag_msg_tbl_init[i].command != HTTP_CMD_NULL)
			while(strncmp((http_data_tag_msg_tbl_init[i].command ), HTTP_CMD_NULL, strlen(HTTP_CMD_NULL)) == 0)
			{
				char cmd[100];
				int cmd_len;
				sprintf(cmd, "%s", http_data_tag_msg_tbl_init[i].command);
				cmd_len = strlen(http_data_tag_msg_tbl_init[i].command);
				if (strncmp(data, cmd, cmd_len) == 0)
				{
					if(http_data_tag_msg_tbl_init[i].msghandler != NULL)
					{
						http_data_tag_msg_tbl_init[i].msghandler(my_param->connfd, data);
						break;
					}
				}
				i++;
			}
			close(my_param->connfd);
			printf("close connect %d\n", my_param->connfd);
			return NULL;
		}
		else
		{
			close(my_param->connfd);
			printf("close connect %d\n", my_param->connfd);
			return NULL;
		}
	}
	return NULL;
}

void* http_accept_thread(void *param)
{
	struct sockaddr_in server_addr;
	int optval = 1;
	int listenfd, connfd;
	if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		return NULL;
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(HTTP_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("fail to bind %d\n", HTTP_PORT);
		return NULL;
	}
	listen(listenfd, 5);
	while ( 1 )
	{
		int sin_size = sizeof(struct sockaddr_in);  
		struct sockaddr_in client_addr;   
		connfd = accept(listenfd, (struct sockaddr *)&client_addr, (socklen_t *)&sin_size);
		if (connfd < 0)
		{
			printf("fail to accept \n");
			continue;
		}
		printf("accept radeay  %d\n", connfd);
		HTTP_RECEIVE_THREAD_S my_http_receive_thread_s;
		my_http_receive_thread_s.connfd = connfd;
		pthread_create(&my_http_receive_thread_s.pid, NULL, http_receive_thread, &my_http_receive_thread_s);
	}

	return NULL;
}


void http_init()
{
	static pthread_t http_accept_thread_id;
	pthread_create(&http_accept_thread_id, NULL, http_accept_thread, NULL);

	printf("Httpd started. \n");
	
}


void prc_cmd_login(int socket_fd, char *data)
{
	printf("GET / ----connfd=%d\n", socket_fd);
	send(socket_fd, login, strlen(login), 0);
	printf("SEND: connfd=%d\n%s\n", socket_fd, login);
}

void prc_cmd_login_submit(int socket_fd, char *data)
{
	printf("GET /login?----connfd=%d\n", socket_fd);
	unsigned char user_name[100], passwd[100];
	unsigned int  name_len, passwd_len;
	unsigned int st = strlen("GET /login?");
	if (find_name_from_recv_data("username", (unsigned char*)(&data[st]), user_name, &name_len, sizeof(user_name)) != 0)
	{
		find_name_from_recv_data("password", (unsigned char*)(&data[st]), passwd, &passwd_len, sizeof(passwd));
		printf("user_name=%s, name_len=%d, passwd=%s, passwd_len=%d\n", user_name, name_len, passwd, passwd_len);
		if (name_len==4 && memcmp(user_name, "root", name_len)==0 && passwd_len==10 && memcmp(passwd, "beihairoot", passwd_len)==0)
		{
			send(socket_fd, page1, strlen(page1), 0);
			printf("SEND: connfd=%d\n%s\n", socket_fd, page1);
		}
		else
		{
			send(socket_fd, log_err, strlen(log_err), 0);
			printf("SEND: connfd=%d\n%s\n", socket_fd, log_err);
		}
	}
	else
	{
		send(socket_fd, log_err, strlen(log_err), 0);
		printf("SEND: connfd=%d\n%s\n", socket_fd, log_err);
	}
}

void prc_cmd_login_err(int socket_fd, char *data)
{
	printf("GET /log_err?----connfd=%d\n", socket_fd);
	send(socket_fd, login, strlen(login), 0);
	printf("SEND: connfd=%d\n%s\n\n", socket_fd, login);
}

void prc_cmd_page1(int socket_fd, char *data)
{
	char page[1024];

	printf("GET /page1.htm----connfd=%d\n", socket_fd);

//	sprintf(page, page1, DevId, AppVersion, AppVersion2);

	sprintf(page, page1, 10, 1, 12);
	
	send(socket_fd, page, strlen(page), 0);
	printf("SEND: connfd=%d\n%s\n", socket_fd, page);
}




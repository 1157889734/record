
//  *****************************************************//
//      MyHttpd.c
//
//  *****************************************************//



#include "MyAll.h"



#define HTTPD_PORT  80



typedef struct 
{
	pthread_t thread_id;
	int connfd;

}HttpdConnInfoTypeDef;




pthread_t HttpdListenThreadId;



const char PageLogin[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<center>\
		<h2>用户登录</h2>\
	</center>\
	<body>\
		<form method=\"get\" action=\"login_submit\">\
			<table width=\"100%%\" border=\"0\">\
				<tr><td align=\"center\">Username<input type=\"text\" name=\"username\" /></td></tr>\
				<tr><td align=\"center\">Password<input type=\"password\" name=\"password\" /></td></tr>\
				<tr><td align=\"center\"><input type=\"submit\" value=\"登陆\" name=\"submit\" /></td></tr>\
			</table>\
		</form>\
	</body>\
</html>";


const char Page1[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<p align=\"center\">  设备ID:%d      </p>\
		<p align=\"center\">  版本号:%d.%d     </p>\
	</body>\
</html>";


const char PageLogErr[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body>\
		<center>\
			<form action=\"log_err_submit\" method=\"get\">\
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

const char PageFrameMain[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<frameset cols=\"15%,85%\">\
		<frame src=\"frame_index.htm\">\
		<frame src=\"frame1.htm\" name=\"show\">\
	</frameset>\
</html>";

const char PageFrameIndex[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<p align=\"right\">  <a href=\"frame1.htm\" target=\"show\">基本信息</a>     </p>\
		<p align=\"right\">  <a href=\"frame2.htm\" target=\"show\">软件升级</a>     </p>\
	</body>\
</html>";

const char PageFrame1[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<p align=\"center\">  设备类型: 录音模块      </p>\
		<p align=\"center\">  设备ID:%d      </p>\
		<p align=\"center\">  版本号:%d.%d     </p>\
	</body>\
</html>";

const char PageFrame2[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<p align=\"center\">  暂时不支持    </p>\
	</body>\
</html>";






void *HttpdListenThread(void *param);
void *HttpdReceiveThread(void *param);

void SendPageLogin(int socket_fd);
void SendPage1(int socket_fd);
void SendPageLogErr(int socket_fd);
void SendPageFrameMain(int socket_fd);
void SendPageFrameIndex(int socket_fd);
void SendPageFrame1(int socket_fd);
void SendPageFrame2(int socket_fd);

void RcvLogin(int socket_fd, u8 *inda);

int GetInputValue(char *name, u8 *inda, u8 *value, int *p_value_len, int value_size);




void Httpd_Init()
{
	pthread_create(&HttpdListenThreadId, NULL, HttpdListenThread, NULL);

	printf("Httpd started. \n");
	
}





void *HttpdListenThread(void *param)
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
	server_addr.sin_port = htons(HTTPD_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("fail to bind %d\n", HTTPD_PORT);
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
		
		HttpdConnInfoTypeDef conn_info;
		conn_info.connfd = connfd;
		pthread_create(&conn_info.thread_id, NULL, HttpdReceiveThread, &conn_info);

	}

	return NULL;
}



void *HttpdReceiveThread(void *param)
{
	HttpdConnInfoTypeDef *my_param = (HttpdConnInfoTypeDef*) param;
	struct sockaddr_in remoteaddr;
	int socklen = sizeof(struct sockaddr_in);

	
	while(1)
	{
		char inda[3000];
		
		int reciveLength = recvfrom(my_param->connfd, inda, 1500, 0, (struct sockaddr *)&remoteaddr, (socklen_t*)&socklen);
		if (reciveLength>0)
		{
			inda[reciveLength] = '\0';
			printf("RECEIVE: connfd=%d, length=%d\n%s\n", my_param->connfd, reciveLength, inda);

			//process HTTP requests
			if ((strncmp(inda, "GET / ", strlen( "GET / ")) ==0))	//index page
			{
     				SendPageLogin(my_param->connfd);
        		}     
        		else if ((strncmp(inda, "GET /login_submit?", strlen( "GET /login_submit?")) ==0))
			{
				RcvLogin(my_param->connfd, (u8 *)inda);
        		}   
			else if ((strncmp(inda, "GET /log_err_submit?", strlen( "GET /log_err_submit?")) ==0))
			{
				SendPageLogin(my_param->connfd);
        		}   
			else if ((strncmp(inda, "GET /frame_index.htm", strlen( "GET /frame_index.htm")) ==0))
			{
				SendPageFrameIndex(my_param->connfd);
        		}   
			else if ((strncmp(inda, "GET /frame1.htm", strlen( "GET /frame1.htm")) ==0))
			{
				SendPageFrame1(my_param->connfd);
        		}   
			else if ((strncmp(inda, "GET /frame_index.htm", strlen( "GET /frame1.htm")) ==0))
			{
				SendPageFrame2(my_param->connfd);
        		}   
			else
			{
				printf("HTTP request other.\n");
					
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





void SendPageLogin(int socket_fd)
{
	send(socket_fd, PageLogin, strlen(PageLogin), 0);	
	printf("HttpdSendPageLogin: connfd=%d\n\n", socket_fd);
	
}




void SendPage1(int socket_fd)
{
	char page[1024];

	sprintf(page, Page1, DevId, AppVersion, AppVersion2);

//	sprintf(page, Page1, 10, 1, 12);
	
	send(socket_fd, page, strlen(page), 0);
	
//	printf("SendPage1: connfd=%d\n%s\n", socket_fd, page);
	printf("SendPage1: connfd=%d\n\n", socket_fd);

}

void SendPageLogErr(int socket_fd)
{
	send(socket_fd, PageLogErr, strlen(PageLogErr), 0);	
	printf("SendPageLogErr: connfd=%d\n\n", socket_fd);
	
}

void SendPageFrameMain(int socket_fd)
{
	send(socket_fd, PageFrameMain, strlen(PageFrameMain), 0);	
	printf("SendPageFrameMain: connfd=%d\n\n", socket_fd);
	
}

void SendPageFrameIndex(int socket_fd)
{
	send(socket_fd, PageFrameIndex, strlen(PageFrameIndex), 0);	
	printf("SendPageFrameIndex: connfd=%d\n\n", socket_fd);
	
}

void SendPageFrame1(int socket_fd)
{
	char page[1024];

	sprintf(page, PageFrame1, DevId, AppVersion, AppVersion2);
//	sprintf(page, PageFrame1, 10, 1, 12);
	
	send(socket_fd, page, strlen(page), 0);

	printf("SendPageFrame1: connfd=%d\n\n", socket_fd);
	
}

void SendPageFrame2(int socket_fd)
{
	send(socket_fd, PageFrame2, strlen(PageFrame2), 0);	
	printf("SendPageFrame2: connfd=%d\n\n", socket_fd);
	
}

void RcvLogin(int socket_fd, u8 *inda)
{
	u8 username[32];
	u8 passwd[32];
	int username_len;
	int passwd_len;

	if (GetInputValue("username", inda, username, &username_len, sizeof(username)) > 0)
	{
		GetInputValue("password", inda, passwd, &passwd_len, sizeof(passwd));
		
		printf("username=%s, name_len=%d, passwd=%s, passwd_len=%d\n", username, username_len, passwd, passwd_len);

		if ((memcmp(username, "root", strlen("root")) == 0) && (memcmp(passwd, "beihairoot", strlen("beihairoot"))==0))
//		if (memcmp(username, "admin", strlen("admin")) == 0)
		{
//			SendPageFrameMain(socket_fd);
			SendPage1(socket_fd);
		}
		else
		{
			SendPageLogErr(socket_fd);
		}

	}
	else
	{
		SendPageLogErr(socket_fd);
	}

//	printf("RcvLogin: connfd=%d\n\n", socket_fd);
	
}



// Get the user input value form http request data 
int GetInputValue(char *name, u8 *inda, u8 *value, int *p_value_len, int value_size)
{
	int pos;
	int name_len;
	int value_len;

	pos = 0;
	name_len = strlen(name);
	value_len = 0;
	
	while ((*inda != 0)  && (*inda != 0x0a) && (*inda != 0x0d))
	{
		if ((*inda == *name) && (memcmp(name, inda, name_len) == 0))
		{
			inda += name_len;	
			pos += name_len;
			if (*inda == '=')
			{
				inda++;
				pos++;
				while ((*inda != '&') && (*inda != 0) && (*inda != 0x0a) && (*inda != 0x0d))
				{
					*value= *inda;
					value++;
					inda ++;
					pos ++;
					value_len++;

					if (value_len + 1 >= value_size)
					{
						break;
					}
				}
				*value= '\0';
				*p_value_len = value_len;
				return pos;
			}
		}
		pos++;
		inda++;
	}

	return -1;

}









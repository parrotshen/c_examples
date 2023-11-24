#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>


char g_buf[2048];

int main(int argc, char *argv[])
{
    char *name = "www.google.com";
    char *port = "443";

    struct hostent *host;
    struct sockaddr_in servAddr;
    socklen_t servAddrLen;
    const char *ip;
    int fd = -1;

    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    char *req;
    int len;


    if (argc > 2)
    {
        name = argv[1];
        port = argv[2];
    }
    else if (argc > 1)
    {
        name = argv[1];
    }
    else
    {
        printf("Usage: https HOST [PORT]\n\n");
        return -1;
    }

    host = gethostbyname( name );
    if (NULL == host)
    {
        perror( "gethostbyname" );
        goto _EXIT;
    }
    ip = inet_ntop(host->h_addrtype, host->h_addr_list[0], g_buf, (sizeof(g_buf) - 1));

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror( "socket" );
        goto _EXIT;
    }

    servAddrLen = sizeof(struct sockaddr_in);
    memset(&servAddr, 0x00, servAddrLen);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port   = htons( atoi(port) );
    servAddr.sin_addr.s_addr = inet_addr( ip );

    if (connect(fd, (struct sockaddr *)&servAddr, servAddrLen) < 0)
    {
        perror( "connect" );
        goto _EXIT;
    }

    printf("Establish HTTPS connection %s:%s\n\n", ip, port);


    SSL_library_init();
    SSL_load_error_strings(); 

    ctx = SSL_CTX_new( TLS_client_method() );
    if (NULL == ctx)
    {
        printf("ERR: SSL_CTX_new ... failed\n");
        goto _EXIT;
    }

    ssl = SSL_new( ctx );
    if (NULL == ssl)
    {
        printf("ERR: SSL_new ... failed\n");
        goto _EXIT;
    }

    if ( !SSL_set_tlsext_host_name(ssl, name) )
    {
        printf("ERR: SSL_set_tlsext_host_name ... failed\n");
        goto _EXIT;
    }

    SSL_set_fd(ssl, fd);

    if (-1 == SSL_connect( ssl ))
    {
        printf("ERR: SSL_connect ... failed\n");
        goto _EXIT;
    }

    printf("SSL/TLS using %s\n", SSL_get_cipher( ssl ));


    req = g_buf;
    req += sprintf(req, "GET / HTTP/1.1\r\n");
    req += sprintf(req, "Host: %s:%s\r\n", name, port);
    req += sprintf(req, "Connection: close\r\n");
    req += sprintf(req, "User-Agent: https_simple\r\n");
    req += sprintf(req, "\r\n");

    SSL_write(ssl, g_buf, strlen(g_buf));
    printf("Sent headers:\n%s\n", g_buf);

    memset(g_buf, 0, sizeof(g_buf));
    len = SSL_read(ssl, g_buf, sizeof(g_buf));
    if (len < 1)
    {
        printf("Connection closed by peer\n");
        goto _EXIT;
    }
    printf("Received (%d bytes):\n%.*s\n", len, len, g_buf);


_EXIT:
    if ( ssl )
    {
        SSL_shutdown( ssl );
        SSL_free( ssl );
        ssl = NULL;
    }
    if ( ctx )
    {
        SSL_CTX_free( ctx );
        ctx = NULL;
    }
    if (fd > 0)
    {
        close( fd );
        fd = -1;
    }
    return 0;
}


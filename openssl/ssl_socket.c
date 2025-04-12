#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void sslServer(int port)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;

    struct sockaddr_in addr;
    socklen_t addrLen;
    int fd_server;
    int fd_client;
    int option;

    unsigned char buf[4096];
    int len;


    method = TLS_server_method();
    if ( !method )
    {
        perror( "TLS_server_method" );
        return;
    }

    ctx = SSL_CTX_new( method );
    if ( !ctx )
    {
        perror( "SSL_CTX_new" );
        return;
    }

    if (SSL_CTX_use_certificate_chain_file(ctx, "ssl_cert.pem") <= 0)
    {
        perror( "SSL_CTX_use_certificate_chain_file" );
        SSL_CTX_free( ctx );
        return;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "ssl_key.pem", SSL_FILETYPE_PEM) <= 0)
    {
        perror( "SSL_CTX_use_PrivateKey_file" );
        SSL_CTX_free( ctx );
        return;
    }

    if ((fd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror( "socket" );
        SSL_CTX_free( ctx );
        return;
    }

    option = 1;
    if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
    {
        perror( "setsockopt" );
        close( fd_server );
        SSL_CTX_free( ctx );
        return;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons( port );
    addr.sin_addr.s_addr = htonl( INADDR_ANY );
    addrLen = sizeof( struct sockaddr_in );
    if (bind(fd_server, (struct sockaddr *)&addr, addrLen) < 0)
    {
        perror( "bind" );
        close( fd_server );
        SSL_CTX_free( ctx );
        return;
    }

    if (listen(fd_server, 1) < 0)
    {
        perror( "listen" );
        close( fd_server );
        SSL_CTX_free( ctx );
        return;
    }

    printf("SSL server: listen on port %u\n", port);

    for (;;)
    {
        fd_client = accept(
                        fd_server,
                        (struct sockaddr *)&addr,
                        &addrLen
                    );
        if (fd_client < 0)
        {
            perror( "accept" );
            continue;
        }

        ssl = SSL_new( ctx );
        if ( !SSL_set_fd(ssl, fd_client) )
        {
            perror( "SSL_set_fd" );
            SSL_free( ssl );
            close( fd_client );
            continue;
        }

        if (SSL_accept( ssl ) <= 0)
        {
            perror( "SSL_accept" );
            SSL_free( ssl );
            close( fd_client );
            continue;
        }

        printf("SSL server: client connected\n");

        for (;;)
        {
            len = SSL_read(ssl, buf, 4095);
            if (len < 0)
            {
                perror( "SSL_read" );
                break;
            }
            else if (len == 0)
            {
                printf("SSL server: client closed\n");
                break;
            }

            buf[len] = 0;
            printf("SSL server:\n%s\n", (char *)buf);

            if (SSL_write(ssl, buf, len) <= 0)
            {
                perror( "SSL_write" );
            }
        }

        SSL_shutdown( ssl );
        SSL_free( ssl );
        close( fd_client );
    }

    close( fd_server );
    SSL_CTX_free( ctx );
    printf("SSL server: exit\n");
}

void sslClient(char *ipaddr, int port)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    int err;

    struct sockaddr_in addr;
    socklen_t addrLen;
    int fd_client;

    time_t now;
    unsigned char buf[4096];
    int len;


    method = TLS_client_method();
    if ( !method )
    {
        perror( "TLS_client_method" );
        return;
    }

    ctx = SSL_CTX_new( method );
    if ( !ctx )
    {
        perror( "SSL_CTX_new" );
        return;
    }

    #if 0
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    if ( !SSL_CTX_load_verify_locations(ctx, "ssl_cert.pem", NULL))
    {
        perror( "SSL_CTX_load_verify_locations" );
        SSL_CTX_free( ctx );
        return;
    }
    #endif

    if ((fd_client = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror( "socket" );
        SSL_CTX_free( ctx );
        return;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons( port );
    addr.sin_addr.s_addr = inet_addr( ipaddr );
    addrLen = sizeof( struct sockaddr_in );
    if (connect(fd_client, (struct sockaddr *)&addr, addrLen) < 0)
    {
        perror( "connect" );
        close( fd_client );
        SSL_CTX_free( ctx );
        return;
    }

    printf("SSL client: socket connection establish\n");

    ssl = SSL_new( ctx );
    if ( !SSL_set_fd(ssl, fd_client) )
    {
        perror( "SSL_set_fd" );
        close( fd_client );
        SSL_CTX_free( ctx );
        return;
    }

    err = SSL_connect( ssl );
    if (err != 1)
    {
        perror( "SSL_connect" );
        close( fd_client );
        SSL_CTX_free( ctx );
        return;
    }

    printf("SSL client: SSL connection establish\n");

    now = time( NULL );
    sprintf((char *)buf, "%s", ctime( &now ));
    if (SSL_write(ssl, buf, strlen((char *)buf)) <= 0)
    {
        perror( "SSL_write" );
        close( fd_client );
        SSL_CTX_free( ctx );
        return;
    }

    len = SSL_read(ssl, buf, 4095);
    if (len <= 0)
    {
        perror( "SSL_read" );
        close( fd_client );
        SSL_CTX_free( ctx );
        return;
    }

    buf[len] = 0;
    printf("SSL client:\n%s\n", (char *)buf);

    close( fd_client );
    SSL_CTX_free( ctx );
    printf("SSL client: exit\n");
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        char *ipaddr = "127.0.0.1";
        int port = 58168;

        if (0 == strcmp("-s", argv[1]))
        {
            if (argc > 2) port = atoi( argv[2] );
            sslServer( port );
            return 0;
        }
        else if (0 == strcmp("-c", argv[1]))
        {
            if (argc > 2) ipaddr = argv[2];
            if (argc > 3) port = atoi( argv[3] );
            sslClient(ipaddr, port);
            return 0;
        }
    }

    printf("Usage: ssl_socket -s [PORT]\n");
    printf("     : ssl_socket -c [IPADDR [PORT]]\n");
    printf("\n");
    return 0;
}


#include <stdio.h>
#include <stdlib.h>

#include <uv.h>

uv_loop_t* loop;
sockaddr_in addr;

int counter = 0;

typedef struct {
	uv_write_t req;
	uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t* req)
{
	write_req_t* p = (write_req_t*)req;
	free(p->buf.base);
	p->buf.len = 0;
	free(p);
}
void alloc_buff(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void write_cb(uv_write_t* req, int status)
{
	if (status)
		fprintf(stderr, "Write error %s\n", uv_strerror(status));
	free_write_req(req);
}

void read_cb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
{
	if (nread > 0)
	{
		write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
		req->buf = uv_buf_init(buf->base, nread);
		uv_write((uv_write_t*)req, client, &req->buf, 1, write_cb);
		return;
	}

	if (nread < 0)
	{
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*)client, NULL);
	}
	free(buf->base);
}

void on_new_connection(uv_stream_t* server, int status)
{
	if (status < 0)
	{
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
		return;
	}

	uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (uv_accept(server, (uv_stream_t*)client) == 0)
	{
		uv_read_start((uv_stream_t*)client, alloc_buff, read_cb);
	}
	else
	{
		uv_close((uv_handle_t*)client, NULL);
	}
}

void idle_cb(uv_idle_t* handle)
{
	counter++;
}

void timer_cb(uv_timer_t* handle)
{
	printf("FPS...%d\n", counter);
	counter = 0;
}

int main()
{
	loop = uv_default_loop();

	//uv_idle_t idler;
	//uv_idle_init(loop, &idler);
	//uv_idle_start(&idler, idle_cb);

	uv_timer_t timer;
	uv_timer_init(loop, &timer);
	uv_timer_start(&timer, timer_cb, 0, 1000);

	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	uv_ip4_addr("0.0.0.0", 9999, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*) & addr, 0);

	int r = uv_listen((uv_stream_t*)& server, 10, on_new_connection);
	if (r)
	{
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}
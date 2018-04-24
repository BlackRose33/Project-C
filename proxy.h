#ifndef PROXY_H
#define PROXY_H
#include "mysocket.h"
#include "aes-test.h"


class CProxy{
    private:
	ushort _stage;
	ushort _num_nodes;
	ulong _num_hops;
	CSocket _mysock;
	int _cc_seq;
	int _tun_fd;
	int _sock_fd;
	int _num_flow;

	char _logfn[MAX_FN_LEN];
	struct router_info rinfo[MAX_ROUTER_COUNT];
	struct flow pflow[MAX_CONNECTIONS];
	int path[MAX_ROUTER_COUNT];
	unsigned char aes_key[KEY_LEN];
	unsigned long _old_src;

    public:
	CProxy();
	~CProxy();

	void print_buf_hex(char* buf, int buf_len, int port);
	
	bool initialize_configure(char* config_fn);
	bool get_all_interface_info();
	bool initialize_socket();
	void accept_connection();
	void output_log(char* out_str);
	bool fork_router();
	void run();

	int send_data_UDP(const char* send_buf, const int len, struct sockaddr_in & ser_addr);
	int recv_data_UDP(char* recv_buf, struct sockaddr_in & si_other);

	bool initialize_tun(char* tun_fn, const int flags);
	int read_data_TUN(char* recv_buf, int fd, int n);
	int write_data_TUN(char* send_buf, int fd, int n);

	bool collect_router_info();
	void update_router_addr(int index, int pid, struct sockaddr_in si_other);
	
	void generate_random_path();
	bool is_dup_hop(int hop);

	void generate_random_key(unsigned char key[], int len);
	void set_router_key(int index);
	bool create_circuit();
	int construct_circuit_ext_msg(char* buf, int len, unsigned short cID, unsigned short nport);
	int construct_deffie_hellman_msg(char* buf, int len, unsigned short cID, unsigned char key[]);

	int construct_deffie_hellman_padding_msg(char* buf, int len, unsigned short cID, unsigned char key[], int keylen);
	int construct_encrypted_circuit_ext_msg_padding(char* buf, int len, unsigned short cID, unsigned char nport[], int port_len);

	void handle_tun_tcp_traffic(char* buf, int len);
	void handle_router_tcp_traffic(char* buf, int len);

	void handle_tun_icmp_traffic(char* buf, int len);
	void handle_router_icmp_traffic(char* buf, int len, struct sockaddr_in si_other);
	void handle_relay_msg(char* buf, int len, struct sockaddr_in si_other);

	void encrypt_multiround_with_padding(char* ctext , int inlen, char**  etext, int* outlen,  int round);
	void decrypt_multiround_with_padding(char* ctext , int inlen, char**  etext, int* outlen,  int round);

};

#endif

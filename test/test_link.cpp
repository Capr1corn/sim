#include <stdio.h>
#include <stdlib.h>
#include "sim/sim.h"
#include "sim/log.h"

int main(int argc, char **argv){
	sim::Link *serv = sim::Link::listen("127.0.0.1", 8800);
	if(!serv){
		log_fatal("");
		exit(0);
	}
	while(1){
		sim::Link *link = serv->accept();
		if(!link){
			log_fatal("");
			exit(0);
		}
		log_debug("accept link from %s:%d", link->remote_ip, link->remote_port);
	
		while(1){
			int ret;
			ret = link->read();
			if(ret == 0){
				delete link;
				log_error("link closed.");
				break;
			}
			while(1){
				sim::Message msg;
				ret = link->recv(&msg);
				if(ret == -1){
					delete link;
					log_error("parse error!");
					break;
				}else if(ret == 0){
					break;
				}
				log_debug("%s", msg.encode().c_str());
			}
		}
	}
	
	return 0;
}

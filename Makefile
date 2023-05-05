# MakeFile for restChat
# server using C++ Microservice
# sudo mkdir /var/www/html/restChat
# sudo chown ubuntu /var/www/html/restChat

all: PutHTML restChat

PutHTML:
	cp restChat.html /var/www/html/chat1/
	cp restChat.css /var/www/html/chat1/
	cp restChat.js /var/www/html/chat1/
	
	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/chat1/

restChat : restChat.cpp httplib.h
	$(CXX) -o restChat $(CXXFLAGS) restChat.cpp $(OPENSSL_SUPPORT) $(ZLIB_SUPPORT) $(BROTLI_SUPPORT) 

clean:
	rm restChat *.o

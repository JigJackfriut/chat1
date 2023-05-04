//
//  namesAPI.cc - a microservice demo program
//
// James Skon
// Kenyon College, 2022
//

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include "httplib.h"


using namespace httplib;
using namespace std;
map<string, string> userCredentials;

const int port = 5005;

void addMessage(string username, string message, map<string,vector<string>> &messageMap) {
	/* iterate through users adding message to each */
	string jsonMessage = "{\"user\":\""+username+"\",\"message\":\""+message+"\"}";
	for (auto userMessagePair : messageMap) {
		username = userMessagePair.first;
		messageMap[username].push_back(jsonMessage);
	}
}

// Return messages for a certain user
string getMessagesJSON(string username, map<string,vector<string>> &messageMap) {
	/* retrieve json list of messages for this user */
	bool first = true;
	string result = "{\"messages\":[";
	for (string message :  messageMap[username]) {
		if (not first) result += ",";
		result += message;
		first = false;
	}
	result += "]}";
	messageMap[username].clear();
	return result;
}






int main(void) {
  Server svr;
  int nextUser=0;
  map<string,vector<string>> messageMap;
  vector<string> userList;
	
  /* "/" just returnsAPI name */
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("Chat API", "text/plain");
  });

svr.Get(R"(/chat/register/(.*)/(.*)/(.*))", [&](const Request& req, Response& res) {
    // Extract the name, email, and password from the request URL
    res.set_header("Access-Control-Allow-Origin","*");
    cout<<"hello";
    std::string name = req.matches[1];
    std::string email = req.matches[2];
    std::string password = req.matches[3];

    // Check if the username is already taken
    if (userCredentials.count(name)) {
      // Return an error response
      res.set_content("{\"error\":\"Username already taken\"}", "application/json");
    } else {
      // Register the new user
      userCredentials[name] = password;

      // Return a success response
      res.set_content("{\"message\":\"User registered successfully\"}", "application/json");
    }
  });

   svr.Get(R"(/chat/send/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	string message = req.matches[2];
	string resultMessage; 
	string resultUser; 
	
    if (!messageMap.count(username)) {
    	resultUser = "{\"status\":\"baduser\"}";
	} else {
		addMessage(username,message,messageMap);
		resultMessage = "{\"status\":\"success\"}";
		
	}
    res.set_content(resultMessage, "text/json");
    res.set_content(resultUser, "text/json");
  });
  
   svr.Get(R"(/chat/fetch/(.*))", [&](const Request& req, Response& res) {
    string username = req.matches[1];
    res.set_header("Access-Control-Allow-Origin","*");
    string resultMessageJSON = getMessagesJSON(username,messageMap);
    res.set_content(resultMessageJSON, "text/json");
 
  });
  
  cout << "Server listening on port " << port << endl;
  svr.listen("0.0.0.0", port);

}

#ifndef SRC_WEB_WEB_SERVER_H_
#define SRC_WEB_WEB_SERVER_H_
#include "../../include/api/API.h"

class WebServer {
 private:
  std::unique_ptr<Game> game;

 public:
  WebServer();
  void Run();
};

#endif
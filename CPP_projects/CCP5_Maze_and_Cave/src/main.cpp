#include <QApplication>
#include <iostream>

#include "include/gui/mainwindow.h"
#include "web/web_server/web_server.h"

int main(int argc, char* argv[]) {
  std::vector<std::string> args(argv, argv + argc);
  bool run_web = false;
  for (const auto& arg : args) {
    if (arg == "--web") {
      run_web = true;
      break;
    }
  }

  int programResult = 0;

  if (run_web) {
    try {
      WebServer server;
      server.Run();
    } catch (const std::exception& e) {
      std::cerr << "Web server failed: " << e.what() << std::endl;
      programResult = 1;
    }
  } else {
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    programResult = a.exec();
  }

  return programResult;
}

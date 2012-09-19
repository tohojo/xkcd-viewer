#include <QtGui/QApplication>
#include <iostream>
#include "gui.h"
#include "message-handler.h"

MessageHandler handler;

void msgHandler(QtMsgType type, const char* msg)
{
  handler.msgHandler(type, msg);
}

int main(int argc, char *argv[])
{
  qInstallMsgHandler(msgHandler);
  QApplication app(argc, argv);

  GUI ui;
  ui.connect(&handler, SIGNAL(newMessage(QString)), &ui, SLOT(newMessage(QString)));
  ui.show();
  return app.exec();
}

#ifndef GUI_H
#define GUI_H

#include <QtGui/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include "ui_gui.h"
class GUI : public QMainWindow, private Ui::MainWindow

{
  Q_OBJECT

public:
  GUI(QWidget *parent =0);
  ~GUI();

  void show();

private:
  bool m_inprogress;
  QRegExp m_filename_match;
  QString open_directory;
  QGraphicsScene *output_scene;
  QString input_filename;
  void readSettings();
  void closeEvent(QCloseEvent *event);


public slots:
  void newMessage(QString msg);

private slots:
  void zoom_output(int value);
  void open_image();
  void load_image(QString filename);
  void add_image(QDir dir, QString filename);
  void setProgress(int value);

signals:
  void progress(int value);
};

#endif

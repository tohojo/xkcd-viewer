#ifndef GUI_H
#define GUI_H

#include <QtGui/QApplication>
#include <QtCore/QFileInfo>
#include "ui_gui.h"
#include "imagegraphicsitem.h"
class GUI : public QMainWindow, private Ui::MainWindow

{
  Q_OBJECT

public:
  GUI(QWidget *parent =0);
  ~GUI();

  void set_args(QMap<QString, QVariant> arguments);
  void show();

private:
  bool m_inprogress;
  bool m_batch;
  QString open_directory;
  QMap<QString, QVariant> args;
  QGraphicsScene *output_scene;
  QString input_filename;
  void readSettings();
  void closeEvent(QCloseEvent *event);


public slots:
  void newMessage(QString msg);

private slots:
  void zoom_output(int value);
  void open_image();
  void save_output();
  void load_image(QString filename);
  void save_image(QString filename);
  void setProgress(int value);
  void process_button_clicked();

signals:
  void progress(int value);
};

#endif

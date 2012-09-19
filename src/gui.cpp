#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QImage>
#include <QtGui/QImageWriter>
#include <QtGui/QPixmap>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QBitmap>
#include <QtGui/QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QTime>
#include <QtCore/QSettings>
#include <QtCore/QMetaProperty>
#include <QDebug>

#include "gui.h"

#define IMAGE_SIZE 2048

GUI::GUI(QWidget *parent)
  : QMainWindow(parent),
    m_inprogress(false),
    m_batch(false),
    m_filename_match("(\\d+)([sn])(\\d+)([we]).png")
{
  setupUi(this);

  open_directory = QDir::currentPath();

  output_scene = new QGraphicsScene(this);
  output_view->setScene(output_scene);


  connect(output_zoom, SIGNAL(valueChanged(int)), this, SLOT(zoom_output(int)));
  connect(output_view, SIGNAL(zoomUpdated(int)),
          output_zoom, SLOT(setValue(int)));

  // Dock widgets showing/hiding w/menu
  connect(textDock, SIGNAL(closed(bool)),
          action_Textual_output, SLOT(setChecked(bool)));

  // Saving and loading images & POIs
  connect(action_open_image, SIGNAL(activated()), this, SLOT(open_image()));
  connect(actionSaveOutput, SIGNAL(activated()), this, SLOT(save_output()));

  connect(this, SIGNAL(progress(int)), SLOT(setProgress(int)));

  readSettings();

}

GUI::~GUI()
{
  delete output_scene;
}

void GUI::show()
{
  QMainWindow::show();
  // Make sure the dock widget menu is updated correctly
  action_Textual_output->setChecked(textDock->isVisible());
}

void GUI::readSettings()
{
  QSettings settings("tohojo", "xkcd viewer");
  restoreGeometry(settings.value("GUI/geometry").toByteArray());
  restoreState(settings.value("GUI/windowState").toByteArray());
}

void GUI::closeEvent(QCloseEvent * event)
{
  QSettings settings("tohojo", "xkcd viewer");
  settings.setValue("GUI/geometry", saveGeometry());
  settings.setValue("GUI/windowState", saveState());
  QMainWindow::closeEvent(event);
}

void GUI::set_args(QMap<QString, QVariant> arguments) {
  args = arguments;

  if(args.contains("batch"))
    m_batch = true;

  if(args.contains("input")) {
    load_image(args.value("input").toString());
  } else if(m_batch) {
    qFatal("No input image given for batch mode.");
  }
}


void GUI::zoom_output(int value)
{
  qreal scale = (qreal)value/100.0;
  QTransform transform = QTransform::fromScale(scale, scale);
  output_view->setTransform(transform);
}

void GUI::open_image()
{
  input_filename = QFileDialog::getOpenFileName(this, tr("Select image"),
                                                  open_directory,
                                                  tr("Images (*.png *.jpg *.jpeg *.tif)"));
  if(!input_filename.isNull()) {
    load_image(input_filename);
  }
}

void GUI::load_image(QString filename)
{
  if(filename.isEmpty()) return;
  QFileInfo fileinfo = QFileInfo(filename);
  if(fileinfo.dir().exists(".")) {
    open_directory = fileinfo.dir().path();
  }

  if(!fileinfo.exists()) {
    if(m_batch) {
      qFatal("File '%s' not found.", filename.toLocal8Bit().data());
      return;
    }
    QMessageBox msgbox(QMessageBox::Critical, tr("File not found"),
                       tr("File '%1' was not found.").arg(filename),
                       QMessageBox::Ok, this);
    msgbox.exec();
    return;
  }
  input_filename = filename;
  QDir dir = fileinfo.dir();
  QStringList filters;
  filters << "*.png";
  dir.setNameFilters(filters);

  QStringList names = dir.entryList();

  for(int i = 0; i < names.size(); i++) {
    add_image(dir,names[i]);
    emit progress(i/(float)names.size()*100);
  }
  emit progress(100);

}

void GUI::add_image(QDir dir, QString filename)
{
  QRegExp rx(m_filename_match);
  int offset_x, offset_y;
  if(rx.indexIn(filename) > -1) {
    offset_y = rx.cap(1).toInt()-1;
    offset_x = rx.cap(3).toInt()-1;
    if(rx.cap(2) == "s") offset_y *= -1;
    if(rx.cap(4) == "w") offset_x *= -1;

    qDebug() << "Loading filename" << filename << "to position" << offset_x << "," << offset_y;
    QImage img(dir.filePath(filename));
  }
}

void GUI::save_output()
{
  QString filename = QFileDialog::getSaveFileName(this, tr("Select file name"),
                                          open_directory,
                                          tr("Images (*.png *.jpg *.jpeg *.tif)"));
  if(!filename.isNull()) {
    save_image(filename);
  }
}

void GUI::save_image(QString filename)
{
  QFileInfo info(filename);

  QImageWriter writer(filename);
  /*  if(!writer.write(img)) {
    if(m_batch) {
      qFatal("Unable to save output to '%s': %s.",
             filename.toLocal8Bit().data(), writer.errorString().toLocal8Bit().data());
      return;
    }
    QMessageBox msgbox(QMessageBox::Critical, tr("Unable to save image"),
                       tr("The output image could not be saved to '%1':\n%2.").arg(filename).arg(writer.errorString()),
                       QMessageBox::Ok, this);
    msgbox.exec();
  } else{
    qDebug() << "Output image saved to:" << filename;
    }*/
}

  void GUI::setProgress(int value)
  {
      progressBar->setValue(value);
    if(value < 100) {
      m_inprogress = true;
      processButton->setText(tr("Cancel processing"));
    } else {
      m_inprogress = false;
      processButton->setText(tr("Reload"));
    }
  }

  void GUI::process_button_clicked()
  {
    if(m_inprogress) {
      setProgress(100);
    } else {
    }
  }

  void GUI::newMessage(QString msg)
  {
    QString message = QString("%1: %2");
    message = message.arg(QTime::currentTime().toString("hh:mm:ss.zzz"), msg);
    textOutput->appendPlainText(message);
  }

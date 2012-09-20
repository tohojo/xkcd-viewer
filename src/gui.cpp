#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QPixmap>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QTime>
#include <QtCore/QSettings>
#include <QDebug>

#include "gui.h"
#include "graphicsscene.h"

#define IMAGE_SIZE 2048

GUI::GUI(QWidget *parent)
  : QMainWindow(parent),
    m_inprogress(false),
    m_filename_match("(\\d+)([sn])(\\d+)([we]).png")
{
  setupUi(this);

  open_directory = QDir::currentPath();

  output_scene = new GraphicsScene(this);
  output_view->setScene(output_scene);


  connect(output_zoom, SIGNAL(valueChanged(int)), this, SLOT(zoom_output(int)));
  connect(output_view, SIGNAL(zoomUpdated(int)),
          output_zoom, SLOT(setValue(int)));

  // Dock widgets showing/hiding w/menu
  connect(textDock, SIGNAL(closed(bool)),
          action_Textual_output, SLOT(setChecked(bool)));

  // Saving and loading images & POIs
  connect(action_open_image, SIGNAL(activated()), this, SLOT(open_image()));

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

  int count = 0;

  for(int i = 0; i < names.size(); i++) {
    if(add_image(dir,names[i])) count++;
    emit progress(i/(float)names.size()*100);
  }
  emit progress(100);

  qDebug() << "Loaded" << count << "tiles taking up a total of" << count*2048*2048 << "pixels and spanning" << output_scene->sceneRect().width() << "x" << output_scene->sceneRect().height() << "pixels";

}

bool GUI::add_image(QDir dir, QString filename)
{
  QRegExp rx(m_filename_match);
  int offset_x, offset_y;
  if(rx.indexIn(filename) > -1) {
    offset_y = rx.cap(1).toInt()-1;
    offset_x = rx.cap(3).toInt();
    if(rx.cap(2) == "n") offset_y *= -1;
    else offset_y += 1;
    if(rx.cap(4) == "w") offset_x *= -1;
    else offset_x -= 1;

    QPixmap pixmap(dir.filePath(filename));
    qDebug() << "Loading filename" << filename << "to position" << offset_x << "," << offset_y << "-" << offset_x * IMAGE_SIZE << "," << offset_y * IMAGE_SIZE;
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
    item->moveBy(offset_x*IMAGE_SIZE, offset_y*IMAGE_SIZE);
    output_scene->addItem(item);
    return true;
  } else {
    qDebug() << "Not loading filename:" << filename;
    return false;
  }
}


  void GUI::setProgress(int value)
  {
      progressBar->setValue(value);
    if(value < 100) {
      m_inprogress = true;
    } else {
      m_inprogress = false;
    }
  }

  void GUI::newMessage(QString msg)
  {
    QString message = QString("%1: %2");
    message = message.arg(QTime::currentTime().toString("hh:mm:ss.zzz"), msg);
    textOutput->appendPlainText(message);
  }

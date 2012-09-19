/**
 * graphicsscene.h
 *
 * Toke Høiland-Jørgensen
 * 2012-09-20
 */

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>

class GraphicsScene : public QGraphicsScene
{
  Q_OBJECT

public:
  GraphicsScene(QObject *parent =0);
  ~GraphicsScene();
private:
  void drawBackground(QPainter *painter, const QRectF &rect);
};

#endif

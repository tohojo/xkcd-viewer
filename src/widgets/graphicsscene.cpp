/**
 * graphicsscene.cpp
 *
 * Toke Høiland-Jørgensen
 * 2012-09-20
 */

#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent)
  :QGraphicsScene(parent)
{
}

GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
  QBrush earthBrush = QBrush(QColor::fromRgb(0, 0, 0, 255));

  QRectF earth(rect.intersected(sceneRect()));
  earth.setTop(0);

  if(earth.height() > 0) {
    painter->setBrush(earthBrush);
    painter->drawPolygon(earth);
  }
}

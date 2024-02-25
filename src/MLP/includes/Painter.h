#ifndef PAINTER_H
#define PAINTER_H

#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

#include "Logger.h"
#include "NetworkControler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Painter;
}
QT_END_NAMESPACE

namespace s21 {
class Painter : public QWidget {
  Q_OBJECT

 public:
  Painter(s21::NetworkControler *c, QWidget *parent = nullptr);
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void Clear();
  ~Painter();
 private slots:
  void Clean();
  void Analyze();

 private:
  Logger logger_{"Painter"};
  void RenderCanvasInPainter(QPainter &p);
  QImage GetPaintedPixelMatrix();
  ::Ui::Painter *ui_;
  bool draw_{false};
  QVector<QPointF> points_;
  s21::NetworkControler *c_;
};
}  // namespace s21

#endif  // PAINTER_H

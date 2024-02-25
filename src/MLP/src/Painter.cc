#include "Painter.h"

#include "ui_painter.h"

namespace s21 {
Painter::Painter(s21::NetworkControler* c, QWidget* parent)
    : QWidget(parent), ui_(new ::Ui::Painter), c_{c} {
  ui_->setupUi(this);
  connect(ui_->CleanB, &QPushButton::clicked, this, &Painter::Clean);
  connect(ui_->AnalyzeB, &QPushButton::clicked, this, &Painter::Analyze);
}

void Painter::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  RenderCanvasInPainter(painter);
}
void Painter::mousePressEvent(QMouseEvent* pe) {
  Q_UNUSED(pe)
  draw_ = true;
}
void Painter::mouseMoveEvent(QMouseEvent* pe) {
  if (draw_) points_.push_back(pe->position());
  update();
}

void Painter::mouseReleaseEvent(QMouseEvent* pe) {
  Q_UNUSED(pe)
  draw_ = false;
  Analyze();
}

void Painter::Clear() {
  points_.clear();
  update();
}

QImage Painter::GetPaintedPixelMatrix() {
  QImage image(this->size(), QImage::Format_ARGB32);

  image.fill(Qt::transparent);

  QPainter painter(&image);
  RenderCanvasInPainter(painter);

  return image;
}

Painter::~Painter() { delete ui_; }

void Painter::Clean() { Clear(); }

void Painter::Analyze() {
  if (!c_) {
    logger_.Log("Missing network controler", Logger::LogLevel::kError);
    return;
  }
  QImage img;
  img = GetPaintedPixelMatrix();

  int ans = c_->AnalyzeImg(img);
  std::string log = "Determined letter: ";
  log += (char)('A' + ans);
  logger_.Log(log, Logger::LogLevel::kInfo);
}

void Painter::RenderCanvasInPainter(QPainter& p) {
  QPalette Pal(palette());
  p.setBackground(Qt::black);
  p.eraseRect(rect());
  setAutoFillBackground(true);
  setPalette(Pal);
  p.setRenderHint(QPainter::Antialiasing, true);
  p.setPen(QPen(Qt::green, 20, Qt::SolidLine));
  for (int i = 0; i < points_.size(); ++i)
    if (i > 0) p.drawEllipse(points_[i - 1], 12, 12);
}

}  // namespace s21

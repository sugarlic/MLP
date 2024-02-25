#include "GraphView.h"

#include <cmath>
#include <vector>

#include "ui_GraphView.h"
using std::vector;
namespace s21 {
GraphView::GraphView(QWidget *parent)
    : QWidget(parent), ui_(new ::Ui::GraphView) {
  ui_->setupUi(this);
  GraphView::SetBaseRanges();
  SetGraph();
}

GraphView::~GraphView() { delete ui_; }

void GraphView::SetGraph() {
  auto graph = ui_->graph;
  graph->setAntialiasedElement(QCP::aeItems);
  graph->clearGraphs();
  graph->addGraph();
  QPen graph_pen(base_graph_color_);
  graph_pen.setWidth(2);
  graph->graph(0)->setPen(graph_pen);
  graph->xAxis->setLabel("Epoch, n");
  graph->yAxis->setLabel("Error, %");

  graph->xAxis->setLabelColor(base_font_color_);
  graph->yAxis->setLabelColor(base_font_color_);

  graph->yAxis->setBasePen(white_pen);
  graph->yAxis->setSubTickPen(white_pen);
  graph->yAxis->setTickLabelColor(base_font_color_);
  graph->yAxis->setTickPen(white_pen);

  graph->xAxis->setBasePen(white_pen);
  graph->xAxis->setSubTickPen(white_pen);
  graph->xAxis->setTickLabelColor(base_font_color_);
  graph->xAxis->setTickPen(white_pen);

  graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  graph->setBackground(QBrush(base_background_color_));
  connect(this, &GraphView::XRangeChanged, this, &GraphView::UpdateAxis);
  connect(this, &GraphView::YRangeChanged, this, &GraphView::UpdateAxis);
  connect(graph->xAxis, SIGNAL(rangeChanged(QCPRange)), graph->xAxis2,
          SLOT(setRange(QCPRange)));
  connect(graph->yAxis, SIGNAL(rangeChanged(QCPRange)), graph->yAxis2,
          SLOT(setRange(QCPRange)));
  connect(graph->xAxis, SIGNAL(rangeChanged(QCPRange)), this,
          SLOT(SetRangeX(QCPRange)));
  connect(graph->yAxis, SIGNAL(rangeChanged(QCPRange)), this,
          SLOT(SetRangeY(QCPRange)));
  connect(this, &GraphView::YRangeChanged, this, &GraphView::UpdateLimitsY);
  connect(this, &GraphView::XRangeChanged, this, &GraphView::UpdateLimitsX);
  connect(ui_->x_lower_limit, &QLineEdit::editingFinished, this,
          &GraphView::UpdateGraphLimits);
  connect(ui_->x_upper_limit, &QLineEdit::editingFinished, this,
          &GraphView::UpdateGraphLimits);
  connect(ui_->y_lower_limit, &QLineEdit::editingFinished, this,
          &GraphView::UpdateGraphLimits);
  connect(ui_->y_upper_limit, &QLineEdit::editingFinished, this,
          &GraphView::UpdateGraphLimits);
  connect(this, &GraphView::LimitsChanged, this, &GraphView::SetRanges);
  UpdateAxis();
}

void GraphView::SetRangeX(const QCPRange &range) {
  x_range_ = range;
  CheckReplotOnRangeChange();
  emit XRangeChanged(x_range_);
}

void GraphView::SetRangeX(double lower, double upper) {
  x_range_.lower = lower;
  x_range_.upper = upper;
  CheckReplotOnRangeChange();
  emit XRangeChanged(x_range_);
}

void GraphView::SetRangeY(const QCPRange &range) {
  y_range_ = range;
  emit YRangeChanged(y_range_);
}

void GraphView::SetRangeY(double lower, double upper) {
  y_range_.lower = lower;
  y_range_.upper = upper;
  emit YRangeChanged(y_range_);
}
void GraphView::SetBaseRanges() { SetRanges(base_x_range_, base_y_range_); }
void GraphView::CheckReplotOnRangeChange() { PlotExpression(data_); }
void GraphView::SetRanges(const QCPRange &rangeX, const QCPRange &rangeY) {
  SetRangeY(rangeY);
  SetRangeX(rangeX);
}

void GraphView::UpdateAxis() {
  ui_->graph->xAxis->setRange(x_range_);
  ui_->graph->yAxis->setRange(y_range_);
  Replot();
}

void GraphView::Replot() { ui_->graph->replot(); }

void GraphView::PlotExpression(GraphData data) {
  if (data.first.empty() || data.second.empty()) return;
  if (data_ == data) return;
  SetRangeX(0, data.first.size() - 1);
  // ui->
  auto &errors = data.second;
  std::for_each(errors.begin(), errors.end(), [](double &a) { a *= 100; });
  data_ = data;
  try {
    ui_->graph->graph(0)->setData(data_.first, data_.second);
    UpdateAxis();
  } catch (...) {
    return;
  }
}
void GraphView::UpdateLimitsX(QCPRange &range) {
  ui_->x_lower_limit->setText(QString::number(range.lower));
  ui_->x_upper_limit->setText(QString::number(range.upper));
}

void GraphView::UpdateLimitsY(QCPRange &range) {
  ui_->y_lower_limit->setText(QString::number(range.lower));
  ui_->y_upper_limit->setText(QString::number(range.upper));
}
void GraphView::UpdateGraphLimits() {
  double lower_x = ui_->x_lower_limit->text().toDouble();
  double lower_y = ui_->y_lower_limit->text().toDouble();
  double upper_x = ui_->x_upper_limit->text().toDouble();
  double upper_y = ui_->y_upper_limit->text().toDouble();
  QCPRange rangeX(lower_x, upper_x);
  QCPRange rangeY(lower_y, upper_y);
  emit LimitsChanged(rangeX, rangeY);
}

}  // namespace s21

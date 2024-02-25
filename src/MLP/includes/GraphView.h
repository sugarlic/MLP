#ifndef GRAPH_VIEW_H_
#define GRAPH_VIEW_H_
#include <QObject>
#include <QWidget>

#include "qcustomplot.h"
namespace Ui {
class GraphView;
}
namespace s21 {

class GraphView : public QWidget {
  Q_OBJECT
 public:
  typedef std::pair<QVector<double>, QVector<double>> GraphData;
  GraphView(QWidget* parent = nullptr);
  ~GraphView();
 signals:
  void LimitsChanged(const QCPRange& rangeX, const QCPRange& rangeY);
  void XRangeChanged(QCPRange& new_range);
  void YRangeChanged(QCPRange& new_range);
 public slots:
  void PlotExpression(GraphData data);

 private slots:
  void SetRangeX(const QCPRange& range);
  void SetRangeX(double lower, double upper);
  void SetRangeY(const QCPRange& range);
  void SetRangeY(double lower, double upper);
  void SetRanges(const QCPRange& rangeX, const QCPRange& rangeY);
  void UpdateAxis();
  void Replot();

  void UpdateLimitsX(QCPRange& range);
  void UpdateLimitsY(QCPRange& range);
  void UpdateGraphLimits();

 private:
  QCPRange x_range_;
  QCPRange y_range_;
  GraphData data_;
  ::Ui::GraphView* ui_;
  void SetBaseRanges();
  QColor base_graph_color_{0xdd2100};
  QColor base_font_color_{0xe7e4f8};
  QColor base_background_color_{0x111617};
  QPen white_pen{base_font_color_};
  const QCPRange base_x_range_{0, 10};
  const QCPRange base_y_range_{0, 100};
  void CheckReplotOnRangeChange();
  void SetGraph();
};
}  // namespace s21
#endif  // GRAPH_VIEW_H_

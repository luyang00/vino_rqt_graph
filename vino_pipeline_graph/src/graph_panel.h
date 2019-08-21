#ifndef GRAPH_PANEL_H
#define GRAPH_PANEL_H
 #include <QMainWindow>
 #include <QPushButton>
#include <QWidget>
#include <QFrame>
#include "ui_graph_node.h"
namespace vino_pipeline_graph
{

class PipelinePaintWidget: public QWidget
{
Q_OBJECT

private:
    UIPipelineGraph * graph;
    QPoint pos_mouse_press;
    bool mouse_press,mouse_release;

    bool isAddingEdge;

public:
  friend class MainPanel;
  // This class is not instantiated by pluginlib::ClassLoader, so the
  // constructor has no restrictions.
  PipelinePaintWidget( QFrame* parent = 0 );

  // We override QWidget::paintEvent() to do custom painting.
  virtual void paintEvent( QPaintEvent* event );
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void AddEdge(void);

//   // We override the mouse events and leaveEvent() to keep track of
//   // what the mouse is doing.
//   virtual void mouseMoveEvent( QMouseEvent* event );
//   virtual void mousePressEvent( QMouseEvent* event );
//   virtual void mouseReleaseEvent( QMouseEvent* event );
//   virtual void leaveEvent( QEvent* event );

  // Override sizeHint() to give the layout managers some idea of a
  // good size for this.
  virtual QSize sizeHint() const { return QSize( 640, 480 ); }

  virtual void resizeEvent( QResizeEvent * event);
  // We emit outputVelocity() whenever it changes.
  void draw(){update();}


Q_SIGNALS:
  
protected:

};


} // vino_pipeline_graph


#endif // DRIVE_WIDGET_H

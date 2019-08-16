#ifndef TELEOP_PANEL_H
#define TELEOP_PANEL_H

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#include <rviz/panel.h>

#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>
#include <QPalette>
#include <QBrush>
#include <QSize>
#include <QInputDialog>
#include "graph_panel.h"


#endif

class QLineEdit;

namespace vino_pipeline_graph
{

class MainPanel: public rviz::Panel
{
// This class uses Qt slots and is a subclass of QObject, so it needs
// the Q_OBJECT macro.
Q_OBJECT
public:
  // QWidget subclass constructors usually take a parent widget
  // parameter (which usually defaults to 0).  At the same time,
  // pluginlib::ClassLoader creates instances by calling the default
  // constructor (with no arguments).  Taking the parameter and giving
  // a default of 0 lets the default constructor work and also lets
  // someone using the class for something else to pass in a parent
  // widget as they normally would with Qt.
  MainPanel( QWidget* parent = 0 );

  // Now we declare overrides of rviz::Panel functions for saving and
  // loading data from the config file.  Here the data is the
  // topic name.
  virtual void load( const rviz::Config& config );
  virtual void save( rviz::Config config ) const;

  // Next come a couple of public Qt slots.
public Q_SLOTS:
//   // The control area, DriveWidget, sends its output to a Qt signal
//   // for ease of re-use, so here we declare a Qt slot to receive it.
//   void setVel( float linear_velocity_, float angular_velocity_ );

//   // In this example setTopic() does not get connected to any signal
//   // (it is called directly), but it is easy to define it as a public
//   // slot instead of a private function in case it would be useful to
//   // some other user.
//   void setTopic( const QString& topic );

  // Here we declare some internal slots.
  virtual void paintEvent( QPaintEvent* event );

private Q_SLOTS:
  void addNodeBtnHandler(void);
  void addEdgeBtnHandler(void);
  void removeBtnHandler(void);

protected:
    QVBoxLayout* main_layout;
    QHBoxLayout *controller_layout;
    QHBoxLayout *editor_layout;
    PipelinePaintWidget *pipeline_widget;

private:
  QPushButton * btn_addNode;
  QPushButton * btn_addEdge;
  QPushButton * btn_remove;

//   QLineEdit* output_topic_editor_;

//   // The current name of the output topic.
//   QString output_topic_;


  // END_TUTORIAL
};

} // end namespace vino_pipeline_graph

#endif // TELEOP_PANEL_H

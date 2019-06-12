/*=========================================================================

   Program: MorphoDig
 

========================================================================*/
#ifndef mqDisplayControlsWidget_h
#define mqDisplayControlsWidget_h


#include <QWidget>
class Ui_mqDisplayControlsWidget;

/**
* mqDiplayControlsWidget is the Widget with actions (and reactions) related to display environment(show grid, show orientatioun helper, stereo, show backface, show clipping)
* "
* QMainWindow to use it.
*/
class  mqDisplayControlsWidget : public QWidget
{
  Q_OBJECT
  typedef QWidget Superclass;

public:
   mqDisplayControlsWidget(QWidget* parentObject = 0)
    : Superclass(parentObject)
  {
    this->constructor();
  }
  

  public slots :
  virtual void slotClippingPlaneOnOff();
  virtual void slotBackfaceCullingOnOff();
  virtual void slotDisplayCellNormals();
  virtual void slotDisplayCellNormals2();
  virtual void slotDisplayPointNormals();
  virtual void slotDisplayPointNormals2();
  virtual void slotDisplayWireframe();
  virtual void slotDisplayPoints();
private:
	Q_DISABLE_COPY(mqDisplayControlsWidget)
  
  Ui_mqDisplayControlsWidget *ui;
  void constructor();
};

#endif

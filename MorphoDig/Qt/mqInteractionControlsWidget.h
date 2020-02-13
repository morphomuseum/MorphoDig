/*=========================================================================

   Program: MorphoDig
   

========================================================================*/
#ifndef mqInteractionControlsWidget_h
#define mqInteractionControlsWidget_h


#include <QWidget>
class Ui_mqInteractionControlsWidget;

/**
* mqInteractionControlsWidget is the Widget with actions (and reactions) for the
* "Interaction Controls" Widget in MophoDig. 
* Simply instantiate this and put it in your application UI file or
* QMainWindow to use it.
*/
class  mqInteractionControlsWidget : public QWidget
{
  Q_OBJECT
  typedef QWidget Superclass;

public:
  mqInteractionControlsWidget( QWidget* parentObject = 0)
    : Superclass( parentObject)
  {
    this->constructor();
  }
 
  

  public slots :
  virtual void slotLandmarkNormalMode();
  virtual void slotLandmarkTargetMode();
  virtual void slotLandmarkNodeMode();
  virtual void slotLandmarkHandleMode();
  virtual void slotFlagMode();
  virtual void slotMoveCamera();
  virtual void slotMoveObjects();
  virtual void slotMoveOnlyLandmarks();
  virtual void slotMoveModeChanged();
  virtual void slotLandmarkModeChanged();
  virtual void slotCurveModeChanged();
private:
	Q_DISABLE_COPY(mqInteractionControlsWidget)
  
  Ui_mqInteractionControlsWidget *ui;
  void constructor();
};

#endif

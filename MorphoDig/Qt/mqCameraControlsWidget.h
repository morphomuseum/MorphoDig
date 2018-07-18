/*=========================================================================

   Program: MorphoDig
   Module:    mqCameraControlsWidget.h

 

========================================================================*/
#ifndef mqCameraControlsWidget_h
#define mqCameraControlsWidget_h


#include <QWidget>
class Ui_mqCameraControlsWidget;
class QDoubleSlider;
class QDial;
class QDoubleReleaseSlider;
class QReleaseSlider;
class QReleaseSliderValue;
class QReleaseScrollBarValue;
/**
* mqCameraControlsWidget is the Widget with actions (and reactions) for the
* "Camera Controls" Widget in MorphoDig. 
*/
class  mqCameraControlsWidget : public QWidget
{
  Q_OBJECT
  typedef QWidget Superclass;

public:
  
  mqCameraControlsWidget(QWidget* parentObject = 0)
    : Superclass(parentObject)
  {
    this->constructor();
  }
 

  public slots :
  
  virtual void slotZrot(int val);
  //virtual void slotZoom(double val);
  virtual void slotZoom();
  virtual void slotCp(int val);
  
private:
	int lastzrot;
	int lastcp;
	double lastzoom;
	Q_DISABLE_COPY(mqCameraControlsWidget);
	//QReleaseSlider * zRot;
	QReleaseSliderValue * zRot;
	//QReleaseScrollBarValue * zRot;
	//QDial * zRot;
	//QDial * camzRot;
	QReleaseSliderValue *cP;
	//QReleaseSlider *cP;
	//QDoubleSlider * cP;
	QDoubleReleaseSlider * zoom;
  Ui_mqCameraControlsWidget *ui;
  void constructor();
};

#endif

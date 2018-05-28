/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsControlsWidget.h

 

========================================================================*/
#ifndef mqScalarsControlsWidget_h
#define mqScalarsControlsWidget_h


#include <QWidget>
class Ui_mqScalarsControlsWidget;
class QComboBox;
/**
* mqScalarsControlsWidget is the Widget with actions (and reactions) for the
* "Scalars Controls" Widget in MorphoDig. 
*/
class  mqScalarsControlsWidget : public QWidget
{
  Q_OBJECT
  typedef QWidget Superclass;

public:
  
  mqScalarsControlsWidget(QWidget* parentObject = 0)
    : Superclass(parentObject)
  {
    this->constructor();
  }
  void RefreshEditButtons();

  public slots :
  virtual void slotScalarVisitiliby();
  virtual void slotActiveScalarChanged(int idx);
  virtual void slotRefreshComboScalars();
private:
	Q_DISABLE_COPY(mqScalarsControlsWidget)
		QComboBox * comboActiveScalars;
  Ui_mqScalarsControlsWidget *ui;
  void constructor();
};

#endif

/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsControlsToolbar.h

 

========================================================================*/
#ifndef mqScalarsControlsToolbar_h
#define mqScalarsControlsToolbar_h


#include <QToolBar>
class Ui_mqScalarsControlsToolbar;
class QComboBox;
/**
* mqScalarsControlsToolbar is the toolbar with actions (and reactions) for the
* "Scalars Controls" toolbar in MorphoDig. 
*/
class  mqScalarsControlsToolbar : public QToolBar
{
  Q_OBJECT
  typedef QToolBar Superclass;

public:
  mqScalarsControlsToolbar(const QString& title, QWidget* parentObject = 0)
    : Superclass(title, parentObject)
  {
    this->constructor();
  }
  mqScalarsControlsToolbar(QWidget* parentObject = 0)
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
	Q_DISABLE_COPY(mqScalarsControlsToolbar)
		QComboBox * comboActiveScalars;
  Ui_mqScalarsControlsToolbar *ui;
  void constructor();
};

#endif

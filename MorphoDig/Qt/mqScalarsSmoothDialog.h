/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsSmoothDialog_h
#define _mqScalarsSmoothDialog_h


#include <QDialog>

namespace Ui
{
class mqScalarsSmoothDialog;
}




/**
* mqScalarsSmoothDialog is the dialog used by MorphoDig to compute Smooth .
*/
class  mqScalarsSmoothDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsSmoothDialog(QWidget* Parent);
  
  ~mqScalarsSmoothDialog();
  void editSmooth();
  
  public slots:
  
  
  
  virtual void sloteditSmooth();
  virtual void slotClose();
  virtual void slotProgressBar(int val);
  virtual void slotEnableDisableCustomArea();
  virtual void slotEnableDisableLocalNeighboursArea();
  virtual void slotEnableDisableAutoArea();
  virtual void slotEnableDisableCutPercent();
  
protected:
	
private:
	

  Q_DISABLE_COPY(mqScalarsSmoothDialog)
  Ui::mqScalarsSmoothDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h

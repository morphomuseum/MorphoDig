/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsThicknessDialog_h
#define _mqScalarsThicknessDialog_h


#include <QDialog>

namespace Ui
{
class mqScalarsThicknessDialog;
}




/**
* mqScalarsThicknessDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqScalarsThicknessDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsThicknessDialog(QWidget* Parent);
  
  ~mqScalarsThicknessDialog();
  void editThickness();
  
  public slots:
  
  
  virtual void slotsmoothNormals();
  virtual void sloteditThickness();
  virtual void slotClose();
  virtual void slotProgressBar(int val);
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqScalarsThicknessDialog)
  Ui::mqScalarsThicknessDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h

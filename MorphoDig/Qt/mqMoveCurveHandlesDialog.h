/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqMoveCurveHandlesDialog_h
#define _mqMoveCurveHandlesDialog_h

#include <QDialog>

namespace Ui
{
class mqMoveCurveHandlesDialog;
}




/**
* mqMoveCurveHandlesDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqMoveCurveHandlesDialog : public QDialog
{
  Q_OBJECT

public:
  mqMoveCurveHandlesDialog(QWidget* Parent);
  
  ~mqMoveCurveHandlesDialog();

  public slots:
  virtual void slotMoveCurveHandles();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqMoveCurveHandlesDialog)
  Ui::mqMoveCurveHandlesDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h

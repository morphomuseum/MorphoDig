/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqFillHolesDialog_h
#define _mqFillHolesDialog_h


#include <QDialog>

namespace Ui
{
class mqFillHolesDialog;
}




/**
* mqFillHolesDialog is the dialog used by MorphoDig to Fill Holes of selected surfaces
*/
class  mqFillHolesDialog : public QDialog
{
  Q_OBJECT

public:
  mqFillHolesDialog(QWidget* Parent);
  
  ~mqFillHolesDialog();
  void FillHoles();
  
  public slots:
  
  
  
  virtual void slotFillHoles();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqFillHolesDialog)
  Ui::mqFillHolesDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h

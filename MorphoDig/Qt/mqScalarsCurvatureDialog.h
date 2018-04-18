/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsCurvatureDialog_h
#define _mqScalarsCurvatureDialog_h


#include <QDialog>

namespace Ui
{
class mqScalarsCurvatureDialog;
}




/**
* mqScalarsCurvatureDialog is the dialog used by MorphoDig to compute curvature scalars
*/
class  mqScalarsCurvatureDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsCurvatureDialog(QWidget* Parent);
  
  ~mqScalarsCurvatureDialog();
  void editCurvature();
  
  public slots:
  
  
  
  virtual void sloteditCurvature();
  virtual void slotClose();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqScalarsCurvatureDialog)
  Ui::mqScalarsCurvatureDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h

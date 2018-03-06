/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSelectLandmarkRangeDialog_h
#define _mqSelectLandmarkRangeDialog_h

#include <QDialog>

namespace Ui
{
class mqSelectLandmarkRangeDialog;
}





class  mqSelectLandmarkRangeDialog : public QDialog
{
  Q_OBJECT

public:
  mqSelectLandmarkRangeDialog(QWidget* Parent);
  
  ~mqSelectLandmarkRangeDialog();

  public slots:
  virtual void slotSelectLandmarkRange();
  
  

protected:
	
private:
	
	
  Q_DISABLE_COPY(mqSelectLandmarkRangeDialog)
  Ui::mqSelectLandmarkRangeDialog* const Ui;
  // Here we should have the file name, no ?
};

#endif 

/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsNormalizationDialog_h
#define _mqScalarsNormalizationDialog_h


#include <QDialog>

namespace Ui
{
class mqScalarsNormalizationDialog;
}




/**
* mqScalarsNormalizationDialog is the dialog used by MorphoDig to compute Normalization scalars
*/
class  mqScalarsNormalizationDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsNormalizationDialog(QWidget* Parent);
  
  ~mqScalarsNormalizationDialog();
  void editNormalization();
  
  public slots:
  
  virtual void slotremovePercent();
  virtual void slotcustomMinMax();
  virtual void slotReinitializeMinMax();
  virtual void sloteditNormalization();
  virtual void slotClose();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqScalarsNormalizationDialog)
  Ui::mqScalarsNormalizationDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h

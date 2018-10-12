/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsInfosDialog_h
#define _mqScalarsInfosDialog_h


#include <QDialog>

namespace Ui
{
class mqScalarsInfosDialog;
}




/**
* mqScalarsInfosDialog is the dialog used by MorphoDig to compute infos (mean, variance etc. ) for active scalars
*/
class  mqScalarsInfosDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsInfosDialog(QWidget* Parent);
  
  ~mqScalarsInfosDialog();
  void editInfos();
  void PopulateComboTags();
  public slots:
  
  
    
  virtual void slotIncludeTags();
  virtual void sloteditInfos();
  virtual void slotClose();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqScalarsInfosDialog)
  Ui::mqScalarsInfosDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h

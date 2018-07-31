/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqTagFromRGBDialog_h
#define _mqTagFromRGBDialog_h


#include <QDialog>

namespace Ui
{
class mqTagFromRGBDialog;
}




/**
* mqTagFromRGBDialog is the dialog used by MorphoDig to create a new Tag Array from currently displayed colors of selected surfaces
*/
class  mqTagFromRGBDialog : public QDialog
{
  Q_OBJECT

public:
  mqTagFromRGBDialog(QWidget* Parent);
  
  ~mqTagFromRGBDialog();
  void TagFromRGB();
  
  public slots:
  
  virtual void slotFirstN();
  virtual void slotExact();
  virtual void slotTagFromRGB();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqTagFromRGBDialog)
  Ui::mqTagFromRGBDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h

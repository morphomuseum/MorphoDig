/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqAboutDialog_h
#define _mqAboutDialog_h

#include <QDialog>

namespace Ui
{
class mqAboutDialog;
}




/**
* mqAboutDialog is the about dialog used by MorphoDig.
* It provides information about MorphoDig.
*/
class  mqAboutDialog : public QDialog
{
  Q_OBJECT

public:
  mqAboutDialog(QWidget* Parent);
  
  ~mqAboutDialog();

protected:
	void AddClientInformation();
private:
  Q_DISABLE_COPY(mqAboutDialog)
  Ui::mqAboutDialog* const Ui;
};

#endif // !_mqAboutDialog_h

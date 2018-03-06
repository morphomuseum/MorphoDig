#ifndef QRELEASEDIAL_H
#define QRELEASEDIAL_H

#include <QDial>

class QReleaseDial : public QDial
{
	Q_OBJECT
public:
	explicit QReleaseDial(QWidget *parent = 0);

	

signals:

	public slots :
		void slotRefresh();

private:
	


	void Refresh();

};

#endif // QRELEASEDial_H

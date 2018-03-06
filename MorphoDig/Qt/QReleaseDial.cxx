#include "QReleaseDial.h"
#include <cmath>

QReleaseDial::QReleaseDial(QWidget *parent) :
	QDial(parent)
{
		connect(this, SIGNAL(sliderReleased()), this, SLOT(slotRefresh()));

}
void QReleaseDial::slotRefresh()
{
	this->Refresh();
}


void QReleaseDial::Refresh()
{
	//std::cout << "Refresh to 0!" << endl;
	this->setValue(0);
}


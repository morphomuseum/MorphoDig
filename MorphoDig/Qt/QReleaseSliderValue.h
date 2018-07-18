#ifndef QRELEASESLIDERVALUE_H
#define QRELEASESLIDERVALUE_H

#include <QWidget>

class QDial;
class QScrollBar;
class QReleaseSlider;
//class QDoubleSlider;
class QSpinBox;
class QVBoxLayout;
class QLabel;

class QReleaseSliderValue : public QWidget
{
	Q_OBJECT

public:
	QReleaseSliderValue(Qt::Orientation orientation, const QString &title,
		QWidget *parent = 0);

signals:
	void valueChanged(int value);

	public slots:
	void setValue(int value);
	void setMinimum(int value);
	void setMaximum(int value);
	void invertAppearance(bool invert);
	void invertKeyBindings(bool invert);
	void setLabelVisible(int visible);
	void setSpinboxVisible(int visible);
	void setLabelText(QString text);

private:
	//QReleaseSlider *slider;
	QReleaseSlider *slider;
	QSpinBox *spinbox;
	QDial *dial;
	QLabel *label;
	QVBoxLayout *layout;
	int labelVisible;
	int spinboxVisible;
};

#endif
#ifndef SET_DATA
#define SET_DATA

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QValidator>
#include <Eigen/Dense>


namespace Ui {
	class SetDataWidget;
}

class SetDataWidget : public QDialog {
	Q_OBJECT
public:
	explicit SetDataWidget(QWidget* parent/*=nullptr */);
	~SetDataWidget();

private slots:
	void addRow();
	void removeRow();
	void startCalculate();

private:
	static Eigen::Matrix4d dhMatrix(double a, double d, double theta, double alpha);
	bool validateInputs(QVector<double>& values);

	Ui::SetDataWidget* _ui = nullptr;
	QDoubleValidator* _validator = nullptr;
	enum TableColumns { A, D, THETA, ALPHA, COL_COUNT };
};

#endif // SET_DATA!

#include "SetDataWidget.h"
#include "ui_SetDataWidget.h"

#include <QMessageBox>
#include <QLocale>
SetDataWidget::SetDataWidget(QWidget* parent) : QDialog(parent), _ui(new Ui::SetDataWidget), _validator(new QDoubleValidator(this)) {
	_ui->setupUi(this);
	_ui->tableData_tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	connect(_ui->addRow_pb, &QPushButton::clicked, this, &SetDataWidget::addRow);
	connect(_ui->removeRow_pb, &QPushButton::clicked, this, &SetDataWidget::removeRow);
	connect(_ui->startCalc_pb, &QPushButton::clicked, this, &SetDataWidget::startCalculate);
	addRow();
	QLocale locale(QLocale::English);
	_validator->setLocale(locale);

}

SetDataWidget::~SetDataWidget() {
	delete _ui;
};

void SetDataWidget::addRow() {
	const int rowCount = _ui->tableData_tw->rowCount();
	_ui->tableData_tw->insertRow(rowCount);
	for (int col = 0; col < COL_COUNT; col++) {
		QLineEdit* lineEdit = new QLineEdit;
		lineEdit->setValidator(_validator);
		_ui->tableData_tw->setCellWidget(rowCount, col, lineEdit);
	}
}

void SetDataWidget::removeRow() {
	const int row = _ui->tableData_tw->currentRow();
	if (row >= 0 && _ui->tableData_tw->rowCount() > 1) {
		_ui->tableData_tw->removeRow(row);
	}
}

bool SetDataWidget::validateInputs(QVector<double>& values) {
	values.clear();
	values.resize(_ui->tableData_tw->rowCount() * COL_COUNT);
	for (int row = 0; row < _ui->tableData_tw->rowCount(); ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			auto* edit = qobject_cast<QLineEdit*>(
				_ui->tableData_tw->cellWidget(row, col));
			if (!edit || edit->text().isEmpty()) {
				QMessageBox::warning(this, "Input Error",
					QString("Row %1, Column %2: no value")
					.arg(row + 1).arg(col + 1), QMessageBox::NoButton);
				return false;
			}
			bool ok;
			const double value = edit->text().toDouble(&ok);
			if (!ok) {
				QMessageBox::warning(this, "Input Error",
					QString("Row %1, Column %2: incorrect value")
					.arg(row + 1).arg(col + 1), QMessageBox::NoButton);
				return false;
			}
			values[row * COL_COUNT + col] = value;
		}
	}
	return true;
}

void SetDataWidget::startCalculate() {
	QVector<double> values;
	if (!validateInputs(values))
		return;
	Eigen::Matrix4d position = Eigen::Matrix4d::Identity();
	const int rowCount = _ui->tableData_tw->rowCount();
	for (int row = 0; row < rowCount; ++row) {
		const int offset = row * COL_COUNT;
		const auto stepMatrix = dhMatrix(
			values[offset + A],
			values[offset + D],
			values[offset + THETA],
			values[offset + ALPHA]
		);
		position *= stepMatrix;
	}
	_ui->result_lbl->setText(
		QString("x: %1, y: %2, z: %3")
		.arg(position(0, 3), 0, 'f', 4)
		.arg(position(1, 3), 0, 'f', 4)
		.arg(position(2, 3), 0, 'f', 4)
	);
}




Eigen::Matrix4d SetDataWidget::dhMatrix(double a, double d, double theta, double alpha) {
	Eigen::Matrix4d matrix;
	matrix << cos(theta), -sin(theta) * cos(alpha), sin(theta)* sin(alpha), a* cos(theta),
		sin(theta), cos(theta)* cos(alpha), -cos(theta) * sin(alpha), a* sin(theta),
		0, sin(alpha), cos(alpha), d,
		0, 0, 0, 1;
	return matrix;
}


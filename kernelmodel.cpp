#include "kernelmodel.h"

KernelModel::KernelModel(QObject *parent)
    : QAbstractTableModel(parent) {

}

KernelModel::KernelModel(cv::Mat &mat, QObject *parent) :
    QAbstractTableModel(parent) {
	mat.copyTo(_kernel);
}

KernelModel::~KernelModel() {
	saveToFile("kernel.data");
}

QHash<int, QByteArray> KernelModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[ElementRole] = "element";
	return roles;
}

int KernelModel::rowCount(const QModelIndex &parent) const {
	if (parent.isValid())
		return 0;
	return _kernel.rows;
}

int KernelModel::columnCount(const QModelIndex &parent) const {
	if (parent.isValid())
		return 0;
	return _kernel.cols;
}

QVariant KernelModel::data(const QModelIndex &index, int role) const {
	switch (role) {
		case (ElementRole):
			return _kernel.at<double>(index.row(), index.column());
		default:
			// TODO: return invalid QVariant
			return QVariant();
	}
}

bool KernelModel::setData(const QModelIndex &index,
                          const QVariant &value,
                          int role) {
	qDebug() << "role:" << role;
	switch (role) {
		case (ElementRole):
			_kernel.at<double>(index.row(), index.column()) = value.toDouble();
			emit dataChanged(index, index, QList<int>{ElementRole});
			return true;
		default:
			return false;
	}
}

Qt::ItemFlags KernelModel::flags(const QModelIndex &index) const {
	const Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
	return defaultFlags | Qt::ItemIsEditable;
}

void KernelModel::saveToFile(const QString &filename) const {
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "Could not open file to save KernelModel!";
	}
	QDataStream stream(&file);
	stream << _kernel.rows << _kernel.cols;
	for (size_t i = 0; i < _kernel.total(); ++i)
		stream << _kernel.at<double>(i);
	file.close();
}

void KernelModel::LoadFromFile(const QString &filename) {
	QFile file(filename);
	if (!file.exists())
		return;
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "Could not open file to load KernelModel!";
	}
	QDataStream stream(&file);
	stream >> _kernel.rows >> _kernel.cols;
	for (int row = 0; row < _kernel.rows; ++row)
		for (int col = 0; col < _kernel.cols; ++col)
			stream >> _kernel.at<double>(row, col);
	file.close();
}

const cv::Mat &KernelModel::mat() const {
	return _kernel;
}

void KernelModel::reset() {
	_kernel = (cv::Mat_<double>(3, 3) <<
	           0, 0, 0,
	           0, 1, 0,
	           0, 0, 0
	           );
	emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

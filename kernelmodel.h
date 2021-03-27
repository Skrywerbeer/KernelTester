#ifndef KERNELMODEL_H
#define KERNELMODEL_H

#include <iostream>

#include <QAbstractTableModel>
#include <QQmlEngine>
#include <QFile>
#include <QDataStream>

#include <opencv2/opencv.hpp>

class KernelModel : public QAbstractTableModel {
		Q_OBJECT
		QML_ELEMENT
//		Q_PROPERTY(int rows
//		           READ rowCount
//		           WRITE setRows
//		           NOTIFY rowsChanged)
//		Q_PROPERTY(int columns
//		           READ columnCount
//		           WRITE setColumns
//		           NOTIFY columnChanged)
	public:
		enum KernelRoles {
			ElementRole = Qt::UserRole + 1
		};
		explicit KernelModel(QObject *parent = nullptr);
		KernelModel(cv::Mat &mat, QObject *parent = nullptr);

		~KernelModel();

		QHash<int, QByteArray> roleNames() const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		int columnCount(const QModelIndex &parent = QModelIndex()) const override;
		QVariant data(const QModelIndex &index,
		              int role = Qt::DisplayRole) const override;
		bool setData(const QModelIndex &index,
		             const QVariant &value,
		             int role = Qt::EditRole) override;
		Qt::ItemFlags flags(const QModelIndex &index) const override;



		const cv::Mat &mat() const;

	public slots:
		void saveToFile(const QString &filename) const;
		void LoadFromFile(const QString &filename);
		void reset();

	signals:
		void rowsChanged();
		void columnChanged();

	private:
		cv::Mat _kernel = (cv::Mat_<double>(3, 3) <<
		                   0, 0, 0,
		                   0, 1, 0,
		                   0, 0, 0
		                   );
};

#endif // KERNELMODEL_H

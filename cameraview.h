#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QQuickItem>
#include <QQmlEngine>
#include <QImage>
#include <QSGNode>
#include <QSGSimpleTextureNode>

#include <opencv2/opencv.hpp>

#include "kernelmodel.h"

class CameraView : public QQuickItem {
		Q_OBJECT
		QML_ELEMENT
		Q_PROPERTY(int captureWidth
		           READ captureWidth
		           WRITE setCaptureWidth
		           NOTIFY captureWidthChanged)
		Q_PROPERTY(int captureHeight
		           READ captureHeight
		           WRITE setCaptureHeight
		           NOTIFY captureHeightChanged)
		Q_PROPERTY(bool live
		           READ live
		           WRITE setLive
		           NOTIFY liveChanged)
		Q_PROPERTY(uint refreshInterval
		           READ refreshInterval
		           WRITE setRefreshInterval
		           NOTIFY refreshIntervalChanged)
		Q_PROPERTY(bool greyscale
		           READ greyscale
		           WRITE setGreyscale
		           NOTIFY greyscaleChanged)
		Q_PROPERTY(bool captureDelta
		           READ captureDelta
		           WRITE setCaptureDelta
		           NOTIFY captureDeltaChanged)
		Q_PROPERTY(double brightness
		           READ brightness
		           WRITE setBrightness
		           NOTIFY brightnessChanged)
		Q_PROPERTY(double contrast
		           READ contrast
		           WRITE setContrast
		           NOTIFY contrastChanged)
		Q_PROPERTY(double saturation
		           READ saturation
		           WRITE setSaturation
		           NOTIFY saturationChanged)
		Q_PROPERTY(double whiteBalance
		           READ whiteBalance
		           WRITE setWhiteBalance
		           NOTIFY whiteBalanceChanged)
		Q_PROPERTY(KernelModel *kernel
		           READ kernel
		           WRITE setKernel
		           NOTIFY kernelChanged)
		Q_PROPERTY(bool applyKernel
		           READ applyKernel
		           WRITE setApplyKernel
		           NOTIFY applyKernelChanged)
	public:
		CameraView(QQuickItem *parent = nullptr);
		~CameraView();


		int captureWidth() const;
		void setCaptureWidth(int width);
		int captureHeight() const;
		void setCaptureHeight(int height);

		bool greyscale() const;
		void setGreyscale(bool greyscale);
		bool captureDelta() const;
		void setCaptureDelta(bool capture);

		double brightness() const;
		void setBrightness(double value);
		double contrast() const;
		void setContrast(double value);
		double saturation() const;
		void setSaturation(double value);
		double whiteBalance() const;
		void setWhiteBalance (double value);
		KernelModel *kernel() const;
		void setKernel(KernelModel *kernel);
		bool applyKernel() const;
		void setApplyKernel(bool apply);

		bool live() const;
		void setLive(bool live);
		uint refreshInterval() const;
		void setRefreshInterval(const uint interval);

		void timerEvent(QTimerEvent *event);

		QSGNode *updatePaintNode(QSGNode *oldNode,
		                         UpdatePaintNodeData *data) override;

	public slots:
		void captureFrame();
		void updateView();

	signals:
		void captureWidthChanged();
		void captureHeightChanged();
		void liveChanged();
		void refreshIntervalChanged();
		void greyscaleChanged();
		void captureDeltaChanged();
		void brightnessChanged();
		void contrastChanged();
		void saturationChanged();
		void whiteBalanceChanged();
		void kernelChanged();
		void applyKernelChanged();

	private:
		bool _live = false;
		int _timerID = 0;
		uint _refreshInterval = 0;
		bool _greyscale = false;
		bool _captureDelta = false;
		KernelModel *_kernel = nullptr;
		bool _applyKernel = true;

		QImage _img;
		cv::VideoCapture _capture;
		cv::Mat _newestFrame;
};

#endif // CAMERAVIEW_H

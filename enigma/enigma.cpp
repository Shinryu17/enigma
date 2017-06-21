#include "enigma.h"
#include "encoder.h"
#include <ui_enigma.h>
#include <qimage.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <QFile>

enigma::enigma(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	EnCode.Flag = true; 
	//信号与槽连接 begin
	connect(ui.btnEncodeChooseAddress, SIGNAL(clicked()), this, SLOT(clickBtnEncodeChooseAddress()));
	connect(ui.rbtnEncodeSaveSide, SIGNAL(clicked()), this, SLOT(clickRbtnEncodeSaveSide()));
	connect(ui.rbtnEncodeChoose, SIGNAL(clicked()), this, SLOT(clickRbtnEncodeChoose()));
	connect(ui.btnEncode, SIGNAL(clicked()), this, SLOT(clickBtnEncode()));
	//信号与槽连接 end
}

enigma::~enigma()
{
}

void enigma::clickBtnEncodeChooseAddress()//选择要编码的txt的文件夹根路径
{
	QString getPath = QFileDialog::getExistingDirectory(this);
	if (!getPath.isNull())
	{
		ui.lineTxtNowAddress->setText(getPath);
	}
	RootPath = getPath;
}

void enigma::clickRbtnEncodeSaveSide()//选择保存图片到文本旁
{
	ui.lineEncodeSaveNowAddress->setEnabled(false);
}

void enigma::clickRbtnEncodeChoose()//选择保存的图片路径
{
	ui.lineEncodeSaveNowAddress->setEnabled(true);
	QString getPath = QFileDialog::getExistingDirectory(this);
	if (!getPath.isNull())
	{
		ui.lineEncodeSaveNowAddress->setText(getPath);
	}
	enigma::SavePath = getPath;
}

void enigma::clickBtnEncode()//开始编码
{
	EnCode.Flag = true;
	RootPath = ui.lineTxtNowAddress->text();
	QDir txtPath(ui.lineTxtNowAddress->text()),savePath(ui.lineEncodeSaveNowAddress->text());
	if (!txtPath.exists())//判断需要编码的根文件夹是否存在
	{
		ui.lblEncodeState->setStyleSheet("color:red;");
		ui.lblEncodeState->setText(QStringLiteral("需要编码的根文件夹不存在"));
		return;
	}
	if (ui.rbtnEncodeChoose->isChecked())
	{//如果另存到同一路径下，则看是否存在或能否创建
		if (!savePath.exists() && !savePath.mkdir(ui.lineEncodeSaveNowAddress->text()))
		{
			ui.lblEncodeState->setStyleSheet("color:red;");
			ui.lblEncodeState->setText(QStringLiteral("保存地址不存在并且创建失败"));
			return;
		}
	}
	//开始编码
	ui.chkEncodeDebug->setEnabled(false);
	ui.btnEncode->setEnabled(false);
	ui.lblEncodeState->setStyleSheet("color:black;");
	ui.lblEncodeState->setText(QStringLiteral("编码中..."));
	count = 0;
	//QStringList allTxtPath = getAllFilePath(true,ui.lineTxtNowAddress->text());
	if (ui.lineEncodeSaveNowAddress->isEnabled() == false) // 设置保存目录为当前路径
		SavePath = RootPath; 
	EnCode.RootPath = enigma::RootPath; // 设置编码线程根目录
	EnCode.SavePath = enigma::SavePath; //设置编码线程保存目录
	EnCode.start(); // 解码线程开始工作
	//结束编码
	//ui.lblEncodeState->setText(QStringLiteral("编码结束。"));

}

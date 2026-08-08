#define Q_(a,b) QString::fromUtf8(_(a))
/********************************************************************************
** Form generated from reading UI file 'video_effects.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef VIDEO_EFFECTS_H
#define VIDEO_EFFECTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "util/customwidgets.hpp"

QT_BEGIN_NAMESPACE

class Ui_ExtVideoWidget
{
public:
    QWidget *tab;
    QGridLayout *gridLayout_8;
    QGroupBox *adjustEnable;
    QGridLayout *gridLayout_6;
    QLabel *label;
    QSlider *hueSlider;
    QLabel *label_3;
    QSlider *brightnessSlider;
    QCheckBox *brightnessThresholdCheck;
    QLabel *label_2;
    QSlider *contrastSlider;
    QLabel *label_4;
    QSlider *saturationSlider;
    QLabel *label_5;
    QSlider *gammaSlider;
    QGroupBox *sharpenEnable;
    QGridLayout *gridLayout;
    QLabel *label_29;
    QSlider *sharpenSigmaSlider;
    QGroupBox *gradfunEnable;
    QGridLayout *gridLayout_2;
    QLabel *label_32;
    QSlider *gradfunRadiusSlider;
    QGroupBox *grainEnable;
    QGridLayout *gridLayout_4;
    QLabel *label_50;
    QSlider *grainVarianceSlider;
    QWidget *tab_3;
    QGridLayout *gridLayout_11;
    QLabel *label_10;
    QSpinBox *cropTopPx;
    QLabel *label_11;
    QSpacerItem *spacerItem;
    QLabel *label_12;
    QLabel *label_7;
    QSpinBox *cropBotPx;
    QCheckBox *topBotCropSync;
    QCheckBox *leftRightCropSync;
    QSpinBox *cropLeftPx;
    QSpinBox *cropRightPx;
    QWidget *tab_2;
    QGridLayout *gridLayout_10;
    QGroupBox *extractEnable;
    QGridLayout *gridLayout1;
    QLabel *label_6;
    QLineEdit *extractComponentText;
    QCheckBox *invertEnable;
    QCheckBox *posterizeEnable;
    QGroupBox *gradientEnable;
    QGridLayout *gridLayout_9;
    QLabel *label_25;
    QComboBox *gradientModeCombo;
    QCheckBox *gradientTypeCheck;
    QCheckBox *gradientCartoonCheck;
    QGroupBox *colorthresEnable;
    QGridLayout *gridLayout_19;
    QLabel *label_8;
    QLineEdit *colorthresColorText;
    QLabel *label_13;
    QSlider *colorthresSaturationthresSlider;
    QLabel *label_14;
    QSlider *colorthresSimilaritythresSlider;
    QGroupBox *sepiaEnable;
    QHBoxLayout *horizontalLayout;
    QLabel *label_18;
    QSpinBox *sepiaIntensitySpin;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_5;
    QGridLayout *gridLayout_13;
    QCheckBox *magnifyEnable;
    QGroupBox *wallEnable;
    QGridLayout *_5;
    QLabel *label_31;
    QLabel *label_30;
    QSpinBox *wallColsSpin;
    QSpinBox *wallRowsSpin;
    QGroupBox *transformEnable;
    QGridLayout *_4;
    QComboBox *transformTypeCombo;
    QGroupBox *rotateEnable;
    QGridLayout *gridLayout2;
    QLabel *label_28;
    VLCQDial *rotateAngleDial;
    QGroupBox *puzzleEnable;
    QGridLayout *gridLayout_3;
    QLabel *label_27;
    QSpinBox *puzzleRowsSpin;
    QLabel *label_26;
    QSpinBox *puzzleColsSpin;
    QWidget *tab_8;
    QGridLayout *gridLayout_5;
    QGroupBox *logoEnable;
    QGridLayout *gridLayout_1;
    QLabel *label_19;
    QLineEdit *logoFileText;
    QToolButton *logoBrowseBtn;
    QLabel *label_20;
    QSpinBox *logoYSpin;
    QLabel *label_21;
    QSpinBox *logoXSpin;
    QSlider *logoOpacitySlider;
    QLabel *label_49;
    QLabel *label_22;
    QLabel *label_48;
    QFrame *line;
    QGroupBox *marqEnable;
    QGridLayout *_2;
    QLabel *label_24;
    QLabel *label_23;
    QLineEdit *marqMarqueeText;
    QComboBox *marqPositionCombo;
    QGroupBox *eraseEnable;
    QGridLayout *gridLayout3;
    QLabel *label_16;
    QLabel *label_17;
    QSpinBox *eraseXSpin;
    QSpinBox *eraseYSpin;
    QLineEdit *eraseMaskText;
    QLabel *label_15;
    QToolButton *eraseBrowseBtn;
    QSpacerItem *verticalSpacer;
    QWidget *tab_9;
    QGridLayout *gridLayout_7;
    QVBoxLayout *verticalLayout;
    QCheckBox *anaglyphEnable;
    QCheckBox *mirrorEnable;
    QCheckBox *psychedelicEnable;
    QCheckBox *waveEnable;
    QCheckBox *rippleEnable;
    QCheckBox *motiondetectEnable;
    QGroupBox *gaussianblurEnable;
    QGridLayout *gridLayout_17;
    QLabel *label_35;
    QSlider *gaussianblurSigmaSlider;
    QGroupBox *motionblurEnable;
    QGridLayout *gridLayout_16;
    QLabel *label_34;
    QSlider *blurFactorSlider;
    QGroupBox *antiflickerEnable;
    QGridLayout *gridLayout_18;
    QLabel *label_36;
    QSlider *antiflickerSofteningSizeSlider;
    QGroupBox *hqdn3dEnable;
    QGridLayout *gridLayout_12;
    QLabel *label_44;
    QSlider *hqdn3dLumaSpatSlider;
    QLabel *label_45;
    QSlider *hqdn3dLumaTempSlider;
    QLabel *label_46;
    QSlider *hqdn3dChromaSpatSlider;
    QLabel *label_47;
    QSlider *hqdn3dChromaTempSlider;
    QGroupBox *cloneEnable;
    QGridLayout *gridLayout_0;
    QLabel *label_9;
    QSpinBox *cloneCountSpin;

    void setupUi(QTabWidget *ExtVideoWidget)
    {
        if (ExtVideoWidget->objectName().isEmpty())
            ExtVideoWidget->setObjectName(QString::fromUtf8("ExtVideoWidget"));
        ExtVideoWidget->resize(654, 345);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_8 = new QGridLayout(tab);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        adjustEnable = new QGroupBox(tab);
        adjustEnable->setObjectName(QString::fromUtf8("adjustEnable"));
        adjustEnable->setEnabled(true);
        adjustEnable->setCheckable(true);
        adjustEnable->setChecked(false);
        gridLayout_6 = new QGridLayout(adjustEnable);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label = new QLabel(adjustEnable);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_6->addWidget(label, 0, 0, 1, 1);

        hueSlider = new QSlider(adjustEnable);
        hueSlider->setObjectName(QString::fromUtf8("hueSlider"));
        hueSlider->setMinimum(-180);
        hueSlider->setMaximum(180);
        hueSlider->setOrientation(Qt::Horizontal);
        hueSlider->setTickPosition(QSlider::TicksBelow);
        hueSlider->setTickInterval(1);

        gridLayout_6->addWidget(hueSlider, 0, 1, 1, 1);

        label_3 = new QLabel(adjustEnable);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_6->addWidget(label_3, 1, 0, 1, 1);

        brightnessSlider = new QSlider(adjustEnable);
        brightnessSlider->setObjectName(QString::fromUtf8("brightnessSlider"));
        brightnessSlider->setMaximum(200);
        brightnessSlider->setOrientation(Qt::Horizontal);
        brightnessSlider->setTickPosition(QSlider::TicksBelow);
        brightnessSlider->setTickInterval(100);

        gridLayout_6->addWidget(brightnessSlider, 1, 1, 1, 1);

        brightnessThresholdCheck = new QCheckBox(adjustEnable);
        brightnessThresholdCheck->setObjectName(QString::fromUtf8("brightnessThresholdCheck"));
        QFont font;
        font.setPointSize(8);
        brightnessThresholdCheck->setFont(font);

        gridLayout_6->addWidget(brightnessThresholdCheck, 2, 1, 1, 1);

        label_2 = new QLabel(adjustEnable);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_6->addWidget(label_2, 3, 0, 1, 1);

        contrastSlider = new QSlider(adjustEnable);
        contrastSlider->setObjectName(QString::fromUtf8("contrastSlider"));
        contrastSlider->setMaximum(200);
        contrastSlider->setOrientation(Qt::Horizontal);
        contrastSlider->setTickPosition(QSlider::TicksBelow);
        contrastSlider->setTickInterval(100);

        gridLayout_6->addWidget(contrastSlider, 3, 1, 1, 1);

        label_4 = new QLabel(adjustEnable);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_6->addWidget(label_4, 4, 0, 1, 1);

        saturationSlider = new QSlider(adjustEnable);
        saturationSlider->setObjectName(QString::fromUtf8("saturationSlider"));
        saturationSlider->setMaximum(300);
        saturationSlider->setOrientation(Qt::Horizontal);
        saturationSlider->setTickPosition(QSlider::TicksBelow);
        saturationSlider->setTickInterval(100);

        gridLayout_6->addWidget(saturationSlider, 4, 1, 1, 1);

        label_5 = new QLabel(adjustEnable);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_6->addWidget(label_5, 5, 0, 1, 1);

        gammaSlider = new QSlider(adjustEnable);
        gammaSlider->setObjectName(QString::fromUtf8("gammaSlider"));
        gammaSlider->setMaximum(1000);
        gammaSlider->setOrientation(Qt::Horizontal);
        gammaSlider->setTickPosition(QSlider::TicksBelow);
        gammaSlider->setTickInterval(100);

        gridLayout_6->addWidget(gammaSlider, 5, 1, 1, 1);


        gridLayout_8->addWidget(adjustEnable, 0, 0, 3, 1);

        sharpenEnable = new QGroupBox(tab);
        sharpenEnable->setObjectName(QString::fromUtf8("sharpenEnable"));
        sharpenEnable->setCheckable(true);
        sharpenEnable->setChecked(false);
        gridLayout = new QGridLayout(sharpenEnable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_29 = new QLabel(sharpenEnable);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout->addWidget(label_29, 0, 0, 1, 1);

        sharpenSigmaSlider = new QSlider(sharpenEnable);
        sharpenSigmaSlider->setObjectName(QString::fromUtf8("sharpenSigmaSlider"));
        sharpenSigmaSlider->setMaximum(200);
        sharpenSigmaSlider->setPageStep(10);
        sharpenSigmaSlider->setOrientation(Qt::Horizontal);
        sharpenSigmaSlider->setTickPosition(QSlider::TicksBelow);
        sharpenSigmaSlider->setTickInterval(50);

        gridLayout->addWidget(sharpenSigmaSlider, 0, 1, 1, 1);


        gridLayout_8->addWidget(sharpenEnable, 0, 1, 1, 1);

        gradfunEnable = new QGroupBox(tab);
        gradfunEnable->setObjectName(QString::fromUtf8("gradfunEnable"));
        gradfunEnable->setCheckable(true);
        gradfunEnable->setChecked(false);
        gridLayout_2 = new QGridLayout(gradfunEnable);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_32 = new QLabel(gradfunEnable);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_2->addWidget(label_32, 0, 0, 1, 1);

        gradfunRadiusSlider = new QSlider(gradfunEnable);
        gradfunRadiusSlider->setObjectName(QString::fromUtf8("gradfunRadiusSlider"));
        gradfunRadiusSlider->setMinimum(4);
        gradfunRadiusSlider->setMaximum(32);
        gradfunRadiusSlider->setPageStep(4);
        gradfunRadiusSlider->setOrientation(Qt::Horizontal);
        gradfunRadiusSlider->setTickPosition(QSlider::TicksBelow);
        gradfunRadiusSlider->setTickInterval(4);

        gridLayout_2->addWidget(gradfunRadiusSlider, 0, 1, 1, 1);


        gridLayout_8->addWidget(gradfunEnable, 1, 1, 1, 1);

        grainEnable = new QGroupBox(tab);
        grainEnable->setObjectName(QString::fromUtf8("grainEnable"));
        grainEnable->setCheckable(true);
        grainEnable->setChecked(false);
        gridLayout_4 = new QGridLayout(grainEnable);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_50 = new QLabel(grainEnable);
        label_50->setObjectName(QString::fromUtf8("label_50"));

        gridLayout_4->addWidget(label_50, 0, 0, 1, 1);

        grainVarianceSlider = new QSlider(grainEnable);
        grainVarianceSlider->setObjectName(QString::fromUtf8("grainVarianceSlider"));
        grainVarianceSlider->setMinimum(0);
        grainVarianceSlider->setMaximum(40);
        grainVarianceSlider->setPageStep(4);
        grainVarianceSlider->setOrientation(Qt::Horizontal);
        grainVarianceSlider->setTickPosition(QSlider::TicksBelow);
        grainVarianceSlider->setTickInterval(4);

        gridLayout_4->addWidget(grainVarianceSlider, 0, 1, 1, 1);


        gridLayout_8->addWidget(grainEnable, 2, 1, 1, 1);

        ExtVideoWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_11 = new QGridLayout(tab_3);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        label_10 = new QLabel(tab_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_11->addWidget(label_10, 0, 2, 1, 1);

        cropTopPx = new QSpinBox(tab_3);
        cropTopPx->setObjectName(QString::fromUtf8("cropTopPx"));
        cropTopPx->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cropTopPx->setMaximum(4095);

        gridLayout_11->addWidget(cropTopPx, 0, 3, 1, 1);

        label_11 = new QLabel(tab_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_11->addWidget(label_11, 1, 0, 1, 1);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        gridLayout_11->addItem(spacerItem, 1, 3, 1, 1);

        label_12 = new QLabel(tab_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_11->addWidget(label_12, 1, 4, 1, 1);

        label_7 = new QLabel(tab_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_11->addWidget(label_7, 3, 2, 1, 1);

        cropBotPx = new QSpinBox(tab_3);
        cropBotPx->setObjectName(QString::fromUtf8("cropBotPx"));
        cropBotPx->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cropBotPx->setMaximum(4095);

        gridLayout_11->addWidget(cropBotPx, 3, 3, 1, 1);

        topBotCropSync = new QCheckBox(tab_3);
        topBotCropSync->setObjectName(QString::fromUtf8("topBotCropSync"));

        gridLayout_11->addWidget(topBotCropSync, 4, 0, 1, 4);

        leftRightCropSync = new QCheckBox(tab_3);
        leftRightCropSync->setObjectName(QString::fromUtf8("leftRightCropSync"));

        gridLayout_11->addWidget(leftRightCropSync, 5, 0, 1, 4);

        cropLeftPx = new QSpinBox(tab_3);
        cropLeftPx->setObjectName(QString::fromUtf8("cropLeftPx"));
        cropLeftPx->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cropLeftPx->setMaximum(4095);

        gridLayout_11->addWidget(cropLeftPx, 1, 1, 1, 1);

        cropRightPx = new QSpinBox(tab_3);
        cropRightPx->setObjectName(QString::fromUtf8("cropRightPx"));
        cropRightPx->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cropRightPx->setMaximum(4095);

        gridLayout_11->addWidget(cropRightPx, 1, 5, 1, 1);

        ExtVideoWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_10 = new QGridLayout(tab_2);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        extractEnable = new QGroupBox(tab_2);
        extractEnable->setObjectName(QString::fromUtf8("extractEnable"));
        extractEnable->setCheckable(true);
        extractEnable->setChecked(false);
        gridLayout1 = new QGridLayout(extractEnable);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        label_6 = new QLabel(extractEnable);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);

        gridLayout1->addWidget(label_6, 0, 0, 1, 1);

        extractComponentText = new QLineEdit(extractEnable);
        extractComponentText->setObjectName(QString::fromUtf8("extractComponentText"));
        extractComponentText->setMaximumSize(QSize(140, 16777215));
        extractComponentText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout1->addWidget(extractComponentText, 0, 1, 1, 1);


        gridLayout_10->addWidget(extractEnable, 0, 0, 3, 1);

        invertEnable = new QCheckBox(tab_2);
        invertEnable->setObjectName(QString::fromUtf8("invertEnable"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(invertEnable->sizePolicy().hasHeightForWidth());
        invertEnable->setSizePolicy(sizePolicy1);

        gridLayout_10->addWidget(invertEnable, 0, 2, 1, 1);

        posterizeEnable = new QCheckBox(tab_2);
        posterizeEnable->setObjectName(QString::fromUtf8("posterizeEnable"));

        gridLayout_10->addWidget(posterizeEnable, 1, 2, 1, 1);

        gradientEnable = new QGroupBox(tab_2);
        gradientEnable->setObjectName(QString::fromUtf8("gradientEnable"));
        sizePolicy.setHeightForWidth(gradientEnable->sizePolicy().hasHeightForWidth());
        gradientEnable->setSizePolicy(sizePolicy);
        gradientEnable->setCheckable(true);
        gradientEnable->setChecked(false);
        gridLayout_9 = new QGridLayout(gradientEnable);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_25 = new QLabel(gradientEnable);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_9->addWidget(label_25, 0, 0, 1, 1);

        gradientModeCombo = new QComboBox(gradientEnable);
        gradientModeCombo->setObjectName(QString::fromUtf8("gradientModeCombo"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(gradientModeCombo->sizePolicy().hasHeightForWidth());
        gradientModeCombo->setSizePolicy(sizePolicy2);

        gridLayout_9->addWidget(gradientModeCombo, 0, 1, 1, 2);

        gradientTypeCheck = new QCheckBox(gradientEnable);
        gradientTypeCheck->setObjectName(QString::fromUtf8("gradientTypeCheck"));

        gridLayout_9->addWidget(gradientTypeCheck, 1, 0, 1, 2);

        gradientCartoonCheck = new QCheckBox(gradientEnable);
        gradientCartoonCheck->setObjectName(QString::fromUtf8("gradientCartoonCheck"));

        gridLayout_9->addWidget(gradientCartoonCheck, 1, 2, 1, 1);


        gridLayout_10->addWidget(gradientEnable, 2, 2, 2, 1);

        colorthresEnable = new QGroupBox(tab_2);
        colorthresEnable->setObjectName(QString::fromUtf8("colorthresEnable"));
        colorthresEnable->setCheckable(true);
        colorthresEnable->setChecked(false);
        gridLayout_19 = new QGridLayout(colorthresEnable);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        label_8 = new QLabel(colorthresEnable);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);

        gridLayout_19->addWidget(label_8, 0, 0, 1, 1);

        colorthresColorText = new QLineEdit(colorthresEnable);
        colorthresColorText->setObjectName(QString::fromUtf8("colorthresColorText"));
        colorthresColorText->setMaximumSize(QSize(140, 16777215));
        colorthresColorText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(colorthresColorText, 0, 1, 1, 1);

        label_13 = new QLabel(colorthresEnable);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_19->addWidget(label_13, 1, 0, 1, 1);

        colorthresSaturationthresSlider = new QSlider(colorthresEnable);
        colorthresSaturationthresSlider->setObjectName(QString::fromUtf8("colorthresSaturationthresSlider"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(colorthresSaturationthresSlider->sizePolicy().hasHeightForWidth());
        colorthresSaturationthresSlider->setSizePolicy(sizePolicy3);
        colorthresSaturationthresSlider->setOrientation(Qt::Horizontal);

        gridLayout_19->addWidget(colorthresSaturationthresSlider, 1, 1, 1, 1);

        label_14 = new QLabel(colorthresEnable);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_19->addWidget(label_14, 2, 0, 1, 1);

        colorthresSimilaritythresSlider = new QSlider(colorthresEnable);
        colorthresSimilaritythresSlider->setObjectName(QString::fromUtf8("colorthresSimilaritythresSlider"));
        sizePolicy2.setHeightForWidth(colorthresSimilaritythresSlider->sizePolicy().hasHeightForWidth());
        colorthresSimilaritythresSlider->setSizePolicy(sizePolicy2);
        colorthresSimilaritythresSlider->setOrientation(Qt::Horizontal);

        gridLayout_19->addWidget(colorthresSimilaritythresSlider, 2, 1, 1, 1);


        gridLayout_10->addWidget(colorthresEnable, 3, 0, 2, 1);

        sepiaEnable = new QGroupBox(tab_2);
        sepiaEnable->setObjectName(QString::fromUtf8("sepiaEnable"));
        sepiaEnable->setCheckable(true);
        sepiaEnable->setChecked(false);
        horizontalLayout = new QHBoxLayout(sepiaEnable);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_18 = new QLabel(sepiaEnable);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout->addWidget(label_18);

        sepiaIntensitySpin = new QSpinBox(sepiaEnable);
        sepiaIntensitySpin->setObjectName(QString::fromUtf8("sepiaIntensitySpin"));
        sepiaIntensitySpin->setMaximum(255);
        sepiaIntensitySpin->setValue(30);

        horizontalLayout->addWidget(sepiaIntensitySpin);


        gridLayout_10->addWidget(sepiaEnable, 4, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_10->addItem(verticalSpacer_3, 5, 2, 1, 1);

        ExtVideoWidget->addTab(tab_2, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        gridLayout_13 = new QGridLayout(tab_5);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        magnifyEnable = new QCheckBox(tab_5);
        magnifyEnable->setObjectName(QString::fromUtf8("magnifyEnable"));

        gridLayout_13->addWidget(magnifyEnable, 0, 0, 1, 1);

        wallEnable = new QGroupBox(tab_5);
        wallEnable->setObjectName(QString::fromUtf8("wallEnable"));
        wallEnable->setCheckable(true);
        wallEnable->setChecked(false);
        _5 = new QGridLayout(wallEnable);
        _5->setObjectName(QString::fromUtf8("_5"));
        label_31 = new QLabel(wallEnable);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        _5->addWidget(label_31, 0, 0, 1, 1);

        label_30 = new QLabel(wallEnable);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        _5->addWidget(label_30, 1, 0, 1, 1);

        wallColsSpin = new QSpinBox(wallEnable);
        wallColsSpin->setObjectName(QString::fromUtf8("wallColsSpin"));
        wallColsSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        wallColsSpin->setMinimum(1);
        wallColsSpin->setValue(3);

        _5->addWidget(wallColsSpin, 1, 1, 1, 1);

        wallRowsSpin = new QSpinBox(wallEnable);
        wallRowsSpin->setObjectName(QString::fromUtf8("wallRowsSpin"));
        wallRowsSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        wallRowsSpin->setMinimum(1);
        wallRowsSpin->setValue(3);

        _5->addWidget(wallRowsSpin, 0, 1, 1, 1);


        gridLayout_13->addWidget(wallEnable, 0, 1, 3, 1);

        transformEnable = new QGroupBox(tab_5);
        transformEnable->setObjectName(QString::fromUtf8("transformEnable"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(transformEnable->sizePolicy().hasHeightForWidth());
        transformEnable->setSizePolicy(sizePolicy4);
        transformEnable->setCheckable(true);
        transformEnable->setChecked(false);
        _4 = new QGridLayout(transformEnable);
        _4->setObjectName(QString::fromUtf8("_4"));
        transformTypeCombo = new QComboBox(transformEnable);
        transformTypeCombo->setObjectName(QString::fromUtf8("transformTypeCombo"));

        _4->addWidget(transformTypeCombo, 0, 0, 1, 1);


        gridLayout_13->addWidget(transformEnable, 1, 0, 1, 1);

        rotateEnable = new QGroupBox(tab_5);
        rotateEnable->setObjectName(QString::fromUtf8("rotateEnable"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(rotateEnable->sizePolicy().hasHeightForWidth());
        rotateEnable->setSizePolicy(sizePolicy5);
        rotateEnable->setCheckable(true);
        rotateEnable->setChecked(false);
        gridLayout2 = new QGridLayout(rotateEnable);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        label_28 = new QLabel(rotateEnable);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        QSizePolicy sizePolicy6(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label_28->sizePolicy().hasHeightForWidth());
        label_28->setSizePolicy(sizePolicy6);

        gridLayout2->addWidget(label_28, 1, 0, 1, 1);

        rotateAngleDial = new VLCQDial(rotateEnable);
        rotateAngleDial->setObjectName(QString::fromUtf8("rotateAngleDial"));
        sizePolicy5.setHeightForWidth(rotateAngleDial->sizePolicy().hasHeightForWidth());
        rotateAngleDial->setSizePolicy(sizePolicy5);
        rotateAngleDial->setMaximum(359);
        rotateAngleDial->setValue(180);
        rotateAngleDial->setSliderPosition(180);

        gridLayout2->addWidget(rotateAngleDial, 1, 1, 1, 1);


        gridLayout_13->addWidget(rotateEnable, 2, 0, 2, 1);

        puzzleEnable = new QGroupBox(tab_5);
        puzzleEnable->setObjectName(QString::fromUtf8("puzzleEnable"));
        puzzleEnable->setCheckable(true);
        puzzleEnable->setChecked(false);
        gridLayout_3 = new QGridLayout(puzzleEnable);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_27 = new QLabel(puzzleEnable);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_3->addWidget(label_27, 0, 0, 1, 1);

        puzzleRowsSpin = new QSpinBox(puzzleEnable);
        puzzleRowsSpin->setObjectName(QString::fromUtf8("puzzleRowsSpin"));
        puzzleRowsSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        puzzleRowsSpin->setMinimum(2);
        puzzleRowsSpin->setMaximum(16);
        puzzleRowsSpin->setValue(4);

        gridLayout_3->addWidget(puzzleRowsSpin, 0, 1, 1, 1);

        label_26 = new QLabel(puzzleEnable);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_3->addWidget(label_26, 1, 0, 1, 1);

        puzzleColsSpin = new QSpinBox(puzzleEnable);
        puzzleColsSpin->setObjectName(QString::fromUtf8("puzzleColsSpin"));
        puzzleColsSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        puzzleColsSpin->setMinimum(2);
        puzzleColsSpin->setMaximum(16);
        puzzleColsSpin->setValue(4);

        gridLayout_3->addWidget(puzzleColsSpin, 1, 1, 1, 1);


        gridLayout_13->addWidget(puzzleEnable, 3, 1, 1, 1);

        ExtVideoWidget->addTab(tab_5, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        gridLayout_5 = new QGridLayout(tab_8);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        logoEnable = new QGroupBox(tab_8);
        logoEnable->setObjectName(QString::fromUtf8("logoEnable"));
        logoEnable->setCheckable(true);
        logoEnable->setChecked(false);
        gridLayout_1 = new QGridLayout(logoEnable);
        gridLayout_1->setObjectName(QString::fromUtf8("gridLayout_1"));
        label_19 = new QLabel(logoEnable);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setMinimumSize(QSize(50, 0));

        gridLayout_1->addWidget(label_19, 0, 0, 1, 1);

        logoFileText = new QLineEdit(logoEnable);
        logoFileText->setObjectName(QString::fromUtf8("logoFileText"));

        gridLayout_1->addWidget(logoFileText, 0, 1, 1, 3);

        logoBrowseBtn = new QToolButton(logoEnable);
        logoBrowseBtn->setObjectName(QString::fromUtf8("logoBrowseBtn"));

        gridLayout_1->addWidget(logoBrowseBtn, 0, 4, 1, 1);

        label_20 = new QLabel(logoEnable);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setMinimumSize(QSize(50, 0));

        gridLayout_1->addWidget(label_20, 1, 0, 1, 1);

        logoYSpin = new QSpinBox(logoEnable);
        logoYSpin->setObjectName(QString::fromUtf8("logoYSpin"));
        logoYSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        logoYSpin->setMaximum(4096);

        gridLayout_1->addWidget(logoYSpin, 1, 1, 1, 2);

        label_21 = new QLabel(logoEnable);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setMinimumSize(QSize(50, 0));

        gridLayout_1->addWidget(label_21, 2, 0, 1, 1);

        logoXSpin = new QSpinBox(logoEnable);
        logoXSpin->setObjectName(QString::fromUtf8("logoXSpin"));
        logoXSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        logoXSpin->setMaximum(4096);

        gridLayout_1->addWidget(logoXSpin, 2, 1, 1, 2);

        logoOpacitySlider = new QSlider(logoEnable);
        logoOpacitySlider->setObjectName(QString::fromUtf8("logoOpacitySlider"));
        logoOpacitySlider->setMaximum(255);
        logoOpacitySlider->setOrientation(Qt::Horizontal);
        logoOpacitySlider->setTickPosition(QSlider::TicksBelow);
        logoOpacitySlider->setTickInterval(32);

        gridLayout_1->addWidget(logoOpacitySlider, 5, 0, 1, 5);

        label_49 = new QLabel(logoEnable);
        label_49->setObjectName(QString::fromUtf8("label_49"));
        label_49->setFont(font);
        label_49->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);

        gridLayout_1->addWidget(label_49, 6, 4, 1, 1);

        label_22 = new QLabel(logoEnable);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy7);
        label_22->setMinimumSize(QSize(50, 0));

        gridLayout_1->addWidget(label_22, 4, 0, 1, 3);

        label_48 = new QLabel(logoEnable);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setFont(font);
        label_48->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_1->addWidget(label_48, 6, 0, 1, 1);

        line = new QFrame(logoEnable);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_1->addWidget(line, 3, 0, 1, 5);


        gridLayout_5->addWidget(logoEnable, 0, 0, 4, 1);

        marqEnable = new QGroupBox(tab_8);
        marqEnable->setObjectName(QString::fromUtf8("marqEnable"));
        sizePolicy7.setHeightForWidth(marqEnable->sizePolicy().hasHeightForWidth());
        marqEnable->setSizePolicy(sizePolicy7);
        marqEnable->setCheckable(true);
        marqEnable->setChecked(false);
        _2 = new QGridLayout(marqEnable);
        _2->setObjectName(QString::fromUtf8("_2"));
        label_24 = new QLabel(marqEnable);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        _2->addWidget(label_24, 1, 0, 1, 1);

        label_23 = new QLabel(marqEnable);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        _2->addWidget(label_23, 0, 0, 1, 1);

        marqMarqueeText = new QLineEdit(marqEnable);
        marqMarqueeText->setObjectName(QString::fromUtf8("marqMarqueeText"));

        _2->addWidget(marqMarqueeText, 0, 1, 1, 1);

        marqPositionCombo = new QComboBox(marqEnable);
        marqPositionCombo->setObjectName(QString::fromUtf8("marqPositionCombo"));

        _2->addWidget(marqPositionCombo, 1, 1, 1, 1);


        gridLayout_5->addWidget(marqEnable, 1, 1, 3, 1);

        eraseEnable = new QGroupBox(tab_8);
        eraseEnable->setObjectName(QString::fromUtf8("eraseEnable"));
        eraseEnable->setCheckable(true);
        eraseEnable->setChecked(false);
        gridLayout3 = new QGridLayout(eraseEnable);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        label_16 = new QLabel(eraseEnable);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(50, 0));

        gridLayout3->addWidget(label_16, 2, 0, 1, 1);

        label_17 = new QLabel(eraseEnable);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setMinimumSize(QSize(50, 0));

        gridLayout3->addWidget(label_17, 3, 0, 1, 1);

        eraseXSpin = new QSpinBox(eraseEnable);
        eraseXSpin->setObjectName(QString::fromUtf8("eraseXSpin"));
        eraseXSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        eraseXSpin->setMaximum(4096);

        gridLayout3->addWidget(eraseXSpin, 3, 1, 1, 1);

        eraseYSpin = new QSpinBox(eraseEnable);
        eraseYSpin->setObjectName(QString::fromUtf8("eraseYSpin"));
        eraseYSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        eraseYSpin->setMaximum(4096);

        gridLayout3->addWidget(eraseYSpin, 2, 1, 1, 1);

        eraseMaskText = new QLineEdit(eraseEnable);
        eraseMaskText->setObjectName(QString::fromUtf8("eraseMaskText"));

        gridLayout3->addWidget(eraseMaskText, 0, 1, 1, 2);

        label_15 = new QLabel(eraseEnable);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(50, 0));

        gridLayout3->addWidget(label_15, 0, 0, 1, 1);

        eraseBrowseBtn = new QToolButton(eraseEnable);
        eraseBrowseBtn->setObjectName(QString::fromUtf8("eraseBrowseBtn"));

        gridLayout3->addWidget(eraseBrowseBtn, 0, 3, 1, 1);


        gridLayout_5->addWidget(eraseEnable, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 4, 1, 1, 1);

        ExtVideoWidget->addTab(tab_8, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QString::fromUtf8("tab_9"));
        gridLayout_7 = new QGridLayout(tab_9);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        anaglyphEnable = new QCheckBox(tab_9);
        anaglyphEnable->setObjectName(QString::fromUtf8("anaglyphEnable"));

        verticalLayout->addWidget(anaglyphEnable);

        mirrorEnable = new QCheckBox(tab_9);
        mirrorEnable->setObjectName(QString::fromUtf8("mirrorEnable"));

        verticalLayout->addWidget(mirrorEnable);

        psychedelicEnable = new QCheckBox(tab_9);
        psychedelicEnable->setObjectName(QString::fromUtf8("psychedelicEnable"));

        verticalLayout->addWidget(psychedelicEnable);

        waveEnable = new QCheckBox(tab_9);
        waveEnable->setObjectName(QString::fromUtf8("waveEnable"));

        verticalLayout->addWidget(waveEnable);

        rippleEnable = new QCheckBox(tab_9);
        rippleEnable->setObjectName(QString::fromUtf8("rippleEnable"));

        verticalLayout->addWidget(rippleEnable);

        motiondetectEnable = new QCheckBox(tab_9);
        motiondetectEnable->setObjectName(QString::fromUtf8("motiondetectEnable"));

        verticalLayout->addWidget(motiondetectEnable);


        gridLayout_7->addLayout(verticalLayout, 2, 1, 2, 1);

        gaussianblurEnable = new QGroupBox(tab_9);
        gaussianblurEnable->setObjectName(QString::fromUtf8("gaussianblurEnable"));
        gaussianblurEnable->setCheckable(true);
        gaussianblurEnable->setChecked(false);
        gridLayout_17 = new QGridLayout(gaussianblurEnable);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        label_35 = new QLabel(gaussianblurEnable);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        gridLayout_17->addWidget(label_35, 0, 0, 1, 1);

        gaussianblurSigmaSlider = new QSlider(gaussianblurEnable);
        gaussianblurSigmaSlider->setObjectName(QString::fromUtf8("gaussianblurSigmaSlider"));
        gaussianblurSigmaSlider->setMinimum(1);
        gaussianblurSigmaSlider->setMaximum(127);
        gaussianblurSigmaSlider->setValue(80);
        gaussianblurSigmaSlider->setOrientation(Qt::Horizontal);
        gaussianblurSigmaSlider->setTickPosition(QSlider::TicksBelow);
        gaussianblurSigmaSlider->setTickInterval(16);

        gridLayout_17->addWidget(gaussianblurSigmaSlider, 0, 1, 1, 1);


        gridLayout_7->addWidget(gaussianblurEnable, 2, 0, 1, 1);

        motionblurEnable = new QGroupBox(tab_9);
        motionblurEnable->setObjectName(QString::fromUtf8("motionblurEnable"));
        motionblurEnable->setCheckable(true);
        motionblurEnable->setChecked(false);
        gridLayout_16 = new QGridLayout(motionblurEnable);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        label_34 = new QLabel(motionblurEnable);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_16->addWidget(label_34, 0, 0, 1, 1);

        blurFactorSlider = new QSlider(motionblurEnable);
        blurFactorSlider->setObjectName(QString::fromUtf8("blurFactorSlider"));
        blurFactorSlider->setMinimum(1);
        blurFactorSlider->setMaximum(127);
        blurFactorSlider->setValue(80);
        blurFactorSlider->setOrientation(Qt::Horizontal);
        blurFactorSlider->setTickPosition(QSlider::TicksBelow);
        blurFactorSlider->setTickInterval(16);

        gridLayout_16->addWidget(blurFactorSlider, 0, 1, 1, 1);


        gridLayout_7->addWidget(motionblurEnable, 1, 0, 1, 1);

        antiflickerEnable = new QGroupBox(tab_9);
        antiflickerEnable->setObjectName(QString::fromUtf8("antiflickerEnable"));
        antiflickerEnable->setCheckable(true);
        antiflickerEnable->setChecked(false);
        gridLayout_18 = new QGridLayout(antiflickerEnable);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        label_36 = new QLabel(antiflickerEnable);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout_18->addWidget(label_36, 0, 0, 1, 1);

        antiflickerSofteningSizeSlider = new QSlider(antiflickerEnable);
        antiflickerSofteningSizeSlider->setObjectName(QString::fromUtf8("antiflickerSofteningSizeSlider"));
        antiflickerSofteningSizeSlider->setMinimum(0);
        antiflickerSofteningSizeSlider->setMaximum(100);
        antiflickerSofteningSizeSlider->setValue(80);
        antiflickerSofteningSizeSlider->setOrientation(Qt::Horizontal);
        antiflickerSofteningSizeSlider->setTickPosition(QSlider::TicksBelow);
        antiflickerSofteningSizeSlider->setTickInterval(16);

        gridLayout_18->addWidget(antiflickerSofteningSizeSlider, 0, 1, 1, 1);


        gridLayout_7->addWidget(antiflickerEnable, 0, 0, 1, 1);

        hqdn3dEnable = new QGroupBox(tab_9);
        hqdn3dEnable->setObjectName(QString::fromUtf8("hqdn3dEnable"));
        hqdn3dEnable->setCheckable(true);
        hqdn3dEnable->setChecked(false);
        gridLayout_12 = new QGridLayout(hqdn3dEnable);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        label_44 = new QLabel(hqdn3dEnable);
        label_44->setObjectName(QString::fromUtf8("label_44"));

        gridLayout_12->addWidget(label_44, 0, 0, 1, 1);

        hqdn3dLumaSpatSlider = new QSlider(hqdn3dEnable);
        hqdn3dLumaSpatSlider->setObjectName(QString::fromUtf8("hqdn3dLumaSpatSlider"));
        hqdn3dLumaSpatSlider->setMinimum(1);
        hqdn3dLumaSpatSlider->setMaximum(127);
        hqdn3dLumaSpatSlider->setValue(80);
        hqdn3dLumaSpatSlider->setOrientation(Qt::Horizontal);
        hqdn3dLumaSpatSlider->setTickPosition(QSlider::TicksBelow);
        hqdn3dLumaSpatSlider->setTickInterval(16);

        gridLayout_12->addWidget(hqdn3dLumaSpatSlider, 0, 1, 1, 2);

        label_45 = new QLabel(hqdn3dEnable);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        gridLayout_12->addWidget(label_45, 1, 0, 1, 2);

        hqdn3dLumaTempSlider = new QSlider(hqdn3dEnable);
        hqdn3dLumaTempSlider->setObjectName(QString::fromUtf8("hqdn3dLumaTempSlider"));
        hqdn3dLumaTempSlider->setMinimum(1);
        hqdn3dLumaTempSlider->setMaximum(127);
        hqdn3dLumaTempSlider->setValue(80);
        hqdn3dLumaTempSlider->setOrientation(Qt::Horizontal);
        hqdn3dLumaTempSlider->setTickPosition(QSlider::TicksBelow);
        hqdn3dLumaTempSlider->setTickInterval(16);

        gridLayout_12->addWidget(hqdn3dLumaTempSlider, 1, 2, 1, 1);

        label_46 = new QLabel(hqdn3dEnable);
        label_46->setObjectName(QString::fromUtf8("label_46"));

        gridLayout_12->addWidget(label_46, 2, 0, 1, 2);

        hqdn3dChromaSpatSlider = new QSlider(hqdn3dEnable);
        hqdn3dChromaSpatSlider->setObjectName(QString::fromUtf8("hqdn3dChromaSpatSlider"));
        hqdn3dChromaSpatSlider->setMinimum(1);
        hqdn3dChromaSpatSlider->setMaximum(127);
        hqdn3dChromaSpatSlider->setValue(80);
        hqdn3dChromaSpatSlider->setOrientation(Qt::Horizontal);
        hqdn3dChromaSpatSlider->setTickPosition(QSlider::TicksBelow);
        hqdn3dChromaSpatSlider->setTickInterval(16);

        gridLayout_12->addWidget(hqdn3dChromaSpatSlider, 2, 2, 1, 1);

        label_47 = new QLabel(hqdn3dEnable);
        label_47->setObjectName(QString::fromUtf8("label_47"));

        gridLayout_12->addWidget(label_47, 3, 0, 1, 1);

        hqdn3dChromaTempSlider = new QSlider(hqdn3dEnable);
        hqdn3dChromaTempSlider->setObjectName(QString::fromUtf8("hqdn3dChromaTempSlider"));
        hqdn3dChromaTempSlider->setMinimum(1);
        hqdn3dChromaTempSlider->setMaximum(127);
        hqdn3dChromaTempSlider->setValue(80);
        hqdn3dChromaTempSlider->setOrientation(Qt::Horizontal);
        hqdn3dChromaTempSlider->setTickPosition(QSlider::TicksBelow);
        hqdn3dChromaTempSlider->setTickInterval(16);

        gridLayout_12->addWidget(hqdn3dChromaTempSlider, 3, 2, 1, 1);


        gridLayout_7->addWidget(hqdn3dEnable, 0, 1, 2, 1);

        cloneEnable = new QGroupBox(tab_9);
        cloneEnable->setObjectName(QString::fromUtf8("cloneEnable"));
        sizePolicy.setHeightForWidth(cloneEnable->sizePolicy().hasHeightForWidth());
        cloneEnable->setSizePolicy(sizePolicy);
        cloneEnable->setCheckable(true);
        cloneEnable->setChecked(false);
        gridLayout_0 = new QGridLayout(cloneEnable);
        gridLayout_0->setObjectName(QString::fromUtf8("gridLayout_0"));
        label_9 = new QLabel(cloneEnable);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_0->addWidget(label_9, 0, 0, 1, 1);

        cloneCountSpin = new QSpinBox(cloneEnable);
        cloneCountSpin->setObjectName(QString::fromUtf8("cloneCountSpin"));
        cloneCountSpin->setMinimum(1);
        cloneCountSpin->setValue(2);

        gridLayout_0->addWidget(cloneCountSpin, 0, 1, 1, 1);


        gridLayout_7->addWidget(cloneEnable, 3, 0, 1, 1);

        ExtVideoWidget->addTab(tab_9, QString());
#if QT_CONFIG(shortcut)
        label->setBuddy(hueSlider);
        label_3->setBuddy(brightnessSlider);
        label_2->setBuddy(contrastSlider);
        label_4->setBuddy(saturationSlider);
        label_5->setBuddy(gammaSlider);
        label_29->setBuddy(sharpenSigmaSlider);
        label_32->setBuddy(gradfunRadiusSlider);
        label_50->setBuddy(grainVarianceSlider);
        label_10->setBuddy(cropTopPx);
        label_11->setBuddy(cropLeftPx);
        label_12->setBuddy(cropRightPx);
        label_7->setBuddy(cropBotPx);
        label_6->setBuddy(extractComponentText);
        label_8->setBuddy(colorthresColorText);
        label_13->setBuddy(colorthresSaturationthresSlider);
        label_14->setBuddy(colorthresSimilaritythresSlider);
        label_31->setBuddy(wallRowsSpin);
        label_30->setBuddy(wallColsSpin);
        label_28->setBuddy(rotateAngleDial);
        label_27->setBuddy(puzzleRowsSpin);
        label_26->setBuddy(puzzleColsSpin);
        label_19->setBuddy(logoFileText);
        label_20->setBuddy(logoYSpin);
        label_21->setBuddy(logoXSpin);
        label_49->setBuddy(logoOpacitySlider);
        label_22->setBuddy(logoOpacitySlider);
        label_48->setBuddy(logoOpacitySlider);
        label_24->setBuddy(marqPositionCombo);
        label_23->setBuddy(marqMarqueeText);
        label_16->setBuddy(eraseYSpin);
        label_17->setBuddy(eraseXSpin);
        label_15->setBuddy(eraseMaskText);
        label_35->setBuddy(blurFactorSlider);
        label_34->setBuddy(blurFactorSlider);
        label_36->setBuddy(blurFactorSlider);
        label_44->setBuddy(blurFactorSlider);
        label_45->setBuddy(blurFactorSlider);
        label_46->setBuddy(blurFactorSlider);
        label_47->setBuddy(blurFactorSlider);
        label_9->setBuddy(cloneCountSpin);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(adjustEnable, hueSlider);
        QWidget::setTabOrder(hueSlider, brightnessSlider);
        QWidget::setTabOrder(brightnessSlider, brightnessThresholdCheck);
        QWidget::setTabOrder(brightnessThresholdCheck, contrastSlider);
        QWidget::setTabOrder(contrastSlider, saturationSlider);
        QWidget::setTabOrder(saturationSlider, gammaSlider);
        QWidget::setTabOrder(gammaSlider, sharpenEnable);
        QWidget::setTabOrder(sharpenEnable, sharpenSigmaSlider);
        QWidget::setTabOrder(sharpenSigmaSlider, gradfunEnable);
        QWidget::setTabOrder(gradfunEnable, gradfunRadiusSlider);
        QWidget::setTabOrder(gradfunRadiusSlider, grainEnable);
        QWidget::setTabOrder(grainEnable, grainVarianceSlider);
        QWidget::setTabOrder(grainVarianceSlider, cropTopPx);
        QWidget::setTabOrder(cropTopPx, cropLeftPx);
        QWidget::setTabOrder(cropLeftPx, cropRightPx);
        QWidget::setTabOrder(cropRightPx, cropBotPx);
        QWidget::setTabOrder(cropBotPx, topBotCropSync);
        QWidget::setTabOrder(topBotCropSync, leftRightCropSync);
        QWidget::setTabOrder(leftRightCropSync, extractEnable);
        QWidget::setTabOrder(extractEnable, extractComponentText);
        QWidget::setTabOrder(extractComponentText, colorthresEnable);
        QWidget::setTabOrder(colorthresEnable, colorthresColorText);
        QWidget::setTabOrder(colorthresColorText, colorthresSaturationthresSlider);
        QWidget::setTabOrder(colorthresSaturationthresSlider, colorthresSimilaritythresSlider);
        QWidget::setTabOrder(colorthresSimilaritythresSlider, invertEnable);
        QWidget::setTabOrder(invertEnable, posterizeEnable);
        QWidget::setTabOrder(posterizeEnable, gradientEnable);
        QWidget::setTabOrder(gradientEnable, gradientModeCombo);
        QWidget::setTabOrder(gradientModeCombo, gradientTypeCheck);
        QWidget::setTabOrder(gradientTypeCheck, gradientCartoonCheck);
        QWidget::setTabOrder(gradientCartoonCheck, sepiaEnable);
        QWidget::setTabOrder(sepiaEnable, sepiaIntensitySpin);
        QWidget::setTabOrder(sepiaIntensitySpin, magnifyEnable);
        QWidget::setTabOrder(magnifyEnable, transformEnable);
        QWidget::setTabOrder(transformEnable, transformTypeCombo);
        QWidget::setTabOrder(transformTypeCombo, rotateEnable);
        QWidget::setTabOrder(rotateEnable, rotateAngleDial);
        QWidget::setTabOrder(rotateAngleDial, wallEnable);
        QWidget::setTabOrder(wallEnable, wallRowsSpin);
        QWidget::setTabOrder(wallRowsSpin, wallColsSpin);
        QWidget::setTabOrder(wallColsSpin, puzzleEnable);
        QWidget::setTabOrder(puzzleEnable, puzzleRowsSpin);
        QWidget::setTabOrder(puzzleRowsSpin, puzzleColsSpin);
        QWidget::setTabOrder(puzzleColsSpin, logoEnable);
        QWidget::setTabOrder(logoEnable, logoFileText);
        QWidget::setTabOrder(logoFileText, logoBrowseBtn);
        QWidget::setTabOrder(logoBrowseBtn, logoYSpin);
        QWidget::setTabOrder(logoYSpin, logoXSpin);
        QWidget::setTabOrder(logoXSpin, logoOpacitySlider);
        QWidget::setTabOrder(logoOpacitySlider, eraseEnable);
        QWidget::setTabOrder(eraseEnable, eraseMaskText);
        QWidget::setTabOrder(eraseMaskText, eraseBrowseBtn);
        QWidget::setTabOrder(eraseBrowseBtn, eraseYSpin);
        QWidget::setTabOrder(eraseYSpin, eraseXSpin);
        QWidget::setTabOrder(eraseXSpin, marqEnable);
        QWidget::setTabOrder(marqEnable, marqMarqueeText);
        QWidget::setTabOrder(marqMarqueeText, marqPositionCombo);
        QWidget::setTabOrder(marqPositionCombo, antiflickerEnable);
        QWidget::setTabOrder(antiflickerEnable, antiflickerSofteningSizeSlider);
        QWidget::setTabOrder(antiflickerSofteningSizeSlider, motionblurEnable);
        QWidget::setTabOrder(motionblurEnable, blurFactorSlider);
        QWidget::setTabOrder(blurFactorSlider, gaussianblurEnable);
        QWidget::setTabOrder(gaussianblurEnable, gaussianblurSigmaSlider);
        QWidget::setTabOrder(gaussianblurSigmaSlider, cloneEnable);
        QWidget::setTabOrder(cloneEnable, cloneCountSpin);
        QWidget::setTabOrder(cloneCountSpin, hqdn3dEnable);
        QWidget::setTabOrder(hqdn3dEnable, hqdn3dLumaSpatSlider);
        QWidget::setTabOrder(hqdn3dLumaSpatSlider, hqdn3dLumaTempSlider);
        QWidget::setTabOrder(hqdn3dLumaTempSlider, hqdn3dChromaSpatSlider);
        QWidget::setTabOrder(hqdn3dChromaSpatSlider, hqdn3dChromaTempSlider);
        QWidget::setTabOrder(hqdn3dChromaTempSlider, anaglyphEnable);
        QWidget::setTabOrder(anaglyphEnable, mirrorEnable);
        QWidget::setTabOrder(mirrorEnable, psychedelicEnable);
        QWidget::setTabOrder(psychedelicEnable, waveEnable);
        QWidget::setTabOrder(waveEnable, rippleEnable);
        QWidget::setTabOrder(rippleEnable, motiondetectEnable);

        retranslateUi(ExtVideoWidget);

        ExtVideoWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ExtVideoWidget);
    } // setupUi

    void retranslateUi(QTabWidget *ExtVideoWidget)
    {
        adjustEnable->setTitle(Q_("Image adjust", nullptr));
        label->setText(Q_("Hue", nullptr));
        label_3->setText(Q_("Brightness", nullptr));
        brightnessThresholdCheck->setText(Q_("Brightness Threshold", nullptr));
        label_2->setText(Q_("Contrast", nullptr));
        label_4->setText(Q_("Saturation", nullptr));
        label_5->setText(Q_("Gamma", nullptr));
        sharpenEnable->setTitle(Q_("Sharpen", nullptr));
        label_29->setText(Q_("Sigma", nullptr));
        gradfunEnable->setTitle(Q_("Banding removal", nullptr));
        label_32->setText(Q_("Radius", nullptr));
        grainEnable->setTitle(Q_("Film Grain", nullptr));
        label_50->setText(Q_("Variance", nullptr));
        ExtVideoWidget->setTabText(ExtVideoWidget->indexOf(tab), Q_("Essential", nullptr));
        label_10->setText(Q_("Top", nullptr));
        cropTopPx->setSuffix(Q_(" px", nullptr));
        label_11->setText(Q_("Left", nullptr));
        label_12->setText(Q_("Right", nullptr));
        label_7->setText(Q_("Bottom", nullptr));
        cropBotPx->setSuffix(Q_(" px", nullptr));
        topBotCropSync->setText(Q_("Synchronize top and bottom", nullptr));
        leftRightCropSync->setText(Q_("Synchronize left and right", nullptr));
        cropLeftPx->setSuffix(Q_(" px", nullptr));
        cropRightPx->setSuffix(Q_(" px", nullptr));
        ExtVideoWidget->setTabText(ExtVideoWidget->indexOf(tab_3), Q_("Crop", nullptr));
        extractEnable->setTitle(Q_("Color extraction", nullptr));
        label_6->setText(Q_("Color", nullptr));
        extractComponentText->setInputMask(Q_(">HHHHHH;#", nullptr));
        invertEnable->setText(Q_("Negate colors", nullptr));
        posterizeEnable->setText(Q_("Posterize", nullptr));
        gradientEnable->setTitle(Q_("Gradient", nullptr));
        label_25->setText(Q_("Mode", nullptr));
        gradientTypeCheck->setText(Q_("Color", nullptr));
        gradientCartoonCheck->setText(Q_("Cartoon", nullptr));
        colorthresEnable->setTitle(Q_("Color threshold", nullptr));
        label_8->setText(Q_("Color", nullptr));
        colorthresColorText->setInputMask(Q_(">HHHHHH;#", nullptr));
        label_13->setText(Q_("Saturation", nullptr));
        label_14->setText(Q_("Similarity", nullptr));
        sepiaEnable->setTitle(Q_("Sepia", nullptr));
        label_18->setText(Q_("Intensity", nullptr));
        ExtVideoWidget->setTabText(ExtVideoWidget->indexOf(tab_2), Q_("Colors", nullptr));
        magnifyEnable->setText(Q_("Interactive Zoom", nullptr));
        wallEnable->setTitle(Q_("Wall", nullptr));
        label_31->setText(Q_("Rows", nullptr));
        label_30->setText(Q_("Columns", nullptr));
        transformEnable->setTitle(Q_("Transform", nullptr));
        rotateEnable->setTitle(Q_("Rotate", nullptr));
        label_28->setText(Q_("Angle", nullptr));
        puzzleEnable->setTitle(Q_("Puzzle game", nullptr));
        label_27->setText(Q_("Rows", nullptr));
        label_26->setText(Q_("Columns", nullptr));
        ExtVideoWidget->setTabText(ExtVideoWidget->indexOf(tab_5), Q_("Geometry", nullptr));
        logoEnable->setTitle(Q_("Add logo", nullptr));
        label_19->setText(Q_("Logo", nullptr));
        logoBrowseBtn->setText(Q_("...", nullptr));
        label_20->setText(Q_("Top", nullptr));
        logoYSpin->setSuffix(Q_(" px", nullptr));
        label_21->setText(Q_("Left", nullptr));
        logoXSpin->setSuffix(Q_(" px", nullptr));
        label_49->setText(Q_("full", nullptr));
        label_22->setText(Q_("Opacity", nullptr));
        label_48->setText(Q_("none", nullptr));
        marqEnable->setTitle(Q_("Add text", nullptr));
        label_24->setText(Q_("Position", nullptr));
        label_23->setText(Q_("Text", nullptr));
        eraseEnable->setTitle(Q_("Logo erase", nullptr));
        label_16->setText(Q_("Top", nullptr));
        label_17->setText(Q_("Left", nullptr));
        eraseXSpin->setSuffix(Q_(" px", nullptr));
        eraseYSpin->setSuffix(Q_(" px", nullptr));
        label_15->setText(Q_("Mask", nullptr));
        eraseBrowseBtn->setText(Q_("...", nullptr));
        ExtVideoWidget->setTabText(ExtVideoWidget->indexOf(tab_8), Q_("Overlay", nullptr));
        anaglyphEnable->setText(Q_("Anaglyph 3D", nullptr));
        mirrorEnable->setText(Q_("Mirror", nullptr));
        psychedelicEnable->setText(Q_("Psychedelic", nullptr));
        waveEnable->setText(Q_("Waves", nullptr));
        rippleEnable->setText(Q_("Water effect", nullptr));
        motiondetectEnable->setText(Q_("Motion detect", nullptr));
        gaussianblurEnable->setTitle(Q_("Spatial blur", nullptr));
        label_35->setText(Q_("Sigma", nullptr));
        motionblurEnable->setTitle(Q_("Motion blur", nullptr));
        label_34->setText(Q_("Factor", nullptr));
        antiflickerEnable->setTitle(Q_("Anti-Flickering", nullptr));
        label_36->setText(Q_("Soften", nullptr));
        hqdn3dEnable->setTitle(Q_("Denoiser", nullptr));
        label_44->setText(Q_("Spatial luma strength", nullptr));
        label_45->setText(Q_("Temporal luma strength", nullptr));
        label_46->setText(Q_("Spatial chroma strength", nullptr));
        label_47->setText(Q_("Temporal chroma strength", nullptr));
        cloneEnable->setTitle(Q_("Clone", nullptr));
        label_9->setText(Q_("Number of clones", nullptr));
        ExtVideoWidget->setTabText(ExtVideoWidget->indexOf(tab_9), Q_("Advanced", nullptr));
        (void)ExtVideoWidget;
    } // retranslateUi

};

namespace Ui {
    class ExtVideoWidget: public Ui_ExtVideoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // VIDEO_EFFECTS_H

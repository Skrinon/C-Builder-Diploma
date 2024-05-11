#ifndef Unit1H
#define Unit1H
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <map>
#include <vector>
#include <cmath>
#include <numeric>
class TForm1 : public TForm
{
__published:    // IDE-managed Components
	TRadioButton *RadioButtonError; // ���������� ��� ������ ������ ���������� �������
	TRadioButton *RadioButtonUncertainty; // ���������� ��� ������ ������ ���������� �������������
	TLabel *Label1; // ̳��� ��� ����������� ��������� ������
	TButton *ButtonLoadData;  // ������ ��� ������������ �����
	TButton *ButtonLoadQuantiles; // ������ ��� ������������ ��������
	TOpenDialog *OpenDialogData;  // ĳ������� ���� ��� ������ ����� �����
	TOpenDialog *OpenDialogQuantiles; // ĳ������� ���� ��� ������ ����� ��������
	TLabel *LabelDataFile; // ̳��� ��� ����������� ���� ������������� ����� �����
	TLabel *LabelQuantilesFile;  // ̳��� ��� ����������� ���� ������������� ����� ��������
	TButton *ButtonCalculate; // ������ ��� ��������� ����������
	TChart *ChartError;  // ������ ��� ����������� ����������
	TBarSeries *BarSeriesError; // ���� ��� ��������� ������� �� �������
	TMemo *MemoResults; // �������� ���� ��� ������ ���������� � ����������
	TFastLineSeries *Series1; // ���� ��� ����� ������� �� �������
	 // ��������� ����
	void __fastcall RadioButtonErrorClick(TObject *Sender); // �������� ��䳿 ���� �� ���������� "���������� �������"
	void __fastcall RadioButtonUncertaintyClick(TObject *Sender); // �������� ��䳿 ���� �� ���������� "���������� �������������"
	void __fastcall ButtonLoadDataClick(TObject *Sender); // �������� ��䳿 ���� �� ������ ������������ �����
	void __fastcall ButtonLoadQuantilesClick(TObject *Sender); // �������� ��䳿 ���� �� ������ ������������ ��������
    void __fastcall ButtonCalculateClick(TObject *Sender); // �������� ��䳿 ���� �� ������ ����������
private:
	std::vector<double> data; // ����� ����� ��� �������
	std::map<int, double> quantiles; // ���� �������� ��� ����������

	 // ������� ������ ��� ������� �����
	void loadData(const UnicodeString& filename); // ����� ��� ������������ ����� � �����
	void loadQuantiles(const UnicodeString& filename);  // ����� ��� ������������ �������� � �����
	void calculateError();  // ����� ��� ���������� �������
	void PerformCalculations();  // ����� ��� ��������� ���������� �������������
	double calculateMean(const std::vector<double>& values); // ����� ��� ���������� ���������� ��������
	double calculateStdDev(const std::vector<double>& values, double mean); // ����� ��� ���������� ������������ ���������
	double calculateConfidenceInterval(double stdDev, int sampleSize);  // ����� ��� ���������� �������� ���������
	double calculateTypeAUncertainty(const std::vector<double>& values, double mean); // ����� ��� ���������� ������������� ���� �
	double calculateTypeBUncertainty(double a_minus, double a_plus, char distributionType);  // ����� ��� ���������� ������������� ���� �
	double calculateCombinedUncertainty(const std::vector<double>& x, const std::vector<double>& y, double uAX, double uAY); // ����� ��� ���������� ���������� �������������
	double calculateCorrelation(const std::vector<double>& x, const std::vector<double>& y); // ����� ��� ���������� ��������� �� ����� �������� �������
	std::vector<int> countIntervals(const std::vector<double>& data, double minVal, double maxVal, int numIntervals);   // ����� ��� ��������� ������� ������� � ����������
	std::vector<double> createNormalCurve(double mean, double stdDev, double minVal, double maxVal, int numPoints); // ����� ��� ��������� ��������� �����
	void updateChartError(); // ����� ��� ��������� ������� �������
    void updateChartUncertainty(double uA, double uB, double combinedUncertainty); // ����� ��� ��������� ������� �������������
public:
    __fastcall TForm1(TComponent* Owner);
};
extern PACKAGE TForm1 *Form1;
#endif

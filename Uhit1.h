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
	TRadioButton *RadioButtonError; // Радіокнопка для вибору режиму розрахунку похибки
	TRadioButton *RadioButtonUncertainty; // Радіокнопка для вибору режиму розрахунку невизначеності
	TLabel *Label1; // Мітка для відображення вибраного режиму
	TButton *ButtonLoadData;  // Кнопка для завантаження даних
	TButton *ButtonLoadQuantiles; // Кнопка для завантаження квантилів
	TOpenDialog *OpenDialogData;  // Діалогове вікно для вибору файлу даних
	TOpenDialog *OpenDialogQuantiles; // Діалогове вікно для вибору файлу квантилів
	TLabel *LabelDataFile; // Мітка для відображення імені завантаженого файлу даних
	TLabel *LabelQuantilesFile;  // Мітка для відображення імені завантаженого файлу квантилів
	TButton *ButtonCalculate; // Кнопка для виконання розрахунків
	TChart *ChartError;  // Графік для відображення результатів
	TBarSeries *BarSeriesError; // Серія для стовпчатої діаграми на графіку
	TMemo *MemoResults; // Текстове поле для виводу результатів і повідомлень
	TFastLineSeries *Series1; // Серія для лінійної діаграми на графіку
	 // Обробники подій
	void __fastcall RadioButtonErrorClick(TObject *Sender); // Обробник події кліку на радіокнопку "Розрахунок похибки"
	void __fastcall RadioButtonUncertaintyClick(TObject *Sender); // Обробник події кліку на радіокнопку "Розрахунок невизначеності"
	void __fastcall ButtonLoadDataClick(TObject *Sender); // Обробник події кліку на кнопку завантаження даних
	void __fastcall ButtonLoadQuantilesClick(TObject *Sender); // Обробник події кліку на кнопку завантаження квантилів
    void __fastcall ButtonCalculateClick(TObject *Sender); // Обробник події кліку на кнопку розрахунків
private:
	std::vector<double> data; // Масив даних для обробки
	std::map<int, double> quantiles; // Мапа квантилів для розрахунків

	 // Приватні методи для обробки даних
	void loadData(const UnicodeString& filename); // Метод для завантаження даних з файлу
	void loadQuantiles(const UnicodeString& filename);  // Метод для завантаження квантилів з файлу
	void calculateError();  // Метод для розрахунку похибки
	void PerformCalculations();  // Метод для виконання розрахунків невизначеності
	double calculateMean(const std::vector<double>& values); // Метод для розрахунку середнього значення
	double calculateStdDev(const std::vector<double>& values, double mean); // Метод для розрахунку стандартного відхилення
	double calculateConfidenceInterval(double stdDev, int sampleSize);  // Метод для розрахунку довірчого інтервалу
	double calculateTypeAUncertainty(const std::vector<double>& values, double mean); // Метод для розрахунку невизначеності типу А
	double calculateTypeBUncertainty(double a_minus, double a_plus, char distributionType);  // Метод для розрахунку невизначеності типу В
	double calculateCombinedUncertainty(const std::vector<double>& x, const std::vector<double>& y, double uAX, double uAY); // Метод для розрахунку комбінованої невизначеності
	double calculateCorrelation(const std::vector<double>& x, const std::vector<double>& y); // Метод для розрахунку кореляції між двома масивами значень
	std::vector<int> countIntervals(const std::vector<double>& data, double minVal, double maxVal, int numIntervals);   // Метод для підрахунку кількості значень в інтервалах
	std::vector<double> createNormalCurve(double mean, double stdDev, double minVal, double maxVal, int numPoints); // Метод для створення нормальної кривої
	void updateChartError(); // Метод для оновлення графіка похибки
    void updateChartUncertainty(double uA, double uB, double combinedUncertainty); // Метод для оновлення графіка невизначеності
public:
    __fastcall TForm1(TComponent* Owner);
};
extern PACKAGE TForm1 *Form1;
#endif

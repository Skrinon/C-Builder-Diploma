#include "Unit1.h"
#include <vcl.h>
#pragma hdrstop
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <numeric> // Для std::accumulate и std::max_element
#include <cmath>   // Для функций exp и sqrt
#include <algorithm> // Для std::min_element и std::max_element
#include <utility> // Для std::pair
#include <System.SysUtils.hpp>
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
// Конструктор форми, ініціалізує компоненти форми
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {}
// Обробник події кліку на радіокнопку "Розрахунок похибки"
void __fastcall TForm1::RadioButtonErrorClick(TObject *Sender) {
  Label1->Caption = "Режим: Розрахунок похибки";
}
// Обробник події кліку на радіокнопку "Розрахунок невизначеності"
void __fastcall TForm1::RadioButtonUncertaintyClick(TObject *Sender) {
    Label1->Caption = "Режим: Розрахунок невизначеності";
}
// Обробник події кліку для завантаження даних через діалогове вікно
void __fastcall TForm1::ButtonLoadDataClick(TObject *Sender) {
    if (OpenDialogData->Execute()) {
    LabelDataFile->Caption = "Обрано: " + ExtractFileName(OpenDialogData->FileName);
        loadData(OpenDialogData->FileName);
    }
}
// Обробник події кліку для завантаження квантилів через діалогове вікно
void __fastcall TForm1::ButtonLoadQuantilesClick(TObject *Sender) {
    if (OpenDialogQuantiles->Execute()) {
        LabelQuantilesFile->Caption = "Обрано: " + ExtractFileName(OpenDialogQuantiles->FileName);
        loadQuantiles(OpenDialogQuantiles->FileName);
    }
}
// Обробник події кліку кнопки розрахунку, визначає який режим вибрано і викликає відповідну функцію
void __fastcall TForm1::ButtonCalculateClick(TObject *Sender) {
    if (RadioButtonError->Checked) {
        calculateError();
    } else if (RadioButtonUncertainty->Checked) {
    PerformCalculations();
    }
}
void TForm1::loadData(const UnicodeString& filename) {
    std::ifstream file(filename.c_str());
    std::string line;
    data.clear();
    while (getline(file, line)) {
		// Замінюємо коми на крапки для коректного читання десяткових значень
        std::replace(line.begin(), line.end(), ',', '.');
        std::istringstream ss(line);
        std::string value;
		while (getline(ss, value, ' ')) { // Використовуємо пробіл як роздільник
            if (!value.empty()) {
                try {
                    data.push_back(std::stod(value));
                } catch (const std::exception& e) {
					MemoResults->Lines->Add("Помилка під час читання даних із файлу: " + String(value.c_str()) + ", ошибка: " + e.what());
				}
            }
        }
    }
    if (!data.empty()) {
		MemoResults->Lines->Add("Дані успішно завантажено.");
	} else {
		MemoResults->Lines->Add("Файл даних порожній або містить некоректні значення.");
    }
}



// Завантажує квантилі з файлу і зберігає їх у мапі 'quantiles'
void TForm1::loadQuantiles(const UnicodeString& filename) {
    std::ifstream file(filename.c_str());
    std::string line;
    quantiles.clear();
	if (!file.is_open()) {
		MemoResults->Lines->Add("Не вдалося відкрити файл квантилів.");
		return;
    }
    while (getline(file, line)) {
		std::replace(line.begin(), line.end(), ',', '.'); // Замінюємо коми на крапки
        std::istringstream ss(line);
		std::string n_str, t_str;
        while (getline(ss, n_str, ' ') && getline(ss, t_str, ' ')) {
            try {
                int n = std::stoi(n_str);
                double t = std::stod(t_str);
                quantiles[n] = t;
            } catch (const std::exception& e) {
			}
        }
    }
    if (!quantiles.empty()) {
		MemoResults->Lines->Add("Квантилі успішно завантажено.");
    } else {
		MemoResults->Lines->Add("Файл квантилів порожній або всі рядки некоректні.");
    }
}

std::vector<int> TForm1::countIntervals(const std::vector<double>& data, double minVal, double maxVal, int numIntervals) {
    std::vector<int> counts(numIntervals, 0);
    double intervalWidth = (maxVal - minVal) / numIntervals;
	for (double value : data) {
        int index = std::min(static_cast<int>((value - minVal) / intervalWidth), numIntervals - 1);
        counts[index]++;
	}
    return counts;
}

std::vector<double> TForm1::createNormalCurve(double mean, double stdDev, double minVal, double maxVal, int numPoints) {
    std::vector<double> curve(numPoints);
    double range = maxVal - minVal;
    for (int i = 0; i < numPoints; ++i) {
        double x = minVal + range * i / (numPoints - 1);
        double exponent = -0.5 * pow((x - mean) / stdDev, 2);
        curve[i] = (1 / (stdDev * sqrt(2 * M_PI))) * exp(exponent);
	}
    return curve;
}

// Розраховує похибку на основі завантажених даних
void TForm1::calculateError() {
    if (data.empty()) {
        MemoResults->Lines->Add("Дані не завантажені.");
        return;
    }
    double mean = calculateMean(data);
    double stdDev = calculateStdDev(data, mean);
    int sampleSize = static_cast<int>(data.size());
	double quantile = (quantiles.find(sampleSize) != quantiles.end()) ? quantiles[sampleSize] : std::nan("1");
    double confidenceInterval = calculateConfidenceInterval(stdDev, sampleSize);
  MemoResults->Lines->Add("Середнє значення: " + FloatToStr(mean));
  MemoResults->Lines->Add("Середньоквадратичне відхилення: " + FloatToStr(stdDev));
  MemoResults->Lines->Add("Довірчий інтервал: " + FloatToStr(confidenceInterval));
	updateChartError();
}
// Оновлює гістограму похибки
void TForm1::updateChartError() {
    if (data.empty()) {
		MemoResults->Lines->Add("Дані не завантажено.");
        return;
    }
    double minVal = *std::min_element(data.begin(), data.end());
	double maxVal = *std::max_element(data.begin(), data.end());
	const int numIntervals = 10; // Кількість інтервалів
    double intervalWidth = (maxVal - minVal) / numIntervals;
    std::vector<int> counts(numIntervals, 0);
    for (double value : data) {
        int index = std::min(int((value - minVal) / intervalWidth), numIntervals - 1);
        counts[index]++;
    }
	// Очищення графіка
    ChartError->RemoveAllSeries();
    BarSeriesError->Clear();
	 // Заповнення стовпчастої діаграми
    for (int i = 0; i < numIntervals; ++i) {
        BarSeriesError->AddXY(minVal + i * intervalWidth + intervalWidth / 2, counts[i], "", clTeeColor);
    }
   // Додавання стовпчастої діаграми в графік
    ChartError->AddSeries(BarSeriesError);
	// Підготовка даних для нормальної кривої
	std::vector<double> curve(1000); // 1000 точок для плавності кривої
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    double stdDev = std::sqrt(std::inner_product(data.begin(), data.end(), data.begin(), 0.0) / data.size() - mean * mean);
    for (int i = 0; i < 1000; ++i) {
        double x = minVal + (maxVal - minVal) * i / 999;
        double exponent = -0.5 * std::pow((x - mean) / stdDev, 2);
        curve[i] = (1 / (stdDev * std::sqrt(2 * M_PI))) * std::exp(exponent);
    }
	  // Нормалізація та додавання нормальної кривої
    double maxCurve = *std::max_element(curve.begin(), curve.end());
    double maxCount = *std::max_element(counts.begin(), counts.end());
    for (double &value : curve) {
		value = (value / maxCurve) * maxCount; // Нормалізація до максимального значення стовпця
    }
    Series1->Clear();
    for (int i = 0; i < 1000; ++i) {
        double x = minVal + (maxVal - minVal) * i / 999;
        Series1->AddXY(x, curve[i], "", clTeeColor);
    }
  // Додавання кривої на графік
    ChartError->AddSeries(Series1);
	// Оновлення графіка
    ChartError->Repaint();
    MemoResults->Lines->Add("График погрешности обновлен.");
}




  // Виконує розрахунки для визначення невизначеності
void TForm1::PerformCalculations() {
	if (data.empty()) {
		MemoResults->Lines->Add("Дані не завантажені.");
		return;
	}
	double mean = calculateMean(data);
	double uA = calculateTypeAUncertainty(data, mean);
	double uB = calculateTypeBUncertainty(0.1, 0.2, 'U');  // Приблизні значення для демонстрації
	double combinedUncertainty = calculateCombinedUncertainty(data, data, uA, uB);

  MemoResults->Lines->Add("Середнє значення: " + FloatToStr(mean));
  MemoResults->Lines->Add("Невизначеність типу A: " + FloatToStr(uA));
  MemoResults->Lines->Add("Невизначеність типу В: " + FloatToStr(uB));
	MemoResults->Lines->Add("Комбінована невизначеність: " + FloatToStr(combinedUncertainty));

  updateChartUncertainty(uA, uB, combinedUncertainty);
}
// Оновлює гістограму невизначеності
void TForm1::updateChartUncertainty(double uA, double uB, double combinedUncertainty) {
	ChartError->RemoveAllSeries(); // Очищення всіх попередніх серій на графіку
	const int numPoints = 1000; // Кількість точок для гладкості кривої
	double mean = calculateMean(data);
    double stdDev = calculateStdDev(data, mean);
    double minVal = *std::min_element(data.begin(), data.end());
    double maxVal = *std::max_element(data.begin(), data.end());
	const int numIntervals = 10; // Кількість інтервалів для гістограми
    std::vector<int> counts = countIntervals(data, minVal, maxVal, numIntervals);
    std::vector<double> curve = createNormalCurve(mean, stdDev, minVal, maxVal, numPoints);
	// Нормалізація та масштабування кривої
    double maxCurve = *std::max_element(curve.begin(), curve.end());
    double maxCount = *std::max_element(counts.begin(), counts.end());
    for (double &value : curve) {
        value = (value / maxCurve) * maxCount;
    }
	 // Додавання стовпчастої діаграми
    TBarSeries *barSeries = new TBarSeries(this);
    double intervalWidth = (maxVal - minVal) / numIntervals;
    for (int i = 0; i < numIntervals; ++i) {
        barSeries->AddXY(minVal + i * intervalWidth + intervalWidth / 2, counts[i], "", clTeeColor);
    }
    ChartError->AddSeries(barSeries);
	 // Додавання нормалізованої нормальної кривої
    TFastLineSeries *lineSeries = new TFastLineSeries(this);
    lineSeries->SeriesColor = clRed;
    for (int i = 0; i < numPoints; ++i) {
        double x = minVal + (maxVal - minVal) * i / (numPoints - 1);
        lineSeries->AddXY(x, curve[i], "", clTeeColor);
    }
    ChartError->AddSeries(lineSeries);
	ChartError->Repaint(); // Оновлюємо графік для відображення всіх змін
}

   // Розраховує середнє значення з набору значень
double TForm1::calculateMean(const std::vector<double>& values) {
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}
  // Розраховує середньоквадратичне відхилення з набору значень
double TForm1::calculateStdDev(const std::vector<double>& values, double mean) {
    double sum = std::accumulate(values.begin(), values.end(), 0.0, [mean](double a, double b) { return a + (b - mean) * (b - mean); });
	return std::sqrt(sum / (values.size() - 1));
}
  // Розраховує довірчий інтервал
double TForm1::calculateConfidenceInterval(double stdDev, int sampleSize) {
	if (sampleSize < 2) {
		MemoResults->Lines->Add("Недостатньо даних для розрахунку довірчого інтервалу.");
        return NAN;  // Повертаємо NAN для індикації проблеми
    }
    double quantile = 0.0;
    if (quantiles.find(sampleSize) != quantiles.end()) {
        quantile = quantiles[sampleSize];
    } else {
        MemoResults->Lines->Add("Квантиль для заданного размера выборки не найден.");
		return NAN;  // Повертаємо NAN, якщо квантиль не знайдено
    }
    if (quantile == 0.0) {
		MemoResults->Lines->Add("Некоректне значення квантиля.");
		return NAN;
    }
    return quantile * stdDev / std::sqrt(static_cast<double>(sampleSize));
}



   // Розраховує невизначеність типу А
double TForm1::calculateTypeAUncertainty(const std::vector<double>& values, double mean) {
    double sum = 0.0;
    for (auto value : values) {
        sum += std::pow(value - mean, 2);
    }
    return std::sqrt(sum / (values.size() * (values.size() - 1)));
}
   // Розраховує невизначеність типу В
double TForm1::calculateTypeBUncertainty(double a_minus, double a_plus, char distributionType) {
    double u_B = 0.0;
    switch (distributionType) {
        case 'U':
            u_B = (a_plus - a_minus) / std::sqrt(12);
            break;
        case 'T':
            u_B = (a_plus - a_minus) / std::sqrt(24);
            break;
        case 'R': {
	  double beta = 0.5;  // Оголосити всередині регістру або перед switch, якщо використовується ззовні
            u_B = (a_plus - a_minus) / (std::sqrt(24) * std::sqrt(1 + beta * beta));
            break;
        }
        case 'E': {
            double lambda = 1.0;  // Оголосити внутрішній регістр
            double x = (a_plus + a_minus) / 2;
            u_B = std::sqrt(((a_plus - x) * (x - a_minus) - ((a_plus - 2 * x + a_minus) / lambda)));
            break;
        }
    }
    return u_B;
}
  // Розраховує комбіновану невизначеність
double TForm1::calculateCombinedUncertainty(const std::vector<double>& x, const std::vector<double>& y, double uAX, double uAY) {
  double dydx = 1.0;
  double cov_xy = calculateCorrelation(x, y) * uAX * uAY;
    double combinedUncertaintySquared = dydx * dydx * (uAX * uAX + uAY * uAY) + 2 * dydx * cov_xy;
    return std::sqrt(combinedUncertaintySquared);
}

// Розраховує кореляцію між двома наборами значень
double TForm1::calculateCorrelation(const std::vector<double>& x, const std::vector<double>& y) {
    double mean_x = calculateMean(x);
    double mean_y = calculateMean(y);
  double sum_xy = 0.0, sum_xx = 0.0, sum_yy = 0.0;

    for (size_t i = 0; i < x.size(); ++i) {
        sum_xy += (x[i] - mean_x) * (y[i] - mean_y);
        sum_xx += (x[i] - mean_x) * (x[i] - mean_x);
        sum_yy += (y[i] - mean_y) * (y[i] - mean_y);
    }
   // Перевірка виконання множення правильно
    double denominator = std::sqrt(sum_xx * sum_yy); // Ensure multiplication is correctly handled
    if (denominator == 0) return std::nan("1");


    return sum_xy / denominator;     // Повертає результат розрахунку кореляції
}


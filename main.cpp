#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

struct StockData {
    std::string date;
    double close;

};

std::vector<StockData> readCSV(const std::string& filename) {
    std::vector<StockData> data;
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line); // Skip header line

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, priceStr;

        std::getline(ss, date, ','); //Date
        std::getline(ss, priceStr, ','); //Price(close)

        try {
            double price = std::stod(priceStr);
            data.push_back({date, price});
        } catch (...) {
            continue; // Skip invalid lines
        }
    //    opk
        
    }

    std::reverse(data.begin(), data.end()); // Reverse the order of the data

    return data;
}

double calculateSMA(const std::vector<StockData>& data, int start, int period) {
    double sum = 0.0;
    for (int i = start; i < start + period; ++i) {
        sum += data[i].close;
    }
    return sum/period;
    
}

int main(){
    std::vector<StockData> data = readCSV("AAPL.csv");

    const int SMA_short = 5;
    const int SMA_long = 20;
    bool holding = false;
    double buyPrice = 0.0;
    double profit = 0.0;

    for (size_t i = 0; i < data.size() - SMA_long; ++i) {
        double shortAvg = calculateSMA(data, i, SMA_short);
        double longAvg = calculateSMA(data, i, SMA_long);
        double currentPrice = data[i + SMA_long].close;
        std::string cUrrentDate = data[i + SMA_long].date;

        if (!holding && shortAvg> longAvg) {
            holding = true;
            buyPrice = currentPrice;
            std::cout << "Buy on " << cUrrentDate << " at price: " << buyPrice << std::endl;
        } else if (holding && shortAvg < longAvg) {
            holding = false;
            double sellprice = currentPrice;
            double tradeProfit = sellprice - buyPrice;
            profit += tradeProfit;                  
            std::cout << "Sell on " << cUrrentDate << " at price: " << currentPrice << std::endl;
        }
    }

    std::cout <<" \nTotal profit: " << profit << std::endl;
    return 0;
    std::cout << "Program started. Data size: " << data.size() << std::endl;
    std::cout << "SMA Short: " << SMA_short << std::endl;
    std::cout << "SMA Long: " << SMA_long << std::endl;
    std::cout << "Data read successfully." << std::endl;
    std::cout << "Data processing completed." << std::endl;


}

#include <bits/stdc++.h>

using namespace std;

void printFile(vector<vector<double>> &data){
    for(int i = 0; i < data.size(); i++){
        for(int j = 0; j < data[i].size(); j++){
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
}

void readFile(vector<vector<double>> &data, string &fileName){
    ifstream file (fileName);
    string curRow;
    double curValue;

    while(getline(file, curRow)){
        stringstream row(curRow);
        vector<double> temp;
        while(row >> curValue){
            temp.push_back(curValue);
        }
        data.push_back(temp);
    }
    file.close();
}

int main(int argc, char* argv[]){
    int dataSize = 0;
    int dataFileNumber = 0;
    vector<vector<double>> data;
    vector<string> fileSizeName = {"small", "large", "XXlarge"};

    //Get data file size + data validation
    cout << "What size dataset are you planning to use?" << endl;
    cout << "1: small" << endl;
    cout << "2: large" << endl;
    cout << "3: XXXLarge" << endl;

    cin >> dataSize;
    while(dataSize < 1 || dataSize > 3){
        cout << "That is not a valid input, please try again." << endl;
        cin >> dataSize;
    }

    //Get File number + data validation
    switch(dataSize){
        case 3:
            do{
                cout << "What dataset file do you want to use?" << endl;
                cout << "Enter a valid number from 1-24: " << endl;
                cin >> dataFileNumber;
            } while(dataFileNumber < 1 || dataFileNumber > 24);
            break;
        default:
            do{
                cout << "What dataset file do you want to use?" << endl;
                cout << "Enter a valid number from 1-33: " << endl;
                cin >> dataFileNumber;
            } while(dataFileNumber < 1 || dataFileNumber > 33);
            break;
    }

    //Print data result
    string fileName = "Datasets/CS170_" + fileSizeName[dataSize-1] + "_Data__" + to_string(dataFileNumber) + ".txt";

    cout << fileName << endl;

    readFile(data, fileName);
    printFile(data);

    return 0;
}
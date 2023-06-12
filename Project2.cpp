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

int nearestNeighbor(vector<vector<double>> &data, vector<double> &dataPoint){           //return the index of the nearest neighbor
    cout << "Data Size: " << data.size() << endl;
    double closestDistance = __SIZEOF_DOUBLE__;
    int indexOfClosest = -1;
    double sum = 0;
    double curDist = 0;
    int len = data[0].size(); 

    for(int i = 0; i < data.size(); i++){
        sum = 0;
        for(int j = 1; j < len; j++){
            sum += pow(data[i][j] - dataPoint[j], 2);
        }

        curDist = sqrt(sum);
        if(curDist < closestDistance){
            closestDistance = curDist;
            indexOfClosest = i;
        }
        cout << " index: " << i << " distance: " << curDist << endl; 
    }
    return indexOfClosest;
}

void leaveOneOutValidator(vector<vector<double>> data, int row){    //data is passed by value (a local copy is created).  given a row to compare the remaining rows to, find the accuracy of the remaining rows
    //int rlen = data.size();
    //int clen = data.size();
    //int r, c;
    vector<vector<double>> leftOutFeat;
    /*
    for(r = 0; r < rlen; r++){
        for(c = 0; c < clen; c++){
            if(c == (col+1)){                                       //ignore the specified column
                leftOutFeat.push_back(data[r][c]);
                continue;
            }
            remainingFeats.push_back(data[r][c]);
            //cout << data[r][c] << "  ";
        }
        cout << '\n';
    }
    */
    leftOutFeat.push_back(data[row]);
    for(int i = 0; i < leftOutFeat.size(); i++){
        printf("leftOutFeat[%d] = %p", i, leftOutFeat[i]);
    }
    data.erase(data.begin() + row);                                 //note: make sure that original data is not be changed
}

bool ifSameClassLabel(double predclass, vector<vector<double>> &data, double instnum){      //check if predicted class is the same as the actual class stored in the first column of data
    return fabs(predclass - data[instnum][0]) < 0.01;
}

int main(int argc, char* argv[]){
    int dataSize = 0;
    int dataFileNumber = 0;
    string fileName;
    vector<vector<double>> data;
    vector<string> fileSizeName = {"small", "large", "XXlarge"};

    //Get data file size + data validation
    cout << "What size dataset are you planning to use?" << endl;
    cout << "1: small" << endl;
    cout << "2: large" << endl;
    cout << "3: XXXLarge" << endl;
    cout << "4: Custom Test Data" << endl;

    cin >> dataSize;
    while(dataSize < 1 || dataSize > 4){
        cout << "That is not a valid input, please try again." << endl;
        cin >> dataSize;
    }

    if(dataSize != 4){
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

        //Testing File Name
        fileName = "Datasets/CS170_" + fileSizeName[dataSize-1] + "_Data__" + to_string(dataFileNumber) + ".txt";
        cout << fileName << endl;
    }
    else{
        fileName = "Datasets/TestData.txt";
    }

    //Testing readFile
    readFile(data, fileName);
    printFile(data);

    //Testing NearestNeighbor
    vector<double> testDataPoint = {2, 1, 2, 3, 4};
    int nearestNeighborindex = nearestNeighbor(data, testDataPoint);
    cout << "Nearest Neighbor Testing Index: " << nearestNeighborindex << endl;

    //forwadSelection(data);
    leaveOneOutValidator(data, 0);
    cout << '\n' << ifSameClassLabel(1, data, 0) << '\n';
    cout << '\n' << ifSameClassLabel(1, data, 0) << '\n';
    return 0;
}

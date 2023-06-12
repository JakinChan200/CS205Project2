#include <bits/stdc++.h>

using namespace std;

double highestAccuracy = 0;
vector<int> highestAccuracyFeatures;

void printFile(vector<vector<double>> &data){
    for(int i = 0; i < data.size(); i++){
        for(int j = 0; j < data[i].size(); j++){
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
}

void readFile(vector<vector<double>> &data, string &fileName){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
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

bool isDupe(vector<int> curFeatures,  int index){
    for(int i = 0; i < curFeatures.size(); i++){
        if(curFeatures[i] == index) return true;
    }
    return false;
    // int originalSize = curFeatures.size();
    // curFeatures.insert(index);
    // return originalSize == curFeatures.size();
}

void printFeatures(vector<int> features){
    cout << "{";
    if(features.size() > 0){
        cout << features[0];
    }
    for(int i = 1; i < features.size(); i++){
        cout << ", " << features[i];
    }
    cout << "}";
}

int nearestNeighbor(vector<vector<double>> &data, int rowNum, vector<int> curSet){
    //cout << "Data Size: " << data.size() << endl;
    //cout << "features: " << curSet.size() << endl;
    double closestDistance = __SIZEOF_DOUBLE__;
    int indexOfClosest = -1;
    double curDist = 0;
    //int len = data[0].size(); 

    for(int i = 0; i < data.size(); i++){
        double sum = 0;
        if(i == rowNum) continue;
        //cout << i << endl;
        for(int j  = 0; j < curSet.size(); j++){
            sum += pow(data[i][curSet[j]] - data[rowNum][curSet[j]], 2);
            //cout << "sum " << sum << endl;
        }

        curDist = sqrt(sum);
        //cout << "Distance between rowNum: " << rowNum << " and  i " << i << " is " << curDist << endl;
        if(curDist < closestDistance){
            closestDistance = curDist;
            indexOfClosest = i;
        }
        //cout << " index: " << i << " distance: " << curDist << endl; 
    }
    return indexOfClosest;
}

double leaveOneOut(vector<vector<double>> &data, vector<int> curSet){
    //cout << "leave one out" << endl;
    int numCorrectlyClassified = 0;
    for(int i = 0; i < data.size(); i++){
        //cout << i << endl;
        int neighborIndex = nearestNeighbor(data, i, curSet);
        if(data[neighborIndex][0] == data[i][0]){
            //cout << "i "  << i << " neighbor " << neighborIndex << endl;
            numCorrectlyClassified++;
        }
    }
    // cout << "Correctly guessed: " << (double)numCorrectlyClassified/(data.size()-1) * 100 << endl;
    // cout << "numClassified: " << (double)numCorrectlyClassified << endl;
    // cout << "data size: " << data.size() << endl;
    return (double)numCorrectlyClassified/(data.size()-1) * 100;
}

void forwardSelection(vector<vector<double>> &data){
    vector<int> indexOfFeaturesPicked = {};
    highestAccuracy = 0;

    for(int i = 1; i < data[0].size(); i++){
        //cout << i << " " << data[0].size() << endl;
        vector<int> indexOfFeaturesConsidering;
        double curBestAccuracy = 0;
        double curBestAccuracyFeatureIndex;
        for(int j = 1; j < data[0].size(); j++){
            if(isDupe(indexOfFeaturesPicked, j)) continue;

            indexOfFeaturesConsidering = indexOfFeaturesPicked;
            indexOfFeaturesConsidering.push_back(j);
            // printFeatures(indexOfFeaturesConsidering);
            // cout << endl;

            double accuracy = leaveOneOut(data, indexOfFeaturesConsidering);
            cout << "Using Features(s) ";
            printFeatures(indexOfFeaturesPicked);
            cout << " while considering " << j << ", the accuracy is " << accuracy << "%" << endl;
            if(accuracy > curBestAccuracy){
                curBestAccuracy = accuracy;
                curBestAccuracyFeatureIndex = j;
                if(curBestAccuracy > highestAccuracy){
                    highestAccuracy = curBestAccuracy;
                    highestAccuracyFeatures = indexOfFeaturesConsidering;
                }
            }
        }
        indexOfFeaturesPicked.push_back(curBestAccuracyFeatureIndex);
        sort(indexOfFeaturesPicked.begin(), indexOfFeaturesPicked.end());
        cout << "For level " << i << ", the index chosen is " << curBestAccuracyFeatureIndex << " with an accuracy of " << curBestAccuracy << "%" << endl;
        cout << "The current feature list is: ";
        printFeatures(indexOfFeaturesPicked);
        cout << endl << endl;
    }
}


int main(int argc, char* argv[]){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout << setprecision(4);
    int dataSize = 0;
    int dataFileNumber = 0;
    string fileName;
    vector<vector<double>> data;
    vector<string> fileSizeName = {"small", "large", "XXXlarge"};

    //Get data file size + data validation
    cout << "What size dataset are you planning to use?" << endl;
    cout << "1: small" << endl;
    cout << "2: large" << endl;
    cout << "3: XXXlarge" << endl;
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
    }else{
        fileName = "Datasets/TestData.txt";
    }

    //Testing readFile
    readFile(data, fileName);
    //printFile(data);

    //Testing NearestNeighbor
    // vector<double> testDataPoint = {2, 1, 2, 3, 4};
    // int nearestNeighborindex = nearestNeighbor(data, testDataPoint);
    // cout << "Nearest Neighbor Testing Index: " << nearestNeighborindex << endl;

    cout << "Type the number of the algorithm you want to run." << endl;
    cout << "1. Forward Selection" << endl;
    cout << "2. Backwards Elimination" << endl;

    int algo = 0;
    cin >> algo;
    while(algo != 1 && algo != 2){
        cout << "That was not a valid input, please try again: " << endl;
        cin >> algo;
    }

    switch(algo){
        case 1:
            forwardSelection(data);
            break;
        default:
            break;
    }

    cout << "The highest accuracy features are: ";
    printFeatures(highestAccuracyFeatures);
    cout << " with an accuracy of: " << highestAccuracy << "%" << endl;
    return 0;
}

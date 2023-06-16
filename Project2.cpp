#include <bits/stdc++.h>
#include <typeinfo>
using namespace std;

double highestAccuracy = 0;
vector<int> highestAccuracyFeatures;
int maxLevelDec = 500;

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
    double closestDistance = __DBL_MAX__;
    int indexOfClosest = -1;
    double sum = 0;
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
        //cout << "Distance between rowNum: " << rowNum << " and i " << i << " is " << curDist  << "   " << closestDistance << endl;
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
        //cout << "i "  << i << " neighbor " << neighborIndex << endl;
        if(data[neighborIndex][0] == data[i][0]){
            //cout << "i "  << i << " neighbor " << neighborIndex << endl;
            //cout << "if data[neighborIndex][0] "  << data[10][0] << endl;    // << "  data[i][0] " << data[i][0] << endl;
            numCorrectlyClassified++;
        }
    }
    //cout << "Correctly guessed: " << (double)numCorrectlyClassified/(data.size()-1) * 100 << endl;
    //cout << "numClassified: " << (double)numCorrectlyClassified << endl;
    //cout << "data size: " << data.size() << endl;
    return (double)numCorrectlyClassified/(data.size()-1) * 100;
}

void forwardSelection(vector<vector<double>> &data){
    vector<int> indexOfFeaturesPicked = {};
    highestAccuracy = 0;
    double curBestAccuracy = 0;
    double prevBestAccuracy = 0;
    int numLevelsDecreasing = 0;

    for(int i = 1; i < data[0].size(); i++){
        //cout << i << " " << data[0].size() << endl;
        vector<int> indexOfFeaturesConsidering;
        prevBestAccuracy = curBestAccuracy;     //save this accuracy for next level
        //cout << "\nprevBestAccuracy = " << prevBestAccuracy << "\n";
        curBestAccuracy = 0;
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
        if(curBestAccuracy > prevBestAccuracy){
            numLevelsDecreasing = 0;
        }
        else{                                   //if accuracy decreased or stayed the same
            if(numLevelsDecreasing == maxLevelDec){
                break;
            }
            numLevelsDecreasing++;
            cout << "\nnumLevelsDecreasing = " << numLevelsDecreasing << "\n";
        }
        indexOfFeaturesPicked.push_back(curBestAccuracyFeatureIndex);
        sort(indexOfFeaturesPicked.begin(), indexOfFeaturesPicked.end());
        cout << "For level " << i << ", the index chosen is " << curBestAccuracyFeatureIndex << " with an accuracy of " << curBestAccuracy << "%" << endl;
        cout << "The current feature list is: ";
        printFeatures(indexOfFeaturesPicked);
        cout << endl << endl;
    }
}

void backwardElimination(vector<vector<double>> &data){
    vector<int> indexOfAllFeaturesPicked;
    int len = data[0].size();
    highestAccuracy = 0;
    vector<int> indexOfFeaturesToBeKept;
    double curBestAccuracy = 0;
    double prevBestAccuracy = 0;
    int numLevelsDecreasing = 0;
    double accuracy = 0;

    for(int i = 1; i < len; i++){
        indexOfAllFeaturesPicked.push_back(i);
    }

    for(int i = 1; i < data[0].size(); i++){   
        indexOfFeaturesToBeKept.clear();
        prevBestAccuracy = curBestAccuracy;     //save this accuracy for next level
        cout << "\nprevBestAccuracy = " << prevBestAccuracy << "\n";
        curBestAccuracy = 0;
        double curFeatureIndexLoweringAccuracy;
        
        for(int j = 1; j < data[0].size(); j++){    //represents a level
            //go through the features columns and choose the subset from that is in indexOfFeaturesPicked that gives you the best accuracy.
            if(!isDupe(indexOfAllFeaturesPicked, j)) continue;

            indexOfFeaturesToBeKept = indexOfAllFeaturesPicked; //start of with the same vector
            indexOfFeaturesToBeKept.erase(remove(indexOfFeaturesToBeKept.begin(), indexOfFeaturesToBeKept.end(), j), indexOfFeaturesToBeKept.end());
            //printFeatures(indexOfFeaturesToBeKept);
            // cout << endl;

            accuracy = leaveOneOut(data, indexOfFeaturesToBeKept);          //seg fault is because of this line
            //accuracy = j;
            cout << "Using Features(s) ";
            printFeatures(indexOfAllFeaturesPicked);
            cout << " while considering removing " << j << ", the accuracy is " << accuracy << "%" << endl;
            if(accuracy > curBestAccuracy){     //update accuracy
                curBestAccuracy = accuracy;
                curFeatureIndexLoweringAccuracy = j;
                if(curBestAccuracy > highestAccuracy){
                    highestAccuracy = curBestAccuracy;
                    highestAccuracyFeatures = indexOfFeaturesToBeKept;
                }
            }
        }
        if(curBestAccuracy > prevBestAccuracy){
            numLevelsDecreasing = 0;
        }
        else{                                   //if accuracy decreased or stayed the same
            if(numLevelsDecreasing == maxLevelDec){
                break;
            }
            numLevelsDecreasing++;
            cout << "\nnumLevelsDecreasing = " << numLevelsDecreasing << "\n";
        }
        indexOfAllFeaturesPicked.erase(remove(indexOfAllFeaturesPicked.begin(), indexOfAllFeaturesPicked.end(), curFeatureIndexLoweringAccuracy), indexOfAllFeaturesPicked.end()); ;
        sort(indexOfAllFeaturesPicked.begin(), indexOfAllFeaturesPicked.end());
        cout << "For level " << i << ", the index removed is " << curFeatureIndexLoweringAccuracy << " with an new accuracy of " << curBestAccuracy << "%" << endl;
        cout << "The current feature list is: ";
        printFeatures(indexOfAllFeaturesPicked);
        cout << endl << endl;
    }
}

bool ifSameClassLabel(double predclass, vector<vector<double>> &data, double instnum){      //check if predicted class is the same as the actual class stored in the first column of data
    return fabs(predclass - data[instnum][0]) < 0.01;
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
    cout << "5: Pokemon Dataset" << endl;

    cin >> dataSize;
    while(dataSize < 1 || dataSize > 5){
        cout << "That is not a valid input, please try again." << endl;
        cin >> dataSize;
    }

    if(dataSize != 4){
        //Get File number + data validation
        switch(dataSize){
            case 1:
                do{
                    cout << "What dataset file do you want to use?" << endl;
                    cout << "Enter a valid number from 1-33: " << endl;
                    cin >> dataFileNumber;
                } while(dataFileNumber < 1 || dataFileNumber > 33);
                break;
            case 2:
                do{
                    cout << "What dataset file do you want to use?" << endl;
                    cout << "Enter a valid number from 1-33: " << endl;
                    cin >> dataFileNumber;
                } while(dataFileNumber < 1 || dataFileNumber > 33);
                break;
            case 3:
                do{
                    cout << "What dataset file do you want to use?" << endl;
                    cout << "Enter a valid number from 1-24: " << endl;
                    cin >> dataFileNumber;
                } while(dataFileNumber < 1 || dataFileNumber > 24);
                break;
            case 5:
                cout << "Using Pokemon Dataset..." << endl;
                break;
            default:
                break;
        }

        //Testing File Name
        if(dataSize != 5){
            fileName = "Datasets/CS170_" + fileSizeName[dataSize-1] + "_Data__" + to_string(dataFileNumber) + ".txt";
        }
        else{
            fileName = "pokemon.txt";
        }
        cout << fileName << endl;

    }
    else{
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
        case 2:
            backwardElimination(data);
            break;
        default:
            break;
    }

    cout << "The highest accuracy features are: ";
    printFeatures(highestAccuracyFeatures);
    cout << " with an accuracy of: " << highestAccuracy << "%" << endl;
    return 0;

    //vector<int> featSubset;
    //featSubset.push_back(1);
    //featSubset.push_back(2);
    //int lOO = leaveOneOutValidator(data, featSubset);
    //cout << "\nifSameClassLabel = " << ifSameClassLabel(1, data, 0) << '\n';
    //cout << "\nlOO accuracy = " << lOO << '\n';
    //return 0;
}

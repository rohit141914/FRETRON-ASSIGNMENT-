#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool distributeApples(const vector<int>& apples, int index, int ramTarget, int shamTarget, int rahimTarget, vector<int>& distribution, vector<int>& currentSums) {
    if (index == apples.size()) {
        return currentSums[0] == ramTarget && currentSums[1] == shamTarget && currentSums[2] == rahimTarget;
    }

    // Trying to assign the current apple to each person
    for (int person = 0; person < 3; person++) {
        int target = (person == 0) ? ramTarget : (person == 1) ? shamTarget : rahimTarget;
        
        // Check if adding this apple doesn't exceed the person's target
        if (currentSums[person] + apples[index] <= target) {
            // Assign apple to this person
            distribution[index] = person;
            currentSums[person] += apples[index];
            
            // Going to the next apple
            if (distributeApples(apples, index + 1, ramTarget, shamTarget, rahimTarget, distribution, currentSums)) {
                return true;
            }
            
            // Backtracking to try all the options and remove the current one
            currentSums[person] -= apples[index];
        }
    }

    // No valid assignment found for this apple   
    distribution[index] = -1;
    return false;
}

void printDistribution(const vector<int>& apples, const vector<int>& distribution) {
    vector<vector<int>> result(3);
    for (int i = 0; i < distribution.size(); i++) {
        if (distribution[i] != -1) {
            result[distribution[i]].push_back(apples[i]);
        }
    }
    cout << "Ram: ";
    for (int weight : result[0]) cout << weight << " ";
    cout << endl;

    cout << "Sham: ";
    for (int weight : result[1]) cout << weight << " ";
    cout << endl;

    cout << "Rahim: ";
    for (int weight : result[2]) cout << weight << " ";
    cout << endl;
}

int main() {
    vector<int> apples;
    int totalWeight = 0;
    int weight;

    while (true) {
        cout << "Enter apple weight in gram (-1 to stop): ";
        cin >> weight;
        if (weight == -1) break;
        apples.push_back(weight);
        totalWeight += weight;
    }

    sort(apples.rbegin(), apples.rend()); // Sort apples in descending order

    int ramTarget = static_cast<int>(totalWeight * 0.5);
    int shamTarget = static_cast<int>(totalWeight * 0.3);
    int rahimTarget = totalWeight - ramTarget - shamTarget; // Ensure exact total

    vector<int> distribution(apples.size(), -1);
    vector<int> currentSums(3, 0);

    if (distributeApples(apples, 0, ramTarget, shamTarget, rahimTarget, distribution, currentSums)) {
        cout << "Distribution Result:" << endl;
        printDistribution(apples, distribution);
    } else {
        cout << "Perfect distribution not possible." << endl;
        cout << "Remaining apples: ";
        for (int weight : apples) cout << weight << " ";
        cout << endl;
    }

    return 0;
}

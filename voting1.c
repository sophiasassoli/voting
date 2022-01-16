# include <stdio.h>
# include <stdlib.h>
# include <string.h>

struct rowscols {
    int M;
    int N;
};

struct rowscols readfile(char *filename) {
    
    FILE *file;
    file = fopen(filename, "r");

    int tempM, tempN, M, N;

    int i = 0;
    while (i < 1) {
        fscanf(file, "%d %d\n", &tempN, &tempM);
        N = tempN;
        M = tempM;
        i++;
    }

    struct rowscols data;
    data.M = M;
    data.N = N;

    fclose(file);

    return data;
}  

void bordacount(int M, int N, int cur, int votes[M][N], int d);
void eliminate(int M, int N, int votes[M][N], int votescopy[M][N], int e);
void runoff(int M, int N, int votes[M][N], int d);

void bordacount(int M, int N, int cur, int votes[M][N], int d) {
    // array to store borda points
    int points[N];
    for (int n = 0; n < N; n++) {
        points[n] = 0;
    }

    // add borda points to count array
    int total = cur;
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            int rank = votes[m][n];
            points[n] += (total - rank + 1);
            if (rank > N) {
                points[n] = -1;
            }
        }
    }

    // output winner
    int winnerpoints = 0;
    int winner;
    int winnercount = 1;

    // find max value of borda points from count array
    for (int i = 0; i < N; i++) {
        if (points[i] > winnerpoints) {
            winnerpoints = points[i];
            winner = i;
            winnercount = 1;
        } else if (points[i] == winnerpoints) {
            winnercount += 1;
        }
    }

    if (winnercount == 1) {
        if (d == 0) {
            printf("Borda count\nWinner: Candidate %d\n", winner);
        } else {
            printf("Borda count\n");
            printf("Points:\n");
            for (int n = 0; n < N; n++) {
                printf("Candidate %d: %d points\n", n, points[n]);
            }
            printf("Winner: Candidate %d\n", winner);
        }
    } else {
        if (d == 0) {
            printf("Borda count\nWinner: Tie! Recalculating...\n");
        } else {
            printf("Borda count\n");
            printf("Points:\n");
            for (int n = 0; n < N; n++) {
                printf("Candidate %d: %d points\n", n, points[n]);
            }
            printf("Winner: Tie! Recalculating...\n");
        }
        int min = 999;
        for (int m = 0; m < M; m++) {
            for (int i = 0; i < N; i++) {
                if(points[i] < min && points[i] >= 0) {
                min = points[i];
                }
            }
        }
        int del;
        for (int i = 0; i < N; i++) {
            if (points[i] == min) {
                del = i;
            }
        }
        int votescopy[M][N];
        eliminate(M, N, votes, votescopy, del);
        cur -= 1;
        bordacount(M, N, cur, votescopy, d);
    }
}

void runoff(int M, int N, int votes[M][N], int d) {  

    int rankedcount[N];           // hold first place counts for each candidate
    int totalfirsts = 0;          // holds total number of first place votes

    // initialize first place counts for each candidate to 0
    for (int i = 0; i < N; i++) {
        rankedcount[i] = 0;
    }

    // if candidate received first place vote, add one to that index in the rankedcount array
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            if (votes[m][n] == 1) {
                rankedcount[n] += 1;
                totalfirsts += 1;
            } else if (votes[m][n] > N) {
                rankedcount[n] = -1;
            }
        }
    }

    // find min for later use
    int min = 999;
    for (int m = 0; m < M; m++) {
        for (int i = 0; i < N; i++) {
            if(rankedcount[i] < min && rankedcount[i] >= 0) {
                min = rankedcount[i];
            }
        }
    }
    
    int numeliminated = 0;
        for (int n = 0; n < N; n++) {
            if (rankedcount[n] == -1) {
                numeliminated += 1;
            }
        }   

    int e;
    int rankedwinner = 100;
    if (N - numeliminated == 0) {
        for (int i = 0; i < N; i++) {
            if ((rankedcount[i]/totalfirsts)*100 > 50) {
                rankedwinner = i;
                if (d == 0) {
                    printf("Instant runoff\nWinner: Candidate %d\n", rankedwinner);
                } else {
                    printf("Instant runoff\n");
                    printf("Number of first place votes:\n");
                    for (int i = 0; i < N; i++) {
                        if (rankedcount[i] == -1) {
                            printf("Candidate %d: %d\n", i, 0);
                        } else {
                            printf("Candidate %d: %d\n", i, rankedcount[i]);
                        }
                    }
                    printf("Winner: Candidate %d\n", rankedwinner);
                }
            }
        }
    } else if (N - numeliminated == 2) {
            int max = -999;
            for (int i = 0; i < N; i++) {
                if(rankedcount[i] > max) {
                    max = rankedcount[i];
                    rankedwinner = i;
                }
            }
            if (d == 0) {
                printf("Instant runoff\nWinner: Candidate %d\n", rankedwinner);
            } else {
                printf("Instant runoff\n");
                printf("Number of first place votes:\n");
                for (int i = 0; i < N; i++) {
                    if (rankedcount[i] == -1) {
                        printf("Candidate %d: %d\n", i, 0);
                    } else {
                        printf("Candidate %d: %d\n", i, rankedcount[i]);
                    }
                }
                printf("Winner: Candidate %d\n", rankedwinner);

        }
    // if a winner hasn't been identified, eliminate the candidate and perform the runoff again
    } else if (rankedwinner == 100 && (N - numeliminated) > 2) { 
        int del;
        for (int i = 0; i < N; i++) {
            if (rankedcount[i] == min) {
                del = i;
            }
        }   
        if (d != 0) {    
            printf("Instant runoff\n");
            printf("Number of first place votes:\n");
            for (int i = 0; i < N; i++) {
                if (rankedcount[i] == -1) {
                    printf("Candidate %d: %d\n", i, 0);
                } else if (i == del) {
                    printf("Candidate %d: %d, eliminate\n", i, 0);
                } else {
                    printf("Candidate %d: %d\n", i, rankedcount[i]);
                }
            }
            printf("Winner: no candidate has majority votes. Recalculating...\n");
        }
        
        int votescopy[M][N];
        eliminate(M, N, votes, votescopy, del);
        numeliminated += 1;
        if (N - numeliminated > 2) {
            runoff(M, N, votescopy, d); 
        } else {
            int max = -999;
            for (int i = 0; i < N; i++) {
                if(rankedcount[i] > max) {
                    max = rankedcount[i];
                    rankedwinner = i;
                }
            }
            if (d == 0) {
                printf("Instant runoff\nWinner: Candidate %d\n", rankedwinner);
            } else {
                printf("Instant runoff\n");
                printf("Number of first place votes:\n");
                for (int i = 0; i < N; i++) {
                    if (rankedcount[i] == -1) {
                        printf("Candidate %d: %d\n", i, 0);
                    } else if (i == del) {
                        printf("Candidate %d: %d\n", i, 0);
                    } else {
                        printf("Candidate %d: %d\n", i, rankedcount[i]);
                    }
                }
                printf("Winner: Candidate %d\n", rankedwinner);
            }
        }
    }    
} 
    


// the least number of first place rankings gets eliminated
// change ranking for eliminated candidate to number greater than N
// look at each ballot (M) and reassign candidates the new rankings
void eliminate(int M, int N, int votes[M][N], int votescopy[M][N], int e) {
    
    // copy the array
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            votescopy[m][n] = votes[m][n];
        }
    }

    // if key ranking is less than eliminated ranking, ranking - 1
    // moves rankings up if they were ranked below eliminated candidate
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            int key = votescopy[m][e];
            if (key < votescopy[m][n]) {
                votescopy[m][n] -= 1;
            }
        }
        votescopy[m][e] = 100;
    }
}

int main(int argc, char *argv[]) {

    struct rowscols data;
    data = readfile("ballots3.txt");

    int M = data.M;
    int N = data.N;
    int votes[M][N];

    FILE *file;
    file = fopen("ballots3.txt", "r");

    int vote;
    char buffer[20];
    fgets(buffer, 20, file);
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            fscanf(file, "%d", &vote);
            votes[m][n] = vote;
        }
    }

    fclose(file);

    char d[2] = "d";
    int details = 0;
    if (argc > 1) {
        if (strcmp(argv[1], d) == 0){
            details = 1;
        }
    }

    if (details == 1) {
        bordacount(M, N, N, votes, 1);
        runoff(M, N, votes, 1);
    } else {
        bordacount(M, N, N, votes, 0);
        runoff(M, N, votes, 0);
    }

    return 0;
}

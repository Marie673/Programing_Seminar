//
// Created by okako on 2021/03/27.
//

#include <stdio.h>

int count_bingo(int a[][5]);

int main(){
    int bingo;
    int data[5][5] = {{1,1,1,1,1},
                      {1,1,1,1,1},
                      {1,1,1,1,1},
                      {1,1,1,1,1},
                      {1,1,1,1,1}};

    bingo = count_bingo(data);
    printf("%d\n", bingo);

    return 0;
}

int count_bingo(int a[][5]){
    int bingo = 0;

    for(int i = 0; i < 5; i++){
        int counter = 0;
        for(int j = 0; j < 5; j++){
            counter += a[i][j];
        }
        if(counter == 5){
            bingo++;
        }
    }

    for(int j = 0; j < 5; j++){
        int counter = 0;
        for(int i = 0; i < 5; i++){
            counter += a[i][j];
        }
        if(counter == 5){
            bingo++;
        }
    }

    {
        int counter = 0;
        for (int i = 0, j = 0; i < 5 && j < 5; i++, j++) {
            counter += a[i][j];
        }
        if(counter == 5){
            bingo++;
        }
    }

    {
        int counter = 0;
        for (int i = 4, j = 0; i >= 0 && j < 5; i--, j++) {
            counter += a[i][j];
        }
        if(counter == 5){
            bingo++;
        }
    }

    return bingo;
}
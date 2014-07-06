#include<stdio.h>
int main(){
	int score[7] = {76, 82, 90, 86, 79, 62,100};
	
	int mean, sum, temp, i;
	
	temp = 0;
	sum = 0;
	int credit[7] = {2, 2, 1, 2, 2, 3,4};
	for(i = 0 ; i < 7; i++){
		printf("Hello World\n");
	}
    for( i = 0; i < 7; i++){
		sum = sum + score[i] * credit[i];
    }
	
	while(i != 0) {
		temp = temp + credit[i - 1];
		i--;
	}
	
    mean = sum / temp;
	printf("mean:%d\n",mean);
	mean=40;
	printf("changed mean:%d\n",mean);
	
    if(mean >= 60){
		mean = mean - 60;
		printf("Your score is %d higher than 60!\n", mean);
    }
    else{
		mean = 60 - mean;
        printf("Your score is %d lower than 60!\n", mean);
	}
    return 0;
}
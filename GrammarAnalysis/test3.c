#include<stdio.h>
int main(){
	int score[6] = {76, 82, 90, 86, 79, 62};
	int credit[6] = {2, 2, 1, 2, 2, 3};
	int mean, sum, tmp, i;

	tmp = 0;
	sum = 0;
    	for( i = 0; i < 6; i++)
	{
		sum = sum + score[i] * credit[i];
    	}
	
	while(i != 0) {
		tmp = tmp + credit[i - 1];
		i--;
	}
    	mean = sum / tmp;
 
    	if(mean >= 60){
		mean = mean - 60;
		printf("Your score is %d higher than 60!\n", mean);
    	}
    	else{
		mean = 60 - mean;
        	printf("Your score is %d lower than 60!\n!\n", mean);
	}
    return 0;
}
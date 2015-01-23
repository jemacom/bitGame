#include <wiringPi.h>
#include <stdio.h>
#include <string.h>

int main(void){
    
    if (wiringPiSetup() == -1){
        printf("Error while setting up wiring lib ! \n");
        return 0;
    }
    
   for (int i = 0 ; i<8 ; i++){
        pinMode( i , OUTPUT);
        digitalWrite( i , HIGH);
   }


   char ch[4];
   char c = 0;
    while (1){
        printf("Number = ");  
        scanf("%s" , ch);

	if(!strlen(ch)) continue;
        else if (ch[0] == 'q') break;

        int num;

	switch(strlen(ch)){
	case 1 :
		num = ch[0] - '0';
		break;
	case 2 :
		num = ch[1] - '0' + (ch[0] - '0')*10;
		break;
	case 3 :
		num = ch[2] - '0' + (ch[1] - '0') * 10 + (ch[0] - '0') * 100;
		break;
	default:
		break;
	}


	if(num > 255) continue;

        printf("num = %d num & 0x01 = %d \n", num, num & 0x01);
//        fflush(stdin);

   digitalWrite( 0 , num & 0x01);
   digitalWrite( 1 , num & 0x02);
   digitalWrite( 2 , num & 0x04);
   digitalWrite( 3 , num & 0x08);
   digitalWrite( 4 , num & 0x10);
   digitalWrite( 5 , num & 0x20);
   digitalWrite( 6 , num & 0x40);
   digitalWrite( 7 , num & 0x80);

    }
    return 0;
}

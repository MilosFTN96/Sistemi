#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

  // Inicijalizacija programa
  
  FILE *f_prekidac;
  FILE *f_tasteri;
  FILE *f_ledovke;
  char *str;
  char pr1;
  char pritisnut1 = 0;
  char pritisnut2 = 0;
  char t0, t1, t2, t3;
  size_t br_bajtova = 6;
  float procenat = 0;
  long int perioda = 20000L;

  // Glavni deo

  while(1){

    // Ocitavanje vrednosti prekidaca "SW0"

    f_prekidac = fopen("/dev/switch", "r");

    if(f_prekidac == NULL){
      printf("Problem pri otvaranju /dev/switch\n");
      return -1;  
    }
  
    str = (char*)malloc(br_bajtova+1);

    getline(&str,&br_bajtova,f_prekidac);

    if(fclose(f_prekidac)){
      printf("Problem pri zatvaranju /dev/switch\n");
      return -1;
    }

    pr1 = str[2] - '0';

    free(str);

    // Ocitavanje vrednosti tastera "BTN0", "BTN1", "BTN2" i "BTN3"

    f_tasteri = fopen("/dev/button", "r");

    if(f_tasteri == NULL){
      printf("Problem pri otvaranju /dev/button\n");
      return -1;
    }

    str = (char*)malloc(br_bajtova+1);

    getline(&str,&br_bajtova,f_tasteri);

    if(fclose(f_tasteri)){
      printf("Problem pri zatvaranju /dev/button\n");
      return -1;
    }

    t3 = str[2] - '0';
    t2 = str[3] - '0';
    t1 = str[4] - '0';
    t0 = str[5] - '0';  
    
    free(str);

    // Impulsno sirinska modulacija

    // Ako je prekidac "SW0" aktivan
    if(pr1 == 1){

		  printf("Procenat je : %f", procenat);

      // Ako je taster "BTN0" aktivan
      if(t0 == 1 && pritisnut1 == 0){
     	  
        pritisnut1 = 1;

        // Povecanje faktora ispune za 20%
        procenat += 0.2;
      }
		  
      if(t0 == 0){
        
        pritisnut1 = 0;

      }

      // Ako je taster "BTN1" aktivan
      if(t1 == 1 && pritisnut2 == 0){
  		  
          pritisnut2 = 1;

          // Smanjenje faktora ispune za 20%
          procenat -= 0.2;
      }

      if(t1 == 0){
        
        pritisnut2 = 0;
      }  
      // Ako je pritisnut taster "BTN2"
      if(t2 == 1){
     		
        // Postavi faktor ispune na 0%
        procenat = 0;
      }
      // Ako je taster "BTN3" aktivan
      if(t3 == 1){
        
        // Postavi faktor ispune na 100%
        procenat = 1;
      }

		  if(procenat < 0){
			  procenat = 0;
		  }

		  if(procenat > 1){
			  procenat = 1;
		  }

		  printf("Procenat je : %f", procenat);
    
      f_ledovke = fopen("/dev/led","w");

      if(f_ledovke == NULL){
        printf("Problem pri otvaranju /dev/led\n");
        return -1;
      }

      fputs("0x0F\n",f_ledovke);

    	if(fclose(f_ledovke)){
      	printf("Problem pri zatvaranju /dev/button\n");
      	return -1;
    	}

      usleep(procenat*perioda);
		
      f_ledovke = fopen("/dev/led","w");
		
      if(f_ledovke == NULL){
        printf("Problem pri otvaranju /dev/led\n");
        return -1;
      }

      fputs("0x00\n",f_ledovke);

      if(fclose(f_ledovke)){
        printf("Problem pri zatvaranju /dev/led\n");
        return -1;
      }

      usleep((1-procenat)*perioda);
    }
      
  }

}
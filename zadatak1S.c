#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  
  FILE *fswitch;
  FILE *fbutton;
  FILE *fled;
  char *str;
  char pr1;
  char pritisnut1 = 0;
  char pritisnut2 = 0;
  char bt0, bt1, bt2, bt3;
  size_t br_bajtova = 6;
  float procenat = 0;
  long int perioda = 20000L;


  while(1){

    // Ocitavanje vrednosti sa prekidaca "SW0"

    fswitch = fopen("/dev/fswitch", "r");
	
	// Ako je fajl "fswitch" prazan izbaci problem:
    if(fswitch == NULL){
      puts("Problem pri otvaranju /dev/fswitch");
      return -1;  
    }
	// Alociramo broj bajtova u memoriji
    str = (char*)malloc(br_bajtova+1);
	
    getline(&str,&br_bajtova,fswitch);
	
	// if 
    if(fclose(fswitch)){
      puts("Problem pri zatvaranju /dev/fswitch");
      return -1;
    }

    pr1 = str[2] - '0';

    free(str);

    // Ocitavanje vrednosti tastera "BTN0", "BTN1", "BTN2" i "BTN3"

    fbutton = fopen("/dev/button", "r");

    if(fbutton == NULL){
      puts("Problem pri otvaranju /dev/button");
      return -1;
    }

    str = (char*)malloc(br_bajtova+1);

    getline(&str,&br_bajtova,fbutton);

    if(fclose(fbutton)){
      puts("Problem pri zatvaranju /dev/button");
      return -1;
    }
	//0b0000 sve na 0 -- > 0-[0]; b-[1]; 0-[2];...
    bt0 = str[2] - '0';
    bt1 = str[3] - '0';
    bt2 = str[4] - '0';
    bt3 = str[5] - '0';  
    
    free(str);

    // Impulsno sirinska modulacija

    // Ako je prekidac "SW0" aktivan
    if(pr1 == 1){

      fled = fopen("/dev/led", "w");

      if(fled == NULL){
        puts("Problem pri otvaranju /dev/led");
        return -1;
      }
	// Upali ledovke 0xf = 1111 => 0b1111 sve 4 ledovke gore.
      fputs("0xf\n",fled);

      // Ako je taster "BTN0" pritisnut
      if(bt0 == 1 && pritisnut1 == 0){
        
        pritisnut1 = 1;

        if(pritisnut1 == 1){

          // Povecanje faktora ispune za 20%
          procenat = procenat + 0.2;
        }
      }  
      // Ako je taster "BTN1" aktivan
      if(bt1 == 1 && pritisnut2 == 0){
        
        pritisnut2 = 1;

        if(pritisnut2 == 1){

          // Smanjenje faktora ispune za 20%
          procenat -= 0.2;
        }
      }
      // Ako je pritisnut taster "BTN2"
      if(bt2 == 1){
     
        // Postavi faktor ispune na 0%
        procenat = 0;
      }
      // Ako je taster "BTN3" aktivan
      if(bt3 == 1){
       
        // Postavi faktor ispune na 100%
        procenat = 1;
      }

      usleep(procenat*perioda);

      fputs("0x0\n",fled);

      if(fclose(fled)){
        puts("Problem pri zatvaranju /dev/led");
        return -1;
      }

      usleep((1-procenat)*perioda);

      if(bt1 == 0){
        
        pritisnut1 = 0;
      }

      if(bt2 == 0){
        
        pritisnut2 = 0;
      }  

    }
      
  }

}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

	FILE* tasteri;
	FILE* ledovke;
	FILE* prekidaci;
	float procenat = 0;
	long int period = 20000L;
	int fleg = 0;
	char *str;
	char bt1, bt2, bt3, bt4, sw1;
	size_t num_of_bytes = 6;
	while(1)
	{	
		prekidaci = fopen ("/dev/switch", "r");
		str = (char *)malloc(num_of_bytes+1);
		getline(&str, &num_of_bytes, prekidaci);
		sw1 = str[2] - '0';
		fclose(prekidaci)
		if(sw1 == 1)
		{
			tasteri = fopen ("/dev/button", "r");
			if(tasteri == NULL)
			{
				puts("Problem prilikom otvaranja button fajla!");
				return -1;
			}
			str = (char *)malloc(num_of_bytes+1);
			getline(&str, &num_of_bytes, tasteri);
			bt1 = str[2] - '0';
			bt2 = str[3] - '0';
			bt3 = str[4] - '0';
			bt4 = str[5] - '0';
			if(bt1 && !fleg)
			{
				fleg = 1;
				procenat += 0.2;
				if(procenat >= 1)
				{
					procenat = 1;
				}
			}
			if(bt2 && !fleg)
			{
				fleg = 1;
				procenat -= 0.2;
				if(procenat <= 0)
				{
					procenat = 0;
				}
			}
			if(bt3 && !fleg)
			{
				fleg = 1;
				procenat = 1;
			}
			if(bt4 && !fleg)
			{
				fleg = 1;
				procenat = 0;
			}
			if(!bt1 && !bt2 && !bt3 && !bt4)
			{
				fleg = 0;
			}
			//Pali ledovke
			ledovke = fopen("/dev/led", "w");
			if(ledovke == NULL)
			{
				puts("Problem prilikom otvaranja led fajla!");
				return -1;
			}
			fputs("0x0F", ledovke);
			if(fclose(ledovke))
			{
				puts("Problem prilikom zatvaranja led fajla!");
			}
			usleep(procenat*period);
			//Gasi ledovke
			ledovke = fopen("/dev/led", "w");
			if(ledovke == NULL)
			{
				puts("Problem prilikom otvaranja led fajla!");
				return -1;
			}
			fputs("0x00", ledovke);
			if(fclose(ledovke))
			{
				puts("Problem prilikom zatvaranja led fajla!");
			}
			usleep((1-procenat)*period);
			if(fclose(tasteri))
			{
				puts("Problem prilikom zatvaranja taster fajla!");
				return -1;
			}

		}
		else
		{
			ledovke = fopen("/dev/led", "w");
			if(ledovke == NULL)
			{
				puts("Problem prilikom otvaranja led fajla!");
				return -1;
			}
			fputs("0x00", ledovke);
			if(fclose(ledovke))
			{
				puts("Problem prilikom zatvaranja led fajla!");
			}
		}
	}
}
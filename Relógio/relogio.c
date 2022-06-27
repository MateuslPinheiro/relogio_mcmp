#include <p18f4520.h>
#define botao_min PORTAbits.RA0
#define botao_hora PORTAbits.RA1
#define botao_alarme PORTAbits.RA2
#define LED PORTAbits.RA3

int i;
unsigned char dezena=5;
unsigned char unidade=7;
unsigned char unidade_min=9;
unsigned char dezena_min=5;
unsigned char unidade_hora=3;
unsigned char dezena_hora=2;
unsigned char us_alarme=0;
unsigned char ds_alarme=0;
unsigned char um_alarme=9;
unsigned char dm_alarme=5;
unsigned char uh_alarme=3;
unsigned char dh_alarme=2;
unsigned char func_alarme=0;

void delay_s();

int modo_alarme(void)
{
	if (botao_alarme)
	{
		delay_s(1);
		if(botao_alarme)
		{
			delay_s(1);
			if(botao_alarme)
			{
				func_alarme=1;
				return 1;
			}
		return 0;
		}
	return 0;
	}
}

void modo_ajuste(void)
{
	if (botao_alarme==0);
	{
		if (botao_min==1)
		{
			unidade_min++;
			if (unidade_min>=10)
			{
				unidade_min=0;
				dezena_min++;
				if(dezena_min>=6)
				{
				dezena_min=0;
				unidade_hora++;
				}
			}
		}
		if (botao_hora==1)
		{
			unidade_hora++;
			if (unidade_hora>=10)
			{
				unidade_hora=0;
				dezena_hora++;
			}
			else if(dezena_hora==2 && unidade_hora==4)
	        {
	        	unidade_hora=0;
	        	dezena_hora=0;
	        }
		}
	}

}

void alarme(void)
{
	delay_s(1);
	if (botao_alarme==1)
	{
		if (botao_min==1)
		{
			um_alarme++;
			if (um_alarme>=10)
			{
				um_alarme=0;
				dm_alarme++;
				if(dm_alarme>=6)
				{
				dm_alarme=0;
				}
			}
		}
		if (botao_hora==1)
		{
			uh_alarme++;
			if (uh_alarme>=10)
			{
				uh_alarme=0;
				dh_alarme++;
		}
			else if(dh_alarme==2 && uh_alarme==4)
        	{
        		uh_alarme=0;
        		dh_alarme=0;
	    	}
		}
	}
}

void contador_decimal(void)
{
    if (++unidade>=10)    // contador em base decimal
    {
        unidade=0;
        if(++dezena>=6)
        {
            dezena=0;
            if(++unidade_min>=10)
            {
                unidade_min=0;
                if(++dezena_min>=6)
                {
                    dezena_min=0;
                    if(++unidade_hora>=10)
                    {
                        unidade_hora=0;
                        dezena_hora++;
                    }
                    
                    else if(dezena_hora==2 && unidade_hora==4)
                    {
                        unidade_hora=0;
                        dezena_hora=0;
                    }
                }
            }
        }
    }
	if (unidade==us_alarme && dezena==ds_alarme && unidade_min==um_alarme && dezena_min==dm_alarme && unidade_hora==uh_alarme && dezena_hora==dh_alarme)
	{
		LED=1;
	}
	else if (unidade==us_alarme && dezena==ds_alarme+1 && unidade_min==um_alarme && dezena_min==dm_alarme && unidade_hora==uh_alarme && dezena_hora==dh_alarme)
	{
		LED=0;
	}
}
void show_alarme()
{
	unsigned char BCD,BCD_m, BCD_h, aux;
	BCD = (ds_alarme<<4|us_alarme); // Passando a dezena para os bits da esquerda
	BCD_m = (dm_alarme<<4|um_alarme);
	BCD_h = (dh_alarme<<4|uh_alarme);
	LATC = BCD;	
	LATD = BCD_m;
	LATB = BCD_h;

}

void show_display()
{
	unsigned char BCD,BCD_m, BCD_h, aux;
	BCD = (dezena<<4|unidade); // Passando a dezena para os bits da esquerda
	BCD_m = (dezena_min<<4|unidade_min);
	BCD_h = (dezena_hora<<4|unidade_hora);
	LATC = BCD;	
	LATD = BCD_m;
	LATB = BCD_h;

}

void delay_s (unsigned int segundos)
{
	unsigned short long i;
	for (i=0;i<90000;i++) {}
}

main ()

{
	//área de configuração do sistema
	TRISC = 0b00000000; // display de 7 segmentos como saída
	TRISD = 0b00000000;
	TRISB = 0b00000000;
	TRISAbits.RA0 = 1;
	TRISAbits.RA1 = 1;
	TRISAbits.RA2 = 1;
	TRISAbits.RA3 = 0;
//	TRISA = 0b01111111; // RA7 como saída para o LED. Resto como entrada.
	ADCON1 = 0b00001111; //Configura portas A e B como IO
	// programa monitor (sistema operacional)
	while (1)
	{	
		if (func_alarme==0 && botao_alarme==1)
		{
			modo_alarme();
		}
		else if (func_alarme==1)
		{
			show_alarme();
			alarme();
			if (botao_alarme==0)
			{
				func_alarme=0;
			}
		}
		else
		{
			show_display();
			delay_s (1);
			contador_decimal ();
			modo_ajuste();
		}
			
	}
}
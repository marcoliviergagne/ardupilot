#include "Copter.h"
#include "AP_Notify/ToneAlarm.h"
// Distance maximale de la cible permise. Au-delà de cette limite, le mode land est activé.
#define MAX_DISTANCE_UWB 3 // en metres
#define FREQUENCE_UWB 10 // en Hz
#define NBR_ITERATIONS_ENCLENCHER_MODE_LAND 3

// int global_compteur = 0;

// Variable globale, permet de compter le nombre d'itération ayant atteint la limite
static int g_compteur_atteinte_limite = 0;
#ifdef USERHOOK_INIT
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
AP_HAL::UARTDriver *uart = nullptr;
uart = hal.serial(2);
uart->begin(115200);
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{

float distance = 0;
char buffer[20];
int counter = 0;

int16_t nbytes= hal.serial(2)->available();

    while(nbytes > 0)
    {
	//if(counter>0)
	//{AP_ToneAlarm son_a_jouer3;
 	 //son_a_jouer3.play_tone(0);}
    char c = hal.serial(2)->read();

        if(c== '\r')
        {
        distance = (float)atof(buffer);
        }

        else
        {
            buffer[counter] = c;
        }
	nbytes--;
    counter++;

    }
buffer[counter]='\0';


// -1000 signifie une mauvaise communication entre les cartes, peut survenir de temps en temps ou si le recepteur n'est pas present

if((int)distance != -1000)
{
// Si la distance est plus grande que la limite
    // ZONE DE VOL INTERDITE
    if(distance > MAX_DISTANCE_UWB)
    {

    // Test de son pour savoir si la condition est atteinte :
    // AP_ToneAlarm son_a_jouer2;
     //son_a_jouer2.play_tone(8);

    // On compte le nombre de fois que le capteur a mesuré un dépassement
    g_compteur_atteinte_limite++;


        // Si les capteurs mesure NBR_ITERATIONS_ENCLENCHER_MODE_LAND fois un dépassement,
        // on enclenche le mode LAND

        if(g_compteur_atteinte_limite >= NBR_ITERATIONS_ENCLENCHER_MODE_LAND)
        {

    AP_ToneAlarm son_a_jouer2;
    son_a_jouer2.play_tone(8);

        set_mode(Mode::Number::LAND, ModeReason::FENCE_BREACHED);
        update_flight_mode();
        g_compteur_atteinte_limite = 0;
        }

    }

    // ZONE DE VOL PERMISE
    // Si on mesure une distance qui n'égale pas -1000, mais qui est plus petite que la limite, alors one ne fait rien
    else
    {
        g_compteur_atteinte_limite = 0;
    }

}
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{

    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{

    // put your 1Hz code here
}
#endif

#ifdef USERHOOK_AUXSWITCH
void Copter::userhook_auxSwitch1(uint8_t ch_flag)
{
    // put your aux switch #1 handler here (CHx_OPT = 47)
}

void Copter::userhook_auxSwitch2(uint8_t ch_flag)
{
    // put your aux switch #2 handler here (CHx_OPT = 48)
}

void Copter::userhook_auxSwitch3(uint8_t ch_flag)
{
    // put your aux switch #3 handler here (CHx_OPT = 49)
}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "bitwise.h"


typedef enum
{
    ID_Andromeda = 0,
    ID_Antlia = 1,
    ID_Apus = 2,
    ID_Aquarius = 3,
    ID_Aquila = 4,
    ID_Ara = 5,
    ID_Aries = 6,
    ID_Auriga = 7,
    ID_Bootes = 8,
    ID_Caelum = 9,
    ID_Camelopardalis = 10,
    ID_Cancer = 11,
    ID_Canes_Venatici = 12,
    ID_Canis_Major = 13,
    ID_Canis_Minor = 14,
    ID_Capricornus = 15,
    ID_Carina = 16,
    ID_Cassiopeia = 17,
    ID_Centaurus = 18,
    ID_Cepheus = 19,
    ID_Cetus = 20,
    ID_Chamaeleon = 21,
    ID_Circinus = 22,
    ID_Columba = 23,
    ID_Coma_Berenices = 24,
    ID_Corona_Australis = 25,
    ID_Corona_Borealis = 26,
    ID_Corvus = 27,
    ID_Crater = 28,
    ID_Crux = 29,
    ID_Cygnus = 30,
    ID_Delphinus = 31,
    ID_Dorado = 32,
    ID_Draco = 33,
    ID_Equuleus = 34,
    ID_Eridanus = 35,
    ID_Fornax = 36,
    ID_Gemini = 37,
    ID_Grus = 38,
    ID_Hercules = 39,
    ID_Horologium = 40,
    ID_Hydra = 41,
    ID_Hydrus = 42,
    ID_Indus = 43,
    ID_Lacerta = 44,
    ID_Leo = 45,
    ID_Leo_Minor = 46,
    ID_Lepus = 47,
    ID_Libra = 48,
    ID_Lupus = 49,
    ID_Lynx = 50,
    ID_Lyra = 51,
    ID_Mensa = 52,
    ID_Microscopium = 53,
    ID_Monoceros = 54,
    ID_Musca = 55,
    ID_Norma = 56,
    ID_Octans = 57,
    ID_Ophiuchus = 58,
    ID_Orion = 59,
    ID_Pavo = 60,
    ID_Pegasus = 61,
    ID_Perseus = 62,
    ID_Phoenix = 63,
    ID_Pictor = 64,
    ID_Pisces = 65,
    ID_Piscis_Austrinus = 66,
    ID_Puppis = 67,
    ID_Pyxis = 68,
    ID_Reticulum = 69,
    ID_Sagitta = 70,
    ID_Sagittarius = 71,
    ID_Scorpius = 72,
    ID_Sculptor = 73,
    ID_Scutum = 74,
    ID_Serpens = 75,
    ID_Sextans = 76,
    ID_Taurus = 77,
    ID_Telescopium = 78,
    ID_Triangulum = 79,
    ID_Triangulum_Australe = 80,
    ID_Tucana = 81,
    ID_Ursa_Major = 82,
    ID_Ursa_Minor = 83,
    ID_Vela = 84,
    ID_Virgo = 85,
    ID_Volans = 86,
    ID_Vulpecula = 87,
    STAR_SIGN_NUM
} tStarSign;


int main(int argc, char *argv[])
{
    unsigned char *pMap;
    int len;
    int i;

    pMap = bitmap_alloc(STAR_SIGN_NUM, &len);
    if ( pMap )
    {
        bitmap_set(pMap, ID_Aquila);
        bitmap_set(pMap, ID_Aries);
        bitmap_set(pMap, ID_Canes_Venatici);
        bitmap_set(pMap, ID_Dorado);
        bitmap_set(pMap, ID_Eridanus);
        bitmap_set(pMap, ID_Lacerta);
        bitmap_set(pMap, ID_Microscopium);
        bitmap_set(pMap, ID_Monoceros);
        bitmap_set(pMap, ID_Perseus);
        bitmap_set(pMap, ID_Vulpecula);

        for (i=1; i<argc; i++)
        {
            if (atoi( argv[i] ) < STAR_SIGN_NUM)
            {
                if ( bitmap_check(pMap, atoi( argv[i] )) )
                {
                    bitmap_clear(pMap, atoi( argv[i] ));
                }
                else
                {
                    bitmap_set(pMap, atoi( argv[i] ));
                }
            }
        }

        printf("\n");
        for (i=0; i<STAR_SIGN_NUM; i++)
        {
            if ( bitmap_check(pMap, i) )
            {
                printf("[%2d] is set\n", i);
            }
        }
        printf("\n");

        bit_dump(pMap, len, 0);

        bitmap_free( pMap );
    }
    else
    {
        printf("ERR: cannnot allocate bitmap buffer\n\n");
    }

    return 0;
}


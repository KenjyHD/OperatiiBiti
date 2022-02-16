#include "hunt.h"
#include <stdio.h>

#define MAXUINT16 65535

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{
    /*
     * The spell is 16 bits and they are in the memory exactly after 5
     * consecutive bits of 1:
     *
     *            high                                             low
     *                [][][x]...[x][x][x][1][1][1][1][1][]...[][][]
     *                    |----16bits---|
     *                          Spell
     *
     * There is only 1 group of 5 consecutive bits of 1 and the other groups
     * have less than consecutive bits of 1
     * Steps:
     * - find 5 consecutive bits of 1
     * - the NEXT 16 bits == spell
     */
    uint16_t res = -1;
    uint8_t i = 0;
    uint8_t c = 0;
    
    for (i = 0;i < 63;i++)
    {
        if ((memory & (((uint64_t)1 << i))) != 0) c++;
        else c = 0;
        if (c == 5) break;
        //Daca gasim 5 biti de 1 consecutivi iesim din ciclul
    }
    //Pozitia primului bit din cei 16(care ne intereseaza) este i+1
    i++;
    //Shiftam de i ori la dreapta si dezactivam din memory bitii 17-64
    //prin operatia de si pe biti cu nr maxim pe 16 biti(65535)
    res = ((memory >> i) & MAXUINT16);
    
    return res;
}


uint16_t find_key(uint64_t memory)
{
    /*
     * The key is 16 bits and they are in the memory exactly before 3
     * consecutive bits of 1:
     *
     *                high                                           low
     *                     [][][][1][1][1][x][x]...[x][x][]...[][][]
     *                                    |----16bits---|
     *                                           Key
     * Steps:
     * - find 3 consecutive bits of 1
     * - the PREVIOUS 16 bits == key
     */

    uint16_t res = -1;
    uint8_t i = 0;
    uint8_t c = 0;
    
    for (i = 0;i < 63;i++)
    {
        if ((memory & (((uint64_t)1 << i))) != 0) c++;
        else c = 0;
        if (c == 3) break;
    }
    //Pozitia primului bit din key este 
    //i-16(nr de biti care repr key)-2(cei 2 biti de 1 dinainte)
    i = i - 18;
    res = ((memory >> i) & MAXUINT16);

    return res;
}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */

    uint16_t res = -1;
    //In urma unor observatii si calcule 
    //am dedus ca xor este o operatie pe biti 
    //reversibila(spre deosebire de & si |)
    res = spell ^ key;

    return res;
}

/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{
    /*
     * Help Geralt construct a sword that will give him an advantage in battle.
     * The steps we will follow:
     * - choose the corect sword
     * - brand the sword with the correct runes such that the equation
     * for that specific enemy type is true
     *
     * How does the sword look (Sword template):
     *  [][][][] [0][0][0]...[0][0][0] [][][][][][][][][][][][][][][][]
     *  -------- --------------------- --------------------------------
     *     Type    Does not matter      The "Value" such that the equation
     *                               from 2 holds depending on the enemy type
     *    4 bits      12 bits                     16 bits
     *
     * 1. Type of the sword:
     * - 1001 - Silver Sword - for monsters
     * - 0110 - Steel Sword - for humans
     *
     *  If the number of active bits from variable "enemy" is even == Monster
     *  else == Human
     *
     * 2.
     * Monster Equation:
     *    Value ^ (Enemy & (1 - Enemy)) = 0
     *
     * Human Equation:
     *    Value + Enemy = 0
     */

    uint32_t res = -1;
    uint8_t i = 0;
    uint8_t c = 0;
    //Aflam numarul de biti activi din enemy
    for (i = 0;i < 32;i++)
    if ((enemy & (1 << i)) != 0) c++;
    //Daca nr par de biti activi atunci shiftam type-ul pt monster
    //pana pe ultimii 4 biti din variabila res
    //si adunam cu value care se situeaza pe primii 16 biti
    //deci nu trb sa shiftam nimic
    if ((c % 2) == 0) 
    res = (1 << 31) + (1 << 28) + 
    (((enemy & (1 - enemy)) ^ 0) & MAXUINT16);
    //Analogic pt type-ul human si value-ul respectiv
    else 
    res = (1 << 30) + (1 << 29) + 
    ((0 - enemy) & MAXUINT16);

    return res;
}

/* Task 3 - The Witcher Trials */

uint32_t trial_of_the_grasses(uint16_t cocktail)
{
    /*
     * To become a witcher one must survive a cocktail of different
     * herbs. The body should generate the correct antibodies to
     * neutralize the given cocktail.
     *
     *
     * The antibodies need to respect the following properties:
     *   (antibodies_high & cocktail) ^ (antibodies_low | cocktail) = 0
     *   antibodies_low & antibodies_high = 0
     *   antibodies_low | antibodies_high = cocktail
     *
     * Where:
     *  [][][]...[][][] | [][][]...[][][]
     *  ---------------   ---------------
     *  antibodies_high    antibodies_low
     *      16 bits           16 bits
     *      -------------------------
     *              antibodies
     */

    uint32_t res = -1;
    //In urma unor calcule si observatii am concluzionat 
    //ca unica varianta admisibila este pt antibodies_low=0
    //si antibodies_high=cocktail
    res = (cocktail << 16);

    return res;
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{
    /*
     * For the next trail, the candidate is tied up and blindfolded.
     * They are also taken to a forest and they should return until
     * the next morning.
     *
     * The candidate knows that there are 4 possibilities for the forest:
     * Brokilon, Hindar, Wolven Glade and Caed Dhu.
     *
     * The candidate also knows those forests by heart, but first you
     * need to identify the forest.
     *
     * The characteristics for 3 of the forests are as follows:
     * - Brokilon - has ONLY groups of 4 trees (or a forest without any tree)
     *   Ex: a group of 4 trees: "...00111100..."
     * - Hindar - has ONLY 2 trees in the MIDDLE
     * - Wolven Glade - FULL of trees
     *
     * The "map" variable encodes the following information:
     * - 1 tree
     * - 0 patch of grass
     *
     * You should put in the "res" variable the index for the identified
     * forrest.
     * - Brokilon - index 0
     * - Hindar - index 1
     * - Wolven Glade - index 2
     * - Caed Dhu - index 3
     */

    uint8_t res = -1;
    uint8_t i = 0;
    uint8_t contor = 0;
    //Presupun ca variabila brokilon are valoarea 1
    //Adica presupun ca e padurea Brokilon
    uint8_t brokilon = 1;
    //Presupun ca este padurea Caed Dhu(indice 3)
    //Daca nu se va dovedi ca e alta padure atunci va ramane aceasta
    res = 3;
    
    for (i=0; i < 64;i++)
    {
    //Numar numarul de biti activi in map
    if ((map & ((uint64_t)1 << i)) != 0) contor++;
    //Daca gasesc un nr nedivizibil cu 4 de biti activi consecutivi
    //atunci cu siguranta nu este padurea Brokilon
    if (((contor % 4) != 0) && (((map & ((uint64_t)1 << i)) == 0))) brokilon = 0; 
    }
    //Daca nu se zero-grafiaza variabila brokilon atunci este padurea brokilon
    if (brokilon == 1) res = 0;
    //Daca sunt 2 biti activi pe mijloc atunci e padurea Hindar
    if (((map & ((uint64_t)1 << 31)) != 0) && 
    ((map & ((uint64_t)1 << 32)) != 0) && (contor == 2)) res = 1;
    //Daca sunt 64 biti activi atunci e padurea Wolven Glade
    if (contor == 64) res = 2;
    
    return res;
}


uint8_t trial_of_the_dreams(uint32_t map)
{
    /*
     * For this final trial, the candidates go into a trance and when
     * they woke up they would be a fully fledged witcher.
     *
     * For this task, you need to help the witcher find the number
     * of steps that would take the candidate from the dream realm to
     * the real world.
     *
     * The "map" variable would have only 2 bits of 1:
     * - candidate location
     * - portal location
     *
     * You should put in the "res" variable the distance from the
     * candidate position to the portal.
     *
     *  Ex:
     *  [0][0][0][0][0][0]....[1][0][0][0][0][1][0]....[0][0][0]
     *
     *  res = 5
     */
    
    uint8_t res = 0;
    uint8_t i=0;
    
    for (i=0;i<32;i++)
    if ((map & (1 << i)) !=0) res = i - res;
    
    return res;
}

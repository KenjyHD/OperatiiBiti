#include "communication.h"
#include "util_comm.h"

#include <stdio.h>

/* Task 1 - The Beginning */

void send_byte_message(void)
{
    /* TODO
     * Send the encoding of the characters: R, I, C, K
     */
    send_squanch(18);
    send_squanch(9);
    send_squanch(3);
    send_squanch(11);
}


void recv_byte_message(void)
{
    /* TODO
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     *
     * ATTENTION!: Use fprintf(stdout, ...)
     */
    fprintf(stdout,"%c",(char)(recv_squanch() + 64));
    fprintf(stdout,"%c",(char)(recv_squanch() + 64));
    fprintf(stdout,"%c",(char)(recv_squanch() + 64));
    fprintf(stdout,"%c",(char)(recv_squanch() + 64));
    fprintf(stdout,"%c",(char)(recv_squanch() + 64));    
}


void comm_byte(void)
{
    /* TODO
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */
    int aux = 0;
    
    //Citesc si trimit de 2 ori
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
    
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
    
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
    
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
    
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
    
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
    
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
    
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
    
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
    
    aux = recv_squanch();
    send_squanch(aux);
    send_squanch(aux);
}

/* Task 2 - Waiting for the Message */

void send_message(void)
{
    /* TODO
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */


    //In ascii codul zecimal pt literele majuscule incepe cu valoarea 65(A)
    //respectiv pana la valoarea 90(Z)
    send_squanch(10 << 2);
    send_squanch((int)'H' - 64);
    send_squanch((int)'E' - 64);
    send_squanch((int)'L' - 64);
    send_squanch((int)'L' - 64);
    send_squanch((int)'O' - 64);
    send_squanch((int)'T' - 64);
    send_squanch((int)'H' - 64);
    send_squanch((int)'E' - 64);
    send_squanch((int)'R' - 64);
    send_squanch((int)'E' - 64);
}


void recv_message(void)
{
    /* TODO
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     * 
     * ATTENTION!: Use fprintf(stdout, ...)
     */

    uint8_t i = 0;
    uint8_t length = 0;
    //Intrucit lungimea e codata in 8 biti cand shiftam de 2 ori la stanga
    //scapam de bitii 7 si 6 pe care nu trb sai luam in considerare
    length = (((uint8_t)(recv_squanch() << 2)) >> 4);   
    fprintf(stdout,"%d",length);
    //De fiecare data cand scriem recv_squanch() valoarea acestuia va fi 
    //diferita. De aceia memoram lungimea in variabila length
    for (i = 0;i < length;i++)    
    fprintf(stdout,"%c",(char)(recv_squanch() + 64));
}

void comm_message(void)
{
    /* TODO
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */


    uint8_t i = 0;
    uint8_t length = 0;
    uint8_t x = 0;
    
    length = (((uint8_t)(recv_squanch() << 2)) >> 4);   
    //Pentru a afla ultima litera a mesajului trb sa apelam functia
    //de atitea ori cit e lungimea mesajului
    for (i = 0;i < length;i++)
    x = recv_squanch();
    
    if  (((char)x + 64) == 'P')
    {
    send_squanch(10 << 2);
    send_squanch((int)'P' - 64);
    send_squanch((int)'I' - 64);
    send_squanch((int)'C' - 64);
    send_squanch((int)'K' - 64);
    send_squanch((int)'L' - 64);
    send_squanch((int)'E' - 64);
    send_squanch((int)'R' - 64);
    send_squanch((int)'I' - 64);
    send_squanch((int)'C' - 64);
    send_squanch((int)'K' - 64);
    }
    else
    {
    send_squanch(11 << 2);
    send_squanch((int)'V' - 64);
    send_squanch((int)'I' - 64);
    send_squanch((int)'N' - 64);
    send_squanch((int)'D' - 64);
    send_squanch((int)'I' - 64);
    send_squanch((int)'C' - 64);
    send_squanch((int)'A' - 64);
    send_squanch((int)'T' - 64);
    send_squanch((int)'O' - 64);
    send_squanch((int)'R' - 64);
    send_squanch((int)'S' - 64);
    }
}

/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    /* TODO
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */
    uint8_t message = 0;
    
    message = ((c1 & (1 << 3)) << 4) + ((c2 & (1 << 3)) << 3) + 
    ((c1 & (1 << 2))<<3) + ((c2 & (1 << 2)) << 2) +
    ((c1 & (1<<1)) << 2) + ((c2 & (1<<1)) << 1) +
    ((c1 & 1) << 1) + (c2 & 1) ;
    send_squanch(message);
}


uint8_t decode_squanch2(uint8_t c)
{
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */
    uint8_t res = -1;
    
    res = (c & (1<<7)) + ((c & (1 << 6)) >> 3) + 
    ((c & (1 << 5)) << 1) + ((c & (1 << 4)) >> 2) + 
    ((c & (1 << 3)) << 2) + ((c & (1 << 2)) >> 1) + 
    ((c & (1 << 1)) << 3) + (c & 1); 

    return res;
}

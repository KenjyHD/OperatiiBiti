#include "gates.h"
#include <stdio.h>
#include <assert.h>


uint8_t get_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);
    
    uint8_t res = -1;
 
    res=(nr&((uint64_t)1<<i))>>i;
    
    return res;
}


uint64_t flip_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    res=nr^((uint64_t)1<<i);

    return res;
}


uint64_t activate_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = 0xFF;

    res=(nr|((uint64_t)1<<i));

    return res;
}


uint64_t clear_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    res=nr & (~((uint64_t)1 << i));

    return res;
}


uint8_t nand_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    return !(a & b);
}


uint8_t and_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;
    
    res=nand_gate(nand_gate(a,b),nand_gate(a,b));

    return res;
}


uint8_t not_gate(uint8_t a)
{
    assert (a == 0 || a == 1);

    uint8_t res = -1;
    
    res=nand_gate(a,a);

    return res;
}


uint8_t or_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    res = nand_gate(nand_gate(a,a),nand_gate(b,b));

    return res;
}


uint8_t xor_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    res = nand_gate(nand_gate(nand_gate(a,b),a),
    nand_gate(nand_gate(a,b),b));

    return res;
}


uint8_t full_adder(uint8_t a, uint8_t b, uint8_t c)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    assert (c == 0 || c == 1);

    uint8_t res = -1;
    
    /*Valoarea sumei S o salvam pe bitul 1,
    iar valoarea restului C o salvam pe bitul 0.
    Astfel vom avea 4 rezultate diferite in dependenta de suma si rest*/
    res = (or_gate(or_gate(and_gate(and_gate(not_gate(a),not_gate(b)),c),
    and_gate(and_gate(not_gate(a),b),not_gate(c))),
    or_gate(and_gate(and_gate(a,b),c),
    and_gate(and_gate(a,not_gate(b)),not_gate(c)))) << 1) + 
    or_gate(or_gate(and_gate(a,b),and_gate(b,c)),and_gate(a,c));
    
    return res;
}


uint64_t ripple_carry_adder(uint64_t a, uint64_t b)
{
    uint64_t res = 0;
    int i = 0;
    int carry = 0;
    
    for (i = 0;i <= 63;i++)
    {
        /*Salvam valoarea restului din fiecare operatie in variabila carry.
        Valoarea sumei o aducem pe bitul i prin operatia de shiftare 
        si adunam intre ele sumele(obtinute prin functia full_adder)*/    
        res = res + ((get_bit(full_adder(get_bit(a,i),
        get_bit(b,i),carry),1)) << i);
        carry = get_bit(full_adder(get_bit(a,i),get_bit(b,i),carry),0);
    }
   /*Ne asiguram ca nu depasim valoarea maxima (2^64)-1*/
    if (res > (((uint64_t)1 << 63)-1))
    return 0;
    else
    return res;
}
